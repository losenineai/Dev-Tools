//
// Created by amlloc on 18-1-30.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include "dx_libc_local.h"
// #include <dx_utils_errno.h>
#include <string.h>
#include "errno.h"


int __dxopen(const char *pathname, int flags, ...){

    mode_t mode = 0;

    flags |= O_LARGEFILE;

    if (flags & O_CREAT)
    {
        va_list  args;

        va_start(args, flags);
        mode = (mode_t) va_arg(args, int);
        va_end(args);
    }

    return dxsys_open(pathname, flags, mode);
}

int __dxsread(void *cookie, char *buf, int n){

    dxFILE *fp = (dxFILE*)cookie;
    int ret;

    ret = dxsys_read(fp->_file, buf, n);
    /* if the read succeeded, update the current offset */
    if(ret >= 0 )
        fp->_offset += ret;
    else
        fp->_flags &= ~__SOFF;
    return (ret);

}

int __dxswrite(void *cookie, const char *buf, int n){
    dxFILE *fp = (dxFILE *)cookie;
    if (fp->_flags & __SAPP)
        (void) lseek(fp->_file, (off_t)0, SEEK_END);
    fp->_flags &= ~__SOFF;	/* in case FAPPEND mode is set */
    return (dxsys_write(fp->_file, buf, (size_t)n));
}

fpos_t __dxsseek(void *cookie, fpos_t offset, int whence){
    dxFILE *fp = (dxFILE *)cookie;
    off_t ret;

    ret = dxsys_lseek(fp->_file, (off_t)offset, whence);
    if (ret == (off_t)-1)
        fp->_flags &= ~__SOFF;
    else {
        fp->_flags |= __SOFF;
        fp->_offset = ret;
    }
    return (ret);
}

int __dxsclose(void *cookie){
    return (dxsys_close(((dxFILE *)cookie)->_file));
}

int __dxsflags(const char *mode, int *optr){

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

    /* [rwa]\+ or [rwa]b\+ means read and write */
    if (*mode == '+' || (*mode == 'b' && mode[1] == '+')) {
        ret = __SRW;
        m = O_RDWR;
    }
    *optr = m | o;
    return (ret);
}

int
dxfflush(dxFILE *fp)
{
	int r;

	if (fp == NULL)
        //这又是啥玩意?????
//		return (_fwalk(__dxsflush_locked));
        return 0;
//	FLOCKFILE(fp);
	if ((fp->_flags & (__SWR | __SRW)) == 0) {
		errno = EBADF;
		r = EOF;
	} else
		r = __dxsflush(fp);
//	FUNLOCKFILE(fp);
	return (r);
}

int
__dxsflush(dxFILE *fp)
{
    unsigned char *p;
    int n, t;

    t = fp->_flags;
    if ((t & __SWR) == 0)
        return (0);

    if ((p = fp->_bf._base) == NULL)
        return (0);

    n = fp->_p - p;		/* write this much */

    /*
     * Set these immediately to avoid problems with longjmp and to allow
     * exchange buffering (via setvbuf) in user write function.
     */
    fp->_p = p;
    fp->_w = t & (__SLBF|__SNBF) ? 0 : fp->_bf._size;

    for (; n > 0; n -= t, p += t) {
        t = (*fp->_write)(fp->_cookie, (char *)p, n);
        if (t <= 0) {
            fp->_flags |= __SERR;
            return (EOF);
        }
    }
    return (0);
}


/*
 * Write some memory regions.  Return zero on success, EOF on error.
 *
 * This routine is large and unsightly, but most of the ugliness due
 * to the three different kinds of output buffering is handled here.
 */
