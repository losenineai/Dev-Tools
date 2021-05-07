#include "anti_debug.h"
#include "elf_utils.h"
#include "log.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <algorithm>

void rand_string(char *s, int len) {
    const char alphanum[] =
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = '\0';
}
#define MAGIC_ADDRESS  0xDEADBAAD

bool anti_debug::add_empty_sections(int n) {
    srand(time(0));

    for( int i = 0; i < n; i++ ) {
        char name[32] = {0};
        rand_string(name, 5 + rand() % 4);
        std::string secname(name);
        if (std::find(section_names.begin(), section_names.end(), secname) != section_names.end()) {
            continue;
        }
        section_names.push_back(secname);
        if (!this->elf.add_section(secname, NULL, (size_t)0, (off_t)MAGIC_ADDRESS, 1, false)) {
            LOGE("add junk section error!!");
            return false;
        }
    }

    if (!this->elf.update()) {
        LOGE("update elf error!!");
        return false;
    }

    return true;
}

bool anti_debug::fix_elf() {
    //fix elf header for dynamic link so file
    size_t file_len = 0;
    uint8_t* buffer = this->elf.read(0, -1, file_len); //read all file content
    if (buffer == NULL) {
        return false;
    }
    /*
    const char *need_fixed_sections[] {
            ".plt", ".text", ".ARM.extab", ".ARM.exidx", ".rodata", ".data.rel.ro.local", ".fini_array", ".init_array",
            ".ARM.attributes", ".got", ".data", ".comment", ".eh_frame", ".data.rel.ro"
        };
    */
   const char *no_fixed_sections[] = {
       ".dynsym", ".dynstr", ".dynamic", ".shstrtab", ".symtab", ".gnu.version", ".gnu.version_d", ".gnu.version_r"
   };
    if (this->elf.get_mode() == ARCH_BIT_32) {
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        for (int i = 1; i < ehdr->e_shstrndx ;i++) {
            Elf32_Shdr* shdr = (Elf32_Shdr *)this->elf.get_shdr(buffer, i);
            if (shdr == NULL) {
                break;
            }
            bool find = false;
            for (size_t j = 0; j < sizeof(no_fixed_sections)/ sizeof(no_fixed_sections[0]); j++) {
                if (!strcmp(this->elf.get_section_name(buffer, shdr->sh_name), no_fixed_sections[j])) {
                    find = true;
                    break;
                }
            }
            if (!find) {
                shdr->sh_offset = 32;
                shdr->sh_size = 32;
            }
        }

    } else if(this->elf.get_mode() == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        for (int i = 1; i < ehdr->e_shstrndx ;i++) {
            Elf64_Shdr* shdr = (Elf64_Shdr *)this->elf.get_shdr(buffer, i);
            if (shdr == NULL) {
                break;
            }
            bool find = false;
            for (size_t j = 0; j < sizeof(no_fixed_sections)/ sizeof(no_fixed_sections[0]); j++) {
                if (!strcmp(this->elf.get_section_name(buffer, shdr->sh_name), no_fixed_sections[j])) {
                    find = true;
                    break;
                }
            }
             if (!find) {
                shdr->sh_offset = 32;
                shdr->sh_size = 32;
            }
        }
    }

    if (buffer != NULL) {
        this->elf.write(buffer, file_len, 0);
        delete[] buffer;
    } else {
        return false;
    }
    return true;
}

