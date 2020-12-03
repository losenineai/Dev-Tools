//
// Created by white on 2020/7/24.
//


#ifndef JNI_UTILS_H
#define JNI_UTILS_H

#include <jni.h>
#include <string>
#include "log.h"

using std::string;
using namespace std;

#define THROW_EXCEPTION 1

/**
 * api & define for set static field
 */ 
#define API_SET_STATIC_FIELD(retType, type) void SetStatic##type##Field(const char *clasName, const char *name, const char *sig, const retType value);
#define DEF_SET_STATIC_FIELD(retType, type) void JniUtils::SetStatic##type##Field(const char *clasName, const char *name, const char *sig, const retType value) {  \
    JNIEnv *env = m_env;                                                                \
    jclass clazz = FindClassByName(clasName);                                           \
    if(!clazz) {                                                                        \
        return;                                                                         \
    }                                                                                   \
    jfieldID fieldID = env->GetStaticFieldID(clazz, name, sig);                         \
    if (!CheckException()) {                                                            \
        env->SetStatic##type##Field(clazz, fieldID, value);                             \
        if (CheckException()) {                                                         \
        }                                                                               \
    }                                                                                   \
    DeleteRef(clazz);                                                                   \
}                                                                                       \

/**
 * api & define for set field
 */ 
#define API_SET_FIELD(retType, type) void Set##type##Field(jobject obj, const char *name, const char *sig, retType value);
#define DEF_SET_FIELD(retType, type) void JniUtils::Set##type##Field(jobject obj, const char *name, const char *sig, retType value) {  \
    JNIEnv *env = m_env;                                                                            \
    jclass clazz = FindClassByObj(obj);                                                 \
    if(!clazz) {                                                                        \
        return;                                                                         \
    }                                                                                   \
    jfieldID fieldID = env->GetFieldID(clazz, name, sig);                               \
    if (!CheckException()) {                                                            \
        env->Set##type##Field(obj, fieldID, value);                                     \
        if (CheckException()) {                                                         \
        }                                                                               \
    }                                                                                   \
    DeleteRef(clazz);                                                                   \
}                                                                                       \


/**
 * api & define for get static field
 */ 
#define API_GET_STATIC_FIELD(retType, type) retType GetStatic##type##Field(const char *clasName, const char *name, const char *sig);
#define DEF_GET_STATIC_FIELD(retType, type) retType JniUtils::GetStatic##type##Field(const char *clasName, const char *name, const char *sig) {  \
    JNIEnv *env = m_env;                                                                            \
    retType ret = 0;                                                                    \
    jclass clazz = FindClassByName(clasName);                                           \
    if(!clazz) {                                                                        \
        return ret;                                                                     \
    }                                                                                   \
    jfieldID fieldID = env->GetStaticFieldID(clazz, name, sig);                         \
    if (!CheckException()) {                                                            \
        ret = env->GetStatic##type##Field(clazz, fieldID);                              \
        if (CheckException()) {                                                         \
        }                                                                               \
    }                                                                                   \
    DeleteRef(clazz);                                                                   \
    return ret;                                                                         \
}                                                                                       \


/**
 * api & define for get field
 */ 
#define API_GET_FIELD(retType, type) retType Get##type##Field(jobject obj, const char *name, const char *sig);
#define DEF_GET_FIELD(retType, type) retType JniUtils::Get##type##Field(jobject obj, const char *name, const char *sig) {  \
    JNIEnv *env = m_env;                                                                            \
    retType ret = 0;                                                                    \
    jclass clazz = FindClassByObj(obj);                                                 \
    if(!clazz) {                                                                        \
        return ret;                                                                     \
    }                                                                                   \
    jfieldID fieldID = env->GetFieldID(clazz, name, sig);                               \
    if (!CheckException()) {                                                            \
        ret = env->Get##type##Field(obj, fieldID);                                      \
        if (CheckException()) {                                                         \
        }                                                                               \
    }                                                                                   \
    DeleteRef(clazz);                                                                   \
    return ret;                                                                         \
}                                                                                       \

