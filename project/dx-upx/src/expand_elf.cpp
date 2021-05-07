#include "expand_elf.h"
#include "file.h"
#include "elf_utils.h"
#include "log.h"

#include <fcntl.h>
#include <unistd.h>

expand_elf::expand_elf(const std::string& input_filename, const std::string& output_filename):
            elf(input_filename),
            i_filename(input_filename),
            o_filename(output_filename)
{
    xct_vaddr = ~0u;
    xct_offset = 0;
    int mode = this->elf.get_mode();
    unsigned char* buffer = elf.get_buffer();

    if(ARCH_BIT_32 == mode){
        int shnum = 0;
        Elf32_Shdr* shdr = (Elf32_Shdr*)elf.get_shdrs(buffer, shnum);
        for(int i=0; i<shnum; i++, shdr++){
            unsigned sh_addr = shdr->sh_addr;
            if(shdr->sh_flags & SHF_EXECINSTR){
                xct_vaddr = xct_vaddr < sh_addr ? xct_vaddr:sh_addr;
            }
        }
        int phnum = 0;
        Elf32_Phdr* phdr = (Elf32_Phdr*)elf.get_phdr(buffer, phnum);
        xct_offset = elf32_get_offset_from_address(phdr, phnum, xct_vaddr);
    }else if(ARCH_BIT_64 == mode){
        int shnum = 0;
        Elf64_Shdr* shdr = (Elf64_Shdr*)elf.get_shdrs(buffer, shnum);
        for(int i=0; i<shnum; i++, shdr++){
            unsigned long sh_addr = shdr->sh_addr;
            if(shdr->sh_flags & SHF_EXECINSTR){
                xct_vaddr = xct_vaddr < sh_addr ? xct_vaddr:sh_addr;
            }
        }
        int phnum = 0;
        Elf64_Phdr* phdr = (Elf64_Phdr*)elf.get_phdr(buffer, phnum);
        xct_offset = elf64_get_offset_from_address(phdr, phnum, xct_vaddr);
    }else{
        xct_vaddr = 0;
    }

    
};

bool expand_elf::expand(){
    long expand_size = 0x1000;
    expand(expand_size);
}

