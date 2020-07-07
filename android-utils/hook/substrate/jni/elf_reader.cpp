#include "elf_reader.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include "dx_hook.h"
#include <sys/mman.h>

#ifdef __aarch64__
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Addr Elf_Addr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Dyn Elf_Dyn;
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Rela Elf_Rel;
typedef Elf64_Shdr Elf_Shdr;
#else
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Addr Elf_Addr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Dyn Elf_Dyn;
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Rel Elf_Rel;
typedef Elf32_Shdr Elf_Shdr;
#endif

#define ELF_INVALID_ADDR (size_t )(-1)

#define ELF_FREE_IF(__p__) \
    if (NULL != __p__) { \
        free(__p__); \
        __p__ = NULL; \
    }

struct DynSym
{
    uintptr_t rva;
    uintptr_t size;
};

class ElfReaderImpl : public IElfReader
{
private:
    void *elf_strtab_data_p_;

    size_t dynsyms_count_;
    DynSym *dynsyms_;

    bool valid_;

private:
    // need to free memory
    void* load_file(const char *filename) {
        void* buf = NULL;
        FILE* file = fopen(filename, "rb");
        if (NULL == file) {
            return NULL;
        }

        fseek(file, 0, SEEK_END);
        size_t filesize = ftell(file);
        if (filesize > 0) {
            buf = malloc(filesize);
            if (NULL != buf) {
                fseek(file, 0, SEEK_SET);
                if (filesize != fread(buf, 1, filesize, file)) {
                    ELF_FREE_IF(buf);
                }
            }
        }

        fclose(file);
        return buf;
    }

    void* find_shstrtab_p(Elf_Ehdr *elf_hdr) {
        Elf_Shdr *shdr = (Elf_Shdr* )((uintptr_t )elf_hdr + elf_hdr->e_shoff);
        shdr += elf_hdr->e_shstrndx;
        return (void* )((uintptr_t )elf_hdr + shdr->sh_offset);
    }

    Elf_Shdr* find_shdr_p(Elf_Ehdr *elf_hdr, const char *sh_name) {
        if (sh_name == NULL || sh_name[0] == '\0') {
            return NULL;
        }

        Elf_Shdr *shdr = (Elf_Shdr* )((uintptr_t )elf_hdr + elf_hdr->e_shoff);
        for (int i = 0; i < elf_hdr->e_shnum; i++, shdr++) {
            uintptr_t name_off = shdr->sh_name;
            if (0 == strcmp(sh_name, (char* )((uintptr_t )elf_strtab_data_p_ + name_off))) {
                return shdr;
            }
        }
        return NULL;
    }

    size_t get_sec_data_size(Elf_Ehdr *elf_hdr, const char *sh_name) {
        Elf_Shdr *shdr = this->find_shdr_p(elf_hdr, sh_name);
        return shdr->sh_size;
    }

    void* find_sec_data_p(Elf_Ehdr *elf_hdr, const char *sh_name) {
        void *data = NULL;
        Elf_Shdr *shdr = this->find_shdr_p(elf_hdr, sh_name);
        if (NULL != shdr) {
            data = (void* )((uintptr_t )elf_hdr + shdr->sh_offset);
        }
        return data;
    }

    DynSym* get_dynsyms(Elf_Ehdr *elf_hdr, size_t &dynsyms_count) {
        Elf_Sym *elf_sym = NULL;
        DynSym *dynsyms = NULL;

        dynsyms_count = this->get_sec_data_size(elf_hdr, ".dynsym")/sizeof(Elf_Sym);
        if (dynsyms_count <= 0) {
            return NULL;
        }

        elf_sym = (Elf_Sym* )this->find_sec_data_p(elf_hdr, ".dynsym");
        if (NULL == elf_sym) {
            return NULL;
        }

        dynsyms = (DynSym* )malloc(sizeof(DynSym) * (dynsyms_count + 2));
        if (NULL == dynsyms) {
            return NULL;
        }

        DynSym *p = dynsyms;
        uintptr_t first_val = (uintptr_t )(-1);
        uintptr_t first_size = 0;
        uintptr_t last_val = 0;
        uintptr_t last_size = 0;
        for (int i = 0; i < dynsyms_count; ++i, ++p, ++elf_sym) {
            p->rva = elf_sym->st_value;
            p->size = elf_sym->st_size;
            if (p->rva > last_val) {
                last_val = p->rva;
                last_size = p->size;
            }
            if (p->rva > 0 && p->rva < first_val) {
                first_val = p->rva;
                first_size = p->size;
            }
        }
        p->rva = first_val;
        p->size = first_size;

        p++;
        p->rva = last_val;
        p->size = last_size;
        return dynsyms;
    }

public:
    ElfReaderImpl(const char *filename) {
        valid_ = false;
        Elf_Ehdr *elf_hdr = (Elf_Ehdr* )this->load_file(filename);
        if (NULL != elf_hdr) {
            elf_strtab_data_p_ = this->find_shstrtab_p(elf_hdr);

            dynsyms_ = this->get_dynsyms(elf_hdr, dynsyms_count_);
            if (NULL != dynsyms_ && dynsyms_count_ > 0) {
                valid_ = true;
            }
            ELF_FREE_IF(elf_hdr);
        }
    }

    ~ElfReaderImpl() {
        ELF_FREE_IF(dynsyms_);
    }

    virtual bool valid() {
        return valid_;
    }

    virtual uintptr_t get_symbol_size(uintptr_t rva) {
        DynSym *p = dynsyms_;
        DynSym *first = dynsyms_ + dynsyms_count_;
        DynSym *last = first + 1;

        if (!this->valid()
            || rva == 0 || rva == ELF_INVALID_ADDR
            || rva < first->rva || rva > last->rva) {
            return 0;
        }

        for (int i = 0; i < dynsyms_count_; ++i, ++p) {
            if (p->rva == rva) {
                return p->size;
            }
        }
        return 0;
    }
};

IElfReader *IElfReader_new(const char *filepath) {
    return new ElfReaderImpl(filepath);
}

void IElfReader_delete(IElfReader *&p) {
    if (NULL != p) {
        // ElfReaderImpl *imp = dynamic_cast<ElfReaderImpl* >(p);
        ElfReaderImpl *imp = (ElfReaderImpl* )p;
        delete imp;
        p = NULL;
    }
}
