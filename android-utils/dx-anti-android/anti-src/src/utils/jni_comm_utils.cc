//
// Created by white on 2018/5/14.
//


#include <include/anti_utils.h>
#include <src/utils/include/comm_utils.h>
#include "include/jni_comm_utils.h"
#include "include/jni_utils.h"
#include "src/utils/include/utils_anti_log.h"
#include <mutex>

jobject g_anti_context = NULL;
std::mutex g_mt_context;

void anti_jni_setContext(JNIEnv *env, jobject jContext){
    g_anti_context = anti_jni_convert_to_global_ref(env, jContext);
}

jclass anti_jni_findClassByName(JNIEnv *env, const char *clsName) {
    jclass lclazz = nullptr;

    if (nullptr != clsName) {
        lclazz = env->FindClass(clsName);
        if (anti_jni_CheckException(env)) {
            lclazz = nullptr;
        }
    }

    return lclazz;
}

jmethodID anti_jni_getStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetStaticMethodID(clazz, name, sig);
        if (anti_jni_CheckException(env)) {
            methodID = nullptr;
        }
    }

    return methodID;
}

jmethodID anti_jni_getMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetMethodID(clazz, name, sig);
        if (anti_jni_CheckException(env)) {
            methodID = nullptr;
        }
    }
    return methodID;
}

jclass anti_jni_findClassByObj(JNIEnv *env, jobject obj) {
    jclass lclazz = nullptr;
    jclass gclazz = nullptr;

    if (nullptr != obj) {
        lclazz = env->GetObjectClass(obj);
        if (anti_jni_CheckException(env)) {
            lclazz = nullptr;
        }
    }

    if (nullptr != lclazz) {
        gclazz = (jclass) env->NewGlobalRef(lclazz);
        if (anti_jni_CheckException(env)) {
            gclazz = nullptr;
        }
        env->DeleteLocalRef(lclazz);
        if (env->ExceptionOccurred()) {
            env->ExceptionClear();
        }
    }

    return gclazz;
}

int anti_jni_deleteGlobalRef(JNIEnv *env, jobject globalRef) {
    if (nullptr != globalRef) {
        env->DeleteGlobalRef(globalRef);
        if (anti_jni_CheckException(env)) {
            return -1;
        }
    }
    return 0;
}

jmethodID anti_jni_getMethodIDByObj(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass clazz = nullptr;
    jmethodID methodID = nullptr;

    clazz = anti_jni_findClassByObj(env, obj);
    if (nullptr != clazz) {
        methodID = anti_jni_getMethodID(env, clazz, name, sig);
        anti_jni_deleteGlobalRef(env, clazz);
    }

    return methodID;
}


jobject anti_jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, jmethodID methodID,
                                           va_list args) {
    jobject lObjRet = nullptr;

    if (nullptr != clazz && nullptr != methodID) {
        lObjRet = env->CallStaticObjectMethodV(clazz, methodID, args);
        if (anti_jni_CheckException(env)){
            lObjRet = nullptr;
        }
    }

    return lObjRet;
}


jobject anti_jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, const char *name,
                                           const char *sig, ...) {
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    jmethodID methodID = anti_jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = anti_jni_callStaticMethodReturnObj(env, clazz, methodID, args);
    }
    va_end(args);
    return ret;
}

jboolean anti_jni_callStaticMethodReturnBool(JNIEnv *env, jclass clazz, const char *name,
                                             const char *sig, ...) {
    jmethodID methodID;
    jboolean ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = env->CallStaticBooleanMethodV(clazz, methodID, args);
    }
    va_end(args);
    return ret;
}


jobject anti_jni_callMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...) {
    jmethodID methodID;
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getMethodIDByObj(env, obj, name, sig);

    if (nullptr != methodID) {
        ret = anti_jni_callMethodById(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}


jobject anti_jni_callMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jobject gObjRet = nullptr;
    jobject lObjRet = nullptr;

    if (nullptr != obj && nullptr != methodID) {
        lObjRet = env->CallObjectMethodV(obj, methodID, args);
        if (anti_jni_CheckException(env)) {
            lObjRet = nullptr;
        }
        if (!lObjRet) {
            return nullptr;
        }

    }

    if (nullptr != lObjRet) {
        gObjRet = env->NewGlobalRef(lObjRet);
        if (anti_jni_CheckException(env)) {
            gObjRet = nullptr;
        }
        env->DeleteLocalRef(lObjRet);
        if (anti_jni_CheckException(env)) {
            gObjRet = nullptr;
        }
    }
    return gObjRet;
}



void anti_jni_callVoidMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                   ...) {
    jmethodID methodID;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getMethodIDByObj(env, obj, name, sig);

    if (nullptr != methodID) {
        anti_jni_callVoidMethodById(env, obj, methodID, args);
    }
    va_end(args);

}


