//
// Created by my on 18-12-10.
//

#include <elf.h>
#include <string.h>
#include <common.h>

template <typename DX_ELF_EHDR, typename DX_ELF_PHDR, typename DX_ELF_DYN, typename DX_ELF_SHDR, typename DX_ELF_SYM, typename DX_SIZE>
void _getDynFromSo(void *load_addr, void *&dynsym, void *&dynstr, size_t &nSyms) {
    DX_ELF_EHDR *elf = 0;

    elf = (DX_ELF_EHDR *) load_addr;
    const char *mapBase = (const char*)elf;

    DX_ELF_PHDR *phdr = (DX_ELF_PHDR*)(mapBase + elf->e_phoff);

    int phNum = elf->e_phnum;
    DX_SIZE dyn_size = 0, dyn_off = 0;

    DX_SIZE bias = 0;
    for (int i = 0; i < phNum; ++i) {
        size_t p_type = phdr[i].p_type;
        if (p_type == PT_DYNAMIC) {
            //get dyn symbol table from dynamic section
            dyn_size = phdr[i].p_filesz;
            dyn_off = phdr[i].p_offset;
            bias = phdr[i].p_vaddr - phdr[i].p_offset;
            break;
        }

    }
    //因为我们要的是文件中的偏移，而不是内存中的偏移，dynamic表获取的所有地址都是linker加载到内存中的偏移，需要修正bias
    const DX_ELF_DYN* dyn = (const DX_ELF_DYN*)(mapBase+dyn_off);
    DX_SIZE n = dyn_size / sizeof(DX_ELF_DYN);
    for (int i = 0; i < n; ++i) {
        DX_SIZE type = dyn[i].d_tag;
        DX_SIZE off = dyn[i].d_un.d_ptr - bias;
        switch (type) {
            case DT_SYMTAB:
                dynsym = (char*)load_addr+off;
                break;

            case DT_STRTAB:
                dynstr = (char*)load_addr+off;
                break;
            default:
                break;
        }
    }
    DX_ELF_SHDR *shdr = (DX_ELF_SHDR*)(mapBase + elf->e_shoff);
    DX_SIZE shNum = elf->e_shnum;

    for (int i = 0; i < shNum; i++) {
        size_t type = shdr[i].sh_type;
        if (type == SHT_DYNSYM) {
            nSyms = shdr[i].sh_size / sizeof(DX_ELF_SYM);
            break;
        }
    }
}

template <typename DX_ELF_SYM, typename DX_SIZE>
void* _findDynSymAddr(void *load_addr, const char *symbol, void *dynsym, void *dynstr, DX_SIZE nsym) {

    DX_ELF_SYM *sym = (DX_ELF_SYM *) dynsym;
    char *strings = (char *) dynstr;

    for(int k = 0; k < nsym; k++, sym++) {
        //符号在符号表里面的偏移，不用考虑文件与内存加载之间bias
        DX_SIZE off = sym->st_name;
        const char *symName = strings + off;
        DX_SIZE symOff = sym->st_value;
        LOGI("ElfSym %s", symName);
        if (strcmp(symName, symbol) == 0) {
            //这种找符号的机制并不完全通用，要找的是文件中的偏移，而符号表里的地址都是内存中的偏移
            //如果要通用需要在Program header里面找到对应的Load段，将vaddr-pffset作为bias去修正
            //这里刚好oat文件的oatdata与oatlastword都在第一、二个load里面，而且bias为0,文件中地址等于内存中地址
            void *ret = (char*)(load_addr) + symOff;
            return ret;
        }
    }
    return 0;
}

size_t getElfBit(void *load_addr){
    uint8_t bit = (uint8_t )(*((char*)load_addr + 4));
    if (bit == 1){
        return 32;
    } else {
        return 64;
    }
}

void getDynFromSo(void *load_addr, void *&dynsym, void *&dynstr, size_t &nSyms) {
    size_t bit = getElfBit(load_addr);

    if (bit == 32) {
        _getDynFromSo<Elf32_Ehdr, Elf32_Phdr, Elf32_Dyn, Elf32_Shdr, Elf32_Sym, size_t>(load_addr, dynsym, dynstr, nSyms);
    } else {
        _getDynFromSo<Elf64_Ehdr, Elf64_Phdr, Elf64_Dyn, Elf64_Shdr, Elf64_Sym, uint64_t >(load_addr, dynsym, dynstr, nSyms);
    }
}

void* findDynSymAddr(void *load_addr, const char *symbol, void *dynsym, void *dynstr, size_t nsym) {
    size_t bit = getElfBit(load_addr);

    void* result = 0;

    if (bit == 32) {
        result = _findDynSymAddr<Elf32_Sym, size_t >(load_addr, symbol, dynsym, dynstr, nsym);
    } else {
        result = _findDynSymAddr<Elf64_Sym, uint64_t >(load_addr, symbol, dynsym, dynstr, nsym);
    }

    return result;
}
