#include "elf_utils.h"
#include "log.h"

#include <private.h>
#include<stdio.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

elf_utils::elf_utils(const std::string &name) : filename(name) {
    this->elf = NULL;
    this->arch = ARCH_NONE;
    this->type = PLATFORM_NONE;
    this->mode = ARCH_BIT_NONE;
    this->file_buffer = NULL;
#if (defined(__MINGW32__) || defined(__MINGW64__))
    this->fd = open(name.c_str(), O_RDWR | O_BINARY, 0666);
#else
    this->fd = open(name.c_str(), O_RDWR, 0666);
#endif

    if (this->fd > 0) {
        if (elf_version(EV_CURRENT) == EV_NONE) {
            LOGE("elf library initialization failed.");
        } else {
            this->elf = elf_begin(fd, ELF_C_RDWR, NULL);
            if(this->elf != NULL){
                if (elf_kind (this->elf) != ELF_K_ELF) {
                    elf_end(this->elf);
                    this->elf = NULL;
                } else {
                    if(!init()){
                        LOGE("elfutil init error.");
                        this->elf = NULL;
                    }
                }
            } else {
                LOGE("elf is NULL.");
            }
        }
    }
}

elf_utils::~elf_utils() {
    if (this->fd > 0) {
        close(this->fd);
        this->fd = 0;
    }
    if (this->elf != NULL) {
        if (elf_end (this->elf) != 0)
            LOGE("free elf failed.");
        else
            this->elf = NULL;
    }
}

bool elf_utils::init() {
    if (this->elf == NULL) {
        LOGE("elf file has been not parsed.");
        return false;
    }
    this->page_size = 0x1000;
    GElf_Ehdr gehdr;
    gelf_getehdr(this->elf, &gehdr);
    if (gehdr.e_machine == EM_386) {
        this->arch = ARCH_X86;
        this->type = PLATFORM_X86;
    } else if(gehdr.e_machine == EM_ARM) {
        this->arch = ARCH_ARM;
        this->type = PLATFORM_ARM;
    } else if(gehdr.e_machine == EM_AARCH64 ) {  //EM_AARCH64
        this->arch = ARCH_ARM64;
        this->type = PLATFORM_ARM64;
    } else if (gehdr.e_machine == EM_X86_64) {
        this->arch = ARCH_X86_64;
        this->type = PLATFORM_X86_64;
    } else {
        return false;
    }

    if(this->elf->e_class == ELFCLASS32) {
        this->mode = ARCH_BIT_32;
    }else if(this->elf->e_class == ELFCLASS64) {
        this->mode = ARCH_BIT_64;
    }else{
        return false;
    }
    this->file_buffer = (uint8_t*)this->elf->e_data;

    store_all_section_headers();

    return true;
}

size_t elf_utils::add_section_name(const std::string &name) {
    Elf* e = this->elf;

    if (e == NULL) {
        LOGE("elf has not been parsed.");
        return 0;
    }

    //update shstr
    size_t newsize  = 0;
    size_t shstrndx = 0;

    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        LOGE("elf_getshstrndx error");
        return 0;
    }

    Elf_Scn* scn = elf_getscn(e, shstrndx);
    Elf_Data* data = elf_getdata(scn, NULL);

    size_t origsize = data->d_size;
    newsize = origsize + (name.length() + sizeof(char));

    if (this->mode == ARCH_BIT_32) {
        Elf32_Shdr *shdr = elf32_getshdr(scn);
        shdr->sh_size = newsize;

    } else if(this->mode == ARCH_BIT_64) {
        Elf64_Shdr *shdr = elf64_getshdr(scn);
        shdr->sh_size = newsize;
    }

    char* namebuf = new char[newsize]();
    memcpy(namebuf, data->d_buf, data->d_size - 1);
    memcpy(namebuf + data->d_size, name.c_str(), name.length() + sizeof(char));
    data->d_size    = newsize;
    data->d_buf     = namebuf; //!MUST be release!

    return origsize;
}

bool elf_utils::add_section(const std::string &name, uint8_t* content, size_t cont_size, void* pshdr, bool loadable){
    bool added = false;

    if (this->elf == NULL) {
        LOGE("elf has not been parsed.");
        return false;
    }

    size_t nameoff = this->add_section_name(name);
    if (nameoff == 0) {
        LOGE("add new section name failed.");
        return false;
    }

    //add new section
    Elf_Scn* scn = elf_newscn(this->elf);
    if (scn == NULL) {
        LOGE("new scn error");
        return false;
    }

    Elf_Data* data = elf_newdata(scn);
    if (data == NULL) {
        LOGE("new data error");
        return false;
    }
    //data->d_align = 0x1000;
    //data->d_off = 0xFFFFFFF;
    data->d_buf = content;
    data->d_type = ELF_T_WORD;
    data->d_size = cont_size;
    data->d_version =  EV_CURRENT;
    if(this->mode == ARCH_BIT_32) {
        Elf32_Shdr* shdr = elf32_getshdr(scn);//(Elf32_Shdr*)shdr;
        Elf32_Shdr* pshdr32 = (Elf32_Shdr*)pshdr;
        if (shdr == NULL) {
            LOGE("get shdr error %d", (int)cont_size);
        } else {
            shdr->sh_name = nameoff;
            shdr->sh_type = pshdr32->sh_type;// SHT_PROGBITS;
            shdr->sh_flags = pshdr32->sh_flags;// SHF_ALLOC | SHF_EXECINSTR;
            shdr->sh_entsize = pshdr32->sh_entsize;// 0;
            shdr->sh_link = pshdr32->sh_link;// 0;
            shdr->sh_size = pshdr32->sh_size;// cont_size;
            shdr->sh_addralign = pshdr32->sh_addralign;//0x1000;
            shdr->sh_addr = pshdr32->sh_addr;//virtual_addr;
            data->d_align = pshdr32->sh_addralign;
            added = true;
        }
    } else if(this->mode == ARCH_BIT_64) {
        Elf64_Shdr *shdr = elf64_getshdr(scn);
        Elf64_Shdr* pshdr64 = (Elf64_Shdr*)pshdr;
        if (shdr == NULL) {
            LOGE("get shdr error %d", (int)cont_size);
        } else {
            shdr->sh_name = nameoff;
            shdr->sh_type = pshdr64->sh_type;//SHT_PROGBITS;
            shdr->sh_flags = pshdr64->sh_flags;//SHF_ALLOC | SHF_EXECINSTR;
            shdr->sh_entsize = pshdr64->sh_entsize;//0;
            shdr->sh_link = pshdr64->sh_link;//0;
            shdr->sh_size = pshdr64->sh_size;//cont_size;
            shdr->sh_addralign = pshdr64->sh_addralign;//0x1000;
            shdr->sh_addr = pshdr64->sh_addr;//virtual_addr;
            data->d_align = pshdr64->sh_addralign;
            added = true;
        }
    }

    if (added && loadable)
        this->added_sections.push_back(name);

    return added;
}

