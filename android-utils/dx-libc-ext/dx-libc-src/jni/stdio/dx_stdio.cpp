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

#include "../log/ErrnoRestorer.h"
#include "../log/bionic_fortify.h"
#include "dx_stdio.h"
#include "dx_stdio_def.h"


static inline void free_fgetln_buffer(dxFILE* fp) {
  if (__predict_false(fp->_lb._base != nullptr)) {
    free(fp->_lb._base);
    fp->_lb._base = nullptr;
  }
}

static dxFILE* __dxfopen(int fd, int flags) {
#if !defined(__LP64__)
  if (fd > SHRT_MAX) {
    errno = EMFILE;
    return nullptr;
  }
#endif

  dxFILE* fp = __dxsfp();
  if (fp != nullptr) {
    fp->_file = fd;
    fp->_flags = flags;
    fp->_cookie = fp;
    fp->_read = __dxsread;
    fp->_write = __dxswrite;
    fp->_close = __dxsclose;
    _EXT(fp)->_seek64 = __dxsseek64;
  }
  return fp;
}

dxFILE* dx_fopen(const char* file, const char* mode) {
  if(isArmOnX86()){
    return (dxFILE*)fopen(file, mode);
  }
  int mode_flags;
  int flags = __dxsflags(mode, &mode_flags);
  if (flags == 0) return nullptr;

  int fd = dx_open(file, mode_flags, DEFFILEMODE);
  if (fd == -1) {
    return nullptr;
  }

  dxFILE* fp = __dxfopen(fd, flags);
  if (fp == nullptr) {
    ErrnoRestorer errno_restorer;
    close(fd);
    return nullptr;
  }

  // For append mode, even though we use O_APPEND, we need to seek to the end now.
  if ((mode_flags & O_APPEND) != 0) __dxsseek64(fp, 0, SEEK_END);
  return fp;
}

int dx_fclose(dxFILE* fp) {
  if(isArmOnX86()){
    return fclose((FILE*)fp);
  }
  CHECK_FP(fp);
  if (fp->_flags == 0) {
    // Already freed!
    errno = EBADF;
    return EOF;
  }

  ScopedFileLock sfl(fp);
  WCIO_FREE(fp);
  int r = fp->_flags & __SWR ? __dxsflush(fp) : 0;
  if (fp->_close != nullptr && (*fp->_close)(fp->_cookie) < 0) {
    r = EOF;
  }
  if (fp->_flags & __SMBF) free(fp->_bf._base);
  if (HASUB(fp)) FREEUB(fp);
  free_fgetln_buffer(fp);

  // Poison this FILE so accesses after fclose will be obvious.
  fp->_file = -1;
  fp->_r = fp->_w = 0;

  // Release this FILE for reuse.
  fp->_flags = 0;
  return r;
}

int dx_fseek(dxFILE* fp, off_t offset, int whence) {
  if(isArmOnX86()){
    return fseek((FILE*)fp, offset, whence);
  }
  CHECK_FP(fp);
  static_assert(sizeof(off_t) == sizeof(long), "sizeof(off_t) != sizeof(long)");
  return __dxfseeko64(fp, offset, whence, 8*sizeof(off_t));
}
__strong_alias(dx_fseeko, dx_fseek);


