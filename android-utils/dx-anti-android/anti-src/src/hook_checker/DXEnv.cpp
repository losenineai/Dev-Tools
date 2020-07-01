#include "include/DXEnv.h"
#include "dx_anti_stee.h"

STEE
jclass DXFindClass(JNIEnv *env, const char *clsName)
{
    jclass lclazz = nullptr;
    jclass gclazz = nullptr;

    if (nullptr != clsName) {
        lclazz = env->FindClass(clsName);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lclazz = nullptr;
        }
    }

    if (nullptr != lclazz) {
        gclazz = (jclass)env->NewGlobalRef(lclazz);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
            gclazz = nullptr;
        }

        env->DeleteLocalRef(lclazz);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }

    return gclazz;
}

STEE
jmethodID DXGetStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig)
{
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetStaticMethodID(clazz, name, sig);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            methodID = nullptr;
        }
    }
    return methodID;
}

STEE
jmethodID DXGetMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig)
{
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetMethodID(clazz, name, sig);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            methodID = nullptr;
        }
    }
    return methodID;
}

STEE
jclass DXGetObjectClass(JNIEnv *env, jobject obj)
{
    jclass lclazz = nullptr;
    jclass gclazz = nullptr;

    if (nullptr != obj) {
        lclazz = env->GetObjectClass(obj);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lclazz = nullptr;
        }
    }

    if (nullptr != lclazz) {
        gclazz = (jclass)env->NewGlobalRef(lclazz);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
            gclazz = nullptr;
        }
        env->DeleteLocalRef(lclazz);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }

    return gclazz;
}

STEE
int DXDeleteGlobalRef(JNIEnv *env, jobject globalRef)
{
    if (nullptr != globalRef) {
        env->DeleteGlobalRef(globalRef);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            return -1;
        }
    }
    return 0;
}

STEE
jmethodID DXGetObjectMethodID(JNIEnv *env, jobject obj, const char *name, const char *sig)
{
    jclass clazz = nullptr;
    jmethodID methodID = nullptr;

    clazz = DXGetObjectClass(env, obj);
    if (nullptr != clazz) {
        methodID = DXGetMethodID(env, clazz, name, sig);
        DXDeleteGlobalRef(env, clazz);
    }

    return methodID;
}

STEE
jboolean DXCallBooleanMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
{
    jmethodID methodID = nullptr;
    jboolean ret = 0;
    va_list args;

    va_start(args, sig);
    methodID = DXGetObjectMethodID(env, obj, name, sig);
    if (methodID) {
        ret = DXCallBooleanMethodV(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jboolean DXCallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
{
    jboolean ret = false;

    if (nullptr != obj) {
        ret = env->CallBooleanMethodV(obj, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            ret = false;
        }
    }
    return ret;
}

STEE
jint DXCallIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
{
    jmethodID methodID = nullptr;
    jint ret = 0;
    va_list args;

    va_start(args, sig);
    methodID = DXGetObjectMethodID(env, obj, name, sig);
    if (methodID) {
        ret = DXCallIntMethodV(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jint DXCallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
{
    jint ret = false;

    if (nullptr != obj) {
        ret = env->CallIntMethodV(obj, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            ret = false;
        }
    }
    return ret;
}

STEE
jobject DXCallObjectMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
{
    jmethodID methodID;
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    methodID = DXGetObjectMethodID(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = DXCallObjectMethodV(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jobject DXCallObjectMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
{
    jobject gObjRet = nullptr;
    jobject lObjRet = nullptr;

    if (nullptr != obj && nullptr != methodID)
    {
        lObjRet = env->CallObjectMethodV(obj, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lObjRet = nullptr;
        }
        if (!lObjRet) {
            return nullptr;
        }

    }

    if (nullptr != lObjRet) {
        gObjRet = env->NewGlobalRef(lObjRet);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            gObjRet = nullptr;
        }
        env->DeleteLocalRef(lObjRet);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }
    return gObjRet;
}

STEE
int DXCallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
{
    if (nullptr != obj) {
        env->CallVoidMethodV(obj, methodID, args);
        if (!env->ExceptionOccurred())
        {
            return 0;
        }
        env->ExceptionClear();
    }
    return -1;
}

STEE
int DXCallVoidMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
{
    jmethodID methodID = nullptr;
    int ret = -1;
    va_list args;

    va_start(args, sig);
    methodID = DXGetObjectMethodID(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = DXCallVoidMethodV(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jboolean DXCallStaticBooleanMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
{
    jboolean ret = false;

    if (nullptr != clazz) {
        ret = env->CallStaticBooleanMethodV(clazz, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            ret = false;
        }
    }
    return ret;
}

STEE
jboolean DXCallStaticBooleanMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
{
    jmethodID methodID = nullptr;
    jboolean ret = 0;
    va_list args;

    va_start(args, sig);
    methodID = DXGetStaticMethodID(env, clazz, name, sig);
    if (methodID) {
        ret = DXCallStaticBooleanMethodV(env, clazz, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jobject DXNewObjectV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
{
    jobject gObjRet = nullptr;
    jobject lObjRet = nullptr;

    if (nullptr != clazz && nullptr != methodID)
    {
        lObjRet = env->NewObjectV(clazz, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lObjRet = nullptr;
        }
        if (!lObjRet) {
            return nullptr;
        }

    }

    if (nullptr != lObjRet) {
        gObjRet = env->NewGlobalRef(lObjRet);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            gObjRet = nullptr;
        }
        env->DeleteLocalRef(lObjRet);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }
    return gObjRet;
}

STEE
jobject DXNewObjectByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
{
    jmethodID methodID;
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    methodID = DXGetMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = DXNewObjectV(env, clazz, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
jobject DXToReflectedMethod(JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic)
{
    jobject gObjRet = nullptr;
    jobject lObjRet = nullptr;

    if (nullptr != cls && nullptr != methodID)
    {
        lObjRet = env->ToReflectedMethod(cls, methodID, isStatic);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lObjRet = nullptr;
        }
        if (!lObjRet) {
            return nullptr;
        }

    }

    if (nullptr != lObjRet) {
        gObjRet = env->NewGlobalRef(lObjRet);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            gObjRet = nullptr;
        }
        env->DeleteLocalRef(lObjRet);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }
    return gObjRet;
}


// jmethod DXFindMethod(JNIEnv* env, 
    // const char* clsName, 
    // const char* name, 
    // const char* sig)
// {
//     jmethod method = NULL;
//     jclass cls = env->FindClass(clsName);


//     if (cls != NULL) {
//         method = env->GetMethodID(cls, name, sig)
//     }
//     return method;
// }



// special fo check whether the method is native
// bool DXIsMethodNative(
//     JNIEnv* env, 
//     const char* clsName, 
//     const char* name, 
//     const char* sig)
// {
//     // get methodID
//     jclass cls = DXFindClass(env, clsName);
//     jmethodID methodID = DXGetMethodID(env, cls, name, sig);
//     DXDeleteGlobalRef(cls);
    
//     // 
// }