void anti_jni_callVoidMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args){

    if (nullptr != obj && nullptr != methodID) {
        env->CallVoidMethodV(obj, methodID, args);
        if (anti_jni_CheckException(env)) {
        }
    }

}

jobject anti_jni_getGlobalContext_internal(JNIEnv *env) {

    if (g_anti_context != NULL) {
        return g_anti_context;
    }

    // 1 crash
    /*jobject jo_binder = anti_jni_getStaticObjectField(env, "com/android/internal/os/RuntimeInit",
                                                      "mApplicationObject", "Landroid/os/IBinder;");

    jobject jo_actiThread = anti_jni_getObjectField(env, jo_binder, "this$0", "Landroid/app/ActivityThread;");

    g_anti_context = anti_jni_getObjectField(env, jo_actiThread, "mInitialApplication",
                                             "Landroid/app/Application;");

    anti_jni_delete_ref(env, jo_binder);
    anti_jni_delete_ref(env, jo_actiThread);
    anti_info_log("anti_jni_getGlobalContext 1 %p", g_anti_context);

    if (g_anti_context != NULL) {
        g_anti_context = anti_jni_convert_to_global_ref(env, g_anti_context);
        return g_anti_context;
    }*/

    jobject anti_context;

    // 2
    jclass j_actiThread = anti_jni_findClassByName(env, "android/app/ActivityThread");
    anti_context = anti_jni_callStaticMethodReturnObj(env, j_actiThread, "currentApplication",
                                                        "()Landroid/app/Application;");

    anti_jni_delete_ref(env, j_actiThread);

    if (anti_context != NULL) {
        g_anti_context = anti_jni_convert_to_global_ref(env, anti_context);
        anti_jni_delete_ref(env, anti_context);
        return g_anti_context;
    }

    // 3
    jclass j_appGlobals = anti_jni_findClassByName(env, "android/app/AppGlobals");
    anti_context = anti_jni_callStaticMethodReturnObj(env, j_appGlobals, "getInitialApplication",
                                                        "()Landroid/app/Application;");

    anti_jni_delete_ref(env, j_appGlobals);

    if (g_anti_context != NULL) {
        g_anti_context = anti_jni_convert_to_global_ref(env, g_anti_context);
        anti_jni_delete_ref(env, anti_context);
        return g_anti_context;
    }

    return NULL;
}

jobject anti_jni_getGlobalContext(JNIEnv *env) {

    if (g_anti_context)
        return g_anti_context;

    g_mt_context.lock();

    if (g_anti_context)
        return g_anti_context;

    bool flag = true;

    while (flag) {

        anti_jni_getGlobalContext_internal(env);

        if (g_anti_context)
            flag = false;

    }

    g_mt_context.unlock();

    return g_anti_context;
}

jobject anti_jni_getObjectField(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jobject ret = nullptr;

    clazz = anti_jni_findClassByObj(env, obj);

    if (clazz != nullptr) {
        fieldID = env->GetFieldID(clazz, name, sig);
        if (!anti_jni_CheckException(env)) {
            ret = env->GetObjectField(obj, fieldID);
            if (anti_jni_CheckException(env)) {
                ret = nullptr;
            }
        }
    }
    return ret;
}

jint anti_jni_getIntField(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jint ret = -1;

    clazz = anti_jni_findClassByObj(env, obj);

    if (clazz != nullptr) {
        fieldID = env->GetFieldID(clazz, name, sig);
        if (!anti_jni_CheckException(env)) {
            ret = env->GetIntField(obj, fieldID);
            if (anti_jni_CheckException(env)) {
                ret = -1;
            }
        }
    }
    return ret;
}

