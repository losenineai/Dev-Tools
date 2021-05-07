
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


#define   LOG_TAG    "dx-protect"
#define   LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define   LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define PAGE_SIZE   4096

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

void* checkSignature(void *param);

typedef long (*dx_pthread_create)(pthread_t *tid, const pthread_attr_t *tattr, void*(*start_routine)(void *), void *arg);

jint JNI_OnLoad(JavaVM *vm, void *reserved) {

/////don't change code here!!!!, make sure the immediate "0x1c"right, in instruction "SUB %0, %0, #0x1c"
    jint (*OriginalJNI_OnLoad)(JavaVM *vm, void *reserved) = 0;
    long (*pdx_syscall)(int n,...) = 0;

    void* (*pdx_checkSignature)(void *param) = checkSignature;

    void* (*pdx_libc_call)(char* callfunName, int name_n) = dx_libc_call;

    pdx_syscall = dx_syscall;
    unsigned int offset = 0;

#ifdef M_ARM64
    asm( 
    "XLABEL:\n"
	"\tADR %0, XLABEL\n"
    "\tSUB %0, %0, #0x18\n"   // have to be calcuate, offset from xlabel to JNI_OnLoad
    "\tLDR %1, OnLoad2OrigOnLoad\n"
    "\tLDR %2, OnLoad2Syscall\n"   
    "\tLDR %3, OnLoad2CheckSign\n"  
    "\tADD %2, %2, %0\n"    
    "\tADD %3, %0, %3\n"
    "\tSUB %0, %0, %1\n"
    "\tB NEXT1\n"
    "OnLoad2OrigOnLoad: .DWORD 0xABABABA0\n"
    "OnLoad2Syscall: .DWORD 0xABABABA1\n"
    "OnLoad2CheckSign: .DWORD 0xABABABA2\n"
    "NEXT1:\n" 
    :"=r"(OriginalJNI_OnLoad),"=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkSignature)
    );
#elif M_ARM
    asm(
    "\tMOV %0, PC\n"    //%0 = current address + 4
    "\tSUB %0, #0x8\n"     
    "\tLDR %1, OnLoad2OrigOnLoad\n"
    "\tLDR %2, OnLoad2Syscall\n"   
    "\tLDR %3, OnLoad2CheckSign\n"   
    "\tADD %2, %0\n"
    "\tADD %3, %0, %3\n"
    "\tADD %3, #1\n"      //function checkSignature's first instruction is thumb after compiled, 
    
    "\tSUB %0, %1\n"
    "\tADD %0, #1\n"     //function OriginalJNI_OnLoad's first instruction is thumb after compiled, 
    "\tB NEXT1\n"
    "OnLoad2OrigOnLoad: .WORD 0xABABABA0\n"
    "OnLoad2Syscall:    .WORD 0xABABABA1\n"
    "OnLoad2CheckSign:  .WORD 0xABABABA2\n"
    "NEXT1:\n"
    :"=r"(OriginalJNI_OnLoad),"=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkSignature)
    :
    );
#else
    asm __volatile__(
        "\tcall xlabel\n"
        "\txlabel:\n"
        "\tpop %0\n"    // %0 will store this instruction's address
        "\tsub $0x18, %0\n"   //calculate JNI_OnLoad's address
        "\tmov $0xabababa0, %1\n"
        "\tmov $0xabababa1, %2\n"
        "\tmov $0xabababa2, %3\n"
        "\tadd %0, %2\n"
        "\tadd %0, %3\n"
        "\tsub %1, %0\n"
    :"=r"(OriginalJNI_OnLoad), "=r"(offset), "=r"(pdx_syscall), "=r"(pdx_checkSignature)
    :
    );
#endif
/////////////////////////////////////////////////////////////////////////////////////////

    jint result = OriginalJNI_OnLoad(vm, reserved);

    //LOGE("libc base address = %llX\n",libc_baddr);

///////////////////////////////////////////////////////////////////////////
//  set thread params
///////////////////////////////////////////////////////////////////////////
#ifdef M_ARM64
    thread_param *tparam = (thread_param*)pdx_syscall(__NR_mmap, NULL,
                                        PAGE_SIZE,
                                        PROT_READ | PROT_WRITE ,
                                        MAP_PRIVATE | MAP_ANONYMOUS ,//| MAP_GROWSDOWN ,
                                        -1,
                                        0);
#else
    thread_param *tparam = (thread_param*)pdx_syscall(__NR_mmap2, NULL,
                                        PAGE_SIZE,
                                        PROT_READ | PROT_WRITE ,
                                        MAP_PRIVATE | MAP_ANONYMOUS ,//| MAP_GROWSDOWN ,
                                        -1,
                                        0);
#endif

    tparam->vm = (JavaVM*)vm;
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
        return result;
    }
    pthread_t  tid;
    //LOGE("start create thread ...\n");
    my_pthread_create(&tid, NULL, pdx_checkSignature, tparam);
    return result;
}


