/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#ifndef DX_DLOPEN_H
#define DX_DLOPEN_H

#include "../dx_libc_prefix.h"
#include "android_private.h"

__BEGIN_DECLS
struct MyHandle{
    size_t load_addr;
    void *dynstr;
    void *dynsym;
    void *dynstr_limit;
    size_t nsym;
    size_t loadBias;
    size_t dynstrsz;

    size_t nbucket_;
    size_t nchain_;
    uint32_t* bucket_;
    uint32_t* chain_;

    size_t gnu_nbucket_;
    uint32_t* gnu_bucket_;
    uint32_t* gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    Elf_Addr* gnu_bloom_filter_;

    bool isgnu;
};

void *dx_dlopen(const char *libpath, int flags);
void *dx_dlsym(void *handle, const char *name);
bool dx_get_symbols(const char* filepath, char*** symbols, int* p_count);
int dx_dlclose(void *handle);

__END_DECLS

#endif