//__LIBC_HIDDEN__ int
int __dxsfvwrite(dxFILE *fp, struct __dxsuio *uio)
{
    size_t len;
    const char *p;
    struct __dxsiov *iov;
    int w, s;
    char *nl;
    int nlknown, nldist;
    // dxFILE* test = fp;
    if ((len = uio->uio_resid) == 0)
        return (0);
    /* make sure we can write */
    if (((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && __dxswsetup(fp))) {
        errno = EBADF;
        return (EOF);
    }

#define	MIN(a, b) ((a) < (b) ? (a) : (b))
#define	COPY(n)	  (void)memcpy((void *)fp->_p, (void *)p, (size_t)(n))

    iov = uio->uio_iov;
    p = (const char*)iov->iov_base;
    len = iov->iov_len;
    iov++;
#define GETIOV(extra_work) \
	while (len == 0) { \
		extra_work; \
		p = (const char*)iov->iov_base; \
		len = iov->iov_len; \
		iov++; \
	}
    if (fp->_flags & __SNBF) {
        /*
         * Unbuffered: write up to BUFSIZ bytes at a time.
         */
        do {
            GETIOV(;);
#if 1  /* BIONIC: don't limit to 1KB writes */
            w = (*fp->_write)(fp->_cookie, p, len);
#else
            w = (*fp->_write)(fp->_cookie, p, MIN(len, BUFSIZ2));
#endif
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
                (__SALC | __SSTR) && fp->_w < (int)len) {
                size_t blen = fp->_p - fp->_bf._base;
                unsigned char *_base;
                int _size;

                /* Allocate space exponentially. */
                _size = fp->_bf._size;
                do {
                    _size = (_size << 1) + 1;
                } while (_size < (int)(blen + len));
                _base = (unsigned char *)realloc(fp->_bf._base, _size + 1);
                if (_base == NULL)
                    goto err;
                fp->_w += _size - fp->_bf._size;
                fp->_bf._base = _base;
                fp->_bf._size = _size;
                fp->_p = _base + blen;
            }
            w = fp->_w;
            if (fp->_flags & __SSTR) {
                if ((int)len < w)
                    w = len;
                COPY(w);	/* copy MIN(fp->_w,len), */
                fp->_w -= w;
                fp->_p += w;
                w = len;	/* but pretend copied all */
            } else if (fp->_p > fp->_bf._base && (int)len > w) {
                /* fill and flush */
                COPY(w);
                /* fp->_w -= w; */ /* unneeded */
                fp->_p += w;
                if (dxfflush(fp))
                    goto err;
            } else if ((int)len >= (w = fp->_bf._size)) {
                /* write directly */
                w = (*fp->_write)(fp->_cookie, p, w);
                if (w <= 0)
                    goto err;
            } else {
                /* fill and done */
                w = len;
                COPY(w);
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
                nldist = nl ? nl + 1 - p : (int)len + 1;
                nlknown = 1;
            }
            s = MIN((int)len, nldist);
            w = fp->_w + fp->_bf._size;
            if (fp->_p > fp->_bf._base && s > w) {
                COPY(w);
                /* fp->_w -= w; */
                fp->_p += w;
                if (dxfflush(fp))
                    goto err;
            } else if (s >= (w = fp->_bf._size)) {
                w = (*fp->_write)(fp->_cookie, p, w);
                if (w <= 0)
                    goto err;
            } else {
                w = s;
                COPY(w);
                fp->_w -= w;
                fp->_p += w;
            }
            if ((nldist -= w) == 0) {
                /* copied the newline: flush and forget */
                if (dxfflush(fp))
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

int
__dxswsetup(dxFILE *fp)
{
//    /* make sure stdio is set up */
//    if (!__sdidinit)
//        __sinit();

    /*
     * If we are not writing, we had better be reading and writing.
     */
    // dxFILE* test = fp;
    if ((fp->_flags & __SWR) == 0) {
        if ((fp->_flags & __SRW) == 0)
            return (EOF);
        if (fp->_flags & __SRD) {
            /* clobber any ungetc data */
            if (dxHASUB(fp))
            dxFREEUB(fp);
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