bool elf_utils::add_section(const std::string &name, uint8_t* content, size_t cont_size, size_t virtual_address, bool loadable) {
    return this->add_section(name, content, cont_size, virtual_address, this->page_size, loadable);
}

bool elf_utils::add_section(const std::string &name, uint8_t* content, size_t cont_size, size_t vir_addr, int align, bool isloadable) {
    bool added = false;

    if (this->mode == ARCH_BIT_32) {
        Elf32_Shdr shdr;
        shdr.sh_addr = vir_addr;
        shdr.sh_addralign = align;
        shdr.sh_type = SHT_PROGBITS;
        shdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
        shdr.sh_entsize = 0;
        shdr.sh_link = 0;
        shdr.sh_size = cont_size;
        added = this->add_section(name, content, cont_size, &shdr, isloadable);
    } else if(this->mode == ARCH_BIT_64) {
        Elf64_Shdr shdr;
        shdr.sh_addr = vir_addr;
        shdr.sh_addralign = align;
        shdr.sh_type = SHT_PROGBITS;
        shdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
        shdr.sh_entsize = 0;
        shdr.sh_link = 0;
        shdr.sh_size = cont_size;
        added = this->add_section(name, content, cont_size, &shdr, isloadable);
    }
    return added;
}

void* elf_utils::get_section(int index) {
    if (this->elf == NULL) {
        return NULL;
    }

    size_t shnum = 0;
    if (LIBELF_FAILURE == elf_getshnum(this->elf, &shnum)) {
        return NULL;
    }
    if((size_t)index >= shnum || index < 0) {
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        return elf32_getshdr(elf_getscn(this->elf, index));
    } else if (this->mode == ARCH_BIT_64) {
        return elf64_getshdr(elf_getscn(this->elf, index));
    }
    return NULL;
}

void* elf_utils::get_section(const std::string &name) {
    Elf* e = this->elf;
    Elf_Scn* scn = NULL;
    size_t shstrndx;

    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        LOGE("in get_section getshdrstrndx error");
        return NULL;
    }

    while ((scn = elf_nextscn (e, scn)) != NULL) {
        char *cur_name = NULL;
        if (this->mode == ARCH_BIT_32) {
            Elf32_Shdr* shdr = elf32_getshdr(scn);
            cur_name = elf_strptr(e, shstrndx, shdr->sh_name);
            if (cur_name == NULL) {
                LOGE("in get_section elf_strptr error, mode=32.");
                return NULL;
            }
            if (strcmp(name.c_str(), cur_name) == 0) {
                //LOGD("in ...secname:%s, name:%s", name.c_str(), cur_name);
                return (void *)shdr;
            }
        } else if (this->mode == ARCH_BIT_64) {
            Elf64_Shdr* shdr = elf64_getshdr(scn);
            cur_name =elf_strptr(e, shstrndx, shdr->sh_name);
            if(cur_name == NULL){
                LOGE("in get_section elf_strptr error, mode=64.");
                return NULL;
            }
            if(strcmp(name.c_str(), cur_name) == 0) {
                //LOGD("in ...secname:%s, name:%s", name.c_str(), cur_name);
                return (void *)shdr;
            }
        }
    }

    return NULL;
}

