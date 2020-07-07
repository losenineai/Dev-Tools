//
// Created by mt on 2017/10/19.
//

#include <jni.h>
#include "include/DXMethodIndexTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/DXCommon.h"
#include "include/env-chain.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"

static DXMethodIndexTable l_methodIndexArr[] = {
        {34,  false, 0, 0}, // "CallObjectMethod",
        {35,  false, 0, 0}, // "CallObjectMethodV",
        {36,  false, 0, 0}, // "CallObjectMethodA",
        {37,  false, 0, 0}, // "CallBooleanMethod",
        {38,  false, 0, 0}, // "CallBooleanMethodV",
        {39,  false, 0, 0}, // "CallBooleanMethodA",
        {40,  false, 0, 0}, // "CallByteMethod",
        {41,  false, 0, 0}, // "CallByteMethodV",
        {42,  false, 0, 0}, // "CallByteMethodA",
        {43,  false, 0, 0}, // "CallCharMethod",
        {44,  false, 0, 0}, // "CallCharMethodV",
        {45,  false, 0, 0}, // "CallCharMethodA",
        {46,  false, 0, 0}, // "CallShortMethod",
        {47,  false, 0, 0}, // "CallShortMethodV",
        {48,  false, 0, 0}, // "CallShortMethodA",
        {49,  false, 0, 0}, // "CallIntMethod",
        {50,  false, 0, 0}, // "CallIntMethodV",
        {51,  false, 0, 0}, // "CallIntMethodA",
        {52,  false, 0, 0}, // "CallLongMethod",
        {53,  false, 0, 0}, // "CallLongMethodV",
        {54,  false, 0, 0}, // "CallLongMethodA",
        {55,  false, 0, 0}, // "CallFloatMethod",
        {56,  false, 0, 0}, // "CallFloatMethodV",
        {57,  false, 0, 0}, // "CallFloatMethodA",
        {58,  false, 0, 0}, // "CallDoubleMethod",
        {59,  false, 0, 0}, // "CallDoubleMethodV",
        {60,  false, 0, 0}, // "CallDoubleMethodA",
        {61,  false, 0, 0}, // "CallVoidMethod",
        {62,  false, 0, 0}, // "CallVoidMethodV",
        {63,  false, 0, 0}, // "CallVoidMethodA",
        {64,  false, 0, 0}, // "CallNonvirtualObjectMethod",
        {65,  false, 0, 0}, // "CallNonvirtualObjectMethodV",
        {66,  false, 0, 0}, // "CallNonvirtualObjectMethodA",
        {67,  false, 0, 0}, // "CallNonvirtualBooleanMethod",
        {68,  false, 0, 0}, // "CallNonvirtualBooleanMethodV",
        {69,  false, 0, 0}, // "CallNonvirtualBooleanMethodA",
        {70,  false, 0, 0}, // "CallNonvirtualByteMethod",
        {71,  false, 0, 0}, // "CallNonvirtualByteMethodV",
        {72,  false, 0, 0}, // "CallNonvirtualByteMethodA",
        {73,  false, 0, 0}, // "CallNonvirtualCharMethod",
        {74,  false, 0, 0}, // "CallNonvirtualCharMethodV",
        {75,  false, 0, 0}, // "CallNonvirtualCharMethodA",
        {76,  false, 0, 0}, // "CallNonvirtualShortMethod",
        {77,  false, 0, 0}, // "CallNonvirtualShortMethodV",
        {78,  false, 0, 0}, // "CallNonvirtualShortMethodA",
        {79,  false, 0, 0}, // "CallNonvirtualIntMethod",
        {80,  false, 0, 0}, // "CallNonvirtualIntMethodV",
        {81,  false, 0, 0}, // "CallNonvirtualIntMethodA",
        {82,  false, 0, 0}, // "CallNonvirtualLongMethod",
        {83,  false, 0, 0}, // "CallNonvirtualLongMethodV",
        {84,  false, 0, 0}, // "CallNonvirtualLongMethodA",
        {85,  false, 0, 0}, // "CallNonvirtualFloatMethod",
        {86,  false, 0, 0}, // "CallNonvirtualFloatMethodV",
        {87,  false, 0, 0}, // "CallNonvirtualFloatMethodA",
        {88,  false, 0, 0}, // "CallNonvirtualDoubleMethod",
        {89,  false, 0, 0}, // "CallNonvirtualDoubleMethodV",
        {90,  false, 0, 0}, // "CallNonvirtualDoubleMethodA",
        {91,  false, 0, 0}, // "CallNonvirtualVoidMethod",
        {92,  false, 0, 0}, // "CallNonvirtualVoidMethodV",
        {93,  false, 0, 0}, // "CallNonvirtualVoidMethodA",
        {114, false, 0, 0}, // "CallStaticObjectMethod",
        {115, false, 0, 0}, // "CallStaticObjectMethodV",
        {116, false, 0, 0}, // "CallStaticObjectMethodA",
        {117, false, 0, 0}, // "CallStaticBooleanMethod",
        {118, false, 0, 0}, // "CallStaticBooleanMethodV",
        {119, false, 0, 0}, // "CallStaticBooleanMethodA",
        {120, false, 0, 0}, // "CallStaticByteMethod",
        {121, false, 0, 0}, // "CallStaticByteMethodV",
        {122, false, 0, 0}, // "CallStaticByteMethodA",
        {123, false, 0, 0}, // "CallStaticCharMethod",
        {124, false, 0, 0}, // "CallStaticCharMethodV",
        {125, false, 0, 0}, // "CallStaticCharMethodA",
        {126, false, 0, 0}, // "CallStaticShortMethod",
        {127, false, 0, 0}, // "CallStaticShortMethodV",
        {128, false, 0, 0}, // "CallStaticShortMethodA",
        {129, false, 0, 0}, // "CallStaticIntMethod",
        {130, false, 0, 0}, // "CallStaticIntMethodV",
        {131, false, 0, 0}, // "CallStaticIntMethodA",
        {132, false, 0, 0}, // "CallStaticLongMethod",
        {133, false, 0, 0}, // "CallStaticLongMethodV",
        {134, false, 0, 0}, // "CallStaticLongMethodA",
        {135, false, 0, 0}, // "CallStaticFloatMethod",
        {136, false, 0, 0}, // "CallStaticFloatMethodV",
        {137, false, 0, 0}, // "CallStaticFloatMethodA",
        {138, false, 0, 0}, // "CallStaticDoubleMethod",
        {139, false, 0, 0}, // "CallStaticDoubleMethodV",
        {140, false, 0, 0}, // "CallStaticDoubleMethodA",
        {141, false, 0, 0}, // "CallStaticVoidMethod",
        {142, false, 0, 0}, // "CallStaticVoidMethodV",
        {143, false, 0, 0}, // "CallStaticVoidMethodA"
};

