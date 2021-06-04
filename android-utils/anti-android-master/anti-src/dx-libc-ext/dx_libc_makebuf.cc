//
// Created by amlloc on 18-1-30.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "dx_libc_local.h"

/*
 * Allocate a file buffer, or switch to unbuffered I/O.
 * Per the ANSI C standard, ALL tty devices default to line buffered.
 *
 * As a side effect, we set __SOPT or __SNPT (en/dis-able fseek
 * optimisation) right after the fstat() that finds the buffer size.
 */
void __dxsmakebuf(dxFILE *fp)
{

    int flags;
    size_t size;
    int couldbetty;
    // dxFILE* test = fp;

    if (fp->_flags & __SNBF) {
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
    flags = __dxswhatbuf(fp, &size, &couldbetty);
    unsigned char *p = NULL;

    /****
     *
     *Question:      这个玩意在写了文件后关闭dxFILE对象会破坏test结构体？？？？？？？？？？？
     *
      Solution：     呵呵，在dxfopen()对结构体dxFILE进行初始化的时候没有用malloc来分配空间

      New Question: 为什么dxfopen()在没有用malloc来分配空间的情况下,用"r"方式，打开，然后读取数据，
                     再对dxFILE对象进行dxclose()不会出现问题
    */
    if ((p = (unsigned char*)malloc(size)) == NULL) {
//    if((p = new unsigned char) == NULL){        //没有对dxFILE进行malloc的时候改为这样就可以了？？？
        fp->_flags |= __SNBF;
        fp->_bf._base = fp->_p = fp->_nbuf;
        fp->_bf._size = 1;
        return;
    }
//    __atexit_register_cleanup(_cleanup);
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
int __dxswhatbuf(dxFILE *fp, size_t *bufsize, int *couldbetty)
{
    struct stat st;
    // dxFILE* test = fp;
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
    }/*
     * Optimise fseek() only if it is a regular file.  (The test for
* __sseek is mainly paranoia.)  It is safe to set _blksize
 * unconditionally; it will only be used if __SOPT is also set.
	 */
    	*bufsize = st.st_blksize;
    	fp->_blksize = st.st_blksize;
    	return ((st.st_mode & S_IFMT) == S_IFREG && fp->_seek == __dxsseek ?
                 	    __SOPT : __SNPT);
    }