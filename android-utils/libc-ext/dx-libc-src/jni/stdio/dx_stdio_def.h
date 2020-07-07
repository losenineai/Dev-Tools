#ifndef DX_STDIO_DEF
#define DX_STDIO_DEF

#include <sys/cdefs.h>
#include <sys/types.h>

#include <stdarg.h>
#include <stddef.h>

#include <bits/seek_constants.h>
#include "../log/bionic_fortify.h"
#include "dx_stdio_utils.h"
#include "../dx_libc_prefix.h"

typedef off_t fpos_t;
typedef off64_t fpos64_t;

struct __dxsFILE;
typedef struct __dxsFILE dxFILE;

#if __ANDROID_API__ >= __ANDROID_API_M__
extern dxFILE* dxstdin __INTRODUCED_IN(23);
extern dxFILE* dxstdout __INTRODUCED_IN(23);
extern dxFILE* dxstderr __INTRODUCED_IN(23);
#endif

#define	DXMIN(a, b) ((a) < (b) ? (a) : (b))
#define	DXCOPY(n)	  (void)memcpy((void *)fp->_p, (void *)p, (size_t)(n))

/* C99 and earlier plus current C++ standards say these must be macros. */
#define dxstdin dxstdin
#define dxstdout dxstdout
#define dxstderr dxstderr

#define WCIO_UNGETWC_BUFSIZE 1

#define	_IOFBF	0		/* setvbuf should set fully buffered */
#define	_IOLBF	1		/* setvbuf should set line buffered */
#define	_IONBF	2		/* setvbuf should set unbuffered */

#define	BUFSIZ	1024		/* size of buffer used by setbuf */
#define	EOF	(-1)

/*
 * Return true if the given FILE cannot be written now.
 */
#define dxcantwrite(fp) ((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && __dxswsetup(fp))

/*
 * FOPEN_MAX is a minimum maximum, and is the number of streams that
 * stdio can provide without attempting to allocate further resources
 * (which could fail).  Do not use this for anything.
 */
#ifndef FOPEN_MAX
#define FOPEN_MAX 20
#endif
#ifndef FILENAME_MAX
#define FILENAME_MAX 4096
#endif

#ifndef L_tmpnam
#define L_tmpnam 4096
#endif

#ifndef TMP_MAX
#define TMP_MAX 308915776
#endif

#define __SLBF 0x0001  // Line buffered.
#define __SNBF 0x0002  // Unbuffered.
// __SRD and __SWR are mutually exclusive because they indicate what we did last.
// If you want to know whether we were opened read-write, check __SRW instead.
#define __SRD 0x0004   // Last operation was read.
#define __SWR 0x0008   // Last operation was write.
#define __SRW 0x0010   // Was opened for reading & writing.
#define __SEOF 0x0020  // Found EOF.
#define __SERR 0x0040  // Found error.
#define __SMBF 0x0080  // `_buf` is from malloc.
// #define __SAPP 0x0100 --- historical (fdopen()ed in append mode).
#define __SSTR 0x0200  // This is an sprintf/snprintf string.
// #define __SOPT 0x0400 --- historical (do fseek() optimization).
// #define __SNPT 0x0800 --- historical (do not do fseek() optimization).
// #define __SOFF 0x1000 --- historical (set iff _offset is in fact correct).
// #define __SMOD 0x2000 --- historical (set iff fgetln modified _p text).
#define __SALC 0x4000  // Allocate string space dynamically.
#define __SIGN 0x8000  // Ignore this file in _fwalk.

// TODO: remove remaining references to these obsolete flags (see above).
#define __SMOD 0
#define __SNPT 0
#define __SOPT 0

#define HASUB(fp) (_UB(fp)._base != NULL)
#define FREEUB(fp)                                         \
  {                                                        \
    if (_UB(fp)._base != (fp)->_ubuf) free(_UB(fp)._base); \
    _UB(fp)._base = NULL;                                  \
  }


#define NO_PRINTF_PERCENT_N

#define __sferror(p) (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR | __SEOF)))
#define __sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))


#define _UB(fp) _EXT(fp)->_ub
#define _FLOCK(fp) _EXT(fp)->_lock

