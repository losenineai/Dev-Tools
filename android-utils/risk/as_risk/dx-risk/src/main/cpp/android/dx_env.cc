#include "dx_env.h"
#include "jni_utils.h"
#include "stee.h"
#include "DXLog.h"
#include "jni_const.h"


jobject g_context = NULL;


STEE
jclass DXFindClass(JNIEnv *env, const char *clsName)
{
    jclass lclazz = nullptr;

    if (nullptr != clsName) {
        lclazz = env->FindClass(clsName);
        jthrowable thro = env->ExceptionOccurred();
        if (thro != NULL)
        {
            env->ExceptionClear();
//            env->Throw(jthrowable1);
            lclazz = nullptr;
        }
    }

    return lclazz;
}

STEE
jmethodID DXGetStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig)
{
    jmethodID methodID = nullptr;

    if (clazz == NULL) {
        LOGD("Can't found the jni class.");
    }
    else{
        methodID = env->GetStaticMethodID(clazz, name, sig);

        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
            methodID = nullptr;
        }

        if (methodID == NULL) {
            LOGD("Can't found the %s method from java.", name);
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

        if (methodID == NULL) {
            LOGD("Can't found the %s method from java.", name);
        }
    }
    return methodID;
}

STEE
jclass DXGetObjectClass(JNIEnv *env, jobject obj)
{
    jclass lclazz = nullptr;

    if (nullptr != obj) {
        lclazz = env->GetObjectClass(obj);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lclazz = nullptr;
        }
    }

    return lclazz;
}

//STEE
//int DXDeleteGlobalRef(JNIEnv *env, jobject globalRef)
//{
//    if (nullptr != globalRef) {
//        env->DeleteGlobalRef(globalRef);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            return -1;
//        }
//    }
//    return 0;
//}

STEE
jmethodID DXGetObjectMethodID(JNIEnv *env, jobject obj, const char *name, const char *sig)
{
    jclass clazz = nullptr;
    jmethodID methodID = nullptr;

    clazz = DXGetObjectClass(env, obj);
    if (nullptr != clazz) {
        methodID = DXGetMethodID(env, clazz, name, sig);
    }

    return methodID;
}

//STEE
//jboolean DXCallBooleanMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
//{
//    jmethodID methodID = nullptr;
//    jboolean ret = 0;
//    va_list args;
//
//    va_start(args, sig);
//    methodID = DXGetObjectMethodID(env, obj, name, sig);
//    if (methodID) {
//        ret = DXCallBooleanMethodV(env, obj, methodID, args);
//    }
//    va_end(args);
//    return ret;
//}
//
//STEE
//jboolean DXCallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
//{
//    jboolean ret = false;
//
//    if (nullptr != obj) {
//        ret = env->CallBooleanMethodV(obj, methodID, args);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            ret = false;
//        }
//    }
//    return ret;
//}
//
//STEE
//jint DXCallIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
//{
//    jmethodID methodID = nullptr;
//    jint ret = 0;
//    va_list args;
//
//    va_start(args, sig);
//    methodID = DXGetObjectMethodID(env, obj, name, sig);
//    if (methodID) {
//        ret = DXCallIntMethodV(env, obj, methodID, args);
//    }
//    va_end(args);
//    return ret;
//}
//
//STEE
//jint DXCallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
//{
//    jint ret = false;
//
//    if (nullptr != obj) {
//        ret = env->CallIntMethodV(obj, methodID, args);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            ret = false;
//        }
//    }
//    return ret;
//}

STEE
jobject DXCallStaticObjectMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
{
    jobject lObjRet = nullptr;

    if (nullptr != clazz && nullptr != methodID)
    {
        lObjRet = env->CallStaticObjectMethodV(clazz, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            lObjRet = nullptr;
        }
    }

    return lObjRet;
}

STEE
jobject DXCallStaticObjectMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
{
    jmethodID methodID;
    jobject ret = nullptr;
    va_list args;

    if (clazz  == nullptr) {
        return ret;
    }

    va_start(args, sig);
    methodID = DXGetStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = DXCallStaticObjectMethodV(env, clazz, methodID, args);
    }
    va_end(args);
    return ret;
}

STEE
int DXCallStaticVoidMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
{
    if (nullptr != clazz && nullptr != methodID)
    {
        env->CallStaticVoidMethodV(clazz, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionClear();
            return -1;
        }
    }

    return 0;
}

STEE
int DXCallStaticVoidMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
{
    jmethodID methodID;
    int ret = -1;
    va_list args;

    va_start(args, sig);
    methodID = DXGetStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = DXCallStaticVoidMethodV(env, clazz, methodID, args);
    }
    va_end(args);
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
    jobject lObjRet = nullptr;

    if (nullptr != obj && nullptr != methodID)
    {
        lObjRet = env->CallObjectMethodV(obj, methodID, args);
        if (env->ExceptionOccurred())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
            lObjRet = nullptr;
        }
    }

    return lObjRet;
}

//STEE
//int DXCallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args)
//{
//    if (nullptr != obj) {
//        env->CallVoidMethodV(obj, methodID, args);
//        if (!env->ExceptionOccurred())
//        {
//            return 0;
//        }
//        env->ExceptionClear();
//    }
//    return -1;
//}
//
//STEE
//int DXCallVoidMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...)
//{
//    jmethodID methodID = nullptr;
//    int ret = -1;
//    va_list args;
//
//    va_start(args, sig);
//    methodID = DXGetObjectMethodID(env, obj, name, sig);
//    if (nullptr != methodID) {
//        ret = DXCallVoidMethodV(env, obj, methodID, args);
//    }
//    va_end(args);
//    return ret;
//}

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