bool elf_utils::update_phdr() {
    if (this->added_sections.size() < 1) {
        return true;
    }

    Elf* e = this->elf;
    //elf_update(e, ELF_C_NULL);
    //find dynseg && note seg

    if (this->mode == ARCH_BIT_32) {
        Elf32_Phdr* phdrs = (Elf32_Phdr*)(e->e_phdr);
        int phnum = e->e_phnum;
        int dynsegindex = 0;
        int notesegindex = 0;
        for(int i=0;i<phnum;i++){
            if(phdrs[i].p_type == PT_DYNAMIC){
                dynsegindex = i;
                notesegindex = i+1;
            }
        }
        if (dynsegindex != 3) {
            LOGD("warning.. dynamic dosen't locate in 3th segment");
        }
        if (notesegindex >= phnum) {
            LOGE("no redundant phdr error");
            return false;
        }

        //copy dynamic segment to note segment
        phdrs[notesegindex].p_align = phdrs[dynsegindex].p_align;
        phdrs[notesegindex].p_type = phdrs[dynsegindex].p_type;
        phdrs[notesegindex].p_filesz = phdrs[dynsegindex].p_filesz;
        phdrs[notesegindex].p_flags = phdrs[dynsegindex].p_flags;
        phdrs[notesegindex].p_memsz = phdrs[dynsegindex].p_memsz;
        phdrs[notesegindex].p_offset = phdrs[dynsegindex].p_offset;
        phdrs[notesegindex].p_paddr = phdrs[dynsegindex].p_paddr;
        phdrs[notesegindex].p_vaddr = phdrs[dynsegindex].p_vaddr;

        //add new pt_load segment in dynamic segment
        //get added section
        Elf32_Word segsize   = 0;
        Elf32_Off segoffset = 0xeFFFFFFF;
        Elf32_Addr segaddr   = 0xeFFFFFFF;
        Elf32_Word segalign  = 0;

        for (unsigned i=0; i<this->added_sections.size(); i++) {
            std::string secname = added_sections.at(i);
            Elf32_Shdr* shdr = (Elf32_Shdr *)get_section(secname.c_str());
            if(shdr==NULL) {
                LOGE("in update32phdr shdr is null");
                return false;
            }
            if(shdr->sh_addralign > segalign) {
                segalign = shdr->sh_addralign;
            }
            if(shdr->sh_addr < segaddr) {
                segaddr = shdr->sh_addr;
            }
            if(shdr->sh_offset < segoffset) {
                segoffset = shdr->sh_offset;
            }
            if((shdr->sh_offset+shdr->sh_size) > segsize) {
                segsize = shdr->sh_offset+shdr->sh_size;
            }
        }
        segsize -= segoffset;
        
        phdrs[dynsegindex].p_align  = segalign;
        phdrs[dynsegindex].p_type   = PT_LOAD;
        phdrs[dynsegindex].p_filesz = segsize;
        //phdrs[dynsegindex].p_flags = phdrs[dynsegindex-2].p_flags;
        phdrs[dynsegindex].p_flags  = PF_W | PF_R;
        /*
        if(this->arch == ARCH_ARM || this->arch == ARCH_X86){
            phdrs[dynsegindex].p_flags  = PF_W | PF_R;
        }else{
            phdrs[dynsegindex].p_flags  = phdrs[dynsegindex-2].p_flags;
        }
        */
        phdrs[dynsegindex].p_memsz  = segsize;
        phdrs[dynsegindex].p_offset = segoffset;
        phdrs[dynsegindex].p_paddr  = segaddr;
        phdrs[dynsegindex].p_vaddr  = segaddr;
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Phdr* phdrs = (Elf64_Phdr*)(e->e_phdr);
        int phnum = e->e_phnum;
        int dynsegindex = 0;
        int notesegindex = 0;
        for (int i=0;i<phnum;i++) {
            if (phdrs[i].p_type == PT_DYNAMIC) {
                dynsegindex = i;
                notesegindex = i+1;
            }
        }
        if (dynsegindex != 2) {
            LOGI("warning.. dynamic dosen't locate in 2ed segment");
        }
        if (notesegindex >= phnum) {
            LOGE("no redundant phdr error");
            return false;
        }

        //copy dynamic segment to note segment
        phdrs[notesegindex].p_align = phdrs[dynsegindex].p_align;
        phdrs[notesegindex].p_type = phdrs[dynsegindex].p_type;
        phdrs[notesegindex].p_filesz = phdrs[dynsegindex].p_filesz;
        phdrs[notesegindex].p_flags = phdrs[dynsegindex].p_flags;
        phdrs[notesegindex].p_memsz = phdrs[dynsegindex].p_memsz;
        phdrs[notesegindex].p_offset = phdrs[dynsegindex].p_offset;
        phdrs[notesegindex].p_paddr = phdrs[dynsegindex].p_paddr;
        phdrs[notesegindex].p_vaddr = phdrs[dynsegindex].p_vaddr;

        //add new pt_load segment in dynamic segment
        //get added section
        Elf64_Xword segsize   = 0;
        Elf64_Off segoffset = 0xeFFFFFFF;
        Elf64_Addr segaddr   = 0xeFFFFFFF;
        Elf64_Xword segalign  = 0;

        for (unsigned i=0; i<this->added_sections.size(); i++) {
            std::string secname = added_sections.at(i);
            Elf64_Shdr* shdr = (Elf64_Shdr* )get_section(secname.c_str());
            if (shdr==NULL) {
                LOGE("in update32phdr shdr is null");
                return false;
            }
            if (shdr->sh_addralign > segalign) {
                segalign = shdr->sh_addralign;
            }
            if (shdr->sh_addr < segaddr) {
                segaddr = shdr->sh_addr;
            }
            if (shdr->sh_offset < segoffset) {
                segoffset = shdr->sh_offset;
            }
            if ((shdr->sh_offset+shdr->sh_size) > segsize) {
                segsize = shdr->sh_offset+shdr->sh_size;
            }
        }
        segsize -= segoffset;

        phdrs[dynsegindex].p_align  = segalign;
        phdrs[dynsegindex].p_type   = PT_LOAD;
        phdrs[dynsegindex].p_filesz = segsize;
        //phdrs[dynsegindex].p_flags  = phdrs[dynsegindex-2].p_flags;
        phdrs[dynsegindex].p_flags  = PF_W | PF_R;
        phdrs[dynsegindex].p_memsz  = segsize;
        phdrs[dynsegindex].p_offset = segoffset;
        phdrs[dynsegindex].p_paddr  = segaddr;
        phdrs[dynsegindex].p_vaddr  = segaddr;
    }

    return true;
}

void* elf_utils::get_symbol(const std::string &name) {
    Elf* e = this->elf;
    if (e == NULL) {
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        Elf32_Shdr* dynsym = (Elf32_Shdr*)this->get_section(".dynsym");
        Elf32_Shdr* dynstr = (Elf32_Shdr*)this->get_section(".dynstr");

        if (dynsym == NULL || dynstr == NULL) {
            LOGE("in get_symbol get dynsym dynstr error");
            return NULL;
        }
        //LOGD("get .dynsym && .dynstr");
        Elf32_Sym* symbol = (Elf32_Sym*)(dynsym->sh_offset + e->e_data);

        for (size_t i = 0; i < dynsym->sh_size / dynsym->sh_entsize; i++) {
            const char* symname = (const char*)(e->e_data + symbol[i].st_name + dynstr->sh_offset);
            //LOGD("sym name:%s", symname);
            if (strcmp(name.c_str(), symname) == 0) {
                return (void *)&symbol[i];
            }
        }
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Shdr* dynsym = (Elf64_Shdr* )this->get_section(".dynsym");
        Elf64_Shdr* dynstr = (Elf64_Shdr* )this->get_section(".dynstr");

        if (dynsym == NULL || dynstr == NULL) {
            LOGE("in get_symbol get dynsym dynstr error");
            return NULL;
        }
        //LOGD("get .dynsym && .dynstr");
        Elf64_Sym* symbol = (Elf64_Sym*)(dynsym->sh_offset + e->e_data);

        for (size_t i = 0; i < dynsym->sh_size / dynsym->sh_entsize; i++) {
            const char* symname = (const char*)(e->e_data + symbol[i].st_name + dynstr->sh_offset);
            //LOGD("sym st name:%d", symbol[i].st_name);
            if (strcmp(name.c_str(), symname) == 0) {
                return (void *)&symbol[i];
            }
        }
    }

    return NULL;
}

//elf_mod
size_t elf_utils::hook(const std::string &name, size_t newaddr) {
    size_t addr_offset = 0;

    if (this->mode == ARCH_BIT_32) {
        Elf32_Sym* symbol = (Elf32_Sym *)this->get_symbol(name);
        if (symbol == NULL) {
            LOGI("can not find:%s", name.c_str());
            return 0;
        }
        addr_offset = newaddr - symbol->st_value;
        if (this->type == PLATFORM_ARM) {
            if((symbol->st_value & 1) == 1){
                arch = ARCH_THUMB;
            }else{
                arch = ARCH_ARM;
            }
        }
        LOGI("update function : %s", name.c_str());
        symbol->st_value = newaddr;
    } else if(this->mode == ARCH_BIT_64) {
        //LOGD("in hook mode 64");
        Elf64_Sym* symbol = (Elf64_Sym *)this->get_symbol(name);
        if(symbol == NULL) {
            LOGI("can not find:%s", name.c_str());
            return 0;
        }
        addr_offset = newaddr - symbol->st_value;
        symbol->st_value = (Elf64_Addr)newaddr;
    }

    return addr_offset;
}

//misc
off_t elf_utils::search_bytes(const char* buffer, size_t buff_len, const char *str, size_t str_len) {
    if (buff_len < str_len) {
        return -1;
    }
    off_t i = 0;
    for (; i < (off_t)(buff_len - str_len) + 1; i++) {
        if (memcmp(buffer + i, str, str_len) == 0) {
            return i;
        }
    }
    return (off_t)-1;
}