jobject anti_jni_getStaticObjectField(JNIEnv *env, const char *clasName, const char *name,
                                      const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jobject ret = nullptr;

    clazz = anti_jni_findClassByName(env, clasName);

    if (clazz != nullptr) {
        fieldID = env->GetStaticFieldID(clazz, name, sig);
        if (!anti_jni_CheckException(env)) {
            ret = env->GetStaticObjectField(clazz, fieldID);
            if (anti_jni_CheckException(env)) {
                ret = nullptr;
            }
        }
    }

    return ret;
}


jint
anti_jni_callStaticMethodReturnInt(JNIEnv *env, jclass clazz, const char *name, const char *sig,
                                   ...) {

    jmethodID methodID = nullptr;
    jint ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = env->CallStaticIntMethodV(clazz, methodID, args);
        if (anti_jni_CheckException(env)) {
            ret = JNI_FALSE;
        }
    }
    va_end(args);
    return ret;
}


jboolean anti_jni_callBoolMethodByName(JNIEnv *env, jobject obj, const char *name,
                                       const char *sig, ...) {
    jmethodID methodID = nullptr;
    jboolean ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getMethodIDByObj(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = anti_jni_callBoolMethodById(env, obj, methodID, args);
        if (anti_jni_CheckException(env)) {
            ret = JNI_FALSE;
        }
    }
    va_end(args);
    return ret;
}


jint anti_jni_callIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...) {
    jmethodID methodID = nullptr;
    jint ret = 0;
    va_list args;

    va_start(args, sig);
    methodID = anti_jni_getMethodIDByObj(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = anti_jni_callIntMethodById(env, obj, methodID, args);
        if (anti_jni_CheckException(env)) {
            ret = 0;
        }
    }
    va_end(args);
    return ret;
}

jint anti_jni_getStaticFieldRetIntByName(JNIEnv *env, const char *clsname, const char *metname,
                                         const char *sig) {
    jint jret = 0;

    jclass jclaxx = anti_jni_findClassByName(env, clsname);
    jfieldID fieldId = env->GetStaticFieldID(jclaxx, metname, sig);
    if (anti_jni_CheckException(env)) {
        jret = env->GetStaticIntField(jclaxx, fieldId);
        if (anti_jni_CheckException(env)) {
            jret = 0;
        }
    }
    return jret;
}

//signature's return type must be void
jobject anti_jni_NewObject(JNIEnv *env, const char *clsname, const char *sig, ...) {

    jmethodID constructorMID = nullptr;
    jobject objRet = nullptr;
    jclass structclaxx = nullptr;
    va_list args;

    structclaxx = anti_jni_findClassByName(env, clsname);

    if (structclaxx != nullptr) {
        va_start(args, sig);
        constructorMID = env->GetMethodID(structclaxx, "<init>", sig);
        if (!anti_jni_CheckException(env)) {
            objRet = env->NewObjectV(structclaxx, constructorMID, args);
            if (anti_jni_CheckException(env)) {
                return nullptr;
            }
        }
        va_end(args);

    }
    return objRet;
}

jobjectArray anti_jni_stringArray2jobjectArray(JNIEnv *env, string strArray[], int len) {
    jstring jstr = nullptr;
    jobjectArray jobjs = nullptr;

    for (int i = 0; i < len; ++i) {
        const char *elem = strArray[i].c_str();
        jstr = env->NewStringUTF(elem);
        env->SetObjectArrayElement(jobjs, i, jstr);
        if (anti_jni_CheckException(env)) {
            return nullptr;
        }
    }
    return jobjs;
}

jint anti_jni_get_sdk_int() {

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    return anti_jni_getStaticFieldRetIntByName(env, "android/os/Build$VERSION", "SDK_INT", "I");
}

jboolean anti_jni_callBoolMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jboolean jboolret = JNI_FALSE;
    if (nullptr != obj && nullptr != methodID) {
        jboolret = env->CallBooleanMethodV(obj, methodID, args);
        if (anti_jni_CheckException(env)) {
            jboolret = JNI_FALSE;
        }
    }

    return jboolret;
}

jint anti_jni_callIntMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jint jintRet = 0;
    if (nullptr != obj && nullptr != methodID) {
        jintRet = env->CallIntMethodV(obj, methodID, args);
        //how to check the exception
        if (anti_jni_CheckException(env)) {
            jintRet = 0;
        }
    }

    return jintRet;
}

