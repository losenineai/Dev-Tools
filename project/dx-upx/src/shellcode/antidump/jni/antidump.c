
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



#ifndef PAGE_SIZE
#define PAGE_SIZE   4096
#endif

#define   LOG_TAG    "so_protector"
#define   LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define   LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define M_PROT_READ  1
#define M_PROT_WRITE 2
#define M_PROT_EXEC 4


typedef struct _thread_param {
    uint64_t base; //libc.so address
    JavaVM *vm;
    uint64_t syscall;// syscall address
}thread_param;


extern long dx_syscall(int n,...);

extern void* dx_libc_call(char* callfunName, int name_n);

#ifdef DEBUG
extern void* dx_liblog_call();
typedef void (*android_log)(int index, const char* tag, const char* format, ...);
#endif

void erase_phdrs() {

/////don't change code here!!!!, make sure the immediate "0x1c"right, in instruction "SUB %0, %0, #0x1c"
    void (*OrigInit)() = 0;
    void* pdx_ehdr = NULL;
    long (*pdx_syscall)(int n,...) = dx_syscall;

    unsigned int offset = 0;
    unsigned int origprot = 0;

#ifdef M_ARM64
    asm __volatile__(
    "XLABEL:\n"
    "\tADR %0, XLABEL\n"
	"\tSUB %0, %0, #0x18\n"
    "\tLDR %1, Curfun2Init\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad  
    "\tLDR %2, CurfunAddress\n"
    "\tLDR %3, OrigProt\n"  
    "\tSUB %2, %0, %2\n"

    "\tSUB %0, %0, %1\n"
    
    "\tB NEXT1\n"  
    "Curfun2Init: .DWORD 0xABABAB10\n"
    "CurfunAddress: .DWORD 0xABABAB11\n"
    "OrigProt: .DWORD 0xABABAB12\n"
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_ehdr), "=r"(origprot)
    );

#elif M_ARM
    asm __volatile__( 
    "\tMOV %0, PC\n"
	"\tSUB %0, #0x8\n"
    "\tLDR %1, Curfun2Init\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad  
    "\tLDR %2, CurfunAddress\n"
    "\tLDR %3, OrigProt\n"  
    "\tSUB %2, %0, %2\n"

    "\tSUB %0, %1\n"
    "\tADD %0, #1\n"     //function OrigInit's first instruction maybe thumb after compiled, 

    "\tB NEXT1\n"  
    "Curfun2Init: .WORD 0xABABAB10\n"
    "CurfunAddress: .WORD 0xABABAB11\n"
    "OrigProt: .WORD 0xABABAB12\n"
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_ehdr), "=r"(origprot)
    );
#else //x86
    unsigned int temp = 0;

    asm __volatile__( 
    "\tcall xlabel\n"
    "\txlabel:\n"
    "\tpop %0\n"
    "\tsub $0x18, %0\n"  
    "\tmov $0xABABAB10, %1\n"  //Curfun2Init
    "\tmov $0xABABAB11, %2\n"  //CurfunAddress
    "\tmov $0xABABAB12, %3\n"  //OrigProt
    
    "\tmov %0, %4\n"
    "\tsub %2, %4\n"
    "\tmov %4, %2\n"
    "\tsub %1, %0\n"
    
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_ehdr), "=r"(origprot), "=r"(temp)
    );
#endif
/////////////////////////////////////////////////////////////////////////////////////////
    
#ifdef DEBUG
    android_log dx_android_log = (android_log)dx_liblog_call();
    unsigned char log_msg[] = {
        0x2F, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2F,
        0x6C, 0x69, 0x62, 0x36, 0x34, 0x2F, 0x6C, 0x69,
        0x62, 0x6c, 0x6f, 0x67, 0x2e, 0x73, 0x6f, 0x3a, 
        0x25, 0x70, 0x00
    }; // /system/lib64/liblog.so:%p
    unsigned char log_tag[] = {
        0x73, 0x6f, 0x5f, 0x70, 0x72, 0x6f, 0x74, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x00
    };
    dx_android_log(ANDROID_LOG_INFO, log_tag, log_msg, OrigInit);
    //LOGD("anti-dump...");
#endif

    if(origprot > 0xffff){
        return;
    }

#ifdef M_ARM64
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*) pdx_ehdr;
#else 
    Elf32_Ehdr* ehdr = (Elf32_Ehdr*) pdx_ehdr;
#endif

    //erase ehdr
    unsigned char* e_ident = (unsigned char*)ehdr;
    if(e_ident[0] == 0x7f && e_ident[1] == 'E' && e_ident[2] == 'L' && e_ident[3] == 'F'){
#ifdef M_ARM64
        Elf64_Off ephoff = ehdr->e_phoff;
        Elf64_Half phnum = ehdr->e_phnum;
        Elf64_Phdr* phdr = (Elf64_Phdr*)((unsigned char*)ehdr + ephoff);

        int length = sizeof(Elf64_Ehdr)+sizeof(Elf64_Phdr)*phnum;
#else 
        Elf32_Off ephoff = ehdr->e_phoff;
        Elf32_Half phnum = ehdr->e_phnum;
        Elf32_Phdr* phdr = (Elf32_Phdr*)((unsigned char*)ehdr + ephoff);

        int length = sizeof(Elf32_Ehdr)+sizeof(Elf32_Phdr)*phnum;
#endif
        pdx_syscall(__NR_mprotect, ehdr, length, M_PROT_READ | M_PROT_WRITE);
        
        int i=0;
        for(i=0; i<16; i++){
            e_ident[i] = i;
        }
        ehdr->e_flags = 0;
        ehdr->e_type = 0;

        //erase phdrs
        for(i=0; i<phnum; i++){
            phdr[i].p_offset = 0;
            phdr[i].p_memsz = 0;
            phdr[i].p_filesz = 0;
        }

        int ret = pdx_syscall(__NR_mprotect, pdx_ehdr, length, origprot);
    }

    if(offset != 0){
        OrigInit();
    }

    asm __volatile__(
        "\tnop\n"
    );
    
}


// int main() {
//     printf("p main begin...\n");
//     // struct timespec ts;
//     // dx_syscall(__NR_clock_gettime, 0, &ts);
//     // printf("sizeof:%d, sizeof_timespec:%d, second:%x, nsecond:%x\n",sizeof(ts.tv_sec), sizeof(ts), ts.tv_sec, ts.tv_nsec);
//     // if(ts.tv_sec < 0xdeadde06){
//     //     printf("sec glt\n");
//     // }else{
//     //     printf("sec gbt\n");
//     // }
//     // //erase_phdrs();
//     // getchar();
//     printf("p end...\n");
// }
