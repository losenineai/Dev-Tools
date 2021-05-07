
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
#include "../../utils/elf.h"


#ifndef PAGE_SIZE
#define PAGE_SIZE   4096
#endif

#define   LOG_TAG    "windy"
#define   LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define   LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


typedef struct _thread_param {
    uint64_t base; //libc.so address
    JavaVM *vm;
#ifdef M_ARM64
    uint64_t syscall;// syscall address
#else
    uint32_t syscall;
#endif
}thread_param;

void deadcall();

extern long dx_syscall(int n,...);

extern void* dx_libc_call(char* callfunName, int name_n);

extern size_t find_uchar(const char* buffer, size_t buff_len, const char *str, size_t str_len);

extern int _memcmp(const void* s1, const void* s2,size_t n);

void dx_init();

void* checkIntegrity(void *param);

typedef long (*dx_pthread_create)(pthread_t *tid, const pthread_attr_t *tattr, void*(*start_routine)(void *), void *arg);

void dx_init() {

/////don't change code here!!!!, make sure the immediate "0x1c"right, in instruction "SUB %0, %0, #0x1c"
    void (*OrigInit)() = 0;
    long (*pdx_syscall)(int n,...) = 0;

    void* (*pdx_checkIntegrity)(void *param) = checkIntegrity;

    void* (*pdx_libc_call)(char* callfunName, int name_n) = dx_libc_call;

    unsigned int offset = 0;

#ifdef M_ARM64
    asm( 
    "XLABEL:\n"
	"\tADR %0, XLABEL\n"
    "\tSUB %0, %0, #0x10\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad
    "\tLDR %1, Curfun2Init\n"
    "\tLDR %2, Curfun2Syscall\n"   
    "\tLDR %3, Curfun2CheckIntegrity\n"  
    "\tADD %2, %2, %0\n"    
    "\tADD %3, %0, %3\n"
    "\tSUB %0, %0, %1\n"
    "\tB NEXT1\n"
    "Curfun2Init: .DWORD 0xABABAB10\n"
    "Curfun2Syscall: .DWORD 0xABABAB11\n"
    "Curfun2CheckIntegrity: .DWORD 0xABABAB12\n"
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkIntegrity)
    );
#elif M_ARM
    asm( 
    "\tMOV %0, PC\n"
	"\tSUB %0, #0x8\n"
    "\tLDR %1, Curfun2Init\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad
    "\tLDR %2, Curfun2Syscall\n"   
    "\tLDR %3, Curfun2CheckIntegrity\n"  
    "\tADD %2, %0\n"    
    "\tADD %3, %0, %3\n"
    "\tADD %3, #1\n"   //function checkIntegrity's first instruction is thumb after compiled, 
    
    "\tSUB %0, %1\n"
    "\tADD %0, #1\n"     //function OrigInit's first instruction maybe thumb after compiled, 
    "\tB NEXT1\n"  
    "Curfun2Init: .WORD 0xABABAB10\n"
    "Curfun2Syscall: .WORD 0xABABAB11\n"
    "Curfun2CheckIntegrity: .WORD 0xABABAB12\n"  
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkIntegrity)
    );
#else
    asm __volatile__( 
    "\tcall xlabel\n"
    "\txlabel:\n"
    "\tpop %0\n"    // %0 will store this instruction's address
	"\tsub $0x18, %0\n"
    "\tmov $0xABABAB10, %1\n"
    "\tmov $0xABABAB11, %2\n"
    "\tmov $0xABABAB12, %3\n"
    "\tadd %0, %2\n"
    "\tadd %0, %3\n"
    "\tsub %1, %0\n"
    
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkIntegrity)
    );
#endif
/////////////////////////////////////////////////////////////////////////////////////////
    
    if(offset != 0){
        OrigInit();
    }

    //pdx_checkIntegrity = checkIntegrity;

    //pdx_syscall = dx_syscall;
    //LOGE("libc base address = %llX\n",libc_baddr);

///////////////////////////////////////////////////////////////////////////
//  set thread params
///////////////////////////////////////////////////////////////////////////
#ifdef M_ARM64
    thread_param *tparam = (thread_param*)pdx_syscall(__NR_mmap, NULL,
                                        PAGE_SIZE * 2,
                                        PROT_READ | PROT_WRITE ,
                                        MAP_PRIVATE | MAP_ANONYMOUS ,//| MAP_GROWSDOWN ,
                                        -1,
                                        0);
#else
    thread_param *tparam = (thread_param*)pdx_syscall(__NR_mmap2, NULL,
                                        PAGE_SIZE * 2,
                                        PROT_READ | PROT_WRITE ,
                                        MAP_PRIVATE | MAP_ANONYMOUS ,//| MAP_GROWSDOWN ,
                                        -1,
                                        0);
