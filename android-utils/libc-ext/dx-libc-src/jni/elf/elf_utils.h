//
// Created by my on 18-6-5.
//

#ifndef DXSTUB_ELFUTILS_H
#define DXSTUB_ELFUTILS_H

#include "../dx_libc_prefix.h"
#include "android_private.h"
#include "dx_dlopen.h"

__BEGIN_DECLS

bool init_handle(Elf_Ehdr *elf, MyHandle* soinfo);

void *get_module_addr(const char *libpath);

void get_sym_in_dynamic(Elf_Ehdr *elf, size_t &dynsym, size_t &dynstr, size_t &dynstrsz, size_t &dynsym_fake_end, size_t &loadBias);

bool find_symbol_by_name(const char* symbol_name, MyHandle* soinfo, Elf_Sym** symbol);

__END_DECLS


#endif //DXSTUB_ELFUTILS_H
