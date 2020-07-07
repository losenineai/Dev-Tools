#include "dx_stdio_utils.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_sscanf(const char* s, const char* fmt, ...);

int dx_vsscanf(const char *str, const char *fmt, __va_list ap);

int __dxsvfscanf(dxFILE* fp, const char* fmt0, va_list ap);

__END_DECLS