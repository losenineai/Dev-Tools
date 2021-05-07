#include <string.h>
#include <android/log.h>
#include <jni.h>
//#include <asm-generic/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sched.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "elf_h.h"

#define PAGE_SIZE   4096

#define LOGD 
#define LOGE

inline
int dx_memcmp(const void* s1, const void* s2,size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1, *p2 = (const unsigned char *)s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}


extern long dx_syscall(int n,...);

void* get_function(const char* lib_name, int lib_len, const char* fun_name, int name_len){
    unsigned char maps_str[] = {
        0x2F, 0x70, 0x72, 0x6F, 0x63, 0x2F, 0x73, 0x65,
        0x6C, 0x66, 0x2F, 0x6D, 0x61, 0x70, 0x73, 0x00
    };// /proc/self/maps  
    
    int fd = 0;
#ifdef M_ARM64
    fd = dx_syscall(__NR_openat, 0, maps_str, O_RDONLY);
#elif M_ARM
    fd = dx_syscall(__NR_open, maps_str, O_RDONLY);
#elif M_X86
    fd = dx_syscall(__NR_open, maps_str, O_RDONLY);
# endif
//------------------------
    int t_fd = 0;
    if (fd < 0) {
        LOGD("open maps fail");
        return 0;   //open fail
    }
    
    char *line_hdr = NULL;
    
    int readsize = 0;
    char readch;
    int maxline = 200;
    char eachline[maxline];
    int index = 0;
    int libc_path_len = 0;

    uint_t libc_baddr = 0;
    char* lib_path = NULL;
    
    while((readsize=dx_syscall(__NR_read, fd, &readch, 1))>0){
        eachline[index++] = readch;
        if(index >= maxline){
            index = 0;
        }
        if(readch=='\n' || readch==EOF){
            eachline[index] = '\0';
            if(index <= 21){
                index = 0;
                continue;
            }
            int i = 0;
            for(i = 0; i<index; i++){
                if(dx_memcmp(eachline+i, lib_name, lib_len) == 0){
                    line_hdr = eachline;
                    //parse base address
                    char hex[32];
                    int w = 0;
                    for (w = 0; w<sizeof(hex)/sizeof(hex[0]); w++)
                        hex[w] = 0;
                    for (w = 0; line_hdr[w] != '-'&& w < sizeof(hex)/sizeof(hex[0]); w++) {
                        hex[w] = line_hdr[w];
                    }
                    
                    unsigned long place = 1;
                    int j = 0;
                    for (j = w-1; j >= 0; j--) {
                        if (hex[j] >= '0' && hex[j] <= '9') {
                            libc_baddr += (hex[j] - '0') * place;
                        } else if (hex[j] >= 'a' && hex[j] <= 'f') {
                            libc_baddr += (hex[j] - 'a' + 10 ) * place;

                        }
                        place *= 0x10;
                    }
                    unsigned char* magic = (unsigned char*)libc_baddr;
                    if(magic[0] != 0x7f || magic[1] != 'E' || magic[2] != 'L' || magic[3] != 'F'){
                        line_hdr = NULL;
                        libc_baddr = 0;
                        break;
                    }
                    
#ifdef M_ARM
                    Elf32_Ehdr* ehdr = (Elf32_Ehdr*)libc_baddr;
                    if(ehdr->e_machine != EM_ARM){
                        line_hdr = NULL;
                        libc_baddr = 0;
                    }
#elif M_X86
                    Elf32_Ehdr* ehdr = (Elf32_Ehdr*)libc_baddr;
                    if(ehdr->e_machine != EM_386){
                        line_hdr = NULL;
                        libc_baddr = 0;
                    }
#else
                    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)libc_baddr;
                    if(ehdr->e_machine != EM_AARCH64){
                        line_hdr = NULL;
                        libc_baddr = 0;
                    }
#endif
                    break;
                }
            }
            if(line_hdr != NULL){
                int t = 0;
                for(t=0; t<index; t++){
                    if(lib_path == NULL && line_hdr[t] == '/'){
                        lib_path = &line_hdr[t];
                    }
                    if(line_hdr[t] == '\n'){
                        line_hdr[t] = '\0';
                    }
                }
                break;
            }
            index = 0;
        }
    }

    if (line_hdr == NULL || lib_path == NULL) {
        // LOGD("can not find:%s", lib_path);
        dx_syscall(__NR_close, fd);  //can not find lib
        return 0;
    }


#ifdef M_ARM64
    fd = dx_syscall(__NR_openat, 0, lib_path, O_RDONLY);
#elif M_ARM
    fd = dx_syscall(__NR_open, lib_path, O_RDONLY);
#elif M_X86
    fd = dx_syscall(__NR_open, lib_path, O_RDONLY);
# endif

    if (fd < 0 ) return NULL;

    long len = dx_syscall(__NR_lseek, fd, 0, SEEK_END);


    dx_syscall(__NR_lseek, fd, 0, SEEK_SET);

    void* buffer = NULL;
