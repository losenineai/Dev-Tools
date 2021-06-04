//
// Created by my on 18-6-5.
//

#include "dx_libc_android_private.h"
#include "dx_libc_elf_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <android/log.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <cstring>

void *get_module_addr(const char *libpath) {

    char buff[256] = {0};
    FILE *maps = fopen("/proc/self/maps", "r");
    if(!maps) {
//        __android_log_print(ANDROID_LOG_ERROR, tag, "failed to open maps");
        return 0;
    }

    void* module_load_addr = 0;
    char* lib_load_addr = 0;

    while(fgets(buff, sizeof(buff), maps)) {

        const char *libPathIndex = strstr(buff, libpath);
        if (libPathIndex) {
            //just get the first line, which includes the base address.

            lib_load_addr = 0;
            if(sscanf(buff, "%p", &lib_load_addr) == 1) {

//                __android_log_print(ANDROID_LOG_ERROR, "DXAnti", "buff: %s ", buff, libPathIndex);

                if (lib_load_addr[0] == 0x7f && lib_load_addr[1] == 0x45 &&
                        lib_load_addr[2] == 0x4c && lib_load_addr[3] == 0x46) {

                    module_load_addr = lib_load_addr;
                    break;
                }

                continue;
            }
        }
    }

    fclose(maps);
//    __android_log_print(ANDROID_LOG_INFO, tag, "%s loaded in Android at %p", libpath, load_addr);

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