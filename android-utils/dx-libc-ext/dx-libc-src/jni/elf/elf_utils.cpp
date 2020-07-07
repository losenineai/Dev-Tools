//
// Created by my on 18-6-5.
//

#include "android_private.h"
#include "elf_utils.h"
#include "dx_dlopen.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <android/log.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <cstring>
#include "../log/log.h"
#include "../dx_libc.h"


void *get_module_addr(const char *libpath) {

    dxFILE *maps = dx_fopen("/proc/self/maps", "rb");
    // int fd_maps = dx_open("/proc/self/maps", O_RDONLY);
    if(maps == NULL ) {
//        __android_log_print(ANDROID_LOG_ERROR, tag, "failed to open maps");
        return NULL;
    }
    
    char eachline[256] = {0};
    char* lib_load_addr = NULL;
    void* module_load_addr = NULL;
    // LOGD("+++++++++retSize:%d, buffsize:%d\n", retSize, buffsize);
    
    while(dx_fgets(eachline, sizeof(eachline), maps) != NULL){
        // LOGD("eachline:%s", eachline);
        const char *libPathIndex = dx_strstr(eachline, libpath);
        if (libPathIndex) {
        //just get the first line, which includes the base address.
            lib_load_addr = NULL;
            if(dx_sscanf(eachline, "%p", &lib_load_addr) == 1) {
                if (lib_load_addr[0] == 0x7f && lib_load_addr[1] == 0x45 &&
                    lib_load_addr[2] == 0x4c && lib_load_addr[3] == 0x46) {
                    
#ifdef __arm__
            Elf32_Ehdr* ehdr = (Elf32_Ehdr*)lib_load_addr;
            if(ehdr->e_machine != EM_ARM){
                break;
            }
#elif __i386__
            Elf32_Ehdr* ehdr = (Elf32_Ehdr*)lib_load_addr;
            if(ehdr->e_machine != EM_386){
                break;
            }
#elif __aarch64__
            Elf64_Ehdr* ehdr = (Elf64_Ehdr*)lib_load_addr;
            if(ehdr->e_machine != EM_AARCH64){
                break;
            }
#elif __x86_x64__
            Elf64_Ehdr* ehdr = (Elf64_Ehdr*)lib_load_addr;
            if(ehdr->e_machine != EM_X86_64){
                break;
            }
#endif
                    module_load_addr = lib_load_addr;
                }
                continue;
            }
        }
    }

    dx_fclose(maps);

    return module_load_addr;
}


void get_sym_in_dynamic(Elf_Ehdr *elf, size_t &dynsym, size_t &dynstr, size_t &dynstrsz, size_t &dynsym_fake_end, size_t &loadBias) {

    const char *elfBase = (const char*)elf;
    //locate elf with phdr.not shdr.
    Elf_Phdr *phdr = (Elf_Phdr*)(elfBase + elf->e_phoff);

    int phNum = elf->e_phnum;

    size_t dyn_size = 0, dyn_off = 0;
    Elf_Addr minLoadAddr = (Elf_Addr)-1;

    for (int i = 0; i < phNum; ++i) {

        Elf_Word p_type = phdr[i].p_type;

        if (p_type == PT_DYNAMIC) { // 2
            //get dyn symbol table from dynamic section
            dyn_size = phdr[i].p_memsz;
            dyn_off = phdr[i].p_vaddr;
        } else if (p_type == PT_LOAD) { // 1
            Elf_Addr loadAddr = phdr[i].p_vaddr;
            if (minLoadAddr > loadAddr) {
                minLoadAddr = loadAddr;
            }
        }
    }

    dyn_off -= minLoadAddr;

    const Elf_Dyn* dyn = (const Elf_Dyn*)(elfBase + dyn_off);
    size_t n = dyn_size / sizeof(Elf_Dyn);

    Elf_Addr* dynArr = new Elf_Addr[n];

    for (int i = 0; i < n; ++i) {
        int type = (int)dyn[i].d_tag;

        dynArr[i] = dyn[i].d_un.d_ptr;

        switch (type) {
            case DT_SYMTAB: // 6
                dynsym = dyn[i].d_un.d_ptr - minLoadAddr;
                break;
            case DT_STRTAB: // 5
                dynstr = dyn[i].d_un.d_ptr - minLoadAddr;
                break;
            case DT_STRSZ:  // 10
                dynstrsz = dyn[i].d_un.d_ptr - minLoadAddr;
                break;
            default:
                break;
        }
    }

    dynsym_fake_end = (size_t)-1;

    for (int i = 0; i < n; ++i) {

        Elf_Addr cur_addr = dynArr[i];

        if (cur_addr > dynsym && cur_addr < dynstr && cur_addr < dynsym_fake_end)
            dynsym_fake_end = cur_addr;
    }

    if (dynsym_fake_end == -1)
        dynsym_fake_end = dynstr;

    loadBias = minLoadAddr;

    delete[] dynArr;
}