#ifdef M_ARM64
    buffer = (void*)dx_syscall(__NR_mmap, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
#elif M_ARM
    buffer = (void*)dx_syscall(__NR_mmap2, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
#elif M_X86
    buffer = (void*)dx_syscall(__NR_mmap2, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
#endif
/*
    void *buffer = (void*)_syscall(__NR_mmap, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
*/

    if (buffer == NULL) {
        dx_syscall(__NR_close, fd);
        return NULL;
    }
    
    long readBytes = dx_syscall(__NR_read, fd, buffer, len);
 
    if (readBytes != len) {
        //LOGE("read libc so failed!\n");
        dx_syscall(__NR_close, fd);
        return NULL;
    }

    dx_syscall(__NR_close, fd);
    
#ifdef M_ARM64
    fd = dx_syscall(__NR_openat, 0, lib_path, O_RDONLY);
#elif M_ARM
    fd = dx_syscall(__NR_open, lib_path, O_RDONLY);
#elif M_X86
    fd = dx_syscall(__NR_open, lib_path, O_RDONLY);
# endif

    uint_t addr = 0;
    
    Elf_Ehdr *elfHeader = (Elf_Ehdr *)buffer;
    Elf_Shdr *elfShdr = (Elf_Shdr*)((unsigned char *)elfHeader + elfHeader->e_shoff);
    //LOGE("elfHeader->e_shoff = %x, elfShdr = %x\n", elfHeader->e_shoff, elfShdr);

    //find section name table
    Elf_Shdr *elfSecNameHdr = &elfShdr[elfHeader->e_shstrndx];
    //LOGE("e_shstrndx = %d\n", elfHeader->e_shstrndx);
    
    Elf_Sym *dynsym_addr = NULL;
    int dynsym_num = 0;
    Elf_Off dynstr_offset = 0;
    int find_dyn = 0;
    unsigned char dynsym_str[] = {
        0x2E, 0x64, 0x79, 0x6E, 0x73, 0x79, 0x6D, 0x00
    };//.dynsym
    unsigned char dynstr_str[] = {
        0x2E, 0x64, 0x79, 0x6E, 0x73, 0x74, 0x72, 0x00
    };//.dynstr

    Elf_Sym *symtab_addr = NULL;
    int symtab_num = 0;
    Elf_Off symtab_offset = 0;
    int find_sym = 0;
    unsigned char symtab_str[] = {
        0x2E, 0x73, 0x79, 0x6D, 0x74, 0x61, 0x62, 0x00
    }; //.symtab
    unsigned char strtab_str[] = {
        0x2E, 0x73, 0x74, 0x72, 0x74, 0x61, 0x62, 0x00
    }; //.strtab
    int i=0;
    for (i = 0; i < elfHeader->e_shnum; i++) {
        char *secName = (char *)elfHeader +  elfSecNameHdr->sh_offset + elfShdr[i].sh_name;
        //LOGE("secName = %x\n", elfShdr[i].sh_name);
        //LOGE("sec name = %s\n", secName);
        if (dx_memcmp(secName, dynsym_str, 7) == 0 && elfShdr[i].sh_type == SHT_DYNSYM) {
            dynsym_addr = (Elf_Sym *)((unsigned long)elfHeader + elfShdr[i].sh_offset);
            dynsym_num = elfShdr[i].sh_size / sizeof(Elf_Sym);
            find_dyn += 1;
        }
        if (dx_memcmp(secName, dynstr_str, 7) == 0 && elfShdr[i].sh_type == SHT_STRTAB) {
            dynstr_offset = elfShdr[i].sh_offset;
            find_dyn += 1;
        }

        if (dx_memcmp(secName, symtab_str, 7) == 0 && elfShdr[i].sh_type == SHT_SYMTAB) {
            symtab_addr = (Elf_Sym *)((unsigned long)elfHeader + elfShdr[i].sh_offset);
            symtab_num = elfShdr[i].sh_size / sizeof(Elf_Sym);
            find_sym += 1;
        }

        if (dx_memcmp(secName, strtab_str, 7) == 0 && elfShdr[i].sh_type == SHT_STRTAB) {
            symtab_offset = elfShdr[i].sh_offset;
            find_sym += 1;
        }

        if (find_dyn == 2 && find_sym == 2)  break;
    }
    
    uint_t fun_addr = 0;

    if (find_dyn ==2) {
        for (i = 0; i < dynsym_num; i++) {
            char *funcName = (char *)elfHeader + dynstr_offset + dynsym_addr[i].st_name;
            //LOGD("fun_name:%s", funcName);
            if (dx_memcmp(funcName, fun_name, name_len) == 0) {
                fun_addr = dynsym_addr[i].st_value;
                //LOGE("func name = %s, addr = %llX\n", funcName, fun_addr);
                break;
            }
        }
    }
    if(fun_addr == 0 && find_sym == 2){
        for (i = 0; i < symtab_num; i++) {
            char *funcName = (char *)elfHeader + symtab_offset + symtab_addr[i].st_name;
            //LOGD("fun_name:%s", funcName);
            if (dx_memcmp(funcName, fun_name, name_len) == 0) {
                fun_addr = symtab_addr[i].st_value;
                //LOGE("func name = %s, addr = %llX\n", funcName, fun_addr);
                break;
            }
        }
    } 

    dx_syscall(__NR_munmap, buffer, len);

    //LOGD("[%s] libc_baddr:%p, fun_addr:%p", fun_name, libc_baddr, fun_addr);

    if(fun_addr == 0){
        LOGE("can not find fun: %s", fun_name);
        return 0;
    }

    return (void*)(libc_baddr + fun_addr);
}

// #ifdef M_ARM64
//     t_fd = dx_syscall(__NR_openat, 0, maps_str, O_RDONLY);

// #elif M_ARM
//     t_fd = dx_syscall(__NR_open, "/data/local/tmp/test.tmp", O_APPEND | O_CREAT | O_RDWR);
//     dx_syscall(__NR_write, t_fd, "dddd", 4); // /system/lib/arm/libc.so
//     // dx_syscall(__NR_write, t_fd, &fun_addr, sizeof(fun_addr));
//     dx_syscall(__NR_close, t_fd);

// #elif M_X86
//     t_fd = dx_syscall(__NR_open, maps_str, O_RDONLY);
// #endif