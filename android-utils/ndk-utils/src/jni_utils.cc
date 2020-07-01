//
// Created by white on 2018/5/14.
//


#include "log.h"
#include "jni_utils.h"
#include <mutex>
#include <stdlib.h>
#include <string>
#include <exception>
#include <sys/system_properties.h>

static jobject g_context = NULL;
static std::mutex g_mt_context;
JavaVM *JNIUtils_AndroidRuntime::jvm_ = NULL;

void jni_setGlobalContext(JNIEnv *env, jobject jContext){
    jni_deleteObj(env, g_context);
    g_context = jni_toGlobalRef(env, jContext);
}

jclass jni_findClassByName(JNIEnv *env, const char *clsName) {
    jclass lclazz = nullptr;

    if (nullptr != clsName) {
        lclazz = env->FindClass(clsName);
        if (jni_checkException(env)) {
            lclazz = nullptr;
        }
    }

    return lclazz;
}

jmethodID jni_getStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetStaticMethodID(clazz, name, sig);
        if (jni_checkException(env)) {
            methodID = nullptr;
        }
    }

    return methodID;
}

jmethodID jni_getMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
    jmethodID methodID = nullptr;

    if (nullptr != clazz) {
        methodID = env->GetMethodID(clazz, name, sig);
        if (jni_checkException(env)) {
            methodID = nullptr;
        }
    }
    return methodID;
}

jclass jni_findClassByObj(JNIEnv *env, jobject obj) {
    jclass lclazz = nullptr;

    if (nullptr != obj) {
        lclazz = env->GetObjectClass(obj);
        if (jni_checkException(env)) {
            lclazz = nullptr;
        }
    }

    return lclazz;
}


jmethodID jni_getMethodIDByObj(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass clazz = nullptr;
    jmethodID methodID = nullptr;

    clazz = jni_findClassByObj(env, obj);
    if (nullptr != clazz) {
        methodID = jni_getMethodID(env, clazz, name, sig);
        jni_deleteObj(env, clazz);
    }

    return methodID;
}


jobject jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, jmethodID methodID,
                                           va_list args) {
    jobject lObjRet = nullptr;

    if (nullptr != clazz && nullptr != methodID) {
        lObjRet = env->CallStaticObjectMethodV(clazz, methodID, args);
        if (jni_checkException(env)){
            lObjRet = nullptr;
        }
    }

    return lObjRet;
}


jobject jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, const char *name,
                                           const char *sig, ...) {
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    jmethodID methodID = jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = jni_callStaticMethodReturnObj(env, clazz, methodID, args);
    }
    va_end(args);
    return ret;
}

jboolean jni_callStaticMethodReturnBool(JNIEnv *env, jclass clazz, const char *name,
                                             const char *sig, ...) {
    jmethodID methodID;
    jboolean ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = env->CallStaticBooleanMethodV(clazz, methodID, args);
    }
    va_end(args);
    return ret;
}


jobject jni_callMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...) {
    jmethodID methodID;
    jobject ret = nullptr;
    va_list args;

    va_start(args, sig);
    methodID = jni_getMethodIDByObj(env, obj, name, sig);

    if (nullptr != methodID) {
        ret = jni_callMethodById(env, obj, methodID, args);
    }
    va_end(args);
    return ret;
}


jobject jni_callMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jobject lObjRet = nullptr;

    if (nullptr != obj && nullptr != methodID) {
        lObjRet = env->CallObjectMethodV(obj, methodID, args);
        if (jni_checkException(env)) {
            lObjRet = nullptr;
        }
    }

    return lObjRet;
}


jobject jni_getGlobalContext_internal(JNIEnv *env) {

    if (g_context != NULL) {
        return g_context;
    }

    // 1 crash
    /*jobject jo_binder = jni_getStaticObjectField(env, "com/android/internal/os/RuntimeInit",
                                                      "mApplicationObject", "Landroid/os/IBinder;");

    jobject jo_actiThread = jni_getObjectField(env, jo_binder, "this$0", "Landroid/app/ActivityThread;");

    g_context = jni_getObjectField(env, jo_actiThread, "mInitialApplication",
                                             "Landroid/app/Application;");

    jni_deleteObj(env, jo_binder);
    jni_deleteObj(env, jo_actiThread);
    info_log("jni_getGlobalContext 1 %p", g_context);

    if (g_context != NULL) {
        g_context = jni_toGlobalRef(env, g_context);
        return g_context;
    }*/

    jobject context;

    // 2
    jclass j_actiThread = jni_findClassByName(env, "android/app/ActivityThread");
    context = jni_callStaticMethodReturnObj(env, j_actiThread, "currentApplication",
                                                        "()Landroid/app/Application;");

    // LOGI("jni_getGlobalContext_internal j_actiThread:%p", j_actiThread);
    // LOGI("jni_getGlobalContext_internal context:%p", context);
    jni_deleteObj(env, j_actiThread);

    if (context != NULL) {
        g_context = jni_toGlobalRef(env, context);
        jni_deleteObj(env, context);
        return g_context;
    }

    // 3
    jclass j_appGlobals = jni_findClassByName(env, "android/app/AppGlobals");
    context = jni_callStaticMethodReturnObj(env, j_appGlobals, "getInitialApplication",
                                                        "()Landroid/app/Application;");

    jni_deleteObj(env, j_appGlobals);

    if (g_context != NULL) {
        g_context = jni_toGlobalRef(env, g_context);
        jni_deleteObj(env, context);
        return g_context;
    }

    return NULL;
}

