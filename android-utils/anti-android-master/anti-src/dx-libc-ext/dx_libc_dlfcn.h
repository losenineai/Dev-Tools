/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#ifndef dx_dlfcn_h
#define dx_dlfcn_h

#include "dx_libc_config.h"

extern void *dx_dlopen(const char *libpath, int flags);
extern void *dx_dlsym(void *handle, const char *name);
extern bool dx_get_symbols(const char* filepath, char*** symbols, int* p_count);
extern int dx_dlclose(void *handle);

#endif // dx_dlfcn_h