/**
 * 校验APP 包名和签名是否合法 返回值为1 表示合法
 */
//__attribute ((visibility("hidden")))
void* checkSignature(void *param) {

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

    pdx_syscall(__NR_munmap, ptp, PAGE_SIZE);

    JNIEnv* env = NULL;

    (*tp.vm)->AttachCurrentThread(tp.vm, &env, NULL);
    if (env == NULL) {
        //LOGE("AttachCurrentThread failed!\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    //获取Activity Thread的实例对象
    //static const char v1[] = {
    unsigned char v1[] = { 
        0x61, 0x6E, 0x64, 0x72, 0x6F, 0x69, 0x64, 0x2F,
        0x61, 0x70, 0x70, 0x2F, 0x41, 0x63, 0x74, 0x69,
        0x76, 0x69, 0x74, 0x79, 0x54, 0x68, 0x72, 0x65,
        0x61, 0x64, 0x00
    };//android/app/ActivityThread
    //static const char *v1 = "android/app/ActivityThread";
    
    jclass activityThread = (*env)->FindClass(env, v1);
    if (activityThread == NULL) {
        //LOGE("activityThread is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    char CURRENTAT[] = {
        0x63, 0x75, 0x72, 0x72, 0x65, 0x6E, 0x74, 0x41,
        0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x54,
        0x68, 0x72, 0x65, 0x61, 0x64, 0x00
    };   //currentActivityThread
    unsigned char LANDROIDAT[] = {
        0x28, 0x29, 0x4C, 0x61, 0x6E, 0x64, 0x72, 0x6F,
        0x69, 0x64, 0x2F, 0x61, 0x70, 0x70, 0x2F, 0x41,
        0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x54,
        0x68, 0x72, 0x65, 0x61, 0x64, 0x3B, 0x00
    }; //"()Landroid/app/ActivityThread;"

    jmethodID currentActivityThread = (*env)->GetStaticMethodID(env, activityThread, CURRENTAT, LANDROIDAT);
    if (currentActivityThread == 0) {
        //LOGE("currentActivityThread is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    jobject at = (*env)->CallStaticObjectMethod(env, activityThread, currentActivityThread);
    if (at == NULL) {
        //LOGE("at is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    //获取Application，也就是全局的Context
    unsigned char GETAPPLICATION[] = {
        0x67, 0x65, 0x74, 0x41, 0x70, 0x70, 0x6C, 0x69,
        0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x00
    };//getApplication

    unsigned char ANDROIDAPP[] = {
        0x28, 0x29, 0x4C, 0x61, 0x6E, 0x64, 0x72, 0x6F,
        0x69, 0x64, 0x2F, 0x61, 0x70, 0x70, 0x2F, 0x41,
        0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69,
        0x6F, 0x6E, 0x3B, 0x00
    }; //()Landroid/app/Application;

    jmethodID getApplication = (*env)->GetMethodID(env, activityThread, GETAPPLICATION, ANDROIDAPP);
    if (getApplication == 0) {
        //LOGE("getApplication is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    jobject context = NULL;
    int i = 0;
    for (i = 0; i < 200; i++) {
        context = (*env)->CallObjectMethod(env, at, getApplication);
        if (context != NULL) {
            break;
        }
        //LOGE("context is NULL\n");
        //usleep(1000 *10);
        struct timespec ts, ts1;
        ts.tv_nsec = 500000000;    // 1500ms
        ts.tv_sec = 1;
        pdx_syscall(__NR_nanosleep, &ts, &ts1);
    }

    if (context == NULL) {
        //LOGE("context final NULL!\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    //Context的类
    jclass context_clazz = (*env)->GetObjectClass(env,  context);
    if (context_clazz == NULL) {
        //LOGE("JNI_DEBUG context_clazz is NULL!\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    //LOGE("context_clazz OK\n");
    // 得到 getPackageManager 方法的 ID
    unsigned char GETPACKAGEMANAGER[] = {
        0x67, 0x65, 0x74, 0x50, 0x61, 0x63, 0x6B, 0x61,
        0x67, 0x65, 0x4D, 0x61, 0x6E, 0x61, 0x67, 0x65,
        0x72, 0x00 
    }; //getPackageManager

    unsigned char ANDROIDCPP[] = {
        0x28, 0x29, 0x4C, 0x61, 0x6E, 0x64, 0x72, 0x6F,
        0x69, 0x64, 0x2F, 0x63, 0x6F, 0x6E, 0x74, 0x65,
        0x6E, 0x74, 0x2F, 0x70, 0x6D, 0x2F, 0x50, 0x61,
        0x63, 0x6B, 0x61, 0x67, 0x65, 0x4D, 0x61, 0x6E,
        0x61, 0x67, 0x65, 0x72, 0x3B, 0x00
    }; //()Landroid/content/pm/PackageManager;

    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env,  context_clazz, GETPACKAGEMANAGER,
                                                                ANDROIDCPP);
    if (methodID_getPackageManager == 0) {
        //LOGE("methodID_getPackageManager is 0\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }
    // 获得PackageManager对象
    jobject packageManager = (*env)->CallObjectMethod(env,  context,
                                                      methodID_getPackageManager);
    if (packageManager == NULL) {
        //LOGE("packageManager is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    // 获得 PackageManager 类
    jclass pm_clazz = (*env)->GetObjectClass(env,  packageManager);
    if (pm_clazz == NULL) {
        //LOGE("pm_clazz is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    // 得到 getPackageInfo 方法的 ID
    unsigned char GETPACKAGEINFO[] = {
        0x67, 0x65, 0x74, 0x50, 0x61, 0x63, 0x6B, 0x61,
        0x67, 0x65, 0x49, 0x6E, 0x66, 0x6F, 0x00
    }; //getPackageInfo

    unsigned char LJLSANDROIDCONPP[] = {
        0x28, 0x4C, 0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C,
        0x61, 0x6E, 0x67, 0x2F, 0x53, 0x74, 0x72, 0x69,
        0x6E, 0x67, 0x3B, 0x49, 0x29, 0x4C, 0x61, 0x6E,
        0x64, 0x72, 0x6F, 0x69, 0x64, 0x2F, 0x63, 0x6F,
        0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2F, 0x70, 0x6D,
        0x2F, 0x50, 0x61, 0x63, 0x6B, 0x61, 0x67, 0x65,
        0x49, 0x6E, 0x66, 0x6F, 0x3B, 0x00
    }; //(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;

    jmethodID methodID_pm = (*env)->GetMethodID(env,  pm_clazz, GETPACKAGEINFO,
                                               LJLSANDROIDCONPP );

    if (methodID_pm == 0) {
        //LOGE("methodID_pm is 0\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    // 得到 getPackageName 方法的 ID
    unsigned char GETPACKAGENAME[] = {
        0x67, 0x65, 0x74, 0x50, 0x61, 0x63, 0x6B, 0x61,
        0x67, 0x65, 0x4E, 0x61, 0x6D, 0x65, 0x00
    }; //getPackageName
    
    unsigned char LJLS[21] = {
        0x28, 0x29, 0x4C, 0x6A, 0x61, 0x76, 0x61, 0x2F,
        0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x53, 0x74, 0x72,
        0x69, 0x6E, 0x67, 0x3B, 0x00
    }; //()Ljava/lang/String;

    jmethodID methodID_pack = (*env)->GetMethodID(env,  context_clazz,
                                                  GETPACKAGENAME, LJLS);
    if (methodID_pack == 0) {
        //LOGE("methodID_pack == 0");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    // 获得当前应用的包名
    jstring application_package = (jstring)(*env)->CallObjectMethod(env, context,
                                                           methodID_pack);
    if (application_package == NULL) {
        //LOGE("application_package is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    const char *package_name = (*env)->GetStringUTFChars(env, application_package, 0);
    if (package_name == NULL) {
        //LOGE("package_name is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    //LOGE("packageName: %s\n", package_name);

    // 获得PackageInfo
    jobject packageInfo = (*env)->CallObjectMethod(env,  packageManager,
                                                   methodID_pm, application_package, 64);
    if (packageInfo == NULL) {
        //LOGE("packageInfo is NULL");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    jclass packageinfo_clazz = (*env)->GetObjectClass(env,  packageInfo);
    if (packageinfo_clazz == NULL) {
        //LOGE("packageinfo_clazz is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    unsigned char SIGNATURE[] = {
        0x73, 0x69, 0x67, 0x6E, 0x61, 0x74, 0x75, 0x72,
        0x65, 0x73, 0x00
    }; //signature
    unsigned char LANDROIDCPS[] = {
        0x5B, 0x4C, 0x61, 0x6E, 0x64, 0x72, 0x6F, 0x69,
        0x64, 0x2F, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E,
        0x74, 0x2F, 0x70, 0x6D, 0x2F, 0x53, 0x69, 0x67,
        0x6E, 0x61, 0x74, 0x75, 0x72, 0x65, 0x3B, 0x00
    }; //[Landroid/content/pm/Signature;

    jfieldID fieldID_signatures = (*env)->GetFieldID(env,  packageinfo_clazz,
                                                     SIGNATURE, LANDROIDCPS);
    if (fieldID_signatures == 0) {
        //LOGE("fieldID_signatures is 0\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    jobjectArray signature_arr = (jobjectArray)(*env)->GetObjectField(env, 
                                                                      packageInfo, fieldID_signatures);
    if (signature_arr == NULL) {
        //LOGE("signature_arr is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    //Signature数组中取出第一个元素
    jobject signature = (*env)->GetObjectArrayElement(env, signature_arr, 0);
    if (signature == NULL) {
        //LOGE("signature is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    //读signature的hashcode
    jclass signature_clazz = (*env)->GetObjectClass(env, signature);
    if (signature_clazz == NULL) {
        //LOGE("signature_clazz is NULL\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    //hashCode
    unsigned char HASHCODE[] = {
        0x68, 0x61, 0x73, 0x68, 0x43, 0x6F, 0x64, 0x65, 0x00
    };//hashCode

    unsigned char ISTR[] = {
        0x28, 0x29, 0x49, 0x00
    }; //()I

    jmethodID methodID_hashcode = (*env)->GetMethodID(env, signature_clazz,
                                                      HASHCODE, ISTR);
    if (methodID_hashcode == 0) {
        //LOGE("methodID_hashcode == 0\n");
        (*tp.vm)->DetachCurrentThread(tp.vm);
        return NULL;
    }

    jint hashCode = (*env)->CallIntMethod(env, signature, methodID_hashcode);
    //LOGE("hashcode: %d\n", hashCode);
__END:
    (*tp.vm)->DetachCurrentThread(tp.vm);

#ifdef M_ARM64
    unsigned int orighashcode = 0;
	asm (
		"LDR %0, HASHVAL\n"
		"B NEXT\n"
		"HASHVAL: .DWORD 0x5F2E081B\n"
		"NEXT:\n"
		:"=r"(orighashcode)
		:
		);
    unsigned int debughashcode = 0;
    asm (
		"LDR %0, DEBUGHASHVAL\n"
		"B NEXT2\n"
		"DEBUGHASHVAL: .DWORD 0x2F5F6E6E\n"
		"NEXT2:\n"
		:"=r"(debughashcode)
		:
		);
        //0x5F2E081B
    if (hashCode != orighashcode && hashCode != debughashcode) {
        //LOGE("hashcode no match\n");
        deadcall();
        /*
        asm (
		".DWORD 0xdeaddead\n"
		);
        */
        pdx_syscall(__NR_exit, -1);
    }
#elif M_ARM
    unsigned int orighashcode = 0;
	asm (
		"LDR %0, HASHVAL\n"
		"B NEXT\n"
		"HASHVAL: .WORD 0x5F2E081B\n"
		"NEXT:\n"
		:"=r"(orighashcode)
		:
		);
    unsigned int debughashcode = 0;
    asm (
		"LDR %0, DEBUGHASHVAL\n"
		"B NEXT2\n"
		"DEBUGHASHVAL: .WORD 0x2F5F6E6E\n"
		"NEXT2:\n"
		:"=r"(debughashcode)
		:
		);
        //0x5F2E081B
    if (hashCode != orighashcode && hashCode != debughashcode) {
        //LOGE("hashcode no match\n");
        deadcall();
        /*
        asm (
		".DWORD 0xdeaddead\n"
		);
        */
        pdx_syscall(__NR_exit, -1);
    }
#else
    if (hashCode != 0x5F2E081B && hashCode != 0x2F5F6E6E) {
        // //LOGE("hash code no match\n");
        deadcall();
        /*
        asm (
		".WORD 0xdead\n"
		);
        */
        pdx_syscall(__NR_exit, -1);
    }
#endif 

    return NULL;
}


/*
int main() {
    
    JNI_OnLoad(NULL, NULL);
    //LOGD("end\n");
    return 0;
}
*/