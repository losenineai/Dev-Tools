//
// Created by amlloc on 18-1-30.
//
// #include "dx_libc_errno.h"
#include "dx_libc_local.h"
#include "errno.h"

int __dxsflush_locked(dxFILE *fp)
{
    int r;

//	FLOCKFILE(fp);
    r = __dxsflush(fp);
//	FUNLOCKFILE(fp);
    return (r);
}

int __dxsrefill(dxFILE *fp)
{

    /* make sure stdio is set up */
//    if (!__sdidinit)
//        __sinit();

    fp->_r = 0;		/* largely a convenience for callers */

    /* SysV does not make this test; take it out for compatibility */
    if (fp->_flags & __SEOF)
        return (EOF);

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
        // int fd = fp->_file;
    } else {
        /*
         * We were reading.  If there is an ungetc buffer,
         * we must have been reading from that.  Drop it,
         * restoring the previous buffer (if any).  If there
         * is anything in that buffer, return.
         */

//            if (dxHASUB(fp)) {
//                dxFREEUB(fp);
//                if ((fp->_r = fp->_ur) != 0) {
//                    fp->_p = fp->_up;
//                    return (0);
//                }
//            }
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
//        (void) _fwalk(dxlflush);
        fp->_flags &= ~__SIGN;

        /* Now flush this file without locking it. */
        if ((fp->_flags & (__SLBF|__SWR)) == (__SLBF|__SWR))
            __dxsflush(fp);
    }
    fp->_p = fp->_bf._base;
    fp->_r = (*fp->_read)(fp->_cookie, (char *)fp->_p, fp->_bf._size);
    fp->_flags &= ~__SMOD;	/* buffer contents are again pristine */
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