/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <android/log.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include "android_private.h"
#include "elf_utils.h"
#include <dlfcn.h>
#include <string.h>
#include "dx_dlopen.h"
#include "../dx_libc.h"

void *dx_dlopen(const char *libpath, int flags) {
    void *load_addr = get_module_addr(libpath);
    if (!load_addr) {
        return 0;
    }
    MyHandle *ctx = (MyHandle *) calloc(1, sizeof(MyHandle));
    if(ctx == NULL){
        return NULL;
    }
    ctx->load_addr = (size_t)load_addr;
    Elf_Ehdr *elf = 0;
    elf = (Elf_Ehdr *)load_addr;
    init_handle(elf, ctx);
    return ctx;
}

void *dx_dlsym(void *handle, const char *name) {
    if (handle != NULL) {
        MyHandle *ctx = (MyHandle*) handle;
        Elf_Sym* sym = nullptr;
        
        if(find_symbol_by_name(name, ctx, &sym) && sym != nullptr){
            void* ret = (char*)ctx->load_addr + sym->st_value - ctx->loadBias;
            return ret;
        }
    }
    return nullptr;
}

void *dx_dlsym(void *handle, const char *name, bool isSubstr) {
    if (handle != NULL) {
        MyHandle *ctx = (MyHandle*) handle;
        Elf_Sym *sym = (Elf_Sym *) ctx->dynsym;
        char *strings = (char *) ctx->dynstr;

        for(int k = 0; k < ctx->nsym; k++, sym++) {

            const char *symName = strings + sym->st_name;

            if((size_t)symName < (size_t)ctx->dynstr || (size_t)symName > (size_t)ctx->dynstr_limit) {
                return 0;
            }
            
            if(isSubstr){
                if (dx_strstr(symName, name) != NULL) {
                void *ret = (char*)ctx->load_addr + sym->st_value - ctx->loadBias;
//                __android_log_print(ANDROID_LOG_INFO, "dx_dlsym", "%s found at %p", name, ret);
                return ret;
            }
            }else{
                if (dx_strcmp(symName, name) == 0) {
                void *ret = (char*)ctx->load_addr + sym->st_value - ctx->loadBias;
//                __android_log_print(ANDROID_LOG_INFO, "dx_dlsym", "%s found at %p", name, ret);
                return ret;
                }
            }
            
        }
    }
    return 0;
}

bool dx_get_symbols(const char* filepath, char*** symbols, int* p_count) {
    void* handle = NULL;
    int length = 0;
    MyHandle *ctx;
    Elf_Sym *sym;
    char *strings = NULL;
    int count = 0;
    char** ppsyms = NULL;

    // LOGE("%s(%d) 1", __FUNCTION__, __LINE__);
    if (dlopen(filepath, RTLD_NOW) != NULL) {
        handle = dx_dlopen(filepath, RTLD_NOW);
    }
    // LOGE("%s(%d) 2", __FUNCTION__, __LINE__);

    if (handle == NULL) {
        // LOGE("%s(%d) false-1", __FUNCTION__, __LINE__);
        return false;
    }

    // LOGE("%s(%d) 3", __FUNCTION__, __LINE__);
    ctx = (MyHandle*) handle;
    sym = (Elf_Sym *) ctx->dynsym;
    strings = (char *) ctx->dynstr;

    for(int k = 0; k < ctx->nsym; k++, sym++) {
        const char *symName = strings + sym->st_name;
        length = dx_strlen(symName);
        if (length > 0) {
            // LOGE("%s(%d) 6, [%.3d]: %s", __FUNCTION__, __LINE__, k, symName);
            count++;
        }
    }

    sym = (Elf_Sym *) ctx->dynsym;
    
    // LOGE("%s(%d) 4", __FUNCTION__, __LINE__);
    if (count <= 0) {
        // LOGE("%s(%d) false-2", __FUNCTION__, __LINE__);
        return false;
    }

    ppsyms = (char** )malloc(sizeof(char*) * count);
    // LOGE("%s(%d) 6, count: %d", __FUNCTION__, __LINE__, count);
    // LOGE("%s(%d) 6, ctx->nsym: %d", __FUNCTION__, __LINE__, ctx->nsym);
    count = 0;
    if (ppsyms != NULL) {
        for(int k = 0; k < ctx->nsym; k++, sym++) {
            const char *symName = strings + sym->st_name;
            length = dx_strlen(symName);
            if (length > 0) {
                // LOGE("%s(%d) 6, length: %d", __FUNCTION__, __LINE__, length);
                // LOGE("%s(%d) 6, symName: %s", __FUNCTION__, __LINE__, symName);
                char* str = (char* )malloc(length + 1);
                if (str != NULL) {
                    memcpy(str, symName, length + 1);
                    ppsyms[count++] = str;
                }
            }
        }
    }
    // LOGE("%s(%d) 7", __FUNCTION__, __LINE__);

    *p_count = count;
    *symbols = ppsyms;
    return true;
}

int dx_dlclose(void *handle) {
    free(handle);
    return 0;
}

