#include <elf.h>

#ifdef M_ARM64

#define Elf_Phdr Elf64_Phdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Dyn Elf64_Dyn
#define Elf_Word Elf64_Word
#define Elf_Rela Elf64_Rela
#define Elf_Sym Elf64_Sym
#define Elf_Addr Elf64_Addr
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Xword Elf64_Xword
#define ELF_R_SYM ELF64_R_SYM
#define Elf_Off Elf64_Off
#define uint_t uint64_t

#else

#define Elf_Phdr Elf32_Phdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Dyn Elf32_Dyn
#define Elf_Word Elf32_Word
#define Elf_Rela Elf32_Rela
#define Elf_Sym Elf32_Sym
#define Elf_Addr Elf32_Addr
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Xword Elf32_Word
#define ELF_R_SYM ELF32_R_SYM
#define Elf_Off Elf32_Off
#define uint_t uint32_t

# endif