bool anti_debug::anti_ida() {
    size_t file_len = 0;
    uint8_t* buffer = (uint8_t *)this->elf.read(0, -1, file_len); //read all file content
    if (buffer == NULL) {
        return false;
    }
    void* m_shdr = NULL;
    for(unsigned i = 0; (m_shdr = this->elf.get_shdr(buffer, i)) != NULL; i++){
        
        if (this->elf.get_mode() == ARCH_BIT_32) {
            Elf32_Shdr *hdr = (Elf32_Shdr*)m_shdr;//elf.get_section(sec_name);
            if (hdr != NULL) {
                if (hdr->sh_addr != MAGIC_ADDRESS) {
                    continue;
                }
                //LOGD("find magic ! %d \n", hdr->sh_name);
            }
            //LOGD("section name:%d", i);
            Elf32_Shdr shdr;
            shdr.sh_offset = rand()%0xfffffff + 0xf0000000;
            shdr.sh_addr = rand()%0xfffffff + 0xf0000000;
            shdr.sh_addralign = 0x1;
            shdr.sh_type = SHT_PROGBITS;
            shdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
            shdr.sh_entsize = 0;
            shdr.sh_link = 0;
            shdr.sh_info = 0;
            shdr.sh_size = rand()%0xffff;

            this->elf.update_sections(buffer, i, (void*)&shdr);
        } else if(this->elf.get_mode() == ARCH_BIT_64) {
            Elf64_Shdr *hdr = (Elf64_Shdr*)m_shdr;
            if (hdr != NULL) {
                if (hdr->sh_addr != MAGIC_ADDRESS) {
                    continue;
                }
                //printf("find magic ! %d \n", hdr->sh_name);
            }
            //LOGD("section name:%d", i);
            Elf64_Shdr shdr;
            shdr.sh_offset = rand()%0xfffffff + 0xf0000000;
            shdr.sh_addr = rand()%0xfffffff + 0xf0000000;
            shdr.sh_addralign = 0x1;
            shdr.sh_type = SHT_PROGBITS;
            shdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
            shdr.sh_entsize = 0;
            shdr.sh_link = 0;
            shdr.sh_info = 0;
            shdr.sh_size = rand()%0xffff;
            this->elf.update_sections(buffer, i, (void*)&shdr);
        }
    }

    if (buffer != NULL) {
        this->elf.write(buffer, file_len, 0);
        delete[] buffer;
    } else {
        return false;
    }

    return true;
}


bool anti_debug::hide_symbols() {
    //LOGD("read file...");
    size_t file_len = 0;
    uint8_t* buffer = this->elf.read(0, -1, file_len); //read all file content
    if (buffer == NULL) {
        return false;
    }

    if (this->elf.get_mode() == ARCH_BIT_32) {
        const char *no_process_section[] {
            ".dynsym", ".dynstr", ".dynamic", ".shstrtab", ".symtab", ".got", ".got.plt"
        };

        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)buffer;
        for (int i = 1; i < ehdr->e_shstrndx ;i++) {
            Elf32_Shdr* shdr = (Elf32_Shdr *)this->elf.get_shdr(buffer, i);
            if (shdr == NULL) {
                break;
            }
            bool match = false;
            for (size_t j = 0; j < sizeof(no_process_section)/ sizeof(no_process_section[0]); j++) {
                if (!strcmp(this->elf.get_section_name(shdr->sh_name), no_process_section[j])) {
                    match = true;
                    break;
                }
            }
            if (!match) {
                shdr->sh_offset = 0xFE000000 + rand() % 0xffffff;
                shdr->sh_addr   = 0xFFFF0000 + rand() % 0xffff;
                shdr->sh_size   = 0xFFFF0000 + rand() % 0xffff;
            } else {
                if (!strcmp(this->elf.get_section_name(shdr->sh_name), ".dynsym")) {
                    shdr->sh_size   = 32;
                    shdr->sh_addr   = 0;
                }
            }
        }

    } else if(this->elf.get_mode() == ARCH_BIT_64) {
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        for (int i = 1; i < ehdr->e_shstrndx ;i++) {
            Elf64_Shdr* shdr = (Elf64_Shdr *)this->elf.get_shdr(buffer, i);
            if(shdr == NULL){
                break;
            }
            if (!strcmp(this->elf.get_section_name(shdr->sh_name), ".dynsym")) {
                shdr->sh_size  = 32;
            }
        }
    }

    if (buffer != NULL) {
        this->elf.write(buffer, file_len, 0);
        delete[] buffer;
    } else {
        return false;
    }

    return true;
}