#define _FILEEXT_SETUP(fp, fext)                                              \
  do {                                                                        \
    (fp)->_ext._base = __BIONIC_CAST(reinterpret_cast, unsigned char*, fext); \
    memset(_EXT(fp), 0, sizeof(struct __dxsfileext));                           \
    _EXT(fp)->_caller_handles_locking = true;                                 \
  } while (0)

// Not actually implemented.
#define __dxsdidinit 1

#define CHECK_FP(fp) \
  if (fp == nullptr) __fortify_fatal("%s: null dxFILE*", __FUNCTION__)

/*
 * Used to tag symbols that should be hidden for 64-bit,
 * but visible to preserve binary compatibility for LP32.
 */
#ifdef __LP64__
#define __LIBC32_LEGACY_PUBLIC__ __attribute__((visibility("hidden")))
#else
#define __LIBC32_LEGACY_PUBLIC__ __attribute__((visibility("default")))
#endif


#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) (((uintptr_t)(p) + ALIGNBYTES) &~ ALIGNBYTES)

#define	NDYNAMIC 10		/* add ten more whenever necessary */

#define PRINTF_IMPL(expr) \
    va_list ap; \
    va_start(ap, fmt); \
    int result = (expr); \
    va_end(ap); \
    return result;


#ifndef DEFFILEMODE
#define DEFFILEMODE  (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#endif

#define __strong_alias(alias, sym) \
    __asm__(".global " #alias "\n" \
            #alias " = " #sym);
            
#define SBUF_INIT {}
#define WCHAR_IO_DATA_INIT {}

#define MAXEXPDIG 6
#if LDBL_MAX_EXP > 999999
#error "floating point buffers too small"
#endif

// char* __hdtoa(double, const char*, int, int*, int*, char**);
// char* __hldtoa(long double, const char*, int, int*, int*, char**);
// char* __ldtoa(long double*, int, int, int*, int*, char**);

#define WCIO_GET(fp) (_EXT(fp) ? &(_EXT(fp)->_wcio) : (struct wchar_io_data*)0)

#define _SET_ORIENTATION(fp, mode)                                 \
  do {                                                             \
    struct wchar_io_data* _wcio = WCIO_GET(fp);                    \
    if (_wcio && _wcio->wcio_mode == 0) _wcio->wcio_mode = (mode); \
  } while (0)

#define WCIO_FREE(fp)                           \
  do {                                          \
    struct wchar_io_data* _wcio = WCIO_GET(fp); \
    if (_wcio) {                                \
      _wcio->wcio_mode = 0;                     \
      _wcio->wcio_ungetwc_inbuf = 0;            \
    }                                           \
  } while (0)


__BEGIN_DECLS

#define __THREAD_NAME(name)	__CONCAT(_thread_tagname_,name)

struct __thread_private_tag_t {
    pthread_mutex_t    _private_lock;
    pthread_key_t      _private_key;
};

#define _THREAD_PRIVATE_MUTEX(name)  \
	static struct __thread_private_tag_t  __THREAD_NAME(name) = { PTHREAD_MUTEX_INITIALIZER, -1 }
#define _THREAD_PRIVATE_MUTEX_LOCK(name)  \
	pthread_mutex_lock( &__THREAD_NAME(name)._private_lock )
#define _THREAD_PRIVATE_MUTEX_UNLOCK(name) \
	pthread_mutex_unlock( &__THREAD_NAME(name)._private_lock )

/* Note that these aren't compatible with the usual OpenBSD ones which lazy-initialize! */
#define _MUTEX_LOCK(l) pthread_mutex_lock((pthread_mutex_t*) l)
#define _MUTEX_UNLOCK(l) pthread_mutex_unlock((pthread_mutex_t*) l)

__LIBC_HIDDEN__ void  _thread_atexit_lock(void);
__LIBC_HIDDEN__ void  _thread_atexit_unlock(void);

#define _ATEXIT_LOCK() _thread_atexit_lock()
#define _ATEXIT_UNLOCK() _thread_atexit_unlock()

__LIBC_HIDDEN__ void    _thread_arc4_lock(void);
__LIBC_HIDDEN__ void    _thread_arc4_unlock(void);

#define _ARC4_LOCK() _thread_arc4_lock()
#define _ARC4_UNLOCK() _thread_arc4_unlock()
#define _ARC4_ATFORK(f) pthread_atfork(NULL, NULL, (f))

extern volatile sig_atomic_t _rs_forked;

__END_DECLS

#endif