#ifndef DX_STDIO_UTILS
#define DX_STDIO_UTILS

#include <sys/cdefs.h>
#include <wchar.h>
#include "dx_stdio_def.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

struct __sbuf {
  unsigned char* _base;
#if defined(__LP64__)
  size_t _size;
#else
  int _size;
#endif
};

struct __dxsFILE {
  unsigned char* _p; /* current position in (some) buffer */
  int _r;            /* read space left for getc() */
  int _w;            /* write space left for putc() */
#if defined(__LP64__)
  int _flags; /* flags, below; this FILE is free if 0 */
  int _file;  /* fileno, if Unix descriptor, else -1 */
#else
  short _flags; /* flags, below; this FILE is free if 0 */
  short _file;  /* fileno, if Unix descriptor, else -1 */
#endif
  struct __sbuf _bf; /* the buffer (at least 1 byte, if !NULL) */
  int _lbfsize;      /* 0 or -_bf._size, for inline putc */

  // Function pointers used by `funopen`.
  // Note that `_seek` is ignored if `_seek64` (in __sfileext) is set.
  // TODO: NetBSD has `funopen2` which corrects the `int`s to `size_t`s.
  // TODO: glibc has `fopencookie` which passes the function pointers in a struct.
  void* _cookie; /* cookie passed to io functions */
  int (*_close)(void*);
  int (*_read)(void*, char*, int);
  fpos_t (*_seek)(void*, fpos_t, int);
  int (*_write)(void*, const char*, int);

  /* extension data, to avoid further ABI breakage */
  struct __sbuf _ext;
  /* data for long sequences of ungetc() */
  unsigned char* _up; /* saved _p when _p is doing ungetc data */
  int _ur;            /* saved _r when _r is counting ungetc data */

  /* tricks to meet minimum requirements even when malloc() fails */
  unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
  unsigned char _nbuf[1]; /* guarantee a getc() buffer */

  /* separate buffer for fgetln() when line crosses buffer boundary */
  struct __sbuf _lb; /* buffer for fgetln() */

  /* Unix stdio files get aligned to block boundaries on fseek() */
  int _blksize; /* stat.st_blksize (may be != _bf._size) */

  fpos_t _unused_0;  // This was the `_offset` field (see below).

  // Do not add new fields here. (Or remove or change the size of any above.)
  // Although bionic currently exports `stdin`, `stdout`, and `stderr` symbols,
  // that still hasn't made it to the NDK. All NDK-built apps index directly
  // into an array of this struct (which was in <stdio.h> historically), so if
  // you need to make any changes, they need to be in the `__sfileext` struct
  // below, and accessed via `_EXT`.
};

struct __dxsFILE;
typedef struct __dxsFILE dxFILE;

/*
 * The first few FILEs are statically allocated; others are dynamically
 * allocated and linked in via this glue structure.
 */
struct dxglue {
	struct	dxglue *next;
	int	niobs;
	dxFILE	*iobs;
};

/* This was referenced by a couple of different pieces of middleware and the Crystax NDK. */
// __LIBC32_LEGACY_PUBLIC__ extern struct dxglue __sglue;

#define WCIO_UNGETWC_BUFSIZE 1

struct wchar_io_data {
  mbstate_t wcio_mbstate_in;
  mbstate_t wcio_mbstate_out;

  wchar_t wcio_ungetwc_buf[WCIO_UNGETWC_BUFSIZE];
  size_t wcio_ungetwc_inbuf;

  int wcio_mode; /* orientation */
};

struct __dxsfileext {
  // ungetc buffer.
  struct __sbuf _ub;

  // Wide char io status.
  struct wchar_io_data _wcio;

  // File lock.
  pthread_mutex_t _lock;

  // __fsetlocking support.
  bool _caller_handles_locking;