//elf_mod
size_t elf_utils::calc_shellcode_address(int align) {
    size_t shellcode_addr = 0;
    Elf* e = this->elf;
    if (e == NULL) {
        return 0;
    }
    if (this->mode == ARCH_BIT_32) {
        Elf32_Phdr* phdr32s = (Elf32_Phdr*)e->e_phdr;
        for (size_t i = 0; i < e->e_phnum; i++) {
            if (phdr32s[i].p_type == PT_LOAD) {
                if ((phdr32s[i].p_vaddr + phdr32s[i].p_memsz) > shellcode_addr) {
                    shellcode_addr = phdr32s[i].p_vaddr + phdr32s[i].p_memsz;
                }
            }
        }

    } else if(this->mode == ARCH_BIT_64) {
        Elf64_Phdr* phdr64s = (Elf64_Phdr*)e->e_phdr;
        for (size_t i = 0; i < e->e_phnum; i++) {
            if(phdr64s[i].p_type == PT_LOAD){
                if (phdr64s[i].p_vaddr + phdr64s[i].p_memsz > shellcode_addr) {
                    shellcode_addr = phdr64s[i].p_vaddr + phdr64s[i].p_memsz;
                }
            }
        }
    } else {
        LOGE("no mode matched.");
        return 0;
    }

    shellcode_addr -= shellcode_addr%align;
    shellcode_addr += align;

    return shellcode_addr;
}

bool elf_utils::store_all_section_headers() {

    Elf* e = this->elf;
    Elf64_Ehdr* ehdr64 = NULL;
    Elf32_Ehdr* ehdr32 = NULL;
    Elf64_Shdr* shdr64 = NULL;
    Elf32_Shdr* shdr32 = NULL;
    int shnum = 0;

    if (e == NULL) {
        return false;
    }

    if (this->mode == ARCH_BIT_32) {
        ehdr32 = (Elf32_Ehdr*)e->e_ehdr;
        shdr32 = (Elf32_Shdr*)(e->e_data + ehdr32->e_shoff);
        shnum = ehdr32->e_shnum;
    } else if (this->mode == ARCH_BIT_64) {
        ehdr64 = (Elf64_Ehdr*)e->e_ehdr;
        shdr64 = (Elf64_Shdr*)(e->e_data + ehdr64->e_shoff);
        shnum = ehdr64->e_shnum;
    }

    for (int i=0; i < shnum; i++) {
        if (this->mode == ARCH_BIT_32) {
            //LOGD("32bit offset:%d", (int)shdr32[i].sh_offset);
            all_sections_offset.push_back((int)shdr32[i].sh_offset);
            all_sections_size.push_back((int)shdr32[i].sh_size);
        } else if(this->mode == ARCH_BIT_64) {
            //LOGD("64bit offset:%d", (int)shdr64[i].sh_offset); 
            all_sections_offset.push_back((int)shdr64[i].sh_offset);
            all_sections_size.push_back((int)shdr64[i].sh_size);
        }
    }
    return true;
}

bool elf_utils::safety_check() {
    if (this->elf == NULL) {
        return false;
    }

    Elf* e = this->elf;
    int shnum = 0;
    size_t shstrndx;

    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        LOGE("elf_getshstrndx() error");
        return 0;
    }
    Elf_Scn *scn = e->e_scn_1;

    for (scn = e->e_scn_1, shnum = 0; scn && shnum < (int)shstrndx-1; scn = scn->s_link, ++shnum) {
        if (shnum >= (int)this->all_sections_offset.size()) {
            break;
        }
        if (this->mode == ARCH_BIT_32) {
            Elf32_Shdr* shdr32 = &scn->s_shdr32;
            if (shdr32->sh_offset != this->all_sections_offset.at(shnum)) {
                //LOGD("section:%d do not match, offset1:%x, offset2:%x", shnum, (int)shdr32->sh_offset, this->all_sections_offset.at(shnum));
                return false;
            }
        } else if (this->mode == ARCH_BIT_64) {
            Elf64_Shdr* shdr64 = &scn->s_shdr64;
            if (shdr64->sh_offset != this->all_sections_offset.at(shnum)) {
                //LOGD("section:%d do not mamtch, offset1:%x, offset2%x", shnum, (int)shdr64->sh_offset, this->all_sections_offset.at(shnum));
                return false;
            }
        }
    }

    return true;
}


bool elf_utils::restore_section_headers(){
    size_t file_len = 0;
    uint8_t* buffer = this->read(0, -1, file_len);
    int shstrndx = 0;
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* ehdr32 = (Elf32_Ehdr*)buffer;
        shstrndx = ehdr32->e_shstrndx;
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* ehdr64 = (Elf64_Ehdr*)buffer;
        shstrndx = ehdr64->e_shstrndx;
    }else{
        LOGE("no match mode");
        return false;
    }
    
    for(int i=0; i<shstrndx; i++){
        if(this->mode == ARCH_BIT_32){
            Elf32_Shdr* shdr = (Elf32_Shdr*)this->get_shdr(buffer, i);
            //shdr->sh_offset = this->all_sections_offset.at(i);
            shdr->sh_size = this->all_sections_size.at(i);
        }else if(this->mode == ARCH_BIT_64){
            Elf64_Shdr* shdr = (Elf64_Shdr*)this->get_shdr(buffer, i);
            //shdr->sh_offset = this->all_sections_offset.at(i);
            shdr->sh_size = this->all_sections_size.at(i);
        }else{
            LOGE("no match mode");
            return false;
        }
    }

    if (buffer != NULL) {
        this->write(buffer, file_len, 0);
        delete[] buffer;
    } else {
        return false;
    }

    return true;
}