//STEE
//jint DXCallStaticIntMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
//{
//    jint ret = -1;
//
//    if (nullptr != clazz) {
//        ret = env->CallStaticIntMethodV(clazz, methodID, args);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            ret = -1;
//        }
//    }
//    return ret;
//}
//
//STEE
//jint DXCallStaticIntMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
//{
//    jmethodID methodID = nullptr;
//    jint ret = -1;
//    va_list args;
//
//    va_start(args, sig);
//    methodID = DXGetStaticMethodID(env, clazz, name, sig);
//
//    if (methodID) {
//        ret = DXCallStaticIntMethodV(env, clazz, methodID, args);
//    }
//    va_end(args);
//
//    return ret;
//}
//
//
//STEE
//jobject DXNewObjectV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args)
//{
//    jobject lObjRet = nullptr;
//
//    if (nullptr != clazz && nullptr != methodID)
//    {
//        lObjRet = env->NewObjectV(clazz, methodID, args);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            lObjRet = nullptr;
//        }
//
//    }
//
//    return lObjRet;
//}
//
//STEE
//jobject DXNewObjectByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...)
//{
//    jmethodID methodID;
//    jobject ret = nullptr;
//    va_list args;
//
//    va_start(args, sig);
//    methodID = DXGetMethodID(env, clazz, name, sig);
//    if (nullptr != methodID) {
//        ret = DXNewObjectV(env, clazz, methodID, args);
//    }
//    va_end(args);
//    return ret;
//}
//
//STEE
//jobject DXToReflectedMethod(JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic)
//{
//    jobject lObjRet = nullptr;
//
//    if (nullptr != cls && nullptr != methodID)
//    {
//        lObjRet = env->ToReflectedMethod(cls, methodID, isStatic);
//        if (env->ExceptionOccurred())
//        {
//            env->ExceptionClear();
//            lObjRet = nullptr;
//        }
//
//    }
//
//    return lObjRet;
//}
//
////jobject DXGetGlobalContext(JNIEnv *env)
////{
////    jclass ActivityThread = DXFindClass(env, "android/app/ActivityThread");
////    info_log("DXFindClassv ActivityThread:%p", ActivityThread);
////    jobject activityThread = DXCallStaticObjectMethodByName(env, ActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
////    jobject context = DXCallObjectMethodByName(env, activityThread, "getApplication", "()Landroid/app/Application;");
////    info_log("DXGetGlobalContext context:%p", context);
////    return context;
////}
//
//
//
//jobject doGetGlobalContext_internal(JNIEnv *env) {
//
//    if (g_anti_context != NULL) {
//        return g_anti_context;
//    }
//
//    // 2
//    jclass j_actiThread = DXFindClass(env, "android/app/ActivityThread");
//
//    g_anti_context = DXCallStaticObjectMethodByName(env, j_actiThread, "currentApplication",
//                                                        "()Landroid/app/Application;");
//
//    delete_ref(env, j_actiThread);
//
//    if (g_anti_context != NULL) {
//        g_anti_context = convert_to_global_ref(env, g_anti_context);
//
//        return g_anti_context;
//    }
//
//    // 3
//    jclass j_appGlobals = DXFindClass(env, "android/app/AppGlobals");
//
//    g_anti_context = DXCallStaticObjectMethodByName(env, j_appGlobals, "getInitialApplication",
//                                                        "()Landroid/app/Application;");
//    delete_ref(env, j_appGlobals);
//
//    if (g_anti_context != NULL) {
//        g_anti_context = convert_to_global_ref(env, g_anti_context);
//        return g_anti_context;
//    }
//
//    return NULL;
//}

jobject DXGetGlobalContext(JNIEnv *env) {

    if (g_context)
        return g_context;

    GetStaticMethod(FUNC_CONTEXT, FUNC_CONTEXT_SIGN);
    jobject j_context = (jobject) CallStaticMethod(nullptr, Object, FUNC_CONTEXT);

    if (j_context != NULL) {
        g_context = convert_to_global_ref(env, j_context);
        return g_context;
    } else {
        LOGE("context don't found");
    }

    return nullptr;

}

STEE
bool DXCheckException(JNIEnv *env) {

    if (env->ExceptionCheck() == JNI_TRUE) {
        jthrowable exp = env->ExceptionOccurred();
        if (exp != NULL) {
//            anti_err_log("occurred jni exception!!!");
//            env->Throw(exp);
            env->ExceptionClear();
        }

        return true;
    }

    return false;
}

STEE
jobject DXGetStaticObjectField(JNIEnv *env, const char *clasName, const char *name,
                               const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jobject ret = nullptr;

    clazz = DXFindClass(env, clasName);

    if (clazz != nullptr) {
        fieldID = env->GetStaticFieldID(clazz, name, sig);
        if (!DXCheckException(env)) {
            ret = env->GetStaticObjectField(clazz, fieldID);
            if (DXCheckException(env)) {
                ret = nullptr;
            }
        }
    }

    return ret;
}







