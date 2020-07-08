#include "HookProcs.h"
#include "common.h"
#include "dalvik/vm/DvmDex.h"
#include "dalvik/vm/JarFile.h"
#include "dalvik/vm/Globals.h"
#include "Shield.h"
#include "HookUtils.h"
#include <jni.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/system_properties.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dalvik/vm/Object.h>
#include <dalvik/vm/Common.h>
#include "inject/ElfUtils.h"
#include "ReadFromApk.h"
#include "decrypt.h"


static JavaVM *gJavaVm = 0;

static char gPkgName[255] = {0};
static char gApkPath[255] = {0};
static char gCsnDatPath[255] = {0};
static char gReleasePath[255] = {0};
static char gBaseDex[255] = {0};
static char gBaseVdex[255] = {0};
static char gBaseOat[255] = {0};
const char* gBaseCsn = "app_csn0";

static int gSdkVersion;
static size_t gMaxEnc = 1024;

static DexPointer* gDexHead = NULL;
static DexPointer* gDexNow = NULL;

struct android_dlextinfo;
void* (*ori_android_dlopen_ext)(const char* filename, int flag, const android_dlextinfo* extinfo);
void *(*ori_dlopen)(const char *path, int type);

//android 6.0 use
typedef ssize_t (*fnread) (int fd, void *buf, size_t count);
fnread ori_read = read;

//android 7.1 use
typedef size_t (*fn__read_chk) (int fd, void *buf, size_t nbytes, size_t buflen);
fn__read_chk ori__read_chk = NULL;

typedef int (*fnfstat) (int filedes, struct stat *buf);
fnfstat ori_fstat = fstat;

typedef void* (*fnmmap) (void* start,size_t length,int prot,int flags,int fd,off_t offset);
fnmmap ori_mmap = mmap;

#ifdef DX_RUN_ON_32
typedef long (*Dalvik_dalvik_system_DexFile_openDexFileNative_func)(const u4* args, JValue* pResult);
Dalvik_dalvik_system_DexFile_openDexFileNative_func Dalvik_dalvik_system_DexFile_openDexFileNative_ptr = NULL;

typedef int (*Dalvik_dalvik_system_DexFile_openDexFileNative_yunos)(VMethodEntryStruct* , void**, void*, uint32_t);
Dalvik_dalvik_system_DexFile_openDexFileNative_yunos Dalvik_dalvik_system_DexFile_openDexFileNative_ptr_yunos = NULL;

typedef int (*HashCompareFunc)(const void* tableItem, const void* looseItem);

static Method* openDexFileNative_med = NULL;
static VMethodEntryStruct* openDexFileNative_med2 = NULL;
#endif

static jint jniCall(JNIEnv* env, jclass cls, jmethodID m){
#ifdef DEBUG
    return env->CallStaticIntMethod(cls, m);
#else
    return 0;
#endif
}

static bool isNamePayloadDex(const char* name){
    bool result = false;

    const char* p = strrchr(name, '.');
    if (p != NULL) {
        if (strcmp(p, ".flock") == 0)
            return result;
    }
    if (strstr(name, gBaseDex)){
        result = true;
    }

    return result;
}

static bool isNamePayloadOat(const char* name){
    bool result = false;

    const char* p = strrchr(name, '.');
    if (p != NULL) {
        if (strcmp(p, ".flock") == 0)
            return result;
    }
    if (strstr(name, gBaseOat)){
        result = true;
    }

    return result;
}

static size_t getFileLength(const char *path)
{
    size_t filesize = 0;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = (size_t)statbuff.st_size;
    }
    return filesize;
}

static DexPointer* findDexByName(const char *name){

    if (NULL == gDexHead){
        LOGE("gDexHead is NULL");
        return NULL;
    }

    DexPointer *pointer = gDexHead->next;
    while(pointer != NULL){

        LOGD("pointer->fileName:%s name:%s", pointer->fileName, name);

        if(strstr(name, pointer->fileName)){

            LOGD("found pointer->fileName:%s name:%s", pointer->fileName, name);
            return pointer;

        }

        pointer = pointer->next;
    }

    return NULL;
}

static void _decryptOatMemory(const char *filename, void *handle) {
    void *begin = dlsym(handle, "oatdata");
    void *end = dlsym(handle, "oatexec");
    size_t dataRange = (size_t)end - (size_t)begin;
    if (!begin || !end) {
        LOGE("warning dec oatfile has invalid begin=%p or end=%p skip decrypt", begin, end);
        return;
    }

    LOGI("oatdata=%p, oatexec=%p, range=%u",begin, end, (unsigned)dataRange);

    size_t sz = gMaxEnc < dataRange ? gMaxEnc : dataRange;
    uint32_t *base = (uint32_t *)begin;
    char *base2 = (char *)begin;
    mprotect(base, sz, PROT_READ|PROT_WRITE);

    char hex1[100] = "", hex2[100] = "";
    getHex(hex1, sizeof(hex1), base2, sz);
    getHex(hex2, sizeof(hex2), base2+sz-1-100, 100);
    LOGI("_decryptOatMemory before dec filename=%s,sz=%d,hex_head=%s,hex_end=%s", filename, sz, hex1, hex2);

    simple_xor_decrypt((unsigned char*)base2, sz);
    getHex(hex1, sizeof(hex1), base2, sz);
    getHex(hex2, sizeof(hex2), base2+sz-1-100, 100);
    LOGI("_decryptOatMemory after dec filename=%s,sz=%d,hex_head=%s,hex_end=%s", filename, sz, hex1, hex2);
    mprotect(base, sz, PROT_READ);
}