bool elf_utils::get_needed_libraries(std::vector<std::string> &needed_libraries) {
    Elf* e = this->elf;
    if (e == NULL) {
        return false;
    }
    if (this->mode == ARCH_BIT_32) {
        Elf32_Phdr* phdr = elf32_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf32_Dyn *dyn_segment = NULL;
        Elf32_Off dyn_offset = 0;
        Elf32_Word dyn_size = 0;
        for (size_t i=0; i<phnum; i++) {
            if (phdr[i].p_type == PT_DYNAMIC) {
                dyn_offset = phdr[i].p_offset;
                dyn_size = phdr[i].p_filesz;
                dyn_segment = (Elf32_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dyn_segment == NULL || dyn_offset == 0 || dyn_size == 0) {
            LOGD("invalid dynamic segment!");
            return !needed_libraries.empty();
        }
        Elf32_Dyn *entry = dyn_segment;
        Elf32_Word string_table_offset = 0;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == DT_STRTAB) {
                //LOGD("find DT_STRTAB, offset = %x", entry->d_un.d_val);
                string_table_offset = entry->d_un.d_val;
                break;
            }
        }
        if (string_table_offset == 0) {
            LOGD("find string table error !");
            return !needed_libraries.empty();
        }

        for (entry = dyn_segment; entry->d_tag != 0 ; entry++) {
            if (entry->d_tag == DT_NEEDED && entry->d_un.d_val != 0) {
                char *cur_name = (char *)e->e_data + string_table_offset + entry->d_un.d_val;
                needed_libraries.push_back(std::string(cur_name));
            }
        }

        return !needed_libraries.empty();

    }else if(this->mode == ARCH_BIT_64){
        Elf64_Phdr* phdr = elf64_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf64_Dyn *dyn_segment = NULL;
        Elf64_Off dyn_offset = 0;
        Elf64_Word dyn_size = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dyn_offset = phdr[i].p_offset;
                dyn_size = phdr[i].p_filesz;
                dyn_segment = (Elf64_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dyn_segment == NULL || dyn_offset == 0 || dyn_size == 0) {
            LOGD("invalid dynamic segment!");
            return !needed_libraries.empty();
        }
        Elf64_Dyn *entry = dyn_segment;
        Elf64_Xword string_table_offset = 0;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == DT_STRTAB) {
                //LOGD("find DT_STRTAB, offset = %x\n ", (uint32_t)entry->d_un.d_val);
                string_table_offset = entry->d_un.d_val;
                break;
            }
        }
        if (string_table_offset == 0) {
            LOGD("find string table error !");
            return !needed_libraries.empty();
        }

        for (entry = dyn_segment; entry->d_tag != 0 ; entry++) {
            if (entry->d_tag == DT_NEEDED && entry->d_un.d_val != 0) {
                char *cur_name = (char *)e->e_data + string_table_offset + entry->d_un.d_val;
                needed_libraries.push_back(std::string(cur_name));
            }
        }

        return !needed_libraries.empty();
    }
    return !needed_libraries.empty();
}

bool elf_utils::is_need_library(const std::string &libName) {
    bool needed = false;
    std::vector<std::string> neededLibraries;
    get_needed_libraries(neededLibraries);
    for(std::vector<std::string>::iterator it = neededLibraries.begin(); it != neededLibraries.end(); ++it) {
        const std::string &name = *it;
        //LOGD("current library :%s", name.c_str());
        if (libName == name) {
            needed = true;
            break;
        }
    }
    return needed;
}

bool elf_utils::has_function(const std::string &name, bool &imported) {
    bool has_fn = false;
    if(this->mode == ARCH_BIT_32){
        Elf32_Sym* symbol = (Elf32_Sym*)get_symbol(name);
        if (symbol != NULL) {
            imported = (symbol->st_value == 0);
        }

        has_fn = (symbol != NULL);
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Sym* symbol = (Elf64_Sym*)get_symbol(name);
        if (symbol != NULL) {
            imported = (symbol->st_value == 0);
        }
        has_fn = (symbol != NULL);
    }

    return has_fn;
}

bool elf_utils::make_function_local(const std::string &name){
    bool has_fn = false;
    if(this->mode == ARCH_BIT_32){
        Elf32_Sym* symbol = (Elf32_Sym*)get_symbol(name);
        if (symbol != NULL) {
            symbol->st_info = STB_LOCAL;
        }

        has_fn = (symbol != NULL);
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Sym* symbol = (Elf64_Sym*)get_symbol(name);
        if (symbol != NULL) {
            symbol->st_info = STB_LOCAL;
        }
        has_fn = (symbol != NULL);
    }

    return has_fn;
}

//by offset
char* elf_utils::get_section_name(int nameoff) {
    if (this->elf == NULL) {
        return NULL;
    }
    size_t shstrndx = 0;

    if (elf_getshdrstrndx(this->elf, &shstrndx) != 0) {
        LOGE("elf_getshstrndx error");
        return 0;
    }

    return elf_strptr(this->elf, shstrndx, nameoff);
};


char* elf_utils::get_section_name(uint8_t* buffer, int nameoff) {
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        Elf32_Shdr* shdr = (Elf32_Shdr*)(buffer + ehdr->e_shoff);
        char* shstr = (char*) (buffer + shdr[ehdr->e_shstrndx].sh_offset);
        return shstr + nameoff;
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Shdr* shdr = (Elf64_Shdr*)(buffer + ehdr->e_shoff);
        char* shstr = (char*) (buffer + shdr[ehdr->e_shstrndx].sh_offset);
        return shstr + nameoff;
    }
    return NULL;
};

uint8_t* elf_utils::read(off_t offset, size_t len, size_t &readbytes) {
    if (this->fd <= 0) {
        LOGE("fd is invalid");
        return NULL;
    }

    size_t fl_len = lseek(this->fd, 0, SEEK_END);
    if (fl_len == 0) {
        LOGE("file size is 0");
        return NULL;
    }

    if (len == (size_t)-1) {
        len = fl_len;
    }

    if (offset + len > fl_len) {
        LOGE("offset is beyond file size.");
        //LOGD("offset:%xl, len:%lx, fl_leln:%lx", offset, len, fl_len);
        return NULL;
    }

    lseek(this->fd, offset, SEEK_SET);

    uint8_t *buffer = new uint8_t[len]();
    if (len != (size_t)::read(this->fd, (void*)buffer, len)) {
        delete [] buffer;
        return NULL;
    }
    readbytes = len;

    return buffer;

}

size_t elf_utils::write(uint8_t *buffer, size_t len, off_t offset) {
    if (this->fd <= 0) {
        LOGE("fd is invalid");
        return 0;
    }

    size_t fl_len = lseek(this->fd, 0, SEEK_END);
    if (offset + len > fl_len) {
        LOGE("write file offset is beyond file!");
        return 0;
    }

    lseek(this->fd, offset, SEEK_SET);

    if (len != (size_t)::write(this->fd, buffer, len)) {
        LOGE("write fail");
        return 0;
    }

    return len;
}

void* elf_utils::get_shdr(uint8_t* buffer, int index){
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* m_Elf32Header = (Elf32_Ehdr*)buffer;
        if(m_Elf32Header->e_shnum <= index){
            return NULL;
        }
        Elf32_Shdr* sechdraddr = (Elf32_Shdr*)((unsigned char*)m_Elf32Header + m_Elf32Header->e_shoff);
        return &sechdraddr[index];
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* m_Elf64Header = (Elf64_Ehdr*)buffer;
        if(m_Elf64Header->e_shnum <= index){
            return NULL;
        }
        Elf64_Shdr* sechdraddr = (Elf64_Shdr*)((unsigned char*)m_Elf64Header + m_Elf64Header->e_shoff);
        return &sechdraddr[index];
    }
    return NULL;
}