  // Equivalent to `_seek` but for _FILE_OFFSET_BITS=64.
  // Callers should use this but fall back to `__sFILE::_seek`.
  off64_t (*_seek64)(void*, off64_t, int);
};

/* OpenBSD declares these in fvwrite.h, but we share them with C++ parts of the implementation. */
struct __siov {
  void* iov_base;
  size_t iov_len;
};
struct __suio {
  struct __siov* uio_iov;
  int uio_iovcnt;
  size_t uio_resid;
};

#define _EXT(fp) __BIONIC_CAST(reinterpret_cast, struct __dxsfileext*, (fp)->_ext._base)

void dx_flockfile(dxFILE* fp);
void dx_funlockfile(dxFILE* fp);

#define DXFLOCKFILE(fp) \
  if (!_EXT(fp)->_caller_handles_locking) dx_flockfile(fp)
#define DXFUNLOCKFILE(fp) \
  if (!_EXT(fp)->_caller_handles_locking) dx_funlockfile(fp)

class ScopedFileLock {
 public:
  explicit ScopedFileLock(dxFILE* fp) : fp_(fp) {
    DXFLOCKFILE(fp_);
  }
  ~ScopedFileLock() {
    DXFUNLOCKFILE(fp_);
  }

 private:
  dxFILE* fp_;
};

extern "C" ssize_t dx_write(int __fd, const void* __buf, size_t __count);

extern "C" ssize_t dx_read(int fd, void* buf, size_t count);

extern "C" off_t dx_lseek(int __fd, off_t __offset, int __whence);

extern "C" int ___dxclose(int);

int dx_close(int fd);

void __dxsinit(void);

int __dxsfvwrite(dxFILE*, struct __suio*);

int __dxsflags(const char *mode, int *optr);

int dx_ftrylockfile(dxFILE* fp);

int __dxfwalk(int (*function)(dxFILE *));

void __dxsmakebuf(dxFILE *fp);

int __dxswhatbuf(dxFILE *fp, size_t *bufsize, int *couldbetty);

int __dxsrefill(dxFILE *fp);

int __dxsfvwrite(dxFILE *fp, struct __suio *uio);

int __dxswsetup(dxFILE *fp);

int __dxfseeko64(dxFILE* fp, off64_t offset, int whence, int off_t_bits);

__LIBC32_LEGACY_PUBLIC__ int __dxsrefill(dxFILE*);

__LIBC32_LEGACY_PUBLIC__ int __dxswsetup(dxFILE*);

__LIBC32_LEGACY_PUBLIC__ int __dxsflags(const char*, int*);

__LIBC32_LEGACY_PUBLIC__ dxFILE* __dxsfp(void);

__LIBC32_LEGACY_PUBLIC__ void __dxsmakebuf(dxFILE*);

__LIBC32_LEGACY_PUBLIC__ int __dxsflush(dxFILE*);

__LIBC32_LEGACY_PUBLIC__ int __dxsread(void*, char*, int);

__LIBC32_LEGACY_PUBLIC__ int __dxswrite(void*, const char*, int);

__LIBC32_LEGACY_PUBLIC__ fpos_t __dxsseek(void*, fpos_t, int);

__LIBC32_LEGACY_PUBLIC__ int __dxsclose(void*);

__LIBC32_LEGACY_PUBLIC__ int __dxfwalk(int (*)(dxFILE*));

off64_t __dxsseek64(void*, off64_t, int);

int __dxsflush_locked(dxFILE*);

void dx_clearerr_unlocked(dxFILE* fp);

void dx_clearerr(dxFILE* fp);

int dx_ftrylockfile(dxFILE* fp);

off64_t dx_ftello64(dxFILE* fp);

int dx_fileno_unlocked(dxFILE* fp);

int dx_fileno(dxFILE*);

wint_t __dxfputwc_unlock(wchar_t wc, dxFILE *fp);

wint_t dx_fputwc(wchar_t wc, dxFILE *fp);

__END_DECLS

#endif