#endif

    tparam->vm = (JavaVM*)NULL;
    tparam->base = 0;//libc_baddr;

#ifdef M_ARM64
    tparam->syscall = (uint64_t)pdx_syscall;
#else
    tparam->syscall = (uint32_t)pdx_syscall;
#endif
    //LOGE("libc base = %p\n",tparam->base);
    unsigned char PTHREADCREATE[] = {
        0x70, 0x74, 0x68, 0x72, 0x65, 0x61, 0x64, 0x5F,
        0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x00
    }; //pthread_create
    
    dx_pthread_create my_pthread_create = (dx_pthread_create)pdx_libc_call(PTHREADCREATE, 15);
    //LOGE("my_pthread_create = %p\n", my_pthread_create);
    if(my_pthread_create == NULL){
        return;
    }
    pthread_t  tid;
    //LOGE("start create thread ...\n");
    my_pthread_create(&tid, NULL, pdx_checkIntegrity, tparam);

    //return;
    //return result;
}


/**
 * 校验APP 包名和签名是否合法 返回值为1 表示合法
 */
//__attribute ((visibility("hidden")))
void* checkIntegrity(void *param) {

    thread_param tp;

    if (param == NULL) return NULL;

    thread_param *ptp = (thread_param *)param;

    tp.base = ptp->base;
    tp.vm = ptp->vm;
    tp.syscall = ptp->syscall;
#ifdef M_ARM64
    long (*pdx_syscall)(int n,...) = (void*)tp.syscall;
#else 
    int (*pdx_syscall)(int n,...) = (void*)tp.syscall;
#endif

    pdx_syscall(__NR_munmap, ptp, PAGE_SIZE * 2);

    void* so_name = 0;
    unsigned int orighashcode = 0;
    unsigned int hashcodeindex = 0;
    unsigned int sonamelen = 0; 
    //unsigned 
    
#ifdef M_ARM64
	asm (
		"LDR %0, HASHVAL\n"
        "LDR %1, HASHCODEINDEX\n"
        "LDR %2, SONAMELEN\n"
        "ADR %3, SONAME\n"
		"B NEXT\n"
		"HASHVAL: .DWORD 0x5F2E082B\n"
        "HASHCODEINDEX: .DWORD 0x5F2E083B\n"
        "SONAMELEN: .DWORD 0x5F2E083C\n"
        "SONAME: .DWORD 0x5F2E083D\n"
        "SONAME1: .DWORD 0x00\n"
        "SONAME2: .DWORD 0x00\n"
        "SONAME3: .DWORD 0x00\n"
        "SONAME4: .DWORD 0x00\n"
        "SONAME5: .DWORD 0x00\n"
        "SONAME6: .DWORD 0x00\n"
        "SONAME7: .DWORD 0x00\n"
		"NEXT:\n"
		:"=r"(orighashcode),"=r"(hashcodeindex),"=r"(sonamelen),"=r"(so_name)
		:
		);
#elif M_ARM
    asm (
		"LDR %0, HASHVAL\n"
        "LDR %1, HASHCODEINDEX\n"
        "LDR %2, SONAMELEN\n"
        "ADR %3, SONAME\n"
		"B NEXT\n"
		"HASHVAL: .WORD 0x5F2E082B\n"
        "HASHCODEINDEX: .WORD 0x5F2E083B\n"
        "SONAMELEN: .WORD 0x5F2E083C\n"
        "SONAME: .WORD 0x5F2E083D\n"
        "SONAME1: .WORD 0x00\n"
        "SONAME2: .WORD 0x00\n"
        "SONAME3: .WORD 0x00\n"
        "SONAME4: .WORD 0x00\n"
        "SONAME5: .WORD 0x00\n"
        "SONAME6: .WORD 0x00\n"
        "SONAME7: .WORD 0x00\n"
        "SONAME8: .WORD 0x00\n"
        "SONAME9: .WORD 0x00\n"
		"NEXT:\n"
		:"=r"(orighashcode),"=r"(hashcodeindex),"=r"(sonamelen),"=r"(so_name)
		:
		);
#else
    asm __volatile__ (
		"mov $0x5F2E082B, %0\n"
        "mov $0x5F2E083B, %1\n"
        "mov $0x5F2E083C, %2\n"
        "call xlabel1\n"
        "xlabel1:\n"
        "pop %3\n"    // %0 will store this instruction's address
        "add $0x6, %3\n"
        "jmp NEXT1\n"
        "SONAME: .WORD 0x083D\n"
        "SONAME1: .WORD 0x5F2E\n"
        "SONAME2: .WORD 0x00\n"
        "SONAME3: .WORD 0x00\n"
        "SONAME4: .WORD 0x00\n"
        "SONAME5: .WORD 0x00\n"
        "SONAME6: .WORD 0x00\n"
        "SONAME7: .WORD 0x00\n"
        "SONAME8: .WORD 0x00\n"
        "SONAME9: .WORD 0x00\n"
		"NEXT1:\n"
		:"=r"(orighashcode),"=r"(hashcodeindex),"=r"(sonamelen),"=r"(so_name)
		:
		);
#endif

    if(sonamelen > 40 ){
        return NULL;
    }
    
    //数组大小要以单数，否则在编译的时候会将数组内容放在.rodata，然后再通过指令去读取，这个指令会涉及对齐问题，可能会出现运行失败。
    unsigned char MAPSTR[] = {
        0x2F, 0x70, 0x72, 0x6F, 0x63, 0x2F, 0x73, 0x65,
        0x6C, 0x66, 0x2F, 0x6D, 0x61, 0x70, 0x73, 0x00,
        0x00
    };// /proc/self/maps

#ifdef M_ARM64
    int fd = pdx_syscall(__NR_openat, 0, MAPSTR, O_RDONLY);
#else 
    int fd = pdx_syscall(__NR_open, MAPSTR, O_RDONLY);
#endif

    if (fd < 0) {
        return NULL;
    }
    
   
    char *line_hdr = NULL;
    int readsize = 0;
    char readch;
    int maxline = 300;
    char eachline[maxline];
    int index = 0;
    
    while((readsize=pdx_syscall(__NR_read, fd, &readch, 1))>0){
        eachline[index++] = readch;
        if(index >= maxline){
            index = 0;
        }
        if(readch=='\n' || readch==EOF){
            eachline[index-1] = '\0';
            if(index <= 21){
                index = 0;
                continue;
            }
            
            //LOGD("each line : %s",eachline);
            int i = 0;
            for(i = 0; i<index; i++){
                
                if(dx_memcmp((const char*)(eachline+i), so_name, sonamelen) == 0){
                    line_hdr = eachline;
                    break;
                }
                
            }
            if(line_hdr != NULL){
                break;
            }
            index = 0;
        }
    }  
  
    if (line_hdr == NULL) {
        //LOGE("%s not found\n", LIBCSTR);
        pdx_syscall(__NR_close, fd);
        deadcall();
        /*
        asm (
		".DWORD 0xdeaddead\n"
		);
        */
        //return 0;
    }
    
    unsigned char* so_path = NULL;
    int i = 0;
    for(i=0; i<300; i++){
        if(*(line_hdr+i) == '/'){
            so_path = line_hdr+i;
            break;
        }
    }

    if(so_path == NULL){
        deadcall();
        /*
        asm (
		".DWORD 0xdeaddead\n"
		);
        */
    }

#ifdef M_ARM64
    fd = pdx_syscall(__NR_openat, 0, so_path, O_RDONLY);
#else
    fd = pdx_syscall(__NR_open, so_path, O_RDONLY);
#endif

    if (fd<0) {return NULL;}
    
    long len = pdx_syscall(__NR_lseek, fd, 0, SEEK_END);

    pdx_syscall(__NR_lseek, fd, 0, SEEK_SET);

    //LOGD("fd : %d\n", fd);
#ifdef M_ARM64
    unsigned char *buffer = (unsigned char*)pdx_syscall(__NR_mmap, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
#else
    unsigned char *buffer = (unsigned char*)pdx_syscall(__NR_mmap2, NULL,
                        (size_t)len,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
                        -1,
                        0);
#endif
    //LOGD("mmaping buffer\n");

    if (buffer == NULL) {
        //LOGE("map buffer failed!\n");
        pdx_syscall(__NR_close, fd);
        return NULL;
    }
    
    long readBytes = pdx_syscall(__NR_read, fd, buffer, len);
    
    if (readBytes != len) {
        //LOGE("read libc so failed!\n");
        pdx_syscall(__NR_close, fd);
        return NULL;
    }

    pdx_syscall(__NR_close, fd);


    //calculate hash value
    readsize = 0;
    index = 0;

    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    
    for(index=0; index<len-4; index++){
        hash = hash * a + (unsigned char)buffer[index];
        a *= b;
    }

    hash = (hash & 0x7FFFFFFF);

    orighashcode = *(unsigned int*)(buffer+index);
    pdx_syscall(__NR_munmap, buffer, len);
    
    
    //0x5F2E081B
    if (hash != orighashcode) {
        deadcall();
        /*
        asm (
		".WORD 0xdeaddead\n"
		);
        */
    }

    return NULL;
}

/*
int main() {
    printf("p main begin...\n");
    
    dx_init();
    getchar();
    printf("p end...\n");
    return 0;
}
*/