bool expand_elf::expand(long expand_size){
    // long expand_size = 0x1000;
    // elf_utils elf(this->i_filename);
    std::string& bak_filename = this->o_filename;
    // size_t index = filename.find_last_of('/');
    // size_t length = filename.length();
    // std::string bak_filename = filename.substr(index+1, length-index-1)+".dx";
    //relocate pt_dynamic
    int mode = elf.get_mode();
    unsigned char* buffer = elf.get_buffer();
    unsigned long filesize = elf.get_file_size();
    
    if(xct_vaddr<=0 || xct_vaddr > filesize){
        return false;
    }

    if(ARCH_BIT_32 == mode){
        //relocate PT_DYNAMIC
        int phnum = 0;
        Elf32_Phdr* phdr = (Elf32_Phdr*)elf.get_phdr(buffer, phnum);
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        Elf32_Dyn* dyn = NULL;
        for(int i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dyn = (Elf32_Dyn*)(buffer + phdr[i].p_offset);
            }
        }

        for(; dyn->d_tag; ++dyn){
            unsigned d_tag = dyn->d_tag;
            if(DT_FINI == d_tag 
            || DT_FINI_ARRAY == d_tag
            || DT_INIT_ARRAY == d_tag
            || DT_PREINIT_ARRAY == d_tag
            || DT_PLTGOT == d_tag
            || DT_INIT ==d_tag ){
                dyn->d_un.d_val += expand_size; 
            }
        }
        
        //relocate dynsym
        Elf32_Shdr* sec_dynsym = (Elf32_Shdr*)elf.get_shdr_by_type(buffer, (Elf32_Word)SHT_DYNSYM);
        if(sec_dynsym != NULL){
            unsigned long off_dynsym = sec_dynsym->sh_offset;
            unsigned long sz_dynsym = sec_dynsym->sh_size;

            Elf32_Sym* sym = (Elf32_Sym*)(buffer + off_dynsym);

            for(int i=sz_dynsym/sizeof(Elf32_Sym); --i>=0; ++sym){
                unsigned symval = sym->st_value;
                unsigned symsec = sym->st_shndx;

                if(SHN_UNDEF != symsec && SHN_ABS != symsec && xct_vaddr <= symval){
                    sym->st_value = symval + expand_size;
                }
            }
        }
        

        //relocate symtab
        Elf32_Shdr* sec_symtab = (Elf32_Shdr*)elf.get_shdr_by_type(buffer, (Elf32_Word)SHT_SYMTAB);
        if(sec_symtab != NULL){
            unsigned long off_symtab = sec_symtab->sh_offset;
            unsigned long sz_symtab = sec_symtab->sh_size;

            Elf32_Sym* sym = (Elf32_Sym*)(buffer + off_symtab);

            for(int i=sz_symtab/sizeof(Elf32_Sym); --i>=0; ++sym){
                unsigned symval = sym->st_value;
                unsigned symsec = sym->st_shndx;

                if(SHN_UNDEF != symsec && SHN_ABS != symsec && xct_vaddr <= symval){
                    sym->st_value = symval + expand_size;
                }
            }
        }
        
        

        //relocate shdr, rela, rel(below xct_va)
        int shnum = 0;
        Elf32_Shdr* shdr = (Elf32_Shdr*)elf.get_shdrs(buffer, shnum);
        for(int i=0; i<shnum; i++, shdr++){
            unsigned sh_type = shdr->sh_type;
            unsigned sh_size = shdr->sh_size;
            unsigned sh_offset = shdr->sh_offset;
            unsigned sh_entsize = shdr->sh_entsize;

            if(xct_vaddr == sh_offset){
                shdr->sh_size += expand_size;
            }

            if(xct_vaddr < sh_offset){
                shdr->sh_addr += expand_size;
                shdr->sh_offset += expand_size;
            }

            if(SHT_RELA == sh_type){
                if(sizeof(Elf32_Rela) != sh_entsize){
                    return false;
                }
                unsigned long plt_off = ~0u;
                Elf32_Rela* relab = (Elf32_Rela*)(buffer + sh_offset);
                for(int j=sh_size/sh_entsize; --j>=0; ++relab){
                    unsigned r_addend = relab->r_addend;
                    unsigned r_offset = relab->r_offset;
                    unsigned r_info = relab->r_info;
                    unsigned r_type = ELF32_R_TYPE(r_info);
                    if(xct_vaddr <= r_offset){
                        relab->r_offset = r_offset + expand_size;
                    }
                    if(R_ARM_RELATIVE == r_type || R_386_RELATIVE == r_type){
                        relab->r_addend = r_addend + expand_size;
                    }
                    if(R_ARM_JUMP_SLOT == r_type || R_386_JMP_SLOT == r_type){
                        if(plt_off > r_offset){
                            plt_off = r_offset;
                        }

                        unsigned d = elf32_get_offset_from_address(phdr, phnum, r_offset);
                        unsigned w = get_uint32(buffer+d);

                        if(xct_vaddr <= w){
                            *(uint32_t*)(buffer+d) = w + expand_size;
                        }
                    }
                }

            }

            if(SHT_REL == sh_type){
                if(sizeof(Elf32_Rel) != sh_entsize){
                    return false;
                }
                unsigned long plt_off = ~0u;
                Elf32_Rel* rel0 = (Elf32_Rel*)(buffer + sh_offset);
                for(int j=sh_size/sh_entsize; --j>=0; ++rel0){
                    unsigned r_offset = rel0->r_offset;
                    unsigned r_info = rel0->r_info;
                    unsigned r_type = ELF32_R_TYPE(r_info);
                    unsigned d = elf32_get_offset_from_address(phdr, phnum, r_offset);
                    unsigned w = get_uint32(buffer+d);
                    
                    if(xct_vaddr <= r_offset){
                        rel0->r_offset = r_offset + expand_size;
                    }
                    if(R_ARM_RELATIVE == r_type || R_386_RELATIVE == r_type){
                        if(xct_vaddr <= w){
                            set_uint32(buffer+d, w+expand_size);
                        }
                    }
                    if (R_ARM_JUMP_SLOT == r_type || R_386_JMP_SLOT == r_type) {
                        if (plt_off > r_offset) {
                            plt_off = r_offset;
                        }
                        if (xct_vaddr <= w) {
                            set_uint32(buffer+d, w+expand_size);
                        }
                    }
                } 
            }
        }
        
        //relocate phdr virtual address and size, physical offsets and sizes
        for(int i=0; i<phnum; i++){
            unsigned offset = phdr[i].p_offset;
            unsigned size = phdr[i].p_filesz;

            if(xct_vaddr <= offset){
                phdr[i].p_offset += expand_size;
                phdr[i].p_vaddr += expand_size;
                phdr[i].p_paddr += expand_size;
            }else{
                if(offset+size > xct_vaddr){
                    phdr[i].p_filesz += expand_size;
                    phdr[i].p_memsz += expand_size;
                }
            }
        }

        //relocate e_hdr
        ehdr->e_entry += expand_size;
        ehdr->e_shoff += expand_size;
    }else if( ARCH_BIT_64 == mode ){
        //relocate PT_DYNAMIC
        int phnum = 0;
        Elf64_Phdr* phdr = (Elf64_Phdr*)elf.get_phdr(buffer, phnum);
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Dyn* dyn = NULL;
        for(int i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dyn = (Elf64_Dyn*)(buffer + phdr[i].p_offset);
            }
        }

        for(; dyn->d_tag; ++dyn){
            unsigned d_tag = dyn->d_tag;
            if(DT_FINI == d_tag 
            || DT_FINI_ARRAY == d_tag
            || DT_INIT_ARRAY == d_tag
            || DT_PREINIT_ARRAY == d_tag
            || DT_PLTGOT == d_tag
            || DT_INIT == d_tag){
                dyn->d_un.d_val += expand_size; 
            }
        }
        
        //relocate dynsym
        Elf64_Shdr* sec_dynsym = (Elf64_Shdr*)elf.get_shdr_by_type(buffer, SHT_DYNSYM);
        if(sec_dynsym != NULL){
            unsigned long off_dynsym = sec_dynsym->sh_offset;
            unsigned long sz_dynsym = sec_dynsym->sh_size;

            Elf64_Sym* sym = (Elf64_Sym*)(buffer + off_dynsym);

            for(int i=sz_dynsym/sizeof(Elf64_Sym); --i>=0; ++sym){
                unsigned symval = sym->st_value;
                unsigned symsec = sym->st_shndx;

                if(SHN_UNDEF != symsec && SHN_ABS != symsec && xct_vaddr <= symval){
                    sym->st_value = symval + expand_size;
                }
            }
        }
        

        //relocate symtab
        Elf64_Shdr* sec_symtab = (Elf64_Shdr*)elf.get_shdr_by_type(buffer, SHT_SYMTAB);
        if(sec_symtab != NULL){
            unsigned long off_symtab = sec_symtab->sh_offset;
            unsigned long sz_symtab = sec_symtab->sh_size;

            Elf64_Sym* sym = (Elf64_Sym*)(buffer + off_symtab);

            for(int i=sz_symtab/sizeof(Elf64_Sym); --i>=0; ++sym){
                unsigned symval = sym->st_value;
                unsigned symsec = sym->st_shndx;

                if(SHN_UNDEF != symsec && SHN_ABS != symsec && xct_vaddr <= symval){
                    sym->st_value = symval + expand_size;
                }
            }
        }
        
        

        //relocate shdr, rela, rel(below xct_vaddr)
        int shnum = 0;
        Elf64_Shdr* shdr = (Elf64_Shdr*)elf.get_shdrs(buffer, shnum);
        for(int i=0; i<shnum; i++, shdr++){
            unsigned sh_type = shdr->sh_type;
            unsigned sh_size = shdr->sh_size;
            unsigned sh_offset = shdr->sh_offset;
            unsigned sh_entsize = shdr->sh_entsize;
            
            if(xct_vaddr == sh_offset){
                shdr->sh_size += expand_size;
            }

            if(xct_vaddr < sh_offset){
                shdr->sh_addr += expand_size;
                shdr->sh_offset += expand_size;
            }

            if(SHT_RELA == sh_type){
                if(sizeof(Elf64_Rela) != sh_entsize){
                    return false;
                }
                unsigned long plt_off = ~0u;
                Elf64_Rela* relab = (Elf64_Rela*)(buffer + sh_offset);
                for(int j=sh_size/sh_entsize; --j>=0; ++relab){
                    unsigned long r_addend = relab->r_addend;
                    unsigned long r_offset = relab->r_offset;
                    unsigned long r_info = relab->r_info;
                    unsigned r_type = ELF64_R_TYPE(r_info);
                    if(xct_vaddr <= r_offset){
                        relab->r_offset = r_offset + expand_size;
                    }
                    if(R_AARCH64_RELATIVE == r_type){
                        relab->r_addend = r_addend + expand_size;
                    }
                    if(R_AARCH64_JUMP_SLOT == r_type){
                        if(plt_off > r_offset){
                            plt_off = r_offset;
                        }

                        unsigned long d = elf64_get_offset_from_address(phdr, phnum, r_offset);
                        unsigned long w = get_uint64(buffer+d);

                        if(xct_vaddr <= w){
                            *(uint64_t*)(buffer+d) = w + expand_size;
                        }
                    }
                }

            }

            if(SHT_REL == sh_type){
                if(sizeof(Elf64_Rel) != sh_entsize){
                    return false;
                }
                unsigned long plt_off = ~0u;
                Elf64_Rel* rel0 = (Elf64_Rel*)(buffer + sh_offset);
                for(int j=sh_size/sh_entsize; --j>=0; ++rel0){
                    unsigned long r_offset = rel0->r_offset;
                    unsigned long r_info = rel0->r_info;
                    unsigned long r_type = ELF64_R_TYPE(r_info);
                    unsigned d = elf64_get_offset_from_address(phdr, phnum, r_offset);
                    unsigned w = get_uint64(buffer+d);
                    
                    if(xct_vaddr <= r_offset){
                        rel0->r_offset = r_offset + expand_size;
                    }
                    if(R_AARCH64_RELATIVE == r_type){
                        if(xct_vaddr <= w){
                            set_uint64(buffer+d, w+expand_size);
                        }
                    }
                    if (R_ARM_JUMP_SLOT == r_type) {
                        if (plt_off > r_offset) {
                            plt_off = r_offset;
                        }
                        if (xct_vaddr <= w) {
                            set_uint64(buffer+d, w+expand_size);
                        }
                    }
                } 
            }
        }
        
        //relocate phdr virtual address and size, physical offsets and sizes
        for(int i=0; i<phnum; i++){
            unsigned offset = phdr[i].p_offset;
            unsigned size = phdr[i].p_filesz;

            if(xct_vaddr <= offset){
                phdr[i].p_offset += expand_size;
                phdr[i].p_vaddr += expand_size;
                phdr[i].p_paddr += expand_size;
            }else{
                if(offset+size > xct_vaddr){
                    phdr[i].p_filesz += expand_size;
                    phdr[i].p_memsz += expand_size;
                }
            }
        }

        //relocate e_hdr
        ehdr->e_entry += expand_size;
        ehdr->e_shoff += expand_size;
    }else{
        return false;
    }

    unsigned char* bak_buffer = (unsigned char*)malloc(filesize + expand_size);
    memset(bak_buffer, 0, filesize + expand_size);
    for(unsigned long i=0; i<xct_vaddr; i++){
        bak_buffer[i] = buffer[i];
    }
    for(unsigned long i=xct_vaddr; i<filesize; i++){
        bak_buffer[expand_size + i] = buffer[i];
    }

    // int fd = open(bak_filename.c_str(), O_CREAT | O_RDWR);
    #if (defined(__MINGW32__) || defined(__MINGW64__))
        int fd = open(bak_filename.c_str(), O_RDWR | O_BINARY, 0666);
    #else
        int fd = open(bak_filename.c_str(), O_RDWR, 0666);
    #endif
    // LOGI("output filename : %s", bak_filename.c_str());
    if(fd <=0 ){
        LOGI("open:%s error", bak_filename.c_str());
        return false;
    }
    if(write(fd, bak_buffer, filesize+expand_size) != (ssize_t)(filesize+expand_size)){
        close(fd);
        return false;
    }

    // File::chmod(bak_filename.c_str(), 0777);
    close(fd);
    return true;
}