static int hook_fstat(int filedes, struct stat *buf){
    int rc = ori_fstat(filedes, buf);

    char name[260] = {0};
    get_fname_by_fd(filedes, name, sizeof(name));

    if(isNamePayloadDex(name) && strstr(name, ".unzip/d-class")){
        LOGD("hook_fstat fd:%d name:%s", filedes, name);
        DexPointer* now = findDexByName(name);
        if(NULL != now){
            LOGD("hook_fstat change fd:%d size:%d", filedes, now->config->len);
            buf->st_size = now->config->len;
        }
    }
    return rc;
}


static ssize_t hook_read(int fd, void *buf, size_t count) {

    ssize_t ret = ori_read(fd, buf, count);

    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    if(isNamePayloadDex(name))
    {
        if (strstr(name, ".unzip/d-class")){
            DexPointer* tmp = findDexByName(name);
            if (NULL != tmp){
                memcpy(buf, (void *)tmp->config->addr, count);
                ret = (ssize_t )count;
            }
        } else if (strstr(name, ".unzip/class")){
            simple_xor_decrypt((unsigned char*)buf, count);

        }

        char hex[256] = {0};
        getHex(hex, 64, (char*)buf, count);
        LOGI("==>hook read unzip fd:%d name:%s count:%d header:%s", fd, name, count, hex);
    }

    return ret;
}

static size_t hook__read_chk(int fd, void *buf, size_t nbytes, size_t buflen){

    size_t ret = ori__read_chk(fd, buf, nbytes, buflen);

    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    if(isNamePayloadDex(name))
    {
        if (strstr(name, ".unzip/d-class")){
            DexPointer* tmp = findDexByName(name);
            if (NULL != tmp){
                memcpy(buf, (void*)tmp->config->addr, nbytes);
                ret = nbytes;
            }
        } else if (strstr(name, ".unzip/class")){
            simple_xor_decrypt((unsigned char*)buf, nbytes);
        }

        char hex[256] = {0};
        getHex(hex, 64, (char*)buf, nbytes);
        LOGI("==>hook read_chk unzip fd:%d name:%s count:%d header:%s", fd, name, nbytes, hex);
    }

    return ret;
}


