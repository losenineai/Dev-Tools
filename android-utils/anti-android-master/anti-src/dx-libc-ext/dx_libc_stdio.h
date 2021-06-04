//
// Created by amlloc on 18-1-28.
//

#ifndef SYSCALLDEMO2_DX_STDIO_H_H
#define SYSCALLDEMO2_DX_STDIO_H_H

#include <unistd.h>
#include <cstdio>

#include "dx_libc_config.h"

#ifndef DEFFILEMODE
#define DEFFILEMODE  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

/*	 p r w flags file _bf z  cookie      close    read    seek    write
	 ext */
/*syscall*/

struct __dxsbuf {
    unsigned char *_base;
    int	_size;
};

//#define	std(flags, file) \
//	{0,0,0,flags,file,{0,0},0,__sF+file,__sclose,__sread,__sseek,__swrite, \
//	 {(unsigned char *)(__sFext+file), 0},NULL,0,{0,0,0},{0},{0,0},0,0}
///*	 p r w flags file _bf z  cookie      close    read    seek    write
//	 ext */
/*stdio buffers */
#define	dxstd(flags, file) {0,0,0,flags,file,{NULL,0},0,NULL,__dxsclose,__dxsread,__dxsseek,__dxswrite, {(unsigned char*)(__dxsFext + file), 0} ,NULL,0,{0,0,0},{0},{0,0},0,0}

typedef	struct __dxsFILE {
    unsigned char *_p;	/* current position in (some) buffer */
    int	_r;		/* read space left for getc() */
    int	_w;		/* write space left for putc() */
    short	_flags;		/* flags, below; this FILE is free if 0 */
    short	_file;		/* fileno, if Unix descriptor, else -1 */
    struct	__dxsbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
    int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

    /* operations */
    void	*_cookie;	/* cookie passed to io functions */
    int	(*_close)(void *);
    int	(*_read)(void *, char *, int);
    fpos_t	(*_seek)(void *, fpos_t, int);
    int	(*_write)(void *, const char *, int);

    /* extension data, to avoid further ABI breakage */
    struct	__dxsbuf _ext;
    /* data for long sequences of ungetc() */
    unsigned char *_up;	/* saved _p when _p is doing ungetc data */
    int	_ur;		/* saved _r when _r is counting ungetc data */

    /* tricks to meet minimum requirements even when malloc() fails */
    unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
    unsigned char _nbuf[1];	/* guarantee a getc() buffer */

    /* separate buffer for fgetln() when line crosses buffer boundary */
    struct	__dxsbuf _lb;	/* buffer for fgetln() */

    /* Unix stdio files get aligned to block boundaries on fseek() */
    int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
    fpos_t	_offset;	/* current lseek offset */
} dxFILE;

#define	__SLBF	0x0001		/* line buffered */
#define	__SNBF	0x0002		/* unbuffered */
#define	__SRD	0x0004		/* OK to read */
#define	__SWR	0x0008		/* OK to write */
/* RD and WR are never simultaneously asserted */
#define	__SRW	0x0010		/* open for reading & writing */
#define	__SEOF	0x0020		/* found EOF */
#define	__SERR	0x0040		/* found error */
#define	__SMBF	0x0080		/* _buf is from malloc */
#define	__SAPP	0x0100		/* fdopen()ed in append mode */
#define	__SSTR	0x0200		/* this is an sprintf/snprintf string */
#define	__SOPT	0x0400		/* do fseek() optimisation */
#define	__SNPT	0x0800		/* do not do fseek() optimisation */
#define	__SOFF	0x1000		/* set iff _offset is in fact correct */
#define	__SMOD	0x2000		/* true => fgetln modified _p text */
#define	__SALC	0x4000		/* allocate string space dynamically */
#define	__SIGN	0x8000		/* ignore this file in _fwalk */

//FILE __sF[3] = {
//	std(__SRD, STDIN_FILENO),		/* stdin */
//	std(__SWR, STDOUT_FILENO),		/* stdout */
//	std(__SWR|__SNBF, STDERR_FILENO)	/* stderr */
//};
__BEGIN_DECLS
//extern FILE __sF[];
__END_DECLS



/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which brilliantly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although numbered as their counterparts above, the implementation
 * does not rely on this.
 */
#define	_IOFBF	0		/* setvbuf should set fully buffered */
#define	_IOLBF	1		/* setvbuf should set line buffered */
#define	_IONBF	2		/* setvbuf should set unbuffered */

#define	BUFSIZ	1024		/* size of buffer used by setbuf */

#define	EOF	(-1)

/*
 * FOPEN_MAX is a minimum maximum, and should be the number of descriptors
 * that the kernel can provide without allocation of a resource that can
 * fail without the process sleeping.  Do not use this for anything.
 */
#define	dxFOPEN_MAX	20	/* must be <= OPEN_MAX <sys/syslimits.h> */
#define	dxFILENAME_MAX	1024	/* must be <= PATH_MAX <sys/syslimits.h> */

/* System V/ANSI C; this is the wrong way to do this, do *not* use these. */
#if __BSD_VISIBLE || __XPG_VISIBLE
#define	P_tmpdir	"/tmp/"
#endif
#define	dxL_tmpnam	1024	/* XXX must be == PATH_MAX */
#define	TMP_MAX		308915776

#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#endif
#ifndef SEEK_END
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

//#define	stdin	(&__sF[0])
//#define	stdout	(&__sF[1])
//#define	stderr	(&__sF[2])







#define _dxEXT(fp) ((struct __dxsfileext *)((fp)->_ext._base))
#define _dxUB(fp) _dxEXT(fp)->_ub
#define _dxFLOCK(fp)  _dxEXT(fp)->_lock

#define	dxHASUB(fp) (_dxUB(fp)._base != NULL)
#define	dxFREEUB(fp) { \
	if (_dxUB(fp)._base != (fp)->_ubuf) \
		free(_dxUB(fp)._base); \
	_dxUB(fp)._base = NULL; \
}
//struct dxglue {
//    	struct	glue *next;
//    	int	niobs;
//    	dxFILE	*iobs;
//    };
//
//static dxFILE dxusual[FOPEN_MAX - 3];
//static struct dxglue uglue = { 0, FOPEN_MAX - 3, usual };
//struct dxglue __sglue = { &uglue, 3, __sF };
#endif //SYSCALLDEMO2_DX_STDIO_H_H
