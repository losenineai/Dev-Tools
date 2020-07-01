//
// Created by mt on 2017/10/19.
//

#ifndef JAVAHOOKCHEKER_DXCOMMON_H
#define JAVAHOOKCHEKER_DXCOMMON_H

#include "env-chain.h"
#include <jni.h>
#include "pthread.h"
//#include "DXBase.h"

#define MODULENAME_DVM "libdvm.so"
#define MODULEPATH_DVM "/system/lib/libdvm.so"

#define MODULENAME_DVM2 "libart.so"
#define MODULEPATH_DVM2 "/system/lib/libart.so"

// void DXAbort(uintptr_t addr = (uintptr_t )__builtin_return_address(0));
void DXAbort_();

#define DXAbort() \
{ \
    /*LOGD("******** Abort: %p ********", (void* )(uintptr_t )__builtin_return_address(0));*/ \
    DXAbort_(); \
}

// void DXAbort();
JNIEnv* DXGetJNIEvn(JniEnvChain* jniEnvChain);
//JNINativeInterface* DXGetJNINativeInterface(JniEnvChain* jniEnvChain);


// The "validSize" is the valid size I caring.
uintptr_t DXGetModuleBase(uintptr_t* validSize, const char* moduleName, pid_t pid = -1);
bool DXIsAddrInSystem(uintptr_t addr, pid_t pid = -1);

void DXSetDvmBase(uintptr_t dvmBase, uintptr_t dvmValidSize);
uintptr_t DXGetDvmBase(uint64_t* dvmValidSize = NULL);
uintptr_t DXInitalizeDvmBase(void);
bool DXIsDvmVadd(uintptr_t va);

bool DXIsMethodNative(
    JNIEnv* env, 
    jobject modifier,
    const char* clsName, 
    const char* name, 
    const char* sig);



#endif //JAVAHOOKCHEKER_DXCOMMON_H