void* elf_utils::get_shdr_by_type(uint8_t* buffer, int shdr_type){
    if(buffer == NULL){
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        Elf32_Shdr* sechdraddr = (Elf32_Shdr*)(buffer + ehdr->e_shoff);
        for (int i=0; i<ehdr->e_shnum; i++) {
            if((Elf32_Word)shdr_type == sechdraddr[i].sh_type){
                return &sechdraddr[i];
            }
        }
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Shdr* sechdraddr = (Elf64_Shdr*)(buffer + ehdr->e_shoff);
        for (int i = 0; i < ehdr->e_shnum; i++) {
            if((Elf64_Word)shdr_type == sechdraddr[i].sh_type){
                return &sechdraddr[i];
            }
        }
    }

    return NULL;
}
void* elf_utils::get_shdr(const std::string & name) {
    if(this->file_buffer == NULL){
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)this->file_buffer;
        Elf32_Shdr* sechdraddr = (Elf32_Shdr*)(this->file_buffer + ehdr->e_shoff);

        Elf32_Shdr* secstrhdr =  &sechdraddr[ehdr->e_shstrndx];
        //LOGD("secname:%s, secnum : %d, shstrndx : %d", secname, ehdr->e_shnum, ehdr->e_shstrndx);
        for (int i=0; i<ehdr->e_shnum; i++) {
            //LOGD("i: %d, offset:%x", i, sechdraddr[i].sh_offset);
            const char* cur_name =  (char*)(sechdraddr[i].sh_name + secstrhdr->sh_offset + this->file_buffer);
            //LOGD("secname : %s", name);
            if(strcmp(name.c_str(), cur_name) == 0){
                return &sechdraddr[i];
            }
        }
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)this->file_buffer;
        Elf64_Shdr* sechdraddr = (Elf64_Shdr*)(this->file_buffer + ehdr->e_shoff);
        Elf64_Shdr* secstrhdr =  &sechdraddr[ehdr->e_shstrndx];
        //LOGD("secnum : %d", elf64Header->e_shnum);
        for (int i = 0; i < ehdr->e_shnum; i++) {
            char* cur_name =  (char*)(sechdraddr[i].sh_name + secstrhdr->sh_offset + this->file_buffer);
            //LOGD("secname : %s", name);
            if (strcmp(name.c_str(), cur_name) == 0) {
                return &sechdraddr[i];
            }
        }
    }

    return NULL;
}
 
void* elf_utils::get_shdr(uint8_t* buffer, const std::string & name) {
    if(buffer == NULL){
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        Elf32_Shdr* sechdraddr = (Elf32_Shdr*)(buffer + ehdr->e_shoff);

        Elf32_Shdr* secstrhdr =  &sechdraddr[ehdr->e_shstrndx];
        //LOGD("secname:%s, secnum : %d, shstrndx : %d", secname, ehdr->e_shnum, ehdr->e_shstrndx);
        for (int i=0; i<ehdr->e_shnum; i++) {
            //LOGD("i: %d, offset:%x", i, sechdraddr[i].sh_offset);
            const char* cur_name =  (char*)(sechdraddr[i].sh_name + secstrhdr->sh_offset + buffer);
            //LOGD("secname : %s", name);
            if(strcmp(name.c_str(), cur_name) == 0){
                return &sechdraddr[i];
            }
        }
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Shdr* sechdraddr = (Elf64_Shdr*)(buffer + ehdr->e_shoff);
        Elf64_Shdr* secstrhdr =  &sechdraddr[ehdr->e_shstrndx];
        //LOGD("secnum : %d", elf64Header->e_shnum);
        for (int i = 0; i < ehdr->e_shnum; i++) {
            char* cur_name =  (char*)(sechdraddr[i].sh_name + secstrhdr->sh_offset + buffer);
            //LOGD("secname : %s", name);
            if (strcmp(name.c_str(), cur_name) == 0) {
                return &sechdraddr[i];
            }
        }
    }

    return NULL;
}

void* elf_utils::get_shdrs(uint8_t* buffer, int& shnum){
    if(buffer == NULL){
        return NULL;
    }

    if (this->mode == ARCH_BIT_32) {
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        Elf32_Shdr* sechdraddr = (Elf32_Shdr*)(buffer + ehdr->e_shoff);
        shnum = ehdr->e_shnum;
        
        return sechdraddr;
    } else if (this->mode == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Shdr* sechdraddr = (Elf64_Shdr*)(buffer + ehdr->e_shoff);
        shnum = ehdr->e_shnum;

        return sechdraddr;
    }

    return NULL;
}


bool elf_utils::update_sections(uint8_t* buffer, const std::string &name, void* pshdr) {
    //LOGD("filename:%s", this->fileName.c_str());
    //get secton header
    bool updated = false;
    if (this->mode == ARCH_BIT_32) {
        Elf32_Shdr* pshdr32 = (Elf32_Shdr*)pshdr;
        Elf32_Shdr* shdr = (Elf32_Shdr*)this->get_shdr(buffer, name.c_str());
        if (shdr != NULL) {
            shdr->sh_size = pshdr32->sh_size;
            shdr->sh_offset = pshdr32->sh_offset;
            shdr->sh_addr = pshdr32->sh_addr;
            shdr->sh_addralign = pshdr32->sh_addralign;
            shdr->sh_entsize = pshdr32->sh_entsize;
            shdr->sh_info = pshdr32->sh_info;
            shdr->sh_type = pshdr32->sh_type;
            shdr->sh_flags = pshdr32->sh_flags;
            shdr->sh_link = pshdr32->sh_link;
            updated = true;
        } else {
            LOGD("can not find section: %s", name.c_str());
        }
    } else if(this->mode== ARCH_BIT_64) {
        Elf64_Shdr* pshdr64 = (Elf64_Shdr*)pshdr;
        Elf64_Shdr* shdr = (Elf64_Shdr*)this->get_shdr(buffer, name.c_str());
        if (shdr != NULL) {
            shdr->sh_size = pshdr64->sh_size;
            shdr->sh_offset = pshdr64->sh_offset;
            shdr->sh_addr = pshdr64->sh_addr;
            shdr->sh_addralign = pshdr64->sh_addralign;
            shdr->sh_entsize = pshdr64->sh_entsize;
            shdr->sh_info = pshdr64->sh_info;
            shdr->sh_type = pshdr64->sh_type;
            shdr->sh_flags = pshdr64->sh_flags;
            shdr->sh_link = pshdr64->sh_link;
            updated = true;
        } else {
            LOGD("can not find section:%s", name.c_str());
        }
    } else {
        LOGE("error...");
    }

    return updated;
}

