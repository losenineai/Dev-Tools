//
// Created by mt on 2017/10/19.
//

#ifndef JAVAHOOKCHEKER_DXMETHODINDEXTABLE_H
#define JAVAHOOKCHEKER_DXMETHODINDEXTABLE_H

#include <cstdint>
#include "jni.h"
#include "env-chain.h"

typedef struct _DXMethodIndexTable {
    int index;
    bool isThumb;
    uintptr_t offset;
    uint64_t checksumInFile;
} DXMethodIndexTable;

bool DXMethodIndexTable_Initalize(JNIEnv* env);
bool DXJavaHookChecker_CheckJniMethodContent(JNINativeInterface* jniNativeInterface);
//bool DXJavaHookChecker_CheckJniMethodContent(JniEnvChain* jniEnvChain);

#endif //JAVAHOOKCHEKER_DXMETHODINDEXTABLE_H
