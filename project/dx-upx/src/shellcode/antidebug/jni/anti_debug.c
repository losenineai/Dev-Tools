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
#include <sys/times.h>
#include "../../utils/elf.h"

void deadcall();

extern long dx_syscall(int n,...);

static inline void clockCheck();

static inline void parentsCheck();

static inline void port23946bytcpCheck();

extern int dx_memcmp(const void* s1, const void* s2,size_t n);

typedef long (*dx_pthread_create)(pthread_t *tid, const pthread_attr_t *tattr, void*(*start_routine)(void *), void *arg);

extern void* dx_libc_call(char* callfunName, int name_n);

void* antidebug(void* arg);

void dx_init(){
    void (*OrigInit)() = 0;
    void* (*pdx_antidebug)() = NULL;
    unsigned int offset = 0;

#ifdef M_ARM64  
    asm( 
    "XLABEL:\n"
	"\tADR %0, XLABEL\n"
    "\tSUB %0, %0, #0x0c\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad
    "\tLDR %1, Curfun2Init\n"
    "\tLDR %2, Curfun2antidebug\n"
    "\tADD %2, %0, %2\n"
    "\tSUB %0, %0, %1\n"
    "\tB NEXT1\n"
    "Curfun2Init: .DWORD 0xABABAB10\n"
    "Curfun2antidebug: .DWORD 0xABABAB11\n"
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_antidebug)
    );
#elif M_ARM
    asm( 
    "\tMOV %0, PC\n"
    "\tSUB %0, %0, #0x06\n"   
    "\tLDR %1, Curfun2Init\n"
    "\tLDR %2, Curfun2antidebug\n"
    "\tADD %2, %0, %2\n"
    "\tSUB %0, %0, %1\n"
    "\tADD %0, #1\n"
    "\tADD %2, #1\n"
    "\tB NEXT1\n"
    "Curfun2Init: .WORD 0xABABAB10\n"
    "Curfun2antidebug: .WORD 0xABABAB11\n"
    "NEXT1:\n" 
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_antidebug)
    );
#else
    asm( 
    "\tcall xlabel\n"
    "\txlabel:\n"
    "\tpop %0\n"
    "\tSUB $0x6, %0\n"   
    "\tmov $0xABABAB10, %1\n"
    "\tmov $0xABABAB11, %2\n"
    "\tADD %0, %2\n"
    "\tSUB %1, %0\n"
    :"=r"(OrigInit),"=r"(offset), "=r"(pdx_antidebug)
    );
#endif
/////////////////////////////////////////////////////////////////////////////////////////
    
    if(offset != 0){
        OrigInit();
    }

    //pdx_antidebug = antidebug;
    unsigned char PTHREADCREATE[] = {
        0x70, 0x74, 0x68, 0x72, 0x65, 0x61, 0x64, 0x5F,
        0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x00
    }; //pthread_create
    
    dx_pthread_create my_pthread_create = (dx_pthread_create)dx_libc_call(PTHREADCREATE, 15);

    if(my_pthread_create == NULL){
        return;
    }

    pthread_t  tid;
    
    my_pthread_create(&tid, NULL, pdx_antidebug, NULL);

    //pthread_join(tid, NULL);

    return;
}

inline void* antidebug(void* arg){

    long time1=0, time2=0;
    clockCheck(&time1);  
    parentsCheck();
    port23946bytcpCheck();
    
    clockCheck(&time2);
    
    if(time2-time1 > 100){
        //deadcall();
        
        asm(
            ".word 0xdeaa"
        );
        
    }

    return NULL;
}


inline void clockCheck(long* l_time){
    struct tms timebuf;
    *l_time = dx_syscall(__NR_times, &timebuf);
}