/**
 * api & define for call static method
 */ 
#define API_CALL_STATIC_METH(retType, type) retType CallStatic##type##Method(const char *clasName, const char *name, const char *sig, ...);
#define DEF_CALL_STATIC_METH(retType, type) retType JniUtils::CallStatic##type##Method(const char *clasName, const char *name, const char *sig, ...) {  \
    JNIEnv *env = m_env;                                                                            \
    retType ret = 0;                                                                                \
    jclass clazz = FindClassByName(clasName);                                                       \
    if(!clazz) {                                                                                    \
        return ret;                                                                                 \
    }                                                                                               \
    va_list args;                                                                                   \
    va_start(args, sig);                                                                            \
    jmethodID methodID = GetStaticMethodID(clazz, name, sig);                                       \
    if (methodID != NULL) {                                                                         \
        ret = env->CallStatic##type##MethodV(clazz, methodID, args);                                \
        if (CheckException()){                                                                      \
        }                                                                                           \
    }                                                                                               \
    va_end(args);                                                                                   \
    DeleteRef(clazz);                                                                               \
    return ret;                                                                                     \
}                                                                                                   \


/**
 * api & define for call method
 */ 
#define API_CALL_METH(retType, type) retType Call##type##Method(jobject obj, const char *name, const char *sig, ...);
#define DEF_CALL_METH(retType, type) retType JniUtils::Call##type##Method(jobject obj, const char *name, const char *sig, ...) {  \
    JNIEnv *env = m_env;                                                                            \
    retType ret = 0;                                                                                \
    jclass clazz = FindClassByObj(obj);                                                             \
    if(!clazz) {                                                                                    \
        return ret;                                                                                 \
    }                                                                                               \
    va_list args;                                                                                   \
    va_start(args, sig);                                                                            \
    jmethodID methodID = GetMethodID(clazz, name, sig);                                             \
    if (methodID != NULL) {                                                                         \
        ret = env->Call##type##MethodV(obj, methodID, args);                                        \
        if (CheckException()){                                                                      \
        }                                                                                           \
    }                                                                                               \
    va_end(args);                                                                                   \
    DeleteRef(clazz);                                                                               \
    return ret;                                                                                     \
}                                                                                                   \




/**
 * Jni utils class
 */
class JniUtils {

private:
    static JavaVM *g_jvm;
    static jobject g_classLoader;
    
    JNIEnv *m_env;
    bool m_attach;

public:

    API_SET_FIELD(jboolean, Boolean)
    API_SET_FIELD(jint, Int)
    API_SET_FIELD(jchar, Char)
    API_SET_FIELD(jshort, Short)
    API_SET_FIELD(jbyte, Byte)
    API_SET_FIELD(jlong, Long)
    API_SET_FIELD(jfloat, Float)
    API_SET_FIELD(jdouble, Double)
    API_SET_FIELD(jobject, Object)

    API_SET_STATIC_FIELD(jboolean, Boolean)
    API_SET_STATIC_FIELD(jint, Int)
    API_SET_STATIC_FIELD(jchar, Char)
    API_SET_STATIC_FIELD(jshort, Short)
    API_SET_STATIC_FIELD(jbyte, Byte)
    API_SET_STATIC_FIELD(jlong, Long)
    API_SET_STATIC_FIELD(jfloat, Float)
    API_SET_STATIC_FIELD(jdouble, Double)
    API_SET_STATIC_FIELD(jobject, Object)

    API_GET_FIELD(jboolean, Boolean)
    API_GET_FIELD(jint, Int)
    API_GET_FIELD(jchar, Char)
    API_GET_FIELD(jshort, Short)
    API_GET_FIELD(jbyte, Byte)
    API_GET_FIELD(jlong, Long)
    API_GET_FIELD(jfloat, Float)
    API_GET_FIELD(jdouble, Double)
    API_GET_FIELD(jobject, Object)