static void* hook_mmap(void* start,size_t length, int prot, int flags, int fd, off_t offset) {

    void* ret = NULL;

    char name[260] = {0};
    get_fname_by_fd(fd, name, sizeof(name));

    if (isNamePayloadDex(name) || isNamePayloadOat(name)){
        if (strstr(name, ".unzip/d-class")){
            DexPointer* tmpInfo = findDexByName(name);
            if (NULL != tmpInfo){

                //需要重新copy的原因是这个start地址不能是其他地址，而是要他系统传进来的start地址，否则会出错
                ret = ori_mmap(start, tmpInfo->config->len, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

                if (ret == MAP_FAILED){
                    LOGE("mmap fail errno:%s",strerror(errno));
                    return ret;
                }

                memcpy(ret, (void*)tmpInfo->config->addr, tmpInfo->config->len);
                mprotect(ret, tmpInfo->config->len, prot);

                char hex[256] = {0};
                getHex(hex, 256, (char*)ret, 64);
                LOGI("==>hook mmap fd:%d name:%s header:%s len:%d prot:%d",fd, name, hex, length, prot);
            }

            return ret;
        }

        if (strstr(name, ".unzip/class")){
            ret = ori_mmap(start, length, prot, flags, fd, offset);
            size_t flen = getFileLength(name);
            if (flen > length){
                flen = length;
            }
            mprotect(ret, flen, PROT_READ | PROT_WRITE);
            simple_xor_decrypt((unsigned char*)ret, flen);
            mprotect(ret, flen, prot);

            char hex[256] = {0};
            getHex(hex, 64, (char*)ret, 4);
            LOGI("==>hook mmap unzip fd:%d name:%s header:%s len:%d prot:%d",fd, name, hex, length, prot);
        } else if (strstr(name, ".opt/class") && gSdkVersion < 23) {
            ret = ori_mmap(start, length, prot, flags, fd, offset);
            char header[20] = {0};
            getHex(header, 20, (char *) ret, 4);
            LOGI("==>hook mmap opt fd:%d name:%s header:%s len:%d prot:%d", fd, name, header,
                 length, prot);

            const size_t oat_data_start = 0x1000;

            PRINT_STACK(__func__);

            if ((prot & PROT_EXEC) == 0 && length > oat_data_start) {
                size_t s_off;
                if (offset < oat_data_start) {
                    s_off = oat_data_start - offset;
                } else {
                    s_off = 0;
                }

                size_t e_off;
                if (length + offset > gMaxEnc + oat_data_start) {
                    e_off = gMaxEnc + oat_data_start - offset;
                } else {
                    e_off = length;
                }

                void *begin = (char *) ret + s_off;
                void *end = (char *) ret + e_off;
                const size_t sz = (char *) end - (char *) begin;
                char *base = (char *) begin;

                char hex1[100] = "", hex2[100] = "";
                getHex(hex1, sizeof(hex1), base, sz);
                getHex(hex2, sizeof(hex2), base + sz - 1 - 100, 100);
                LOGI("self mmap oat before dec filename=%s,sz=%d,hex_head=%s,hex_end=%s", name, sz,
                     hex1, hex2);

                mprotect(base, sz, PROT_READ | PROT_WRITE);
                simple_xor_decrypt((unsigned char*)base, sz);
                mprotect(base, sz, prot);

                getHex(hex1, sizeof(hex1), base, sz);
                getHex(hex2, sizeof(hex2), base + sz - 1 - 100, 100);
                LOGI("self mmap oat after dec filename=%s,sz=%d,hex_head=%s,hex_end=%s", name, sz,
                     hex1, hex2);
            }
        }

        else if (strstr(name, gBaseVdex) && strstr(name, ".vdex") && gSdkVersion >= 26) {
            size_t fixSkip = (size_t)getpagesize();
            if (length > fixSkip) {
                void *addrMem = ori_mmap(start, length, prot | PROT_WRITE, MAP_ANONYMOUS | flags, 0, 0);
                LOGI("vdex dec %s begin", name);
                off64_t pos = lseek64(fd, 0, SEEK_CUR);
                lseek(fd, 0, SEEK_SET);
                ssize_t r = 0;
                char *p = (char*)addrMem;
                //读文件不要mmmap进来再读，因为vdex要求返回的地址一定要以start参数开始，如果再做一次mmap有地址被占用的风险
                do {
                    r = ori_read(fd, p, length - r);
                    p += r;
                }while(r > 0);

                lseek64(fd, pos, SEEK_SET);
                //注意，返回的地址需要与start一致，否则会导致校验不过，每次都会重新生成vdex
                void *begin = (void*)((size_t)addrMem + fixSkip);
                size_t dataRange = length - offset - fixSkip;

                size_t sz = gMaxEnc < dataRange ? gMaxEnc : dataRange;
                char *base = (char *) begin;

                LOGI("vdex dec base=%p,sz=%u", base, sz);
                simple_xor_decrypt((unsigned char*)base, sz);
                LOGI("vdex dec %s end", name);
                mprotect(addrMem, length, prot);
                return addrMem;
            }
            else {
                ret = ori_mmap(start, length, prot, flags, fd, offset);
            }
        }
    } else {
        ret = ori_mmap(start, length, prot, flags, fd, offset);
    }
    return ret;
}

void *my_dlopen(const char *filename, int type) {
    LOGI("android_dlopen %s",filename);
    void *r = ori_dlopen(filename, type);
    if (!r) {
        return 0;
    }
    const char *baseOat = "app_csn0/.opt/";
    if (strstr(filename, baseOat)) {
        _decryptOatMemory(filename, r);
    }
    LOGI("android_dlopen_ext %s exit",filename);
    return r;
}

void* my_android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo) {

    LOGI("android_dlopen_ext %s",filename);
    void *r = ori_android_dlopen_ext(filename, flag, extinfo);
    if (!r) {
        return 0;
    }
    const char *baseOat = "app_csn0/.opt/";
    if (strstr(filename, baseOat)) {
        _decryptOatMemory(filename, r);
    }
    LOGI("android_dlopen_ext %s exit",filename);
    return r;
}

#ifdef DX_RUN_ON_32
static int lookup(JNINativeMethod *table, const char *name, const char *sig, void (**fnPtrout)(u4 const *, union JValue *))
{
    int i = 0;
    while (table[i].name != NULL)
    {
        if ((strcmp(name, table[i].name) == 0)
            && (strcmp(sig, table[i].signature) == 0))
        {
            *fnPtrout = (void (*)(u4 const *, union JValue *))table[i].fnPtr;
            return 1;
        }
        i++;
    }
    return 0;
}

static int hashcmpDexOrJar(const void* tableVal, const void* newVal)
{
    return (int) newVal - (int) tableVal;
}