off_t dx_ftell(dxFILE* fp) {
  if(isArmOnX86()){
    return ftell((FILE*)fp);
  }
  CHECK_FP(fp);
  static_assert(sizeof(off_t) == sizeof(long), "sizeof(off_t) != sizeof(long)");
  off64_t result = dx_ftello64(fp);
  if (result > LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  return result;
}
__strong_alias(dx_ftello, dx_ftell);

char* dx_fgets_unlocked(char* buf, int n, dxFILE* fp) {
  if (n <= 0) {
    errno = EINVAL;
    return nullptr;
  }

  _SET_ORIENTATION(fp, -1);

  char* s = buf;
  n--; // Leave space for NUL.
  while (n != 0) {
    // If the buffer is empty, refill it.
    if (fp->_r <= 0) {
      if (__dxsrefill(fp)) {
        // EOF/error: stop with partial or no line.
        if (s == buf) return nullptr;
        break;
      }
    }
    size_t len = fp->_r;
    unsigned char* p = fp->_p;

    // Scan through at most n bytes of the current buffer,
    // looking for '\n'.  If found, copy up to and including
    // newline, and stop.  Otherwise, copy entire chunk and loop.
    if (len > static_cast<size_t>(n)) len = n;
    unsigned char* t = static_cast<unsigned char*>(memchr(p, '\n', len));
    if (t != nullptr) {
      len = ++t - p;
      fp->_r -= len;
      fp->_p = t;
      memcpy(s, p, len);
      s[len] = '\0';
      return buf;
    }
    fp->_r -= len;
    fp->_p += len;
    memcpy(s, p, len);
    s += len;
    n -= len;
  }
  *s = '\0';
  return buf;
}

char* dx_fgets(char* buf, int n, dxFILE* fp) {
  if(isArmOnX86()){
    return fgets(buf, n, (FILE*)fp);
  }
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_fgets_unlocked(buf, n, fp);
}

size_t dx_fread_unlocked(void* buf, size_t size, size_t count, dxFILE* fp) {
  CHECK_FP(fp);

  size_t desired_total;
  if (__builtin_mul_overflow(size, count, &desired_total)) {
    errno = EOVERFLOW;
    fp->_flags |= __SERR;
    return 0;
  }

  size_t total = desired_total;
  if (total == 0) return 0;

  _SET_ORIENTATION(fp, -1);

  // TODO: how can this ever happen?!
  if (fp->_r < 0) fp->_r = 0;

  // Ensure _bf._size is valid.
  if (fp->_bf._base == nullptr) __dxsmakebuf(fp);

  char* dst = static_cast<char*>(buf);

  while (total > 0) {
    // Copy data out of the buffer.
    size_t buffered_bytes = MIN(static_cast<size_t>(fp->_r), total);
    memcpy(dst, fp->_p, buffered_bytes);
    fp->_p += buffered_bytes;
    fp->_r -= buffered_bytes;
    dst += buffered_bytes;
    total -= buffered_bytes;

    // Are we done?
    if (total == 0) goto out;

    // Do we have so much more to read that we should avoid copying it through the buffer?
    if (total > static_cast<size_t>(fp->_bf._size)) break;

    // Less than a buffer to go, so refill the buffer and go around the loop again.
    if (__dxsrefill(fp)) goto out;
  }

  // Read directly into the caller's buffer.
  while (total > 0) {
    ssize_t bytes_read = (*fp->_read)(fp->_cookie, dst, total);
    if (bytes_read <= 0) {
      fp->_flags |= (bytes_read == 0) ? __SEOF : __SERR;
      break;
    }
    dst += bytes_read;
    total -= bytes_read;
  }

out:
  return ((desired_total - total) / size);
}


size_t dx_fread(void* buf, size_t size, size_t count, dxFILE* fp) {
  if(isArmOnX86()){
    return fread(buf, size, count, (FILE*)fp);
  }
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_fread_unlocked(buf, size, count, fp);
}


size_t dx_fwrite_unlocked(const void* buf, size_t size, size_t count, dxFILE* fp) {
  CHECK_FP(fp);

  size_t n;
  if (__builtin_mul_overflow(size, count, &n)) {
    errno = EOVERFLOW;
    fp->_flags |= __SERR;
    return 0;
  }

  if (n == 0) return 0;

  __siov iov = { .iov_base = const_cast<void*>(buf), .iov_len = n };
  __suio uio = { .uio_iov = &iov, .uio_iovcnt = 1, .uio_resid = n };

  _SET_ORIENTATION(fp, -1);

  // The usual case is success (__sfvwrite returns 0); skip the divide if this happens,
  // since divides are generally slow.
  return (__dxsfvwrite(fp, &uio) == 0) ? count : ((n - uio.uio_resid) / size);
}

size_t dx_fwrite(const void* buf, size_t size, size_t count, dxFILE* fp) {
  if(isArmOnX86()){
    return fwrite(buf, size, count, (FILE*)fp);
  }
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_fwrite_unlocked(buf, size, count, fp);
}


static inline int force_O_LARGEFILE(int flags) {
#if defined(__LP64__)
  return flags; // No need, and aarch64's strace gets confused.
#else
  return flags | O_LARGEFILE;
#endif
}

static inline bool needs_mode(int flags) {
  return ((flags & O_CREAT) == O_CREAT) || ((flags & O_TMPFILE) == O_TMPFILE);
}

int dx_open(const char* pathname, int flags, ...) {
  mode_t mode = 0;

  if (needs_mode(flags)) {
    va_list args;
    va_start(args, flags);
    mode = static_cast<mode_t>(va_arg(args, int));
    va_end(args);
  }

  return __dxopenat(AT_FDCWD, pathname, force_O_LARGEFILE(flags), mode);
}
__strong_alias(open64, open);

dxFILE* dx_fdopen(int fd, const char* mode) {
  if(isArmOnX86()){
    return (dxFILE*)fdopen(fd, mode);
  }
  int mode_flags;
  int flags = __dxsflags(mode, &mode_flags);
  if (flags == 0) return nullptr;

  // Make sure the mode the user wants is a subset of the actual mode.
  int fd_flags = fcntl(fd, F_GETFL, 0);
  if (fd_flags == -1) return nullptr;
  int tmp = fd_flags & O_ACCMODE;
  if (tmp != O_RDWR && (tmp != (mode_flags & O_ACCMODE))) {
    errno = EINVAL;
    return nullptr;
  }

  // Make sure O_APPEND is set on the underlying fd if our mode has 'a'.
  // POSIX says we just take the current offset of the underlying fd.
  if ((mode_flags & O_APPEND) && !(fd_flags & O_APPEND)) {
    if (fcntl(fd, F_SETFL, fd_flags | O_APPEND) == -1) return nullptr;
  }

  // Make sure O_CLOEXEC is set on the underlying fd if our mode has 'x'.
  if ((mode_flags & O_CLOEXEC) && !((tmp = fcntl(fd, F_GETFD)) & FD_CLOEXEC)) {
    fcntl(fd, F_SETFD, tmp | FD_CLOEXEC);
  }

  return __dxfopen(fd, flags);
}


int dx_feof_unlocked(dxFILE* fp) {
  CHECK_FP(fp);
  return ((fp->_flags & __SEOF) != 0);
}

int dx_feof(dxFILE* fp) {
  if(isArmOnX86()){
    return feof((FILE*)fp);
  }
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_feof_unlocked(fp);
}

int dx_ferror_unlocked(dxFILE* fp) {
  CHECK_FP(fp);
  return __sferror(fp);
}

int dx_ferror(dxFILE* fp) {
  if((isArmOnX86())){
    return ferror((FILE*)fp);
  }
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return dx_ferror_unlocked(fp);
}

static off64_t __seek_unlocked(dxFILE* fp, off64_t offset, int whence) {
  // Use `_seek64` if set, but fall back to `_seek`.
  if (_EXT(fp)->_seek64 != nullptr) {
    return (*_EXT(fp)->_seek64)(fp->_cookie, offset, whence);
  } else if (fp->_seek != nullptr) {
    off64_t result = (*fp->_seek)(fp->_cookie, offset, whence);
#if !defined(__LP64__)
    // Avoid sign extension if off64_t is larger than off_t.
    if (result != -1) result &= 0xffffffff;
#endif
    return result;
  } else {
    errno = ESPIPE;
    return -1;
  }
}

static off64_t __dxftello64_unlocked(dxFILE* fp) {
  // Find offset of underlying I/O object, then adjust for buffered bytes.
  __dxsflush(fp);  // May adjust seek offset on append stream.

  off64_t result = __seek_unlocked(fp, 0, SEEK_CUR);
  if (result == -1) {
    return -1;
  }

  if (fp->_flags & __SRD) {
    // Reading.  Any unread characters (including
    // those from ungetc) cause the position to be
    // smaller than that in the underlying object.
    result -= fp->_r;
    if (HASUB(fp)) result -= fp->_ur;
  } else if (fp->_flags & __SWR && fp->_p != nullptr) {
    // Writing.  Any buffered characters cause the
    // position to be greater than that in the
    // underlying object.
    result += fp->_p - fp->_bf._base;
  }
  return result;
}

off64_t dx_ftello64(dxFILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return __dxftello64_unlocked(fp);
}

int __dxfseeko64(dxFILE* fp, off64_t offset, int whence, int off_t_bits) {
  ScopedFileLock sfl(fp);

  // Change any SEEK_CUR to SEEK_SET, and check `whence` argument.
  // After this, whence is either SEEK_SET or SEEK_END.
  if (whence == SEEK_CUR) {
    fpos64_t current_offset = __dxftello64_unlocked(fp);
    if (current_offset == -1) {
      return -1;
    }
    offset += current_offset;
    whence = SEEK_SET;
  } else if (whence != SEEK_SET && whence != SEEK_END) {
    errno = EINVAL;
    return -1;
  }

  // If our caller has a 32-bit interface, refuse to go past a 32-bit file offset.
  if (off_t_bits == 32 && offset > LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }

  if (fp->_bf._base == nullptr) __dxsmakebuf(fp);

  // Flush unwritten data and attempt the seek.
  if (__dxsflush(fp) || __seek_unlocked(fp, offset, whence) == -1) {
    return -1;
  }

  // Success: clear EOF indicator and discard ungetc() data.
  if (HASUB(fp)) FREEUB(fp);
  fp->_p = fp->_bf._base;
  fp->_r = 0;
  /* fp->_w = 0; */	/* unnecessary (I think...) */
  fp->_flags &= ~__SEOF;
  return 0;
}

void dx_clearerr(dxFILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  dx_clearerr_unlocked(fp);
}


void dx_rewind(dxFILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  dx_fseek(fp, 0, SEEK_SET);
  dx_clearerr_unlocked(fp);
}

#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))

void *
reallocarray(void *optr, size_t nmemb, size_t size)
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) &&
	    nmemb > 0 && SIZE_MAX / nmemb < size) {
		errno = ENOMEM;
		return NULL;
	}
	return realloc(optr, size * nmemb);
}