    API_GET_STATIC_FIELD(jboolean, Boolean)
    API_GET_STATIC_FIELD(jint, Int)
    API_GET_STATIC_FIELD(jchar, Char)
    API_GET_STATIC_FIELD(jshort, Short)
    API_GET_STATIC_FIELD(jbyte, Byte)
    API_GET_STATIC_FIELD(jlong, Long)
    API_GET_STATIC_FIELD(jfloat, Float)
    API_GET_STATIC_FIELD(jdouble, Double)
    API_GET_STATIC_FIELD(jobject, Object)

    API_CALL_METH(jboolean, Boolean)
    API_CALL_METH(jint, Int)
    API_CALL_METH(jchar, Char)
    API_CALL_METH(jshort, Short)
    API_CALL_METH(jbyte, Byte)
    API_CALL_METH(jlong, Long)
    API_CALL_METH(jfloat, Float)
    API_CALL_METH(jdouble, Double)
    API_CALL_METH(jobject, Object)

    API_CALL_STATIC_METH(jboolean, Boolean)
    API_CALL_STATIC_METH(jint, Int)
    API_CALL_STATIC_METH(jchar, Char)
    API_CALL_STATIC_METH(jshort, Short)
    API_CALL_STATIC_METH(jbyte, Byte)
    API_CALL_STATIC_METH(jlong, Long)
    API_CALL_STATIC_METH(jfloat, Float)
    API_CALL_STATIC_METH(jdouble, Double)
    API_CALL_STATIC_METH(jobject, Object)
    void CallStaticVoidMethod(const char *clasName, const char *name, const char *sig, ...);
    void CallVoidMethod(jobject obj, const char *name, const char *sig, ...);

    jmethodID GetMethodIDByObj(jobject obj, const char *name, const char *sig);
    jclass FindClassByObj(jobject obj);
    jclass FindClassByName(const char *clsName);
    jmethodID GetStaticMethodID(jclass clazz, const char *name, const char *sig);
    jmethodID GetMethodID(jclass clazz, const char *name, const char *sig);

    jobject NewGlobalRef(jobject obj);
    void DeleteRef(jobject obj);
    string Jstring2string(const jstring in);
    jstring String2jstring(const string &in);
    void JbyteArray2cbytes(const jbyteArray obj, uint8_t **cbytes, uint32_t *psize);
    string JbyteArray2string(const jbyteArray in);
    jobjectArray StrArray2objectArray(string strArray[], int len);


    static void Init(JavaVM* jvm, const char *classname = NULL) {
        g_jvm = jvm;

        JniUtils jni;
        if (classname) {
            jclass patchClass = jni.FindClassByName(classname);
            jobject jo_classLoader = jni.CallObjectMethod(patchClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
            g_classLoader = jni.NewGlobalRef(jo_classLoader);
            jni.DeleteRef(patchClass);
            jni.DeleteRef(jo_classLoader);
            LOGD("g_classLoader :%p", g_classLoader);
        }
    }

    JniUtils() : m_env(NULL),
                 m_attach(false) {
        if (g_jvm == NULL) {
            LOGE("JniUtils The current process isn't a jvm process.");
            return;
        }

        g_jvm->GetEnv((void **) &m_env, JNI_VERSION_1_4);

        if (m_env == NULL) {
            g_jvm->AttachCurrentThread(&m_env, NULL);

            if (m_env != NULL) {
                m_attach = true;
            }
        }
    }

     ~JniUtils() {
        if (g_jvm && m_env && m_attach) {
            g_jvm->DetachCurrentThread();
            m_env = NULL;
            g_jvm = NULL;
        }
    }



private:
    bool CheckException();

   
};


#endif // JNI_UTILS_H