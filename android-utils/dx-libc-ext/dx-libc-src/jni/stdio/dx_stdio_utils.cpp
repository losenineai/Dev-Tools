#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <paths.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <malloc.h>

#include "dx_stdio_def.h"
#include "dx_stdio_utils.h"
#include "../log/bionic_fortify.h"
#include "dx_stdio.h"
#include "../dx_libc_prefix.h"
#include "../dx_libc.h"

static struct __dxsfileext __sFext[3] = {
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __dxsseek64 },
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __dxsseek64 },
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __dxsseek64 },
};

#define std(flags, file) \
    {0,0,0,flags,file,{0,0},0,__dxsF+file,__dxsclose,__dxsread,nullptr,__dxswrite, \
    {(unsigned char *)(__sFext+file), 0},nullptr,0,{0},{0},{0,0},0,0}

// __sF is exported for backwards compatibility. Until M, we didn't have symbols
// for stdin/stdout/stderr; they were macros accessing __sF.
dxFILE __dxsF[3] = {
  std(__SRD, STDIN_FILENO),
  std(__SWR, STDOUT_FILENO),
  std(__SWR|__SNBF, STDERR_FILENO),
};

dxFILE* dxstdin = &__dxsF[0];
dxFILE* dxstdout = &__dxsF[1];
dxFILE* dxstderr = &__dxsF[2];

struct dxglue __sglue = { nullptr, 3, __dxsF };
static struct dxglue* lastglue = &__sglue;

_THREAD_PRIVATE_MUTEX(__sfp_mutex);
/*
 * Return the (stdio) flags for a given mode.  Store the flags
 * to be passed to an open() syscall through *optr.
 * Return 0 on error.
 */
int __dxsflags(const char *mode, int *optr)
{
	int ret, m, o;

	switch (*mode++) {

	case 'r':	/* open for reading */
		ret = __SRD;
		m = O_RDONLY;
		o = 0;
		break;

	case 'w':	/* open for writing */
		ret = __SWR;
		m = O_WRONLY;
		o = O_CREAT | O_TRUNC;
		break;

	case 'a':	/* open for appending */
		ret = __SWR;
		m = O_WRONLY;
		o = O_CREAT | O_APPEND;
		break;

	default:	/* illegal mode */
		errno = EINVAL;
		return (0);
	}

	while (*mode != '\0') 
		switch (*mode++) {
		case 'b':
			break;
		case '+':
			ret = __SRW;
			m = O_RDWR;
			break;
		case 'e':
			o |= O_CLOEXEC;
			break;
		case 'x':
			if (o & O_CREAT)
				o |= O_EXCL;
			break;
		default:
			/*
			 * Lots of software passes other extension mode
			 * letters, like Window's 't'
			 */
#if 0
			errno = EINVAL;
			return (0);
#else
			break;
#endif
		}

	*optr = m | o;
	return (ret);
}

void dx_flockfile(dxFILE* fp) {
  if (fp != nullptr) {
    pthread_mutex_lock(&_FLOCK(fp));
  }
}

int dx_ftrylockfile(dxFILE* fp) {
  // The specification for ftrylockfile() says it returns 0 on success,
  // or non-zero on error. So return an errno code directly on error.
  if (fp == nullptr) {
    return EINVAL;
  }

  return pthread_mutex_trylock(&_FLOCK(fp));
}

void dx_funlockfile(dxFILE* fp) {
  if (fp != nullptr) {
    pthread_mutex_unlock(&_FLOCK(fp));
  }
}


int
__dxfwalk(int (*function)(dxFILE *))
{
	dxFILE *fp;
	int n, ret;
	struct dxglue *g;

	ret = 0;
	for (g = &__sglue; g != NULL; g = g->next)
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++) {
			if ((fp->_flags != 0) && ((fp->_flags & __SIGN) == 0))
				ret |= (*function)(fp);
		}
	return (ret);
}



/*
 * Allocate a file buffer, or switch to unbuffered I/O.
 * Per the ANSI C standard, ALL tty devices default to line buffered.
 *
 * As a side effect, we set __SOPT or __SNPT (en/dis-able fseek
 * optimisation) right after the fstat() that finds the buffer size.
 */