bool elf_utils::update_sections(uint8_t* buffer, int index, void* pshdr){
    bool updated = false;
    if (this->mode == ARCH_BIT_32) {
        Elf32_Shdr* pshdr32 = (Elf32_Shdr*)pshdr;
        Elf32_Shdr* shdr = (Elf32_Shdr*)this->get_shdr(buffer, index);
        if (shdr != NULL) {
            shdr->sh_size = pshdr32->sh_size;
            shdr->sh_offset = pshdr32->sh_offset;
            shdr->sh_addr = pshdr32->sh_addr;
            shdr->sh_addralign = pshdr32->sh_addralign;
            shdr->sh_entsize = pshdr32->sh_entsize;
            shdr->sh_info = pshdr32->sh_info;
            shdr->sh_type = pshdr32->sh_type;
            shdr->sh_flags = pshdr32->sh_flags;
            shdr->sh_link = pshdr32->sh_link;
            updated = true;
        } else {
            LOGD("can not find section: %d", index);
        }
    } else if(this->mode== ARCH_BIT_64) {
        Elf64_Shdr* pshdr64 = (Elf64_Shdr*)pshdr;
        Elf64_Shdr* shdr = (Elf64_Shdr*)this->get_shdr(buffer, index);
        if (shdr != NULL) {
            shdr->sh_size = pshdr64->sh_size;
            shdr->sh_offset = pshdr64->sh_offset;
            shdr->sh_addr = pshdr64->sh_addr;
            shdr->sh_addralign = pshdr64->sh_addralign;
            shdr->sh_entsize = pshdr64->sh_entsize;
            shdr->sh_info = pshdr64->sh_info;
            shdr->sh_type = pshdr64->sh_type;
            shdr->sh_flags = pshdr64->sh_flags;
            shdr->sh_link = pshdr64->sh_link;
            updated = true;
        } else {
            LOGD("can not find section:%d", index);
        }
    } else {
        LOGE("error...");
    }

    return updated;
}

bool elf_utils::update() {
    Elf* e = this->elf;
    if (e == NULL) {
        LOGE("elf is not parse.");
        return false;
    }

    fix_section();

    if (elf_update(e, ELF_C_NULL) < 0) {
        LOGE("in write elf_update error.");
        return false;
    };

    if (this->mode == ARCH_BIT_32 || this->mode == ARCH_BIT_64) {
        if (!this->update_phdr()) {
            LOGE("update_phdr error.");
            return false;
        }
    } else {
        LOGE("no match mode");
        return false;
    }

    
    //check section offset..
    if (!this->safety_check()) {
        LOGE("section offset do not match, do not patch shellcode!!");
        return false;
    }
    

    if (elf_update(e , ELF_C_WRITE ) < 0) {
        LOGE("elf update error.");
        return false;
    }

    return this->restore_section_headers();
    //(void) elf_end (e);
    //return true;
}


bool elf_utils::fix_section() {
    Elf* e = this->elf;
    Elf_Scn *scn = e->e_scn_1;
    Scn_Data *pre_sd = scn->s_data_1;
    Elf32_Shdr *shdr32 = NULL;
    Elf64_Shdr* shdr64 = NULL;

    size_t pre_shoffset = 0;
    size_t pre_shsize = 0;
    if (this->mode == ARCH_BIT_32) {
        shdr32 = &scn->s_shdr32;
        pre_shoffset = shdr32->sh_offset;
        pre_shsize = shdr32->sh_size;
    } else if (this->mode == ARCH_BIT_64) {
        shdr64 = &scn->s_shdr64;
        pre_shoffset = shdr64->sh_offset;
        pre_shsize = shdr64->sh_size;
    } else {
        LOGE("in fix_section no match mode");
        return false;
    }

    Scn_Data* cur_sd = NULL;
    size_t cur_shoffset = 0;
    size_t cur_shsize = 0;
    int cur_align = 0;
    int index = 0;

    while (true) {
        scn = scn->s_link;
        if (scn == NULL) {
            break;
        }
        cur_sd = scn->s_data_1;
        if (this->mode == ARCH_BIT_32) {
            shdr32 = &scn->s_shdr32;
            cur_shoffset = shdr32->sh_offset;
            cur_shsize = shdr32->sh_size;
            cur_align = shdr32->sh_addralign;
        } else if(this->mode == ARCH_BIT_64) {
            shdr64 = &scn->s_shdr64;
            cur_shoffset = shdr64->sh_offset;
            cur_shsize = shdr64->sh_size;
            cur_align = shdr64->sh_addralign;
        }

        //LOGD("index:%d, shoff:%x, shsize:%x", index, (int)cur_shoffset, (int)cur_shsize);
        pre_sd->sd_data.d_type = ELF_T_BYTE;
        if (index != 0 && ((int)cur_shoffset - (int)(pre_shoffset+pre_shsize)) >= (int)cur_align) {
            int appendsize = cur_shoffset - (pre_shoffset + pre_shsize);
            appendsize -= appendsize%cur_align;
            //LOGD("index:%d, appendsize : %x", index, appendsize);
            pre_sd->sd_data.d_size += appendsize;
            //pre_sd->sd_data.d_align = 4;
            pre_sd->sd_data.d_type = ELF_T_BYTE;
            //LOGD("index:%d, d_size:%x", index, (int)pre_sd->sd_data.d_size);
        }
        if ( cur_shoffset%cur_align != 0) {
            cur_align = 4;
            if (cur_shoffset%cur_align != 0) {
                cur_shoffset = 1;
            }
        }
        cur_sd->sd_data.d_align = cur_align;

        pre_sd = cur_sd;
        pre_shoffset = cur_shoffset;
        pre_shsize = cur_shsize;
        //LOGD("index:%d, off:%x", index, (int)(pre_shdr32->sh_offset));
        index++;
    }

    return true;
}


long elf_utils::get_dt_func_val(int dt_func){
    size_t m_value = 0; 
    Elf* e = this->elf;
    if(this->mode == ARCH_BIT_32){
        Elf32_Phdr* phdr = elf32_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf32_Dyn *dynSeg = NULL;
        Elf32_Off dynOffset = 0;
        Elf32_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf32_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return m_value;
        }
        Elf32_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            //LOGD("%s[%d]: find DT_STRTAB, tag = %x, offset = %x ", __FUNCTION__, __LINE__, entry->d_tag, entry->d_un.d_val);
            if (entry->d_tag == dt_func) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val);
                m_value = entry->d_un.d_val;
                break;
            }
        }

        if(m_value == 0){
            entry->d_tag = dt_func;
            m_value = 0;
            entry++;
            if(entry->d_tag != 0){
                LOGE("no enough space to increase dt_init...");
                return -1;
            }
        }

    }else if(this->mode == ARCH_BIT_64){
        Elf64_Phdr* phdr = elf64_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf64_Dyn *dynSeg = NULL;
        Elf64_Off dynOffset = 0;
        Elf64_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf64_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return m_value;
        }
        Elf64_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == dt_func) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val);
                m_value = entry->d_un.d_val;
                break;
            }
        }
        if(m_value == 0){
            entry->d_tag = dt_func;
            m_value = 0;
            entry++;
            if(entry->d_tag != 0){
                LOGE("no enough space to increase dt_init...");
                return -1;
            }
        }
        
    }
    return m_value;
}