STEE
static unsigned char* DXMethodIndexTable_ReadText(const char* filePath)
{
    unsigned char* content = NULL;
    size_t length = 0;
    size_t left = 0;
    size_t readed = 0;
    FILE* fp = NULL;

    if (NULL != (fp = fopen(filePath, "rb"))) {
        fseek(fp, 0, SEEK_END);
        length = (int )ftell(fp);

        if (length > 0) {
            content = (unsigned char* )malloc((length) * sizeof(unsigned char));
        }

        if (content != NULL) {
            memset(content, 0, (length) * sizeof(unsigned char));
            fseek(fp, 0, SEEK_SET);

            left = length;

            while (left > 0) {
                readed = fread(content, 1, left, fp);
                if (readed > left || readed == 0) {
                    break;
                }
                left -= readed;
            }
            if (left > 0) {
                free(content);
                content = NULL;
            }
        }

        fclose(fp);
    }

    return content;
}

STEE
bool DXMethodIndexTable_Initalize(JNIEnv* env)
{
    JNINativeInterface* jniNativeInterface = *(JNINativeInterface** )env;
    uintptr_t* funcPtr = (uintptr_t* )jniNativeInterface;
    uintptr_t dvmBase = DXGetDvmBase();

    int methodIndexCount = (int )(sizeof(l_methodIndexArr) / sizeof(DXMethodIndexTable));
    uintptr_t methodAddr = 0;
    unsigned char* dvmContent = DXMethodIndexTable_ReadText(MODULEPATH_DVM);
    if (dvmContent == NULL) {
        dvmContent = DXMethodIndexTable_ReadText(MODULEPATH_DVM2);
    }

    // LOGD("dvmContent: %p", dvmContent);

    if (dvmContent != NULL) {
        for (int i = 0; i < methodIndexCount; ++i) {
            methodAddr = funcPtr[l_methodIndexArr[i].index];
            l_methodIndexArr[i].isThumb = methodAddr & 1;
            l_methodIndexArr[i].offset = methodAddr - dvmBase - (uintptr_t )l_methodIndexArr[i].isThumb;
            l_methodIndexArr[i].checksumInFile = *(uint64_t* )(dvmContent + l_methodIndexArr[i].offset);

            // LOGD("callTable[%d]: %d, %p, %d, %p, %llX", 
            //     i, 
            //     l_methodIndexArr[i].index,
            //     (void* )methodAddr,
            //     l_methodIndexArr[i].isThumb, 
            //     (void* )l_methodIndexArr[i].offset, 
            //     l_methodIndexArr[i].checksumInFile);
        }
        // LOGD("true");
        return true;
    }
    // LOGD("false");
    return false;
}

STEE
bool DXMethodIndexTable_IsJniMethodOk(JNINativeInterface* jniNativeInterface) {
    int methodIndexCount = (int )(sizeof(l_methodIndexArr) / sizeof(DXMethodIndexTable));
    uintptr_t methodAddr = 0;
//    uintptr_t* funcPtr = (uintptr_t* )jniNativeInterface;
    uintptr_t dvmBase = DXGetDvmBase();

    for (int i = 0; i < methodIndexCount; ++i) {
        methodAddr = dvmBase + l_methodIndexArr[i].offset;
        if (*(uint64_t* )methodAddr != l_methodIndexArr[i].checksumInFile) {
            return false;
        }
    }
    return true;
}

STEE
bool DXJavaHookChecker_CheckJniMethodContent(JNINativeInterface* jniNativeInterface)
{
    if (!DXMethodIndexTable_IsJniMethodOk(jniNativeInterface)) {
//        DXAbort();
        return false;
    }
    return true;
}

//bool DXJavaHookChecker_CheckJniMethodContent(JniEnvChain* jniEnvChain)
//{
//    return DXJavaHookChecker_CheckJniMethodContent(DXGetJNINativeInterface(jniEnvChain));
//}