void Dalvik_dalvik_system_DexFile_my_openDexFileNative(const u4* args, JValue* pResult)
{
    StringObject* sourceNameObj = (StringObject*) args[0];

    if (sourceNameObj == NULL)
    {
        return;
    }

    void *ldvm = dlopen("libdvm.so", RTLD_LAZY);

    typedef char* (*dvmCreateCstrFromStringFunc)(const StringObject* jstr);
    dvmCreateCstrFromStringFunc dvmCreateCstrFromString_ptr = (dvmCreateCstrFromStringFunc)dlsym(ldvm, "_Z23dvmCreateCstrFromStringPK12StringObject");
    char *sourceName = dvmCreateCstrFromString_ptr(sourceNameObj);

    if (!isNamePayloadDex(sourceName)) {
        Dalvik_dalvik_system_DexFile_openDexFileNative_ptr(args, pResult);
        return;
    }

    JNINativeMethod *dvm_dalvik_system_DexFile;
    void (*openDexFile)(const u4* args, JValue* pResult);

    u1 *dexByte = NULL;

    dvm_dalvik_system_DexFile = (JNINativeMethod*) dlsym(ldvm, "dvm_dalvik_system_DexFile");
    if(0 == lookup(dvm_dalvik_system_DexFile, "openDexFile", "([B)I", &openDexFile))
    {
        LOGE("method openDexFile not found !");
        openDexFile = NULL;
        return;
    }

    LOGD("my_openDexFileNative find openDexFile method sourceName:%s", sourceName);

    DexPointer* tmpInfo = findDexByName(sourceName);

    if (NULL == tmpInfo) {
        LOGE("my_openDexFileNative dexFile not found.");
        return;
    }

    dexByte = (u1*) tmpInfo->config->addr;
    size_t ucl = tmpInfo->config->len;

    JNIEnv *env = 0;
    gJavaVm->GetEnv((void**)&env, JNI_VERSION_1_4);
    bool attach = false;
    if (!env) {
        gJavaVm->AttachCurrentThread(&env, 0);
        attach = true;
    }

    jbyte *by = (jbyte*)dexByte;
    jbyteArray jDexArray = env->NewByteArray(ucl);
    env->SetByteArrayRegion(jDexArray, 0, ucl, by);

    typedef Object* (*dvmDecodeIndirectRefFunc) (Thread* self, jobject jobj);
    dvmDecodeIndirectRefFunc dvmDecodeIndirectRef_ptr = (dvmDecodeIndirectRefFunc)dlsym(ldvm, "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject");

    ArrayObject* arrObj = (ArrayObject*) dvmDecodeIndirectRef_ptr(((JNIEnvExt*)env)->self, jDexArray);

    u4 open_args[] = { (u4)arrObj };
    openDexFile(open_args, pResult);

    if (gSdkVersion == 19) {
        //android 4.4 上，使用Dalvik_dalvik_system_DexFile_openDexFile_bytearray加载Dex，下面两个变量不会被初始化，需要手动赋值
        DexOrJar *dexOrJar = (DexOrJar *) pResult->l;
        LOGD("***my_openDexFileNative pResult->l:%p", dexOrJar);

        MemMapping memMap = {0};
        memMap.baseAddr = memMap.addr = dexOrJar->pDexMemory;
        memMap.baseLength = memMap.length = ucl;
        dexOrJar->pRawDexFile->pDvmDex->memMap = memMap;
    }

    //此处存在注释存在内存泄漏，但是如果打开的话因为该内存由dvm分配，asan并不知道，而在个so释放，会误报成free错误
    //free(sourceName);
    if (attach) {
        gJavaVm->DetachCurrentThread();
    }

    LOGD("my_openDexFileNative run end.");
}

int Dalvik_dalvik_system_DexFile_yunos_openDexFileNative(VMethodEntryStruct* method, void** nativeArgs, void* selfThread, uint32_t retValue)
{
    void *libvm = dlopen("libvmkid_lemur.so", RTLD_NOW);
    typedef char* (*GetChar)(void*);
    GetChar getChar = (GetChar)dlsym(libvm, "vGetStringUTFChars");
    char *sourceName = getChar(*nativeArgs);

    if(!isNamePayloadDex(sourceName))
    {
        return Dalvik_dalvik_system_DexFile_openDexFileNative_ptr_yunos(method, nativeArgs, selfThread, retValue);
    }

    u1* pBytes = NULL;
    RawDexFile* pRawDexFile;
    DexOrJar* pDexOrJar = NULL;

    DexPointer* tmpInfo = findDexByName(sourceName);

    if (NULL == tmpInfo) {
        return -1;
    }

    pBytes = (u1*) tmpInfo->config->addr;
    size_t ucl = tmpInfo->config->len;

    typedef int (*dvmRawDexFileOpenArray)(unsigned char *, unsigned int, RawDexFile **);
    dvmRawDexFileOpenArray RawDexFileOpenArray = (dvmRawDexFileOpenArray)dlsym(libvm, "_Z22dvmRawDexFileOpenArrayPhjPP10RawDexFile");

    if (RawDexFileOpenArray(pBytes, ucl, &pRawDexFile) != 0) {
        LOGE("Unable to open in-memory DEX file");
        free(pBytes);
        return NULL;
    }

    pDexOrJar = (DexOrJar*) malloc(sizeof(DexOrJar));
    pDexOrJar->isDex = true;
    pDexOrJar->pRawDexFile = pRawDexFile;
    if (pRawDexFile->pDvmDex->memMap.addr == NULL) {
        pRawDexFile->pDvmDex->memMap.addr = pBytes;
        pRawDexFile->pDvmDex->memMap.length = ucl;
    }
    pDexOrJar->pDexMemory = pBytes;
    pDexOrJar->fileName = strdup("<memory>");

    void* gDvm = dlsym(libvm, "gDvm");

    typedef void* (*dvmHashTableLookup)(void*, u4 , void*, HashCompareFunc, bool);
    dvmHashTableLookup hashTableLookup = (dvmHashTableLookup)dlsym(libvm, "_Z18kvmHashTableLookupP9HashTablejPvPFiPKvS3_Eb");

    u4 hash = (u4) pDexOrJar;

    HashTable *table = *(HashTable**)(gDvm);

    pthread_mutex_lock(&table->lock);
    void* result = hashTableLookup(table, hash, pDexOrJar, hashcmpDexOrJar, true);
    pthread_mutex_unlock(&table->lock);

    if (result != pDexOrJar) {
        LOGE("Pointer has already been added?");
        return NULL;
    }

    pDexOrJar->okayToFree = true;

    //此处存在注释存在内存泄漏，但是如果打开的话因为该内存由dvm分配，asan并不知道，而在个so释放，会误报成free错误
    //free(sourceName);
    *((int*)retValue) = ((int)pDexOrJar);

    return (int)pDexOrJar;
}
#endif

//#include "dx_hook.h"
static bool isArtEnabled(JNIEnv *env) {
    env->PushLocalFrame(20);

    jclass shellSupportClass = env->FindClass("com/security/shell/ShellSupporter");

    if (shellSupportClass == NULL) {
        LOGE("Could not find class ShellSupporter.");
        return false;
    }

    jmethodID isArtEnable = env->GetStaticMethodID(
            shellSupportClass, "isArtEnable", "()Z");

    if (isArtEnable == NULL) {
        LOGE("Could not find ShellSupporter.isArtEnable().");
        return false;
    }

    jboolean result = env->CallStaticBooleanMethod(
            shellSupportClass, isArtEnable);
    env->PopLocalFrame(0);
    return result;
}

static bool try_for_android10_hook() {

    void *artBase = getTargetSI("libartbase.so");
    if (!artBase) {
        //for android 9
        return false;
    }
    else {
        // for android 10
        // android 10 这些调用被分散到不同的so里面了，为了避免问题，这些so都got hook一次
        replaceFunc(artBase, "read", (void *) hook_read, (void **) &ori_read);
        replaceFunc(artBase, "mmap", (void *) hook_mmap, (void **) &ori_mmap);

        void *dexfile = getTargetSI("libdexfile.so");
        replaceFunc(dexfile, "fstat", (void *) hook_fstat, (void **) &ori_fstat);
    }
    return true;
}

static void initHookProcs(JNIEnv *env, int sdkVersion) {

    gSdkVersion = sdkVersion;
    /*
    dx_hook_hookfun((void*)read, (void*)hook_read, (void**)&ori_read);
    dx_hook_hookfun((void*)fstat, (void*)hook_fstat, (void**)&ori_fstat);
    dx_hook_hookfun((void*)mmap, (void*)hook_mmap, (void**)&ori_mmap);
    return;
     */

    bool isArt = isArtEnabled(env);
    if(isArt)
    {
        LOGI("initHookProcs start, running on art runtime.");
        if (sdkVersion >= 28) {
            //10.0的sdkVersion也是28,应该是测试版所以没改，用这种办法判断10.0
            if (try_for_android10_hook()) {
                return;
            }
        }

        void *libArt = getTargetSI("libart.so");
        if (!libArt) {
            libArt = getTargetSI("libaoc.so");
        }

        if(libArt == NULL) {
            LOGE("can not find libart, return");
            abort();
        }

        if (gSdkVersion < 26 && gSdkVersion > 23) {
            replaceFunc(libArt, "__read_chk", (void *) hook__read_chk, (void **) &ori__read_chk);
        } else {
            replaceFunc(libArt, "read", (void *) hook_read, (void **) &ori_read);
        }
        replaceFunc(libArt, "mmap", (void *) hook_mmap, (void **) &ori_mmap);
        replaceFunc(libArt, "fstat", (void *) hook_fstat, (void **) &ori_fstat);

        if (gSdkVersion > 22 && gSdkVersion < 26) {
            replaceFunc(libArt, "android_dlopen_ext", (void *) my_android_dlopen_ext, (void **) &ori_android_dlopen_ext);
            LOGI("in initHookProcs ,ori_android_dlopen_ext:%p", ori_android_dlopen_ext);
        }else if (gSdkVersion <= 22){
            replaceFunc(libArt, "dlopen", (void *) my_dlopen, (void **) &ori_dlopen);
            LOGI("in initHookProcs ,ori_dlopen:%p", ori_dlopen);
        }
    }
#ifdef DX_RUN_ON_32
    else {

        void *libvm = dlopen("libvmkid_lemur.so", RTLD_NOW);
        if (libvm){
            LOGE("initHookProcs, ApkProtect not support system yunOS 4.4, return!");
        } else {
            LOGI("initHookProcs start, running on dalvik runtime.");
        }

        jclass jf = env->FindClass("dalvik/system/DexFile");
        if (gSdkVersion < 19) {
            //4.0-4.3 hook openDexFile
            openDexFileNative_med = (Method *) env->GetStaticMethodID(jf, "openDexFile",
                    "(Ljava/lang/String;Ljava/lang/String;I)I");
            openDexFileNative_med2 = (VMethodEntryStruct*)env->GetStaticMethodID(jf, "openDexFile",
                    "(Ljava/lang/String;Ljava/lang/String;I)I");
        } else {
            //4.4 hook openDexFileNative
            openDexFileNative_med = (Method*)env->GetStaticMethodID(jf, "openDexFileNative",
                    "(Ljava/lang/String;Ljava/lang/String;I)I");
            openDexFileNative_med2 = (VMethodEntryStruct*)env->GetStaticMethodID(jf, "openDexFileNative",
                    "(Ljava/lang/String;Ljava/lang/String;I)I");
        }

        if(openDexFileNative_med == NULL)
        {
            LOGE("openDexFileNative_med,Can not find open, return!");
            return;
        } else {
            LOGI("openDexFileNative_med found");
        }

        u4 **addr = (u4**)openDexFileNative_med->insns;

        if(gSdkVersion >= 14)
        {
            if (libvm){
                Dalvik_dalvik_system_DexFile_openDexFileNative_ptr_yunos = (Dalvik_dalvik_system_DexFile_openDexFileNative_yunos)openDexFileNative_med2->point5;
                openDexFileNative_med2->point5 = (void*)&Dalvik_dalvik_system_DexFile_yunos_openDexFileNative;
            } else
            if (openDexFileNative_med->insns == 0) {
                Dalvik_dalvik_system_DexFile_openDexFileNative_ptr = (Dalvik_dalvik_system_DexFile_openDexFileNative_func)openDexFileNative_med->nativeFunc;
                openDexFileNative_med->nativeFunc = (void (*)(const unsigned int *, JValue *, const Method *, Thread *))Dalvik_dalvik_system_DexFile_my_openDexFileNative;
            } else {
                Dalvik_dalvik_system_DexFile_openDexFileNative_ptr = (Dalvik_dalvik_system_DexFile_openDexFileNative_func)addr[10];
                addr[10] = (u4*)Dalvik_dalvik_system_DexFile_my_openDexFileNative;
            }
        } else {
            LOGE("initHookProcs, ApkProtect not support system lower than android 4.0, return!");
        }
    }
#endif
    LOGI("initHookProcs is over!!!");
}

static void save2DexInfo(uint32_t index, unsigned char *_dexByte, uint32_t len){
    char* name = (char*)calloc(256, sizeof(char));

    if (NULL == name){
        LOGE("in save2DexInfo, malloc name fail");
        abort();
    }

    if (index){
        sprintf(name, "%sd-classes%d.dex", gBaseDex, index+1);
    } else {
        sprintf(name, "%sd-classes.dex", gBaseDex);
    }

    uintptr_t dexByte = (uintptr_t) malloc(len);
    memcpy((void*)dexByte, _dexByte, len);

    DexConfig* config = (DexConfig*)malloc(sizeof(DexConfig));
    config->id = index;
    config->addr = dexByte;
    config->len = len;

    DexPointer* pointer = (DexPointer*) malloc(sizeof(DexPointer));
    pointer->config = config;
    pointer->fileName = name;
    pointer->next = NULL;

    gDexNow->next = pointer;
    gDexNow = gDexNow->next;
}

void initNative(JNIEnv* env, jobject , jstring sourceDirJs, jstring extraDirJs, jstring releaseDirJs, jstring pkgName, jint encSize)
{
    LOGI("=====>init Native start!!!");

    gMaxEnc = (size_t )encSize;

    sprintf(gBaseDex, "%s/.unzip/", gBaseCsn);
    sprintf(gBaseOat, "%s/.opt/", gBaseCsn);
    ///data/data/<pkgname>/app_csn0/.unzip/oat/arm/*.vdex
    //避免判断平台，是否arm目录或者x86不管了，反正都能用
    sprintf(gBaseVdex, "%s/.unzip/oat/", gBaseCsn);

    env->GetJavaVM(&gJavaVm);
    const char *packageName = env->GetStringUTFChars(pkgName, 0);
    const char *apkPath = env->GetStringUTFChars(sourceDirJs, 0);
    const char *extraPath = env->GetStringUTFChars(extraDirJs, 0);
    const char *releasePath = env->GetStringUTFChars(releaseDirJs, 0);
    strcpy(gPkgName, packageName);
    strcpy(gApkPath, apkPath);
    sprintf(gCsnDatPath, "%s/%s", extraPath, ".csn.dex");
    strcpy(gReleasePath, releasePath);

    env->ReleaseStringUTFChars(pkgName, packageName);
    env->ReleaseStringUTFChars(sourceDirJs, apkPath);

    char sdkchar[16];
    __system_property_get("ro.build.version.sdk", sdkchar);
    int sdkVersion = atoi(sdkchar);
    initHookProcs(env, sdkVersion);
}

jbyteArray getAddr(JNIEnv *env, jclass){

    if (NULL == gDexHead)
        return NULL;

    DexPointer* counter = gDexHead;
    size_t size = 0;
    while (counter->next != NULL){
        counter = counter->next;
        size++;
    }

    DexConfig* data = (DexConfig*)malloc(sizeof(DexConfig)*size);
    DexConfig* indx = data;
    counter = gDexHead->next;
    for (int i = 0; i < size; ++i) {
        memcpy(indx, counter->config, sizeof(DexConfig));
        indx++;
        counter = counter->next;
    }

    size_t hexSz = sizeof(DexConfig)*size*2;
    char *buf = (char*)malloc(hexSz);
    const char *d = getHex(buf, hexSz, (char*)data, sizeof(DexConfig)*size);
    LOGD("data:%s", d);
    free(buf);

    jbyte *by = (jbyte*)data;
    jbyteArray jDexArray = env->NewByteArray(sizeof(DexConfig)*size);
    env->SetByteArrayRegion(jDexArray, 0, sizeof(DexConfig)*size, by);

    free(data);

    DexPointer *p = gDexHead->next;
    while(p->next != NULL){
        DexPointer *ptr = p;
        p = p->next;
        free(ptr);
    }
    gDexHead = 0;
    gDexNow = 0;

    return jDexArray;
}

static bool write_dex_file(char *addr, size_t len, const char *outPath) {

    LOGI("in write_dex_file start!!!");
    int fdOut = open(outPath, O_CREAT | O_WRONLY, 0666);
    LOGI("in write_dex_file fdOut:%d", fdOut);

    if (fdOut == -1){
        LOGE("write file fail, %s", strerror(errno));
        return false;
    }

    struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int ret = fcntl(fdOut, F_SETLKW, &lock);
    LOGI("flock return %d", ret);
    if (ret < 0) {
        LOGE("flock error %s", strerror(errno));
        return false;
    }

    char hex1[256] = {0};
    getHex(hex1, sizeof(hex1), addr, 64);
    LOGD("write_dex_file before simple_xor_decrypt:%s", hex1);
    simple_xor_decrypt((unsigned char*)addr, len);
    char hex2[256] = {0};
    getHex(hex2, sizeof(hex2), addr, 64);
    LOGD("write_dex_file after simple_xor_decrypt:%s", hex2);

    ssize_t count = write(fdOut, addr, len);
    if (count == -1){
        LOGE("write error %s", strerror(errno));
        return false;
    }

    int r = fcntl(fdOut, F_UNLCK, &lock);
    LOGI("flock unlock return %d", r);

    close(fdOut);

    return true;
}

static const char* getDexNameInMemBySdkInt(int sdkInt){
    if (sdkInt < 20){
        return "classes.dex";
    } else if (sdkInt < 23){
        return "base.apk@classes.dex";
    } else if (sdkInt < 26){
        return "base.odex";
    } else {
        return "base.vdex";
    }
}

static size_t findDexDateInMem(JNIEnv* env, jclass cls, jmethodID m, void*& data){
    const char* dexName = getDexNameInMemBySdkInt(gSdkVersion);

    uint32_t len = 0;

    LOGD("findDexDateInMem before get_dex_in_mem memory : %d", jniCall(env, cls, m));
    data = get_dex_in_mem(dexName, gPkgName,  len);
    LOGD("findDexDateInMem after get_dex_in_mem memory : %d", jniCall(env, cls, m));

    if (data == NULL){
        uint8_t* dexAddr = NULL;
        //add decompress apk
        LOGD("findDexDateInMem before readFromApk memory : %d", jniCall(env, cls, m));
        size_t shellLen = readFromApk(gApkPath, "classes.dex", &dexAddr);
        LOGD("findDexDateInMem after readFromApk memory : %d", jniCall(env, cls, m));
        len = findDex((void*)dexAddr, (void*)(dexAddr+shellLen), data);
        LOGD("findDexDateInMem after findDex memory : %d", jniCall(env, cls, m));
    }

    if (data == NULL){
        LOGE("findDexDateInMem, dex data not found, return!");
        return 0;
    }

    LOGI("===>findDexDateInMem end");
    return len;
}

static size_t findDexDataInFile(JNIEnv* env, jclass cls, jmethodID m, void* data, size_t len){

    int fd = open(gCsnDatPath, O_RDONLY);

    if (fd == -1) {
        char *a = strerror(errno);
        LOGE("findDexDataInFile get dex file data error:%s", a);
        return 0;
    }

    LOGD("before payloadType1 read : %d", jniCall(env, cls, m));
    ssize_t fileLen = read(fd, data, len);
    char* err = strerror(errno);
    LOGI("%s", err);
    LOGD("after payloadType1 read memory : %d", jniCall(env, cls, m));

    close(fd);

    return (size_t)fileLen;
}