void
__dxsmakebuf(dxFILE *fp)
{
	void *p;
	int flags;
	size_t size;
	int couldbetty;

	if (fp->_flags & __SNBF) {
		fp->_bf._base = fp->_p = fp->_nbuf;
		fp->_bf._size = 1;
		return;
	}
	flags = __dxswhatbuf(fp, &size, &couldbetty);
	if ((p = malloc(size)) == NULL) {
		fp->_flags |= __SNBF;
		fp->_bf._base = fp->_p = fp->_nbuf;
		fp->_bf._size = 1;
		return;
	}
	flags |= __SMBF;
	fp->_bf._base = fp->_p = (unsigned char*)p;
	fp->_bf._size = size;
	if (couldbetty && isatty(fp->_file))
		flags |= __SLBF;
	fp->_flags |= flags;
}

/*
 * Internal routine to determine `proper' buffering for a file.
 */
int
__dxswhatbuf(dxFILE *fp, size_t *bufsize, int *couldbetty)
{
	struct stat st;

	if (fp->_file < 0 || fstat(fp->_file, &st) < 0) {
		*couldbetty = 0;
		*bufsize = BUFSIZ;
		return (__SNPT);
	}

	/* could be a tty iff it is a character device */
	*couldbetty = S_ISCHR(st.st_mode);
	if (st.st_blksize == 0) {
		*bufsize = BUFSIZ;
		return (__SNPT);
	}

	/*
	 * Optimise fseek() only if it is a regular file.  (The test for
	 * __sseek is mainly paranoia.)  It is safe to set _blksize
	 * unconditionally; it will only be used if __SOPT is also set.
	 */
	*bufsize = st.st_blksize;
	fp->_blksize = st.st_blksize;
	return ((st.st_mode & S_IFMT) == S_IFREG && fp->_seek == __dxsseek ?
	    __SOPT : __SNPT);
}


static int
dxlflush(dxFILE *fp)
{
	if ((fp->_flags & (__SLBF|__SWR)) == (__SLBF|__SWR))
		return (__dxsflush_locked(fp));	/* ignored... */
	return (0);
}


int __dxsflush_locked(dxFILE* fp) {
  ScopedFileLock sfl(fp);
  return __dxsflush(fp);
}

/*
 * Refill a stdio buffer.
 * Return EOF on eof or error, 0 otherwise.
 */
int
__dxsrefill(dxFILE *fp)
{
	fp->_r = 0;		/* largely a convenience for callers */

#if !defined(__BIONIC__)
	/* SysV does not make this test; take it out for compatibility */
	if (fp->_flags & __SEOF)
		return (EOF);
#endif

	/* if not already reading, have to be reading and writing */
	if ((fp->_flags & __SRD) == 0) {
		if ((fp->_flags & __SRW) == 0) {
			errno = EBADF;
			fp->_flags |= __SERR;
			return (EOF);
		}
		/* switch to reading */
		if (fp->_flags & __SWR) {
			if (__dxsflush(fp))
				return (EOF);
			fp->_flags &= ~__SWR;
			fp->_w = 0;
			fp->_lbfsize = 0;
		}
		fp->_flags |= __SRD;
	} else {
		/*
		 * We were reading.  If there is an ungetc buffer,
		 * we must have been reading from that.  Drop it,
		 * restoring the previous buffer (if any).  If there
		 * is anything in that buffer, return.
		 */
		if (HASUB(fp)) {
			FREEUB(fp);
			if ((fp->_r = fp->_ur) != 0) {
				fp->_p = fp->_up;
				return (0);
			}
		}
	}

	if (fp->_bf._base == NULL)
		__dxsmakebuf(fp);

	/*
	 * Before reading from a line buffered or unbuffered file,
	 * flush all line buffered output files, per the ANSI C
	 * standard.
	 */
	if (fp->_flags & (__SLBF|__SNBF)) {
		/* Ignore this file in _fwalk to avoid potential deadlock. */
		fp->_flags |= __SIGN;
		(void) __dxfwalk(dxlflush);
		fp->_flags &= ~__SIGN;

		/* Now flush this file without locking it. */
		if ((fp->_flags & (__SLBF|__SWR)) == (__SLBF|__SWR))
			__dxsflush(fp);
	}
	fp->_p = fp->_bf._base;
	fp->_r = (*fp->_read)(fp->_cookie, (char *)fp->_p, fp->_bf._size);
	if (fp->_r <= 0) {
		if (fp->_r == 0)
			fp->_flags |= __SEOF;
		else {
			fp->_r = 0;
			fp->_flags |= __SERR;
		}
		return (EOF);
	}
	return (0);
}