bool elf_utils::set_dt_func_val(int dt_func, size_t new_addr){
    size_t m_value = new_addr;
    Elf* e = this->elf;
    if(this->mode == ARCH_BIT_32){
        Elf32_Phdr* phdr = elf32_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf32_Dyn *dynSeg = NULL;
        Elf32_Off dynOffset = 0;
        Elf32_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf32_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return false;
        }
        Elf32_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == dt_func) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x, m_value = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val, m_value);
                entry->d_un.d_val = (Elf32_Word)m_value;
                return true;
            }
        }
        

    }else if(this->mode == ARCH_BIT_64){
        Elf64_Phdr* phdr = elf64_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf64_Dyn *dynSeg = NULL;
        Elf64_Off dynOffset = 0;
        Elf64_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf64_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return false;
        }
        Elf64_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == dt_func) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val);
                entry->d_un.d_val = (Elf64_Word)m_value;
                return true;
            }
        }

    }
    return false;
}


bool elf_utils::set_dt_type(int src_type, int des_type){
    Elf* e = this->elf;
    
    if(this->mode == ARCH_BIT_32){
        Elf32_Phdr* phdr = elf32_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf32_Dyn *dynSeg = NULL;
        Elf32_Off dynOffset = 0;
        Elf32_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf32_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return false;
        }
        Elf32_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            //LOGD("%s[%d]: find DT_STRTAB, d_tag:%d, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_tag, entry->d_un.d_val);
            if (entry->d_tag == src_type) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val);
                entry->d_tag = (Elf32_Sword)des_type;
                entry->d_un.d_val = 0;
                return true;
            }
        }
        

    }else if(this->mode == ARCH_BIT_64){
        Elf64_Phdr* phdr = elf64_getphdr(e);
        size_t phnum = 0;
        elf_getphdrnum(e, &phnum);
        Elf64_Dyn *dynSeg = NULL;
        Elf64_Off dynOffset = 0;
        Elf64_Word dynSize = 0;
        for(size_t i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynOffset = phdr[i].p_offset;
                dynSize = phdr[i].p_filesz;
                dynSeg = (Elf64_Dyn*)(e->e_data + phdr[i].p_offset);
                break;
            }
        }
        if (dynSeg == NULL || dynOffset == 0 || dynSize == 0) {
            LOGD("invalid dynamic segment!\n");
            return false;
        }
        Elf64_Dyn *entry = dynSeg;
        for (; entry->d_tag != 0 ; entry++ ) {
            if (entry->d_tag == src_type) {
                //LOGD("%s[%d]: find DT_STRTAB, offset = %x\n ", __FUNCTION__, __LINE__, entry->d_un.d_val);
                entry->d_tag = (Elf64_Sword)des_type;
                entry->d_un.d_val = 0;
                return true;
            }
        }
        
    }
    return false;
}

bool elf_utils::update_file(){
    if (this->fd <= 0) {
        return false;
    }
    Elf *e = this->elf;
    if (e == NULL) {
        return false;
    }
    if (e->e_data == NULL) {
        LOGE("e_data is NULL....");
        return false;
    }

    lseek(this->fd, 0, SEEK_SET);
    return (size_t)::write(this->fd, e->e_data, e->e_dsize) == e->e_dsize;

}


void* elf_utils::get_phdr(int& phnum){
    phnum = 0;
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)this->elf->e_data;
        phnum = ehdr->e_phnum;
        Elf32_Phdr* phdr = (Elf32_Phdr*)(this->elf->e_data + ehdr->e_phoff);

        return phdr;
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)this->elf->e_data;
        //LOGD("phoff:%ld", ehdr->e_phoff);
        phnum = ehdr->e_phnum;
        Elf64_Phdr* phdr = (Elf64_Phdr*)(this->elf->e_data + ehdr->e_phoff);
        //LOGD("type:%d", phdr[0].p_type);
        return phdr;
    }
    return NULL;
}


void* elf_utils::get_phdr(uint8_t* buffer, int& phnum){
        phnum = 0;
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        phnum = ehdr->e_phnum;
        Elf32_Phdr* phdr = (Elf32_Phdr*)(buffer + ehdr->e_phoff);

        return phdr;
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        //LOGD("phoff:%ld", ehdr->e_phoff);
        phnum = ehdr->e_phnum;
        Elf64_Phdr* phdr = (Elf64_Phdr*)(buffer + ehdr->e_phoff);
        //LOGD("type:%d", phdr[0].p_type);
        return phdr;
    }
    return NULL;
}

void* elf_utils::get_pt_load(int n){
    if(this->mode == ARCH_BIT_32){
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)this->elf->e_data;
        int phnum = ehdr->e_phnum;
        Elf32_Phdr* phdr = NULL;
        int count = 0;
        for(int i=0; i<phnum; i++){
            phdr = (Elf32_Phdr*)(this->elf->e_data + ehdr->e_phoff);
            if(phdr[i].p_type == PT_LOAD){
                count++;
                if(count == n){
                    return &phdr[i];
                }
            }
        }
    }else if(this->mode == ARCH_BIT_64){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)this->elf->e_data;
        int phnum = ehdr->e_phnum;
        Elf64_Phdr* phdr = NULL;
        int count = 0;
        for(int i=0; i<phnum; i++){
            phdr = (Elf64_Phdr*)(this->elf->e_data + ehdr->e_phoff);
            if(phdr[i].p_type == PT_LOAD){
                count++;
                if(count == n){
                    return &phdr[i];
                }
            }
        }
    }
    return NULL;
}


unsigned char* elf_utils::get_buffer(){
    if(this->elf != NULL){
        return (unsigned char*)this->elf->e_data;
    } 
    return NULL;
}


size_t elf_utils::get_file_size(){
    if(this->elf != NULL){
        return this->elf->e_dsize;
    } 
    return 0;
}


bool elf_utils::attach_shellcode(size_t target_offset, uint8_t* content, size_t content_len){
    if(this->file_buffer == NULL){
        return false;
    }

    memcpy(this->file_buffer+target_offset, content, content_len);

    return true;
}