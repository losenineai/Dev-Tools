#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include <assert.h>
#include <errno.h>
#include <paths.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "dx_stdio_utils.h"
#include "dx_stdio.h"
#include "dx_popen.h"
#include "dx_exec.h"
#include "dx_popen.h"


static struct pid {
	struct pid *next;
	dxFILE *fp;
#ifdef _REENTRANT
	int fd;
#endif
	pid_t pid;
} *pidlist; 


#define rwlock_t pthread_rwlock_t
#define RWLOCK_INITIALIZER PTHREAD_RWLOCK_INITIALIZER

#define rwlock_rdlock pthread_rwlock_rdlock
#define rwlock_unlock pthread_rwlock_unlock
#define rwlock_wrlock pthread_rwlock_wrlock

// #ifdef _REENTRANT
static rwlock_t pidlist_lock = RWLOCK_INITIALIZER;
// #endif

#define _DIAGASSERT(e)
#define __readlockenv() 0
#define __unlockenv() 0

extern "C" int dx_pipe2(int __fds[2], int __flags);
extern "C" int dxdup3(int __old_fd, int __new_fd, int __flags);
extern "C" pid_t  dx_vfork(void);

#if defined(__arm__) || defined(__i386__)
extern "C" int __dxfcntl64(int, int, void*);
int dx_fcntl(int fd, int cmd, ...) {
  va_list ap;

  va_start(ap, cmd);
  void* arg = va_arg(ap, void*);
  va_end(ap);

  return __dxfcntl64(fd, cmd, arg);
}

#endif




int dxdup2(int old_fd, int new_fd) {
  // If old_fd is equal to new_fd and a valid file descriptor, dup2 returns
  // old_fd without closing it. This is not true of dup3, so we have to
  // handle this case ourselves.
  if (old_fd == new_fd) {
    if (dx_fcntl(old_fd, F_GETFD) == -1) {
      return -1;
    }
    return old_fd;
  }

  return dxdup3(old_fd, new_fd, 0);
}


dxFILE *dx_popen(const char *command, const char *type)
{
	if(isArmOnX86()){
		return (dxFILE*)popen(command, type);
	}
	struct pid *cur, *old;
	dxFILE *iop;
	const char * volatile xtype = type;
	int pdes[2], pid, serrno;
	volatile int twoway;
	int flags;

	_DIAGASSERT(command != NULL);
	_DIAGASSERT(xtype != NULL);

	flags = strchr(xtype, 'e') ? O_CLOEXEC : 0;
	if (strchr(xtype, '+')) {
		int stype = flags ? (SOCK_STREAM | SOCK_CLOEXEC) : SOCK_STREAM;
		twoway = 1;
		xtype = "r+";
		if (socketpair(AF_LOCAL, stype, 0, pdes) < 0)
			return NULL;
	} else  {
		twoway = 0;
		xtype = strrchr(xtype, 'r') ? "r" : "w";
		if (dx_pipe2(pdes, flags) == -1)
			return NULL;
	}

	if ((cur = (struct pid*)malloc(sizeof(struct pid))) == NULL) {
		(void)dx_close(pdes[0]);
		(void)dx_close(pdes[1]);
		errno = ENOMEM;
		return (NULL);
	}

	(void)rwlock_rdlock(&pidlist_lock);
	(void)__readlockenv();
	switch (pid = dx_vfork()) {
	case -1:			/* Error. */
		serrno = errno;
		(void)__unlockenv();
		(void)rwlock_unlock(&pidlist_lock);
		free(cur);
		(void)dx_close(pdes[0]);
		(void)dx_close(pdes[1]);
		errno = serrno;
		return (NULL);
		/* NOTREACHED */
	case 0:				/* Child. */
		/* POSIX.2 B.3.2.2 "popen() shall ensure that any streams
		   from previous popen() calls that remain open in the 
		   parent process are closed in the new child process. */
		for (old = pidlist; old; old = old->next)
#ifdef _REENTRANT
			dx_close(old->fd); /* don't allow a flush */
#else
			dx_close(dx_fileno(old->fp)); /* don't allow a flush */
#endif

		if (*xtype == 'r') {
			(void)dx_close(pdes[0]);
			if (pdes[1] != STDOUT_FILENO) {
				(void)dxdup2(pdes[1], STDOUT_FILENO);
				(void)dx_close(pdes[1]);
			}
			if (twoway)
				(void)dxdup2(STDOUT_FILENO, STDIN_FILENO);
		} else {
			(void)dx_close(pdes[1]);
			if (pdes[0] != STDIN_FILENO) {
				(void)dxdup2(pdes[0], STDIN_FILENO);
				(void)dx_close(pdes[0]);
			}
		}

		dx_execl(_PATH_BSHELL, "sh", "-c", command, NULL);
		_exit(127);
		/* NOTREACHED */
	}
	(void)__unlockenv();

	/* Parent; assume fdopen can't fail. */
	if (*xtype == 'r') {
		iop = dx_fdopen(pdes[0], xtype);
#ifdef _REENTRANT
		cur->fd = pdes[0];
#endif
		(void)dx_close(pdes[1]);
	} else {
		iop = dx_fdopen(pdes[1], xtype);
#ifdef _REENTRANT
		cur->fd = pdes[1];
#endif
		(void)dx_close(pdes[0]);
	}

	/* Link into list of file descriptors. */
	cur->fp = iop;
	cur->pid =  pid;
	cur->next = pidlist;
	pidlist = cur;
	(void)rwlock_unlock(&pidlist_lock);

	return (iop);
}

/*
 * pclose --
 *	Pclose returns -1 if stream is not associated with a `popened' command,
 *	if already `pclosed', or waitpid returns an error.
 */
int dx_pclose(dxFILE *iop)
{
	if(isArmOnX86()){
		return pclose((FILE*)iop);
	}
	struct pid *cur, *last;
	int pstat;
	pid_t pid;

	_DIAGASSERT(iop != NULL);

	rwlock_wrlock(&pidlist_lock);

	/* Find the appropriate file pointer. */
	for (last = NULL, cur = pidlist; cur; last = cur, cur = cur->next)
		if (cur->fp == iop)
			break;
	if (cur == NULL) {
		(void)rwlock_unlock(&pidlist_lock);
		return (-1);
	}

	(void)dx_fclose(iop);

	/* Remove the entry from the linked list. */
	if (last == NULL)
		pidlist = cur->next;
	else
		last->next = cur->next;

	(void)rwlock_unlock(&pidlist_lock);

	do {
		pid = waitpid(cur->pid, &pstat, 0);
	} while (pid == -1 && errno == EINTR);

	free(cur);

	return (pid == -1 ? -1 : pstat);
}

int dx_pipe(int pipefd[2]) {
	if(isArmOnX86()){
		return pipe(pipefd);
	}
  	return dx_pipe2(pipefd, 0);
}