jobject jni_getGlobalContext(JNIEnv *env) {

    if (g_context)
        return g_context;

    g_mt_context.lock();

    if (g_context)
        return g_context;

    int loop = 0;
    static int MAX = 1000;

    while (loop < MAX) {

        jni_getGlobalContext_internal(env);

        if (g_context)
            break;

        loop++;
    }

    g_mt_context.unlock();

    return g_context;
}

jobject jni_getObjectField(JNIEnv *env, jobject obj, const char *name, const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jobject ret = nullptr;

    clazz = jni_findClassByObj(env, obj);

    if (clazz != nullptr) {
        fieldID = env->GetFieldID(clazz, name, sig);
        if (!jni_checkException(env)) {
            ret = env->GetObjectField(obj, fieldID);
            if (jni_checkException(env)) {
                ret = nullptr;
            }
        }
    }

    return ret;

}

jobject jni_getStaticObjectField(JNIEnv *env, const char *clasName, const char *name,
                                      const char *sig) {
    jclass clazz = nullptr;
    jfieldID fieldID = nullptr;
    jobject ret = nullptr;

    clazz = jni_findClassByName(env, clasName);

    if (clazz != nullptr) {
        fieldID = env->GetStaticFieldID(clazz, name, sig);
        if (!jni_checkException(env)) {
            ret = env->GetStaticObjectField(clazz, fieldID);
            if (jni_checkException(env)) {
                ret = nullptr;
            }
        }
    }

    return ret;
}


jint
jni_callStaticMethodReturnInt(JNIEnv *env, jclass clazz, const char *name, const char *sig,
                                   ...) {

    jmethodID methodID = nullptr;
    jint ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = jni_getStaticMethodID(env, clazz, name, sig);
    if (nullptr != methodID) {
        ret = env->CallStaticIntMethodV(clazz, methodID, args);
        if (jni_checkException(env)) {
            ret = JNI_FALSE;
        }
    }
    va_end(args);
    return ret;
}


jboolean jni_callBoolMethodByName(JNIEnv *env, jobject obj, const char *name,
                                       const char *sig, ...) {
    jmethodID methodID = nullptr;
    jboolean ret = JNI_FALSE;
    va_list args;

    va_start(args, sig);
    methodID = jni_getMethodIDByObj(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = jni_callBoolMethodById(env, obj, methodID, args);
        if (jni_checkException(env)) {
            ret = JNI_FALSE;
        }
    }
    va_end(args);
    return ret;
}


jint jni_callIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...) {
    jmethodID methodID = nullptr;
    jint ret = 0;
    va_list args;

    va_start(args, sig);
    methodID = jni_getMethodIDByObj(env, obj, name, sig);
    if (nullptr != methodID) {
        ret = jni_callIntMethodById(env, obj, methodID, args);
        if (jni_checkException(env)) {
            ret = 0;
        }
    }
    va_end(args);
    return ret;
}

jint jni_getStaticFieldRetIntByName(JNIEnv *env, const char *clsname, const char *metname,
                                         const char *sig) {
    jint jret = 0;

    jclass jclaxx = jni_findClassByName(env, clsname);
    jfieldID fieldId = env->GetStaticFieldID(jclaxx, metname, sig);
    if (jni_checkException(env)) {
        jret = env->GetStaticIntField(jclaxx, fieldId);
        if (jni_checkException(env)) {
            jret = 0;
        }
    }
    return jret;
}

//signature's return type must be void
jobject jni_newObject(JNIEnv *env, const char *clsname, const char *sig, ...) {

    jmethodID constructorMID = nullptr;
    jobject objRet = nullptr;
    jclass structclaxx = nullptr;
    va_list args;

    structclaxx = jni_findClassByName(env, clsname);

    if (structclaxx != nullptr) {
        va_start(args, sig);
        constructorMID = env->GetMethodID(structclaxx, "<init>", sig);
        if (!jni_checkException(env)) {
            objRet = env->NewObjectV(structclaxx, constructorMID, args);
            if (jni_checkException(env)) {
                return nullptr;
            }
        }
        va_end(args);

    }
    return objRet;
}

