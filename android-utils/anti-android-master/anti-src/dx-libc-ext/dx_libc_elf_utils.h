//
// Created by my on 18-6-5.
//

#ifndef DXSTUB_ELFUTILS_H
#define DXSTUB_ELFUTILS_H

#include "dx_libc_config.h"

void *get_module_addr(const char *libpath);

void get_sym_in_dynamic(Elf_Ehdr *elf, size_t &dynsym, size_t &dynstr, size_t &dynstrsz, size_t &dynsym_fake_end, size_t &loadBias);

#endif //DXSTUB_ELFUTILS_H