static int __dxsubmore(dxFILE *fp)
{
	int i;
	unsigned char *p;

	if (_UB(fp)._base == fp->_ubuf) {
		/*
		 * Get a new buffer (rather than expanding the old one).
		 */
		if ((p = (unsigned char*)malloc(BUFSIZ)) == NULL)
			return (EOF);
		_UB(fp)._base = p;
		_UB(fp)._size = BUFSIZ;
		p += BUFSIZ - sizeof(fp->_ubuf);
		for (i = sizeof(fp->_ubuf); --i >= 0;)
			p[i] = fp->_ubuf[i];
		fp->_p = p;
		return (0);
	}
	i = _UB(fp)._size;
	p = (unsigned char*)reallocarray(_UB(fp)._base, i, 2);
	if (p == NULL)
		return (EOF);
	/* no overlap (hence can use memcpy) because we doubled the size */
	(void)memcpy(p + i, p, i);
	fp->_p = p + i;
	_UB(fp)._base = p;
	_UB(fp)._size = i * 2;
	return (0);
}


int dx_ungetc(int c, dxFILE *fp)
{
	if (c == EOF)
		return (EOF);
	if (!__dxsdidinit)
		__dxsinit();
	DXFLOCKFILE(fp);
	_SET_ORIENTATION(fp, -1);
	if ((fp->_flags & __SRD) == 0) {
		/*
		 * Not already reading: no good unless reading-and-writing.
		 * Otherwise, flush any current write stuff.
		 */
		if ((fp->_flags & __SRW) == 0) {
error:			DXFUNLOCKFILE(fp);
			return (EOF);
		}
		if (fp->_flags & __SWR) {
			if (__dxsflush(fp))
				goto error;
			fp->_flags &= ~__SWR;
			fp->_w = 0;
			fp->_lbfsize = 0;
		}
		fp->_flags |= __SRD;
	}
	c = (unsigned char)c;

	/*
	 * If we are in the middle of ungetc'ing, just continue.
	 * This may require expanding the current ungetc buffer.
	 */
	if (HASUB(fp)) {
		if (fp->_r >= _UB(fp)._size && __dxsubmore(fp))
			goto error;
		*--fp->_p = c;
inc_ret:	fp->_r++;
		DXFUNLOCKFILE(fp);
		return (c);
	}
	fp->_flags &= ~__SEOF;

	/*
	 * If we can handle this by simply backing up, do so,
	 * but never replace the original character.
	 * (This makes sscanf() work when scanning `const' data.)
	 */
	if (fp->_bf._base != NULL && fp->_p > fp->_bf._base &&
	    fp->_p[-1] == c) {
		fp->_p--;
		goto inc_ret;
	}

	/*
	 * Create an ungetc buffer.
	 * Initially, we will use the `reserve' buffer.
	 */
	fp->_ur = fp->_r;
	fp->_up = fp->_p;
	_UB(fp)._base = fp->_ubuf;
	_UB(fp)._size = sizeof(fp->_ubuf);
	fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
	fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
	fp->_r = 1;
	DXFUNLOCKFILE(fp);
	return (c);
}