jobjectArray jni_stringArray2jobjectArray(JNIEnv *env, string strArray[], int len) {
    jstring jstr = nullptr;
    jobjectArray jobjs = nullptr;

    for (int i = 0; i < len; ++i) {
        const char *elem = strArray[i].c_str();
        jstr = env->NewStringUTF(elem);
        env->SetObjectArrayElement(jobjs, i, jstr);
        if (jni_checkException(env)) {
            return nullptr;
        }
    }
    return jobjs;
}

int jni_getOsVersion() {

    static int version = 0;
    if(version != 0){
        return version;
    }

    char sdkchar[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", sdkchar);
    if(strlen(sdkchar) > 0){
        version = atoi(sdkchar);
    }
    return version;

    // JNIUtils_JNIEnvGuard guard;
    // JNIEnv *env = jni_getEnv();

    // return jni_getStaticFieldRetIntByName(env, "android/os/Build$VERSION", "SDK_INT", "I");
}

jboolean jni_callBoolMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jboolean jboolret = JNI_FALSE;
    if (nullptr != obj && nullptr != methodID) {
        jboolret = env->CallBooleanMethodV(obj, methodID, args);
        if (jni_checkException(env)) {
            jboolret = JNI_FALSE;
        }
    }

    return jboolret;
}

jint jni_callIntMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
    jint jintRet = 0;
    if (nullptr != obj && nullptr != methodID) {
        jintRet = env->CallIntMethodV(obj, methodID, args);
        //how to check the exception
        if (jni_checkException(env)) {
            jintRet = 0;
        }
    }

    return jintRet;
}


JNIEnv *jni_getEnv() {

    JNIEnv *env = JNIUtils_AndroidRuntime::getJNIEnv();

    if (env == NULL) {
        LOGE("The current process isn't a jvm thread!");
        exit(-1);
    }

    return env;

}

jobject jni_toGlobalRef(JNIEnv *env, jobject obj) {
    jobject res = env->NewGlobalRef(obj);

//    jobjectRefType ref_type = env->GetObjectRefType(obj);
//    if (ref_type != JNIGlobalRefType) {
//        res = env->NewGlobalRef(obj);
//        env->DeleteLocalRef(obj);
//    }

    return res;
}

void jni_deleteObj(JNIEnv *env, jobject obj) {
    jobjectRefType ref_type = obj != NULL ? env->GetObjectRefType(obj) : JNIInvalidRefType;

    if (ref_type == JNIGlobalRefType) {
        env->DeleteGlobalRef(obj);
    } else if (ref_type == JNILocalRefType) {
        env->DeleteLocalRef(obj);
    } else if (ref_type == JNIWeakGlobalRefType) {
        env->DeleteWeakGlobalRef(obj);
    }
}

string jni_jstring2string(JNIEnv *env, const jstring in) {
    jboolean iscopy = JNI_FALSE;
    const char *c_str = in != NULL ? env->GetStringUTFChars(in, &iscopy) : NULL;

    string result("");
    if (c_str != NULL) {
        result = string(c_str, strlen(c_str));
    }

    //设备指纹 bugfix 3月1日 有问题！！
    //env->ReleaseStringUTFChars(in, c_str);

    return result;
}

jstring jni_string2jstring(JNIEnv *env, const string &in) {
    return env->NewStringUTF(in.data());
}


void jni_jbyteArray2cbytes(JNIEnv *env, const jbyteArray obj, uint8_t **cbytes,
                                   uint32_t *psize) {
    uint32_t size = obj != NULL ? env->GetArrayLength(obj) : 0;

    if (obj != NULL) {
        jbyte *out = new jbyte[size];
        env->GetByteArrayRegion(obj, 0, size, out);
        *cbytes = (uint8_t *) out;
        *psize = size;
    } else {
        *cbytes = NULL;
        *psize = size;
    }
}

string jni_jbyteArray2string(JNIEnv *env, const jbyteArray in) {
    uint8_t *bytes = NULL;
    uint32_t bytes_size = 0;
    jni_jbyteArray2cbytes(env, in, &bytes, &bytes_size);

    string out;
    if (bytes != NULL) {
        out.assign((const char *) bytes, bytes_size);
        delete [](bytes);
        bytes = NULL;
    }

    return out;
}


bool jni_checkException(JNIEnv *env) {

    if (env->ExceptionCheck() == JNI_TRUE) {
        jthrowable exp = env->ExceptionOccurred();
        if (exp != NULL) {
//            LOGE("occurred jni exception!!!");
//            env->Throw(exp);
            env->ExceptionClear();
        }

        return true;
    }

    return false;
}