bool init_handle(Elf_Ehdr *elf, MyHandle* soinfo){
    char *elfBase = (char*) elf;
    //locate elf with phdr.not shdr.
    Elf_Phdr *phdr = (Elf_Phdr*)(elfBase + elf->e_phoff);

    int phNum = elf->e_phnum;

    size_t dyn_size = 0, dyn_off = 0;
    Elf_Addr minLoadAddr = (Elf_Addr)-1;

    for (int i = 0; i < phNum; ++i) {

        Elf_Word p_type = phdr[i].p_type;

        if (p_type == PT_DYNAMIC) { // 2
            //get dyn symbol table from dynamic section
            dyn_size = phdr[i].p_memsz;
            dyn_off = phdr[i].p_vaddr;
        } else if (p_type == PT_LOAD) { // 1
            Elf_Addr loadAddr = phdr[i].p_vaddr;
            if (minLoadAddr > loadAddr) {
                minLoadAddr = loadAddr;
            }
        }
    }

    dyn_off -= minLoadAddr;

    Elf_Dyn* dyn = (Elf_Dyn*)(elfBase + dyn_off);

    soinfo->load_addr = (size_t)elfBase;

    soinfo->isgnu = false;
    
    for (Elf_Dyn* d = dyn; (int)d->d_tag != DT_NULL; ++d) {
        
        switch (d->d_tag) {
            case DT_GNU_HASH:
                soinfo->gnu_nbucket_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[0];
                // skip symndx
                soinfo->gnu_maskwords_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[2];
                soinfo->gnu_shift2_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[3];

                soinfo->gnu_bloom_filter_ = reinterpret_cast<Elf_Addr*>(elfBase + d->d_un.d_ptr - minLoadAddr + 16);
                soinfo->gnu_bucket_ = reinterpret_cast<uint32_t*>(soinfo->gnu_bloom_filter_ + soinfo->gnu_maskwords_);
                // amend chain for symndx = header[1]
                soinfo->gnu_chain_ = soinfo->gnu_bucket_ + soinfo->gnu_nbucket_ -
                reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[1];

                soinfo->isgnu = true;
                break;
            
            case DT_HASH:
                soinfo->nbucket_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[0];
                soinfo->nchain_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr)[1];
                soinfo->bucket_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr + 8);
                soinfo->chain_ = reinterpret_cast<uint32_t*>(elfBase + d->d_un.d_ptr - minLoadAddr + 8 + soinfo->nbucket_ * 4);
                
                break;

            case DT_SYMTAB: // 6
                soinfo->dynsym = (void*)(d->d_un.d_ptr - minLoadAddr + elfBase);
                break;
            case DT_STRTAB: // 5
                soinfo->dynstr = (void*)(d->d_un.d_ptr - minLoadAddr + elfBase);
                break;
            case DT_STRSZ:  // 10
                soinfo->dynstrsz = (size_t)(d->d_un.d_ptr - minLoadAddr + elfBase);
                break;
            default:
                break;
        }
    }

    soinfo->loadBias = minLoadAddr;
    return true;
} 


uint32_t elf_hash(const char* _name) {
  
    const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(_name);
    uint32_t h = 0, g;

    while (*name_bytes) {
        h = (h << 4) + *name_bytes++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }

    return h;
}

uint32_t gnu_hash(const char* _name) {
  
    uint32_t h = 5381;
    const uint8_t* name = reinterpret_cast<const uint8_t*>(_name);
    while (*name != 0) {
      h += (h << 5) + *name++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
    }

    return h;
}

bool gnu_lookup(const char* symbol_name, MyHandle* soinfo,
                        uint32_t* symbol_index) {
  uint32_t hash = gnu_hash(symbol_name);
  uint32_t h2 = hash >> soinfo->gnu_shift2_;

  uint32_t bloom_mask_bits = sizeof(Elf_Addr)*8;
  uint32_t word_num = (hash / bloom_mask_bits) & soinfo->gnu_maskwords_;
  Elf_Addr bloom_word = soinfo->gnu_bloom_filter_[word_num];

  *symbol_index = 0;

  // test against bloom filter
//   if ((1 & (bloom_word >> (hash % bloom_mask_bits)) & (bloom_word >> (h2 % bloom_mask_bits))) == 0) {
//     printf("%s NOT FOUND(test)!\n", symbol_name);
//     return true;
//   }

  // bloom test says "probably yes"...
  uint32_t n = soinfo->gnu_bucket_[hash % soinfo->gnu_nbucket_];

  if (n == 0) {
    printf("%s NOT FOUND!\n", symbol_name);
    return true;
  }

  do {
    Elf_Sym* s = ((Elf_Sym*)soinfo->dynsym) + n;
    
    if (((soinfo->gnu_chain_[n] ^ hash) >> 1) == 0 &&
        strcmp(((const char*)soinfo->dynstr) + s->st_name, symbol_name) == 0 ) {
        *symbol_index = n;
        return true;
    }
  } while ((soinfo->gnu_chain_[n++] & 1) == 0);

  return false;
}

bool elf_lookup(const char* symbol_name, MyHandle* soinfo,
                        uint32_t* symbol_index) {
    uint32_t hash = elf_hash(symbol_name);
    
    for (uint32_t n = soinfo->bucket_[hash % soinfo->nbucket_]; n != 0; n = soinfo->chain_[n]) {
        Elf_Sym* s = (Elf_Sym*)soinfo->dynsym + n;
        if (strcmp((const char*)(soinfo->dynstr) + s->st_name, symbol_name) == 0 ) {
            *symbol_index = n;
            return true;
        }
    }

    *symbol_index = 0;
    return false;
}

bool find_symbol_by_name(const char* symbol_name, MyHandle* soinfo,
                                Elf_Sym** symbol) {
  uint32_t symbol_index;
  bool success =
      soinfo->isgnu == true ?
      gnu_lookup(symbol_name, soinfo, &symbol_index) :
      elf_lookup(symbol_name, soinfo, &symbol_index) ;

  if (success) {
    *symbol = symbol_index == 0 ? nullptr : ((Elf_Sym*)soinfo->dynsym) + symbol_index;
  }

  return success;
}