/*
 * Write some memory regions.  Return zero on success, EOF on error.
 *
 * This routine is large and unsightly, but most of the ugliness due
 * to the three different kinds of output buffering is handled here.
 */
int
__dxsfvwrite(dxFILE *fp, struct __suio *uio)
{
	size_t len;
	char *p;
	struct __siov *iov;
	int w, s;
	char *nl;
	int nlknown, nldist;

	if ((len = uio->uio_resid) == 0)
		return (0);
	/* make sure we can write */
	if (dxcantwrite(fp)) {
		errno = EBADF;
		return (EOF);
	}


	iov = uio->uio_iov;
	p = (char*)iov->iov_base;
	len = iov->iov_len;
	iov++;
#define GETIOV(extra_work) \
	while (len == 0) { \
		extra_work; \
		p = (char*)iov->iov_base; \
		len = iov->iov_len; \
		iov++; \
	}
	if (fp->_flags & __SNBF) {
		/*
		 * Unbuffered: write up to BUFSIZ bytes at a time.
		 */
		do {
			GETIOV(;);
			w = (*fp->_write)(fp->_cookie, p, DXMIN(len, BUFSIZ));
			if (w <= 0)
				goto err;
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	} else if ((fp->_flags & __SLBF) == 0) {
		/*
		 * Fully buffered: fill partially full buffer, if any,
		 * and then flush.  If there is no partial buffer, write
		 * one _bf._size byte chunk directly (without copying).
		 *
		 * String output is a special case: write as many bytes
		 * as fit, but pretend we wrote everything.  This makes
		 * snprintf() return the number of bytes needed, rather
		 * than the number used, and avoids its write function
		 * (so that the write function can be invalid).
		 */
		do {
			GETIOV(;);
			if ((fp->_flags & (__SALC | __SSTR)) ==
			    (__SALC | __SSTR) && fp->_w < len) {
				size_t blen = fp->_p - fp->_bf._base;
				unsigned char *_base;
				int _size;

				/* Allocate space exponentially. */
				_size = fp->_bf._size;
				do {
					_size = (_size << 1) + 1;
				} while (_size < blen + len);
				_base = (unsigned char*)realloc(fp->_bf._base, _size + 1);
				if (_base == NULL)
					goto err;
				fp->_w += _size - fp->_bf._size;
				fp->_bf._base = _base;
				fp->_bf._size = _size;
				fp->_p = _base + blen;
			}
			w = fp->_w;
			if (fp->_flags & __SSTR) {
				if (len < w)
					w = len;
				DXCOPY(w);	/* copy MIN(fp->_w,len), */
				fp->_w -= w;
				fp->_p += w;
				w = len;	/* but pretend copied all */
			} else if (fp->_p > fp->_bf._base && len > w) {
				/* fill and flush */
				DXCOPY(w);
				/* fp->_w -= w; */ /* unneeded */
				fp->_p += w;
				if (__dxsflush(fp))
					goto err;
			} else if (len >= (w = fp->_bf._size)) {
				/* write directly */
				w = (*fp->_write)(fp->_cookie, p, w);
				if (w <= 0)
					goto err;
			} else {
				/* fill and done */
				w = len;
				DXCOPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	} else {
		/*
		 * Line buffered: like fully buffered, but we
		 * must check for newlines.  Compute the distance
		 * to the first newline (including the newline),
		 * or `infinity' if there is none, then pretend
		 * that the amount to write is MIN(len,nldist).
		 */
		nlknown = 0;
		nldist = 0;	/* XXX just to keep gcc happy */
		do {
			GETIOV(nlknown = 0);
			if (!nlknown) {
				nl = (char*)memchr((void *)p, '\n', len);
				nldist = nl ? nl + 1 - p : len + 1;
				nlknown = 1;
			}
			s = DXMIN(len, nldist);
			w = fp->_w + fp->_bf._size;
			if (fp->_p > fp->_bf._base && s > w) {
				DXCOPY(w);
				/* fp->_w -= w; */
				fp->_p += w;
				if (__dxsflush(fp))
					goto err;
			} else if (s >= (w = fp->_bf._size)) {
				w = (*fp->_write)(fp->_cookie, p, w);
				if (w <= 0)
				 	goto err;
			} else {
				w = s;
				DXCOPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			if ((nldist -= w) == 0) {
				/* copied the newline: flush and forget */
				if (__dxsflush(fp))
					goto err;
				nlknown = 0;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	}
	return (0);

err:
	fp->_flags |= __SERR;
	return (EOF);
}


/*
 * Various output routines call wsetup to be sure it is safe to write,
 * because either _flags does not include __SWR, or _buf is NULL.
 * _wsetup returns 0 if OK to write, nonzero otherwise.
 */
int
__dxswsetup(dxFILE *fp)
{
	/* make sure stdio is set up */
	if (!__dxsdidinit)
		__dxsinit();

	/*
	 * If we are not writing, we had better be reading and writing.
	 */
	if ((fp->_flags & __SWR) == 0) {
		if ((fp->_flags & __SRW) == 0)
			return (EOF);
		if (fp->_flags & __SRD) {
			/* clobber any ungetc data */
			if (HASUB(fp))
				FREEUB(fp);
			fp->_flags &= ~(__SRD|__SEOF);
			fp->_r = 0;
			fp->_p = fp->_bf._base;
		}
		fp->_flags |= __SWR;
	}

	/*
	 * Make a buffer if necessary, then set _w.
	 */
	if (fp->_bf._base == NULL) {
		if ((fp->_flags & (__SSTR | __SALC)) == __SSTR)
			return (EOF);
		__dxsmakebuf(fp);
	}
	if (fp->_flags & __SLBF) {
		/*
		 * It is line buffered, so make _lbfsize be -_bufsize
		 * for the putc() macro.  We will change _lbfsize back
		 * to 0 whenever we turn off __SWR.
		 */
		fp->_w = 0;
		fp->_lbfsize = -fp->_bf._size;
	} else
		fp->_w = fp->_flags & __SNBF ? 0 : fp->_bf._size;
	return (0);
}

static dxglue* dxmoreglue(int n) {
  static dxFILE empty;

  char* data = new char[sizeof(dxglue) + ALIGNBYTES + n * sizeof(dxFILE) + n * sizeof(__dxsfileext)];
  if (data == nullptr) return nullptr;

  dxglue* g = reinterpret_cast<dxglue*>(data);
  dxFILE* p = reinterpret_cast<dxFILE*>(ALIGN(data + sizeof(*g)));
  __dxsfileext* pext = reinterpret_cast<__dxsfileext*>(ALIGN(data + sizeof(*g)) + n * sizeof(dxFILE));
  g->next = nullptr;
  g->niobs = n;
  g->iobs = p;
  while (--n >= 0) {
    *p = empty;
    _FILEEXT_SETUP(p, pext);
    p++;
    pext++;
  }
  return g;
}

/*
 * Find a free FILE for fopen et al.
 */
dxFILE* __dxsfp(void) {
	dxFILE *fp;
	int n;
	struct dxglue *g;

	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	for (g = &__sglue; g != nullptr; g = g->next) {
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags == 0)
				goto found;
	}

	/* release lock while mallocing */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	if ((g = dxmoreglue(NDYNAMIC)) == nullptr) return nullptr;
	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	lastglue->next = g;
	lastglue = g;
	fp = g->iobs;
found:
	fp->_flags = 1;		/* reserve this slot; caller sets real flags */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	fp->_p = nullptr;		/* no current pointer */
	fp->_w = 0;		/* nothing to read or write */
	fp->_r = 0;
	fp->_bf._base = nullptr;	/* no buffer */
	fp->_bf._size = 0;
	fp->_lbfsize = 0;	/* not line buffered */
	fp->_file = -1;		/* no file */

	fp->_lb._base = nullptr;	/* no line buffer */
	fp->_lb._size = 0;

	memset(_EXT(fp), 0, sizeof(struct __dxsfileext));
	_FLOCK(fp) = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	_EXT(fp)->_caller_handles_locking = false;

	// Caller sets cookie, _read/_write etc.
	// We explicitly clear _seek and _seek64 to prevent subtle bugs.
	fp->_seek = nullptr;
	_EXT(fp)->_seek64 = nullptr;

	return fp;
}

int __dxsread(void* cookie, char* buf, int n) {
  dxFILE* fp = reinterpret_cast<dxFILE*>(cookie);
  return TEMP_FAILURE_RETRY(dx_read(fp->_file, buf, n));
}

int __dxswrite(void* cookie, const char* buf, int n) {
  dxFILE* fp = reinterpret_cast<dxFILE*>(cookie);
  return TEMP_FAILURE_RETRY(dx_write(fp->_file, buf, n));
}

fpos_t __dxsseek(void* cookie, fpos_t offset, int whence) {
  dxFILE* fp = reinterpret_cast<dxFILE*>(cookie);\
  return TEMP_FAILURE_RETRY(dx_lseek(fp->_file, offset, whence));
}

off64_t __dxsseek64(void* cookie, off64_t offset, int whence) {
  dxFILE* fp = reinterpret_cast<dxFILE*>(cookie);
  return TEMP_FAILURE_RETRY(lseek64(fp->_file, offset, whence));
}

int __dxsclose(void* cookie) {
  dxFILE* fp = reinterpret_cast<dxFILE*>(cookie);
  return dx_close(fp->_file);
}

int __dxsflush(dxFILE* fp) {
  // Flushing a read-only file is a no-op.
  if ((fp->_flags & __SWR) == 0) return 0;

  // Flushing a file without a buffer is a no-op.
  unsigned char* p = fp->_bf._base;
  if (p == nullptr) return 0;

  // Set these immediately to avoid problems with longjmp and to allow
  // exchange buffering (via setvbuf) in user write function.
  int n = fp->_p - p;
  fp->_p = p;
  fp->_w = (fp->_flags & (__SLBF|__SNBF)) ? 0 : fp->_bf._size;

  while (n > 0) {
    int written = (*fp->_write)(fp->_cookie, reinterpret_cast<char*>(p), n);
    if (written <= 0) {
      fp->_flags |= __SERR;
      return EOF;
    }
    n -= written, p += written;
  }
  return 0;
}

void dx_clearerr_unlocked(dxFILE* fp) {
  return __sclearerr(fp);
}

void __dxsinit(void){}; 


int dx_close(int fd) {
	if(isArmOnX86()){
		return close(fd);
	}
  int rc = ___dxclose(fd);
  if (rc == -1 && errno == EINTR) {
    return 0;
  }
  return rc;
}

int dx_fileno_unlocked(dxFILE* fp) {
  CHECK_FP(fp);
  int fd = fp->_file;
  if (fd == -1) {
    errno = EBADF;
    return -1;
  }
  return fd;
}

int dx_fileno(dxFILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_fileno_unlocked(fp);
}

wint_t __dxfputwc_unlock(wchar_t wc, dxFILE *fp)
{
	struct wchar_io_data *wcio;
	mbstate_t *st;
	size_t size;
	char buf[MB_LEN_MAX];
	struct __suio uio;
	struct __siov iov;

	iov.iov_base = buf;
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;

	_SET_ORIENTATION(fp, 1);
	wcio = WCIO_GET(fp);
	if (wcio == 0) {
		errno = ENOMEM;
		return WEOF;
	}

	wcio->wcio_ungetwc_inbuf = 0;
	st = &wcio->wcio_mbstate_out;

	size = wcrtomb(buf, wc, st);
	if (size == (size_t)-1) {
		errno = EILSEQ;
		return WEOF;
	}

	uio.uio_resid = iov.iov_len = size;
	if (__dxsfvwrite(fp, &uio)) {
		return WEOF;
	}

	return (wint_t)wc;
}

wint_t dx_fputwc(wchar_t wc, dxFILE *fp)
{
	wint_t r;

	DXFLOCKFILE(fp);
	r = __dxfputwc_unlock(wc, fp);
	DXFUNLOCKFILE(fp);

	return (r);
}

static int isInit = 0;

bool isArmOnX86(){
    if(isInit != 0){
        if(isInit == 1){
            return true;
        }else{
            return false;
        }
    }
    pid_t pid = getpid();
    char path[32] = {0};
    if (dx_sprintf(path, "/proc/%d/maps", pid) == -1){
        isInit = 2;
        return false;
    }

    FILE* file = fopen(path, "rb");
    if(file == NULL){
        isInit = 2;
        return false;
    }
    char line[1024] = {0};
    while(fgets(line, sizeof(line), file) != NULL){
        if(dx_strstr(line, "/system/lib/arm/") && dx_strstr(line, "libc.so")){
            fclose(file);
            isInit = 1;
            return true;
        }
        memset(line, 0, sizeof(line));
    }
    fclose(file);
    isInit = 2;
    return false;
}
