#include <jni.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <android/log.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dlfcn.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
void* getTargetSI(const char *soname);
void replaceFunc(void *handle, const char *name, void *pNewFun, void **pOldFun);
void* get_library_address(const char*  libname);
void* get_dex_in_mem(const char* dexName, const char* pkgName, uint32_t &length);
uint32_t findDex(void* start, void* end, void *&addr);
#ifdef __cplusplus
}
#endif