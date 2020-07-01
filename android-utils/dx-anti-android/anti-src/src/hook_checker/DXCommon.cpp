//
// Created by mt on 2017/10/19.
//

#include "utils_anti_log.h"
#include "include/DXCommon.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"
#include "include/env-chain-default.h"
#include "include/DXEnv.h"

// void DXAbort(uintptr_t addr)
// {
//     LOGD("******** Abort: %p ********", (void* )addr);
//     while (1) {

//     }
// }

STEE
void DXAbort_()
{
    uintptr_t a = 0;
    *(uintptr_t* )a = 100;
}

STEE
JNIEnv* DXGetJNIEvn(JniEnvChain* jniEnvChain)
{
    if (NULL != jniEnvChain) {
        JniEnvChain* p = jniEnvChain;

        while (NULL != p->next) {
            p = p->next;
        }

        JniEnvChainDefault* def = (JniEnvChainDefault* )p;
        return def->GetEnv();
    }
    return NULL;
}

//JNINativeInterface* DXGetJNINativeInterface(JniEnvChain* jniEnvChain)
//{
//    if (NULL != jniEnvChain) {
//        JniEnvChain* p = jniEnvChain;
//
//        while (NULL != p->next) {
//            p = p->next;
//        }
//
//        JNIEnv* jniEnv = (JNIEnv* )p;
//        return *(JNINativeInterface** )jniEnv;
//    }
//    return NULL;
//}

STEE
uintptr_t DXGetModuleBase(uintptr_t* validSize, const char* moduleName, pid_t pid)
{
    char buff[512] = {0};

    if (validSize != NULL) {
        *validSize = 0;
    }

    if (moduleName == NULL || moduleName[0] == '\0') {
        return 0;
    }
    int libNameLen = (int )strlen(moduleName);

    char filename[128] = {0};
    if (pid < 0) {
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    } else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }

    FILE* file = fopen(filename, "rt");
    anti_info_log("file: %p", file);
    if (file == NULL) {
        return 0;
    }

    char flags[4] = {0};
    size_t addr = 0;
    size_t start, end, offset;
    while (fgets(buff, sizeof buff, file) != NULL)
    {
        // LOGD("line: %s", buff);
        int len = (int)strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';
        if (len <= libNameLen || memcmp(buff + len - libNameLen, moduleName, libNameLen))
            continue;
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        // LOGD("start = %p, end = %p", (void* )start, (void* )end);
        if (flags[0] != 'r' || flags[2] != 'x')
            continue;
        addr = start - offset;
        if (validSize != NULL) {
            *validSize = end - start;
        }
        break;
    }
    fclose(file);

    return addr;
}

STEE
bool DXIsAddrInSystem(uintptr_t addr, pid_t pid)
{
    bool flag = false;
    char buff[0x1000] = {0};
    // int libNameLen = (int )strlen(moduleName);

    char filename[128] = {0};
    if (pid < 0) {
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    } else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }

    FILE* file = fopen(filename, "rt");
    anti_info_log("file: %p", file);
    if (file == NULL) {
        return 0;
    }

    char flags[4] = {0};
    size_t start, end, offset;
    while (fgets(buff, sizeof buff, file) != NULL)
    {
        // LOGD("line: %s", buff);
        int len = (int)strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';
        // if (len <= libNameLen || memcmp(buff + len - libNameLen, moduleName, libNameLen))
        //     continue;
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        // LOGD("start = %p, end = %p", (void* )start, (void* )end);
        if (flags[0] != 'r' || flags[2] != 'x')
            continue;
        if (addr >= start && addr < end) {
            if (strstr(buff, "libdvm.so")
                || strstr(buff, "libart.so")
                || strstr(buff, ".odex"))
            {
                flag = true;
                break;
            }
        }
    }
    fclose(file);

    return flag;
}

static uintptr_t l_dvmBase = 0;
static uintptr_t l_dvmValidSize = 0;
STEE
void DXSetDvmBase(uintptr_t dvmBase, uintptr_t dvmValidSize)
{
    l_dvmValidSize = dvmValidSize;
    l_dvmBase = dvmBase;
}

STEE
uintptr_t DXGetDvmBase(uint64_t* dvmValidSize)
{
    if (dvmValidSize != NULL) {
        *dvmValidSize = l_dvmValidSize;
    }
    return l_dvmBase;
}

STEE
uintptr_t DXInitalizeDvmBase(void)
{
    uintptr_t dvmBase = 0;
    uintptr_t dvmValidSize = 0;

    dvmBase = DXGetModuleBase(&dvmValidSize, MODULENAME_DVM);
    anti_info_log("dvmBase = %p", (void* )dvmBase);
    if (dvmBase == 0) {
        dvmBase = DXGetModuleBase(&dvmValidSize, MODULENAME_DVM2);
        anti_info_log("dvmBase2 = %p", (void* )dvmBase);
    }
    DXSetDvmBase(dvmBase, dvmValidSize);
    return dvmBase;
}

STEE
bool DXIsDvmVadd(uintptr_t va)
{
//    anti_info_log("dvm(%p, %p), va: %p", l_dvmBase, l_dvmValidSize, va);
    if (l_dvmBase != 0 && va >= l_dvmBase && va - l_dvmBase < l_dvmValidSize) {
        return true;
    }
    return false;
}

STEE
bool DXIsMethodNative(
    JNIEnv* env, 
    jobject modifier,
    // jclass Modifier,
    const char* clsName, 
    const char* name, 
    const char* sig)
{

    // get methodID
    jmethodID methodID;
    jclass cls = DXFindClass(env, clsName);
    methodID = DXGetMethodID(env, cls, name, sig);
    if (nullptr == methodID) {
        methodID = DXGetStaticMethodID(env, cls, name, sig);
    }
    // get method by reflect
    jobject method = DXToReflectedMethod(env, cls, methodID, false);

    // int mod = method.getModifiers();
    jint mod = DXCallIntMethodByName(env, method, "getModifiers", "()I");

    // bool isNative = modifier.isNative(mod);
    jclass ccc = DXFindClass(env, "java/lang/reflect/Modifier");
    jboolean isNative = DXCallStaticBooleanMethodByName(env, ccc, "isNative", "(I)Z", mod);


    anti_info_log("Jni expotr method: %p-%p-%p-%d-%d", cls, methodID, method, mod, isNative);


    DXDeleteGlobalRef(env, method);
    DXDeleteGlobalRef(env, cls);
    return isNative;
}



