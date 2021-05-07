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
#include "elf.h"



// #define PAGE_SIZE   4096

// inline int dx_memcmp(const void* s1, const void* s2,size_t n) __attribute__((always_inline));

// inline
// int dx_memcmp(const void* s1, const void* s2,size_t n) {
//     const unsigned char *p1 = (const unsigned char *)s1, *p2 = (const unsigned char *)s2;
//     while(n--)
//         if( *p1 != *p2 )
//             return *p1 - *p2;
//         else
//             p1++,p2++;
//     return 0;
// }


// size_t find_uchar(const char* buffer, size_t buff_len, const char *str, size_t str_len) {
//     if (buff_len < str_len) {
//         return (size_t)-1;
//     }
//     size_t i = 0;
//     for (; i < buff_len - str_len + 1; i++) {
//         if (memcmp(buffer + i, str, str_len) == 0) {
//             return i;
//         }
//     }
//     return (size_t)-1;
// }


extern void* get_function(const char* lib_path, int lib_len, const char* fun_name, int name_len);


void* dx_libc_call(char* callfunName, int name_n){
///////////////////////////////////////////////////////////////////////////
//  get base address of libc
///////////////////////////////////////////////////////////////////////////
// #ifdef M_ARM64
//     unsigned char LIBCSTR[] = {
//         0x2F, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2F,
//         0x6C, 0x69, 0x62, 0x36, 0x34, 0x2F, 0x6C, 0x69,
//         0x62, 0x63, 0x2E, 0x73, 0x6F, 0x00
//     }; //system/lib64/libc.so
//     int lib_length = 21;
// #else 
//     unsigned char LIBCSTR[] = {
//         0x2F, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2F,
//         0x6C, 0x69, 0x62, 0x2F, 0x6C, 0x69, 0x62, 0x63, 
//         0x2E, 0x73, 0x6F, 0x00
//     }; //system/lib/libc.so
//     int lib_length = 19;
// #endif
    unsigned char LIBCSTR[] = {
        0x6C, 0x69, 0x62, 0x63, 0x2E, 0x73, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    int lib_length = 7;
    return get_function(LIBCSTR, lib_length, callfunName, name_n);


//     long (*pdx_syscall)(int n,...) = 0;
//     pdx_syscall = dx_syscall;

//     unsigned char MAPSTR[] = {
//         0x2F, 0x70, 0x72, 0x6F, 0x63, 0x2F, 0x73, 0x65,
//         0x6C, 0x66, 0x2F, 0x6D, 0x61, 0x70, 0x73, 0x00
//     };


// ////get the size of maps    
    
// //------------------------
//     int fd = pdx_syscall(__NR_openat, 0, MAPSTR, O_RDONLY);
//     if (fd < 0) {
//         //LOGE("open /proc/self/maps error!\n");
//         return 0;
//     }
    
    
//     char *line_hdr = NULL;
//     unsigned char LIBCSTR[] = {
//         0x2F, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2F,
//         0x6C, 0x69, 0x62, 0x36, 0x34, 0x2F, 0x6C, 0x69,
//         0x62, 0x63, 0x2E, 0x73, 0x6F, 0x00
//     }; //system/lib64/libc.so

    
//     int readsize = 0;
//     char readch;
//     int maxline = 200;
//     char eachline[maxline];
//     int index = 0;
//     while((readsize=pdx_syscall(__NR_read, fd, &readch, 1))>0){
//         eachline[index++] = readch;
//         if(index >= maxline){
//             index = 0;
//         }
//         if(readch=='\n' || readch==EOF){
//             eachline[index] = '\0';
//             if(index <= 21){
//                 index = 0;
//                 continue;
//             }
//             //LOGD("each line : %s",eachline);
//             int i = 0;
//             for(i = 0; i<index; i++){
//                 if(dx_memcmp(eachline+i, LIBCSTR, 21) == 0){
//                     line_hdr = eachline;
//                     break;
//                 }
//             }
//             if(line_hdr != NULL){
//                 break;
//             }
//             index = 0;
//         }
//     }  
    
//     if (line_hdr == NULL) {
//         //LOGE("%s not found\n", LIBCSTR);
//         pdx_syscall(__NR_close, fd);
//         return 0;
//     }
    
//     //parse base address
//     char hex[32];
//     int i = 0;
//     for (i = 0; i<sizeof(hex)/sizeof(hex[0]); i++)
//         hex[i] = 0;
//     for (i = 0; line_hdr[i] != '-'&& i < sizeof(hex)/sizeof(hex[0]); i++) {
//         hex[i] = line_hdr[i];
//     }
//     //LOGE("base = %s, len = %d\n", hex, i);
//     uint64_t libc_baddr = 0;
//     unsigned long place = 1;
//     int j = 0;
//     for (j = i-1; j >= 0; j--) {
//         if (hex[j] >= '0' && hex[j] <= '9') {
//             libc_baddr += (hex[j] - '0') * place;
//         } else if (hex[j] >= 'a' && hex[j] <= 'f') {
//             libc_baddr += (hex[j] - 'a' + 10 ) * place;

//         }
//         place *= 0x10;
//     }
//     pdx_syscall(__NR_close, fd);
//     //LOGE("libc base address = %llX\n",libc_baddr);

// ///////////////////////////////////////////////////////////////////////////
// //  set thread params
// ///////////////////////////////////////////////////////////////////////////
// /*    
//     thread_param *tparam = (thread_param*)pdx_syscall(__NR_mmap, NULL,
//                                         PAGE_SIZE * 2,
//                                         PROT_READ | PROT_WRITE ,
//                                         MAP_PRIVATE | MAP_ANONYMOUS ,//| MAP_GROWSDOWN ,
//                                         -1,
//                                         0);
//     tparam->vm = vm;
//     tparam->base = libc_baddr;
//     tparam->syscall = pdx_syscall;
// */
//     //LOGE("libc base = %p\n",tparam->base);

// ///////////////////////////////////////////////////////////////////////////
// //  find symbols addresses of libc

//     fd = pdx_syscall(__NR_openat, 0 , LIBCSTR, O_RDONLY);
//     if (fd < 0 ) return NULL;
    
//     long len = pdx_syscall(__NR_lseek, fd, 0, SEEK_END);

//     pdx_syscall(__NR_lseek, fd, 0, SEEK_SET);

//     //LOGD("fd : %d\n", fd);
//     void *buffer = pdx_syscall(__NR_mmap, NULL,
//                         (size_t)len,
//                         PROT_READ | PROT_WRITE,
//                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
//                         -1,
//                         0);
//     //LOGD("mmaping buffer\n");

//     if (buffer == NULL) {
//         //LOGE("map buffer failed!\n");
//         pdx_syscall(__NR_close, fd);
//         return NULL;
//     }
    
//     //LOGD("buffer len : %ld\n", len);

//     long readBytes = pdx_syscall(__NR_read, fd, buffer, len);
    

//     if (readBytes != len) {
//         //LOGE("read libc so failed!\n");
//         pdx_syscall(__NR_close, fd);
//         return NULL;
//     }

//     pdx_syscall(__NR_close, fd);
//     uint64_t addr = 0;
    
//     Elf64_Ehdr *elfHeader = (Elf64_Ehdr *)buffer;
//     Elf64_Shdr *elfShdr = (Elf64_Shdr*)((unsigned char *)elfHeader + elfHeader->e_shoff);
//     //LOGE("elfHeader->e_shoff = %x, elfShdr = %x\n", elfHeader->e_shoff, elfShdr);

//     //find section name table
//     Elf64_Shdr *elfSecNameHdr = &elfShdr[elfHeader->e_shstrndx];
//     //LOGE("e_shstrndx = %d\n", elfHeader->e_shstrndx);
    
//     Elf64_Sym *dynSymTable = NULL;
//     int numOfSymbols = 0;
//     Elf64_Off dynSymStrOffset = 0;
//     int find = 0;

//     unsigned char DYNSYM[] = {
//         0x2E, 0x64, 0x79, 0x6E, 0x73, 0x79, 0x6D, 0x00
//     };//.dynsym

//     unsigned char DYNSTR[] = {
//         0x2E, 0x64, 0x79, 0x6E, 0x73, 0x74, 0x72, 0x00
//     };//.dynstr

//     for (i = 0; i < elfHeader->e_shnum; i++) {
//         char *secName = (char *)elfHeader +  elfSecNameHdr->sh_offset + elfShdr[i].sh_name;
//         //LOGE("secName = %x\n", elfShdr[i].sh_name);
//         //LOGE("sec name = %s\n", secName);
//         if (dx_memcmp(secName, DYNSYM, 7) == 0 && elfShdr[i].sh_type == SHT_DYNSYM) {
//             dynSymTable = (Elf64_Sym *)((unsigned long)elfHeader + elfShdr[i].sh_offset);
//             numOfSymbols = elfShdr[i].sh_size / sizeof(Elf64_Sym);
//             find += 1;
//         }
//         if (dx_memcmp(secName, DYNSTR, 7) == 0 && elfShdr[i].sh_type == SHT_STRTAB) {
//             dynSymStrOffset = elfShdr[i].sh_offset;
//             find += 1;
//         }
//         if (find == 2)  break;
//     }

//     uint64_t pthread_create_addr = 0;

// /*
//     unsigned char PTHREADCREATE[] = {
//         0x70, 0x74, 0x68, 0x72, 0x65, 0x61, 0x64, 0x5F,
//         0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x00
//     }; //pthread_create
// */
//     if (find ==2) {
//         for (i = 0; i < numOfSymbols; i++) {
//             //if (dynSymTable[i].st_info == 0x12) {
//                 char *funcName = (char *)elfHeader + dynSymStrOffset + dynSymTable[i].st_name;
//                 if (dx_memcmp(funcName, callfunName, name_n) == 0) {
//                     pthread_create_addr = dynSymTable[i].st_value;
//                     //LOGE("func name = %s, addr = %llX\n", funcName, pthread_create_addr);
//                     break;
//                 }
//             //}
//         }
//     }
    
//     pdx_syscall(__NR_munmap, buffer, len);

//     return (void*)(libc_baddr + pthread_create_addr);
    //dx_pthread_create my_pthread_create = (dx_pthread_create)(tparam->base + pthread_create_addr);
}