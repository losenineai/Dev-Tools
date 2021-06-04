//
// Created by amlloc on 18-1-30.
//

#ifndef SYSCALLDEMO2_DXLOCAL_H
#define SYSCALLDEMO2_DXLOCAL_H
//#include "dxwcio.h"
#include <stdio.h>
#include "dx_libc_fileext.h"
#include "dx_libc_sfvwrite.h"
#include "dx_libc_stdio.h"

#include "dx_libc_config.h"


#ifndef DEFFILEMODE
#define DEFFILEMODE  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

using namespace std;

/*
 * Information local to this implementation of stdio,
 * in particular, macros and private variables.
 */

/*syscall*/
long dxsyscall(long num, long a1 = 0, void* buff = 0, long a3 = 0, long a4 = 0, long a5 = 0, long a6 = 0);
long dxsyscall2(long num, long a1 = 0, long a2 = 0, long a3 = 0, long a4 = 0, long a5 = 0, long a6 = 0);
long dxsyscall3(long num, long a1 = 0, void* buff = 0, void* a3 = 0, long a4 = 0, long a5 = 0, long a6 = 0);
//
int dxsys_open(const char* name, int oflag, mode_t mode,int dirfd = 0);
//
ssize_t dxsys_read(int fd, void* buff, size_t size);
//
ssize_t dxsys_write(int fd, const void* buff, size_t size);

off_t dxsys_lseek(int fd, off_t offset, int whence);
//
int dxsys_close(int fd);

pid_t dxsys_getpid();

int dxsys_sigaction(int __signal, const struct sigaction* __new_action, struct sigaction* __old_action);


/*__s*** call*/
int __dxopen(const char *pathname, int flags, ...);

int dxfflush(dxFILE *fp);
int	__dxsflush(dxFILE *);
int	__dxsflush_locked(dxFILE *);
FILE	*__dxsfp(void);
int	__dxsrefill(dxFILE *);
int	__dxsread(void *, char *, int);
int	__dxswrite(void *, const char *, int);
fpos_t	__dxsseek(void *, fpos_t, int);
int	__dxsclose(void *);
void	__dxsinit(void);
void	_dxcleanup(void);
void	__dxsmakebuf(dxFILE *);
int	__dxswhatbuf(dxFILE *, size_t *, int *);
int	_dxfwalk(int (*)(dxFILE *));
int	__dxswsetup(dxFILE *);
int	__dxsflags(const char *, int *);
int	__dxvfprintf(FILE *, const char *, __va_list);
int __dxsfvwrite(dxFILE *, struct __dxsuio *);




//extern void __dxatexit_register_cleanup(void (*)(void));
//extern int __dxsdidinit;

/*
 * Return true if the given FILE cannot be written now.
 */
#define	dxcantwrite(fp) \
	((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
	 __dxswsetup(fp))

/*
 * Test whether the given stdio file has an active ungetc buffer;
 * release such a buffer, without restoring ordinary unread data.
 */
#define	dxHASUB(fp) (_dxUB(fp)._base != NULL)
#define	dxFREEUB(fp) { \
	if (_dxUB(fp)._base != (fp)->_ubuf) \
		free(_dxUB(fp)._base); \
	_dxUB(fp)._base = NULL; \
}

/*
 * test for an fgetln() buffer.
 */
#define	dxHASLB(fp) ((fp)->_lb._base != NULL)
#define	dxFREELB(fp) { \
	free((char *)(fp)->_lb._base); \
	(fp)->_lb._base = NULL; \
}

#define dxFLOCKFILE(fp)   do { if (__dxisthreaded) flockfile(fp); } while (0)
#define dxFUNLOCKFILE(fp) do { if (__dxisthreaded) funlockfile(fp); } while (0)

#endif //SYSCALLDEMO2_DXLOCAL_H