inline void parentsCheck(){
    
    char pid_str[10];
    long pid = dx_syscall(__NR_getppid);
    unsigned char strproc[] = {0x7c, 0x23, 0x21, 0x3c, 0x30, 0x7c}; // "/proc/" ^ 0x53
    unsigned char strcmdline[] = {0x7c, 0x30, 0x3e, 0x37, 0x3f, 0x3a, 0x3d, 0x36, 0x53}; // "/cmdline" ^ 0x53
    char strpidcmdline[20];

    //char strpidcmdline[20] = {0x7c, 0x23, 0x21, 0x3c, 0x30, 0x7c ,0x53};  // strpidcmdline=/proc/pid/cmdline  ^ 0x53
    int t=0,i=0;
    for(i=0; i<sizeof(strproc); i++){
        strpidcmdline[t++] = strproc[i] ^ 0x53;
    }
    
    i = 0;
    while(pid){
        pid_str[i++] = pid % 10 + '0';
        pid /= 10;
    }
    
    while(i){
        strpidcmdline[t++] = pid_str[i-1];
        i--;
    }
    for(i=0; i<sizeof(strcmdline); i++){
        strpidcmdline[t++] = strcmdline[i] ^ 0x53;
    }

#ifdef M_ARM64
    int fd = dx_syscall(__NR_openat, 0, strpidcmdline, O_RDONLY);
#else
    int fd = dx_syscall(__NR_open, strpidcmdline, O_RDONLY);
#endif

    if (fd < 0) {
        dx_syscall(__NR_close, fd);
        return;
    }


    int readsize = 0;
    unsigned char readstr[100];
    
    readsize=dx_syscall(__NR_read, fd, &readstr, 100);
    if(readsize <=0){
        dx_syscall(__NR_close, fd);
        return;
    }

    unsigned char zygote[]={
        3, 0, 30, 22, 13, 28
    }; //zygote ^ 0x79

    
    for(i=0; i<sizeof(zygote); i++){
        zygote[i] = zygote[i] ^ 0x79;
    }
    
    int k=0;
    int ret = 0;
    if (readsize < sizeof(zygote)) {
        dx_syscall(__NR_close, fd);
        // deadcall();s
        asm(
            ".word 0xdeab"
        );
    }

    for(k=0; k<readsize-sizeof(zygote); k++){
        ret = dx_memcmp(readstr+k , zygote, sizeof(zygote));
        if(ret == 0){
            break;
        }
    }
    
    dx_syscall(__NR_close, fd);
    
    if(ret != 0){
        // deadcall();
        asm(
            ".word 0xdeac"
        );
    }
}

inline void port23946bytcpCheck(){
    unsigned char strnet_tcp[14] = {
        0xf1, 0xae, 0xac, 0xb1, 0xbd, 0xf1, 0xb0, 0xbb, 0xaa, 0xf1, 0xaa, 0xbd, 0xae, 0xde
    };// /proc/net/tcp
    int i=0;
    for(i=0; i<sizeof(strnet_tcp); i++){
        strnet_tcp[i] = strnet_tcp[i] ^ 0xde;
        //printf("%x, ", strnet_tcp[i]);
    }

#ifdef M_ARM64   
    int fd = dx_syscall(__NR_openat, 0, strnet_tcp, O_RDONLY);
#else
    int fd = dx_syscall(__NR_open, strnet_tcp, O_RDONLY);
#endif
    if (fd < 0) {
        return;
    }
    
    unsigned char port[5] = {
        0x9b, 0xea, 0x96, 0xef, 0xae
    };  // 0x5d8a
    
    for(i=0; i<5; i++){
        port[i] = port[i] ^ 0xae;
    }
    
    int isfound = 0;
    int readsize = 0;
    char readch;
    int maxline = 300;
    char eachline[maxline];
    int index = 0;
    
    while((readsize=dx_syscall(__NR_read, fd, &readch, 1))>0){
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
            int i = 0;
            for(i = 0; i<index; i++){ 
                if(dx_memcmp((const char*)(eachline+i), port, 4) == 0){
                    isfound = 1;
                    break;
                }
            }
            if(isfound == 1){
                dx_syscall(__NR_close, fd);
                //deadcall();
                
                asm(
                    ".word 0xdead"
                );
                
            }
            index = 0;
        }
    }

    dx_syscall(__NR_close, fd);
  
    return;
}


/*
int main(){
    printf("main start\n");

    dx_init();

    //char a = getchar();
    //printf("getchar : %c\n", a);
    printf("main ret\n");
    return 0;
}
*/