
#ifndef	_DXSTDIO_H_
#define	_DXSTDIO_H_

#include <sys/cdefs.h>
#include <sys/types.h>

#include <stdarg.h>
#include <stddef.h>

#include <bits/seek_constants.h>
#include "dx_stdio_utils.h"
#include "dx_stdio_def.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

extern "C" int __dxopenat(int, const char*, int, int);

bool isArmOnX86();

int dx_fclose(dxFILE* __fp);
int dx_feof(dxFILE* __fp);
int dx_ferror(dxFILE* __fp);
int dx_fflush(dxFILE* fp);
int dx_ferror_unlocked(FILE* fp) __INTRODUCED_IN(23);
int dx_ungetc(int c, dxFILE *fp);
size_t dx_parsefloat(dxFILE *fp, char *buf, char *end);

char* dx_fgets(char* __buf, int __size, dxFILE* __fp);
size_t dx_fread(void* __buf, size_t __size, size_t __count, dxFILE* __fp);
size_t dx_fwrite(const void* __buf, size_t __size, size_t __count, dxFILE* __fp);


int dx_fseek(dxFILE* __fp, long __offset, int __whence);
long dx_ftell(dxFILE* __fp);
void dx_rewind(dxFILE* fp);

dxFILE* dx_fopen(const char* __path, const char* __mode);
int dx_open(const char* pathname, int flags, ...);
dxFILE* dx_fdopen(int fd, const char* mode);

#if defined(__USE_BSD)
int dx_fflush_unlocked(FILE* __fp) __INTRODUCED_IN(28);
size_t dx_fread_unlocked(void* __buf, size_t __size, size_t __count, FILE* __fp) __INTRODUCED_IN(28);
size_t dx_fwrite_unlocked(const void* __buf, size_t __size, size_t __count, FILE* __fp) __INTRODUCED_IN(28);
#endif

#if defined(__USE_GNU)
// int fputs_unlocked(const char* __s, FILE* __fp) __INTRODUCED_IN(28);
char* dx_fgets_unlocked(char* __buf, int __size, FILE* __fp) __INTRODUCED_IN(28);
#endif
int dx_feof_unlocked(FILE* fp) __INTRODUCED_IN(23);

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L) || \
    (defined(__cplusplus) && __cplusplus <= 201103L)
char* gets(char* __buf) __attribute__((deprecated("gets is unsafe, use fgets instead")));
#endif
int sprintf(char* __s, const char* __fmt, ...)
    __printflike(2, 3) __warnattr_strict("sprintf is often misused; please use snprintf");
int vsprintf(char* __s, const char* __fmt, va_list __args)
    __printflike(2, 0) __warnattr_strict("vsprintf is often misused; please use vsnprintf");
char* tmpnam(char* __s)
    __warnattr("tempnam is unsafe, use mkstemp or tmpfile instead");
#define P_tmpdir "/tmp/" /* deprecated */
char* tempnam(const char* __dir, const char* __prefix)
    __warnattr("tempnam is unsafe, use mkstemp or tmpfile instead");


__END_DECLS

#endif