static size_t findDexData(JNIEnv* env, jclass cls, jmethodID m, jint payloadType, unsigned char*& target){

    void* buf = NULL;
    size_t len = 0;
    if (payloadType == 0){
        void* data = NULL;
        len = findDexDateInMem(env, cls, m, data);
        if (len > 0){
            buf = malloc(len);
            memcpy(buf, data, len);
        }
    } else {
        len = getFileLength(gCsnDatPath);
        buf = malloc(len);
        len = findDexDataInFile(env, cls, m, buf, len);
    }
    target = (unsigned char*)buf;

    return len;
}

int initNomalDexLoad(JNIEnv* env, jobject jobj, jint payloadType, jint encLen){

    jclass cls = env->FindClass("com/keep/DebugUtils");
    jmethodID m = env->GetStaticMethodID(cls, "getMemorySize", "()I");

    gDexHead = (DexPointer*)malloc(sizeof(DexPointer));
    memset(gDexHead, 0, sizeof(DexPointer));
    gDexNow = gDexHead;

    unsigned char *target = 0;
    LOGD("initNomalDexLoad before findDexData memory : %d", jniCall(env, cls, m));
    size_t dataLen = findDexData(env, cls, m, payloadType, target);
    LOGD("initNomalDexLoad after findDexData memory : %d", jniCall(env, cls, m));
    if (dataLen <= 0) {
        return -1;
    }

    int k=0;

    uint32_t total = 0;
    total = *(uint32_t*) target;

    size_t uint32Len = sizeof(uint32_t);
    off_t offset = (off_t)uint32Len;

    for (int i = 0; i < total; ++i) {

        uint32_t index = 0;
        index = *(uint32_t*)(target + offset);
        offset += uint32Len;

        uint32_t len = 0;
        len = *(uint32_t*)(target + offset);
        offset += uint32Len;

        unsigned char *dexByte = target + offset;
        offset += len;

        size_t srcEncLen = (size_t )encLen;
        if (encLen == -1){
            srcEncLen = len;
            LOGI("initNomalDexLoad encLen:%d fileLen:%d", srcEncLen, len);
        }

        src_xor_decrypt(dexByte, srcEncLen, len);
        char hex2[256] = {0};
        getHex(hex2, sizeof(hex2), dexByte, 64);
        LOGI("initNomalDexLoad after decrypt:%s", hex2);

        LOGD("initNomalDexLoad before save2DexInfo memory : %d", jniCall(env, cls, m));
        save2DexInfo(index, dexByte, len);
        LOGD("initNomalDexLoad after save2DexInfo memory : %d", jniCall(env, cls, m));
        k++;
    }

    free(target);

    env->DeleteLocalRef(cls);
    LOGI("=====>initNomalDexLoad end!!!");

    return k;
}

int initDexOptDexLoad(JNIEnv* env, jobject jobj, jint payloadType, jint encLen){

    jclass cls = env->FindClass("com/keep/DebugUtils");
    jmethodID m = env->GetStaticMethodID(cls, "getMemorySize", "()I");

    unsigned char *target = 0;
    size_t dataLen = findDexData(env, cls, m, payloadType, target);
    if (dataLen <= 0) {
        return -1;
    }

    int k=0;

    uint32_t total = 0;
    total = *(uint32_t*) target;

    size_t uint32Len = sizeof(uint32_t);
    off_t offset = (off_t)uint32Len;

    for (int i = 0; i < total; ++i) {

        uint32_t index = 0;
        index = *(uint32_t*)(target + offset);
        offset += uint32Len;

        uint32_t len = 0;
        len = *(uint32_t*)(target + offset);
        offset += uint32Len;

        unsigned char *dexByte = target + offset;
        offset += len;

        size_t srcEncLen = (size_t )encLen;
        if (encLen == -1){
            srcEncLen = len;
        }

        src_xor_decrypt(dexByte, srcEncLen, len);

        char hex2[256] = {0};
        getHex(hex2, sizeof(hex2), dexByte, 64);
        LOGI("initDexOptDexLoad after decrypt:%s", hex2);

        char outPath[256] = {0};
        if (index == 0){
            sprintf(outPath, "%s/classes.dex", gReleasePath);
        } else {
            sprintf(outPath, "%s/classes%d.dex", gReleasePath, index+1);
        }

        LOGD("initDexOptDexLoad before write_dex_file memory : %d", jniCall(env, cls, m));
        bool r = write_dex_file((char*)dexByte, len, outPath);
        LOGD("initDexOptDexLoad after write_dex_file memory : %d", jniCall(env, cls, m));

        if (r){
            k++;
        }
    }

    free(target);

    env->DeleteLocalRef(cls);
    LOGI("=====>initDexOptDexLoad end!!!");

    return k;
}

void cleanup(JNIEnv *env, jclass _) {
    if (!gDexHead)
        return;
    DexPointer *p = gDexHead->next;
    while(p->next != NULL){
        DexPointer *ptr = p;
        p = p->next;
        free((void*)(ptr->config->addr));
        free(ptr);
    }
    gDexHead = 0;
    gDexNow = 0;
}