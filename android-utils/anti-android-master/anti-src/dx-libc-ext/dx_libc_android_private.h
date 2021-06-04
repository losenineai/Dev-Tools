//
// Created by boyliang on 2018/5/19.
//

#ifndef DXSTUB_ANDROID_PRIVATE_H
#define DXSTUB_ANDROID_PRIVATE_H

#include <elf.h>
#include <unistd.h>

#if defined(__arm__) || defined(__i386__)

#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym Elf32_Sym
#define Elf_Rel Elf32_Rel
#define Elf_Dyn Elf32_Dyn
#define Elf_Addr Elf32_Addr
#define Elf_Word Elf32_Word
#define Elf_Rela Elf32_Rela

#define ELF_R_TYPE ELF32_R_TYPE
#define ELF_R_SYM ELF32_R_SYM

#else

#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym Elf64_Sym
#define Elf_Rel Elf64_Rel
#define Elf_Dyn Elf64_Dyn
#define Elf_Addr Elf64_Addr
#define Elf_Word Elf64_Word
#define Elf_Rela Elf64_Rela

#define ELF_R_TYPE ELF64_R_TYPE
#define ELF_R_SYM ELF64_R_SYM

#endif

#define SOINFO_NAME_LEN 128

struct link_map
{
    uintptr_t l_addr;
    char * l_name;
    uintptr_t l_ld;
    struct link_map * l_next;
    struct link_map * l_prev;
};

struct soinfo
{
    const char name[SOINFO_NAME_LEN];
    Elf_Phdr *phdr;
    int phnum;
    unsigned entry;
//    unsigned base;
    intptr_t base;
    unsigned size;

    int unused;  // DO NOT USE, maintained for compatibility.

    unsigned *dynamic;

    unsigned wrprotect_start;
    unsigned wrprotect_end;

    soinfo *next;
    unsigned flags;

    const char *strtab;
    Elf_Sym *symtab;

    unsigned nbucket;
    unsigned nchain;
    unsigned *bucket;
    unsigned *chain;

    unsigned *plt_got;

    Elf_Rel *plt_rel;
    unsigned plt_rel_count;

    Elf_Rel *rel;
    unsigned rel_count;

#ifdef ANDROID_SH_LINKER
    Elf_Rela *plt_rela;
    unsigned plt_rela_count;

    Elf_Rela *rela;
    unsigned rela_count;
#endif /* ANDROID_SH_LINKER */

    unsigned *preinit_array;
    unsigned preinit_array_count;

    unsigned *init_array;
    unsigned init_array_count;
    unsigned *fini_array;
    unsigned fini_array_count;

    void (*init_func)(void);
    void (*fini_func)(void);

#ifdef ANDROID_ARM_LINKER
    /* ARM EABI section used for stack unwinding. */
    unsigned *ARM_exidx;
    unsigned ARM_exidx_count;
#endif

    unsigned refcount;
    struct link_map linkmap;
};

#endif //DXSTUB_ANDROID_PRIVATE_H