size_t dx_parsefloat(dxFILE *fp, char *buf, char *end) {
	char *commit, *p;
	int infnanpos = 0;
	enum {
		S_START, S_GOTSIGN, S_INF, S_NAN, S_MAYBEHEX,
		S_DIGITS, S_FRAC, S_EXP, S_EXPDIGITS
	} state = S_START;
	unsigned char c;
	int gotmantdig = 0, ishex = 0;

	/*
	 * We set commit = p whenever the string we have read so far
	 * constitutes a valid representation of a floating point
	 * number by itself.  At some point, the parse will complete
	 * or fail, and we will ungetc() back to the last commit point.
	 * To ensure that the file offset gets updated properly, it is
	 * always necessary to read at least one character that doesn't
	 * match; thus, we can't short-circuit "infinity" or "nan(...)".
	 */
	commit = buf - 1;
	for (p = buf; p < end; ) {
		c = *fp->_p;
reswitch:
		switch (state) {
		case S_START:
			state = S_GOTSIGN;
			if (c == '-' || c == '+')
				break;
			else
				goto reswitch;
		case S_GOTSIGN:
			switch (c) {
			case '0':
				state = S_MAYBEHEX;
				commit = p;
				break;
			case 'I':
			case 'i':
				state = S_INF;
				break;
			case 'N':
			case 'n':
				state = S_NAN;
				break;
			default:
				state = S_DIGITS;
				goto reswitch;
			}
			break;
		case S_INF:
			if (infnanpos > 6 ||
			    (c != "nfinity"[infnanpos] &&
			     c != "NFINITY"[infnanpos]))
				goto parsedone;
			if (infnanpos == 1 || infnanpos == 6)
				commit = p;	/* inf or infinity */
			infnanpos++;
			break;
		case S_NAN:
			switch (infnanpos) {
			case -1:	/* XXX kludge to deal with nan(...) */
				goto parsedone;
			case 0:
				if (c != 'A' && c != 'a')
					goto parsedone;
				break;
			case 1:
				if (c != 'N' && c != 'n')
					goto parsedone;
				else
					commit = p;
				break;
			case 2:
				if (c != '(')
					goto parsedone;
				break;
			default:
				if (c == ')') {
					commit = p;
					infnanpos = -2;
				} else if (!isalnum(c) && c != '_')
					goto parsedone;
				break;
			}
			infnanpos++;
			break;
		case S_MAYBEHEX:
			state = S_DIGITS;
			if (c == 'X' || c == 'x') {
				ishex = 1;
				break;
			} else {	/* we saw a '0', but no 'x' */
				gotmantdig = 1;
				goto reswitch;
			}
		case S_DIGITS:
			if ((ishex && isxdigit(c)) || isdigit(c))
				gotmantdig = 1;
			else {
				state = S_FRAC;
				if (c != '.')
					goto reswitch;
			}
			if (gotmantdig)
				commit = p;
			break;
		case S_FRAC:
			if (((c == 'E' || c == 'e') && !ishex) ||
			    ((c == 'P' || c == 'p') && ishex)) {
				if (!gotmantdig)
					goto parsedone;
				else
					state = S_EXP;
			} else if ((ishex && isxdigit(c)) || isdigit(c)) {
				commit = p;
				gotmantdig = 1;
			} else
				goto parsedone;
			break;
		case S_EXP:
			state = S_EXPDIGITS;
			if (c == '-' || c == '+')
				break;
			else
				goto reswitch;
		case S_EXPDIGITS:
			if (isdigit(c))
				commit = p;
			else
				goto parsedone;
			break;
		default:
			abort();
		}
		*p++ = c;
		if (--fp->_r > 0)
			fp->_p++;
		else if (__dxsrefill(fp))
			break;	/* EOF */
	}

parsedone:
	while (commit < --p)
		(void)dx_ungetc(*(unsigned char *)p, fp);
	*++commit = '\0';
	return commit - buf;
}

static int dx_fflush_all() {
  return __dxfwalk(__dxsflush_locked);
}

int dx_fflush_unlocked(dxFILE* fp) {
  if (fp == nullptr) return dx_fflush_all();
  if ((fp->_flags & (__SWR | __SRW)) == 0) {
    errno = EBADF;
    return EOF;
  }
  return __dxsflush(fp);
}

int dx_fflush(dxFILE* fp) {
  if(isArmOnX86()){
    return fflush((FILE*)fp);
  }
  if (fp == nullptr) return dx_fflush_all();
  ScopedFileLock sfl(fp);
  return dx_fflush_unlocked(fp);
}