bool expand_elf::remove_so(){
    std::string dxfname = this->o_filename;
    if(access(dxfname.c_str(), F_OK)){
        return false;
    }

    File::unlink(dxfname.c_str());
    return true;
}

bool expand_elf::replace_so(std::string dst_filename){
    if(access(dst_filename.c_str(), F_OK) || access(this->o_filename.c_str(), F_OK)){
        return false;
    }
    File::unlink(dst_filename.c_str());
    File::rename(this->o_filename.c_str(), dst_filename.c_str());
    
    return true;
}


unsigned expand_elf::elf32_get_offset_from_address(Elf32_Phdr* phdr, int phnum, unsigned addr) {
    Elf32_Phdr* l_phdr = phdr;
    int j = phnum;
    for (; --j>=0; ++l_phdr) if (PT_LOAD == l_phdr->p_type) {
        unsigned const t = addr - l_phdr->p_vaddr;
        if (t < l_phdr->p_filesz) {
            return (unsigned)(t + l_phdr->p_offset);
        }
    }
    
    return 0;
}

unsigned long expand_elf::elf64_get_offset_from_address(Elf64_Phdr* phdr, int phnum, unsigned addr) {
    Elf64_Phdr* l_phdr = phdr;
    int j = phnum;
    for (; --j>=0; ++l_phdr) if (PT_LOAD == l_phdr->p_type) {
        unsigned const t = addr - l_phdr->p_vaddr;
        if (t < l_phdr->p_filesz) {
            return (unsigned long)(t + l_phdr->p_offset);
        }
    }
    return 0;
}


void expand_elf::set_uint32(uint8_t* src, uint32_t dst){
    *(uint32_t*)src = dst;
}

uint32_t expand_elf::get_uint32(uint8_t* src){
    return *(uint32_t*)src;
}

void expand_elf::set_uint64(uint8_t* src, uint64_t dst){
    *(uint64_t*)src = dst;
}

uint64_t expand_elf::get_uint64(uint8_t* src){
    return *(uint64_t*)src;
}

unsigned long expand_elf::get_xct_vaddr(){
    return xct_vaddr;
}

unsigned long expand_elf::get_xct_offset(){
    return xct_offset;
}