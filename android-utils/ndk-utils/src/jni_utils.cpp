//
// Created by white on 2020/7/24.
//


#include "log.h"
#include "jni_utils.h"
#include <string>
#include <exception>



JavaVM *JniUtils::g_jvm = NULL;
jobject JniUtils::g_classLoader = NULL;

DEF_SET_FIELD(jboolean, Boolean)
DEF_SET_FIELD(jint, Int)
DEF_SET_FIELD(jchar, Char)
DEF_SET_FIELD(jshort, Short)
DEF_SET_FIELD(jbyte, Byte)
DEF_SET_FIELD(jlong, Long)
DEF_SET_FIELD(jfloat, Float)
DEF_SET_FIELD(jdouble, Double)
DEF_SET_FIELD(jobject, Object)

DEF_SET_STATIC_FIELD(jboolean, Boolean)
DEF_SET_STATIC_FIELD(jint, Int)
DEF_SET_STATIC_FIELD(jchar, Char)
DEF_SET_STATIC_FIELD(jshort, Short)
DEF_SET_STATIC_FIELD(jbyte, Byte)
DEF_SET_STATIC_FIELD(jlong, Long)
DEF_SET_STATIC_FIELD(jfloat, Float)
DEF_SET_STATIC_FIELD(jdouble, Double)
DEF_SET_STATIC_FIELD(jobject, Object)

DEF_GET_FIELD(jboolean, Boolean)
DEF_GET_FIELD(jint, Int)
DEF_GET_FIELD(jchar, Char)
DEF_GET_FIELD(jshort, Short)
DEF_GET_FIELD(jbyte, Byte)
DEF_GET_FIELD(jlong, Long)
DEF_GET_FIELD(jfloat, Float)
DEF_GET_FIELD(jdouble, Double)
DEF_GET_FIELD(jobject, Object)

DEF_GET_STATIC_FIELD(jboolean, Boolean)
DEF_GET_STATIC_FIELD(jint, Int)
DEF_GET_STATIC_FIELD(jchar, Char)
DEF_GET_STATIC_FIELD(jshort, Short)
DEF_GET_STATIC_FIELD(jbyte, Byte)
DEF_GET_STATIC_FIELD(jlong, Long)
DEF_GET_STATIC_FIELD(jfloat, Float)
DEF_GET_STATIC_FIELD(jdouble, Double)
DEF_GET_STATIC_FIELD(jobject, Object)

DEF_CALL_METH(jboolean, Boolean)
DEF_CALL_METH(jint, Int)
DEF_CALL_METH(jchar, Char)
DEF_CALL_METH(jshort, Short)
DEF_CALL_METH(jbyte, Byte)
DEF_CALL_METH(jlong, Long)
DEF_CALL_METH(jfloat, Float)
DEF_CALL_METH(jdouble, Double)
DEF_CALL_METH(jobject, Object)

DEF_CALL_STATIC_METH(jboolean, Boolean)
DEF_CALL_STATIC_METH(jint, Int)
DEF_CALL_STATIC_METH(jchar, Char)
DEF_CALL_STATIC_METH(jshort, Short)
DEF_CALL_STATIC_METH(jbyte, Byte)
DEF_CALL_STATIC_METH(jlong, Long)
DEF_CALL_STATIC_METH(jfloat, Float)
DEF_CALL_STATIC_METH(jdouble, Double)
DEF_CALL_STATIC_METH(jobject, Object)


void JniUtils::CallStaticVoidMethod(const char *clasName, const char *name, const char *sig, ...) {  
    JNIEnv *env = m_env;                                                                            
    jclass clazz = FindClassByName(clasName);                                                       
    if(!clazz) {                                                                                    
        return;                                                                                 
    }                                                                                               
    va_list args;                                                                                   
    va_start(args, sig);                                                                            
    jmethodID methodID = GetStaticMethodID(clazz, name, sig);                                       
    if (methodID != NULL) {                                                                         
        env->CallStaticVoidMethodV(clazz, methodID, args);                                
        if (CheckException()){                                                                      
        }                                                                                           
    }                                                                                               
    va_end(args);                                                                                   
    DeleteRef(clazz);                                                                               
}  

void JniUtils::CallVoidMethod(jobject obj, const char *name, const char *sig, ...) {  
    JNIEnv *env = m_env;                                                                            
    jclass clazz = FindClassByObj(obj);                                                             
    if(!clazz) {                                                                                    
        return;                                                                                 
    }                                                                                               
    va_list args;                                                                                   
    va_start(args, sig);                                                                            
    jmethodID methodID = GetMethodID(clazz, name, sig);                                             
    if (methodID != NULL) {                                                                         
        env->CallVoidMethodV(obj, methodID, args);                                        
        if (CheckException()){                                                                      
        }                                                                                           
    }                                                                                               
    va_end(args);                                                                                   
    DeleteRef(clazz);                                                                               
}                                                                                                   





jmethodID JniUtils::GetMethodIDByObj(jobject obj, const char *name, const char *sig) {
    jclass clazz = NULL;
    jmethodID methodID = NULL;

    clazz = FindClassByObj(obj);
    if (clazz) {
        methodID = GetMethodID(clazz, name, sig);
        DeleteRef(clazz);
    }

    return methodID;
}



jclass JniUtils::FindClassByObj(jobject obj) {
    JNIEnv *env = m_env;
    jclass lclazz = NULL;

    if (obj) {
        lclazz = env->GetObjectClass(obj);
        if (CheckException()) {
            lclazz = NULL;
        }
    }

    return lclazz;
}

jclass JniUtils::FindClassByName(const char *clsName) {
    JNIEnv *env = m_env;
    jclass lclazz = NULL;

    // fix can not found PatchClassLoader's class in native thread.
    if(g_classLoader) {
        return static_cast<jclass>(CallObjectMethod(g_classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", env->NewStringUTF(clsName)));
    }

    if (clsName) {
        // run on native thread, mabe occured exception.
        lclazz = env->FindClass(clsName);
        if (CheckException()) {
            lclazz = NULL;
        }
    }

    return lclazz;
}


jmethodID JniUtils::GetStaticMethodID(jclass clazz, const char *name, const char *sig) {
    JNIEnv *env = m_env;
    jmethodID methodID = NULL;

    if (clazz) {
        methodID = env->GetStaticMethodID(clazz, name, sig);
        if (CheckException()) {
            methodID = NULL;
        }
    }

    return methodID;
}

jmethodID JniUtils::GetMethodID(jclass clazz, const char *name, const char *sig) {
    JNIEnv *env = m_env;
    jmethodID methodID = NULL;

    if (clazz) {
        methodID = env->GetMethodID(clazz, name, sig);
        if (CheckException()) {
            methodID = NULL;
        }
    }
    return methodID;
}





jobject JniUtils::NewGlobalRef(jobject obj) {
    JNIEnv *env = m_env;
    return env->NewGlobalRef(obj);

    // jobjectRefType ref_type = env->GetObjectRefType(obj);
    // if (ref_type != JNIGlobalRefType) {
    //     obj = env->NewGlobalRef(obj);
    //     env->DeleteLocalRef(obj);
    // }

    // return obj;
}

void JniUtils::DeleteRef(jobject obj) {
    JNIEnv *env = m_env;
    jobjectRefType ref_type = obj ? env->GetObjectRefType(obj) : JNIInvalidRefType;

    if (ref_type == JNIGlobalRefType) {
        env->DeleteGlobalRef(obj);
    } else if (ref_type == JNILocalRefType) {
        env->DeleteLocalRef(obj);
    } else if (ref_type == JNIWeakGlobalRefType) {
        env->DeleteWeakGlobalRef(obj);
    }
}

string JniUtils::Jstring2string(const jstring in) {
    JNIEnv *env = m_env;
    jboolean iscopy = JNI_FALSE;
    const char *c_str = in ? env->GetStringUTFChars(in, &iscopy) : NULL;

    string result("");
    if (c_str) {
        result = string(c_str, strlen(c_str));
    }

    // FIXME maybe wrong？
    env->ReleaseStringUTFChars(in, c_str);

    return result;
}

jstring JniUtils::String2jstring(const string &in) {
    JNIEnv *env = m_env;
    jstring jstr = env->NewStringUTF(in.data());
    if(CheckException()) {
        jstr = NULL;
    }
    return jstr;
}


void JniUtils::JbyteArray2cbytes(const jbyteArray obj, uint8_t **cbytes,
                                   uint32_t *psize) {
    JNIEnv *env = m_env;
    uint32_t size = obj ? env->GetArrayLength(obj) : 0;

    if (obj) {
        jbyte *out = new jbyte[size];
        env->GetByteArrayRegion(obj, 0, size, out);
        *cbytes = (uint8_t *) out;
        *psize = size;
    } else {
        *cbytes = NULL;
        *psize = size;
    }
}

string JniUtils::JbyteArray2string(const jbyteArray in) {
    uint8_t *bytes = NULL;
    uint32_t bytes_size = 0;
    JbyteArray2cbytes(in, &bytes, &bytes_size);

    string out;
    if (bytes) {
        out.assign((const char *) bytes, bytes_size);
        delete [](bytes);
        bytes = NULL;
    }

    return out;
}

jobjectArray JniUtils::StrArray2objectArray(string strArray[], int len) {
    JNIEnv *env = m_env;
    jstring jstr = NULL;
    jobjectArray jobjs = NULL;

    for (int i = 0; i < len; ++i) {
        const char *elem = strArray[i].c_str();
        jstr = env->NewStringUTF(elem);
        env->SetObjectArrayElement(jobjs, i, jstr);
        if (CheckException()) {
            jobjs = NULL;
        }
    }
    return jobjs;
}


bool JniUtils::CheckException() {
    JNIEnv *env = m_env;

    if (env->ExceptionCheck() == JNI_TRUE) {
        jthrowable exp = env->ExceptionOccurred();
        if (exp) {
#if THROW_EXCEPTION
            env->Throw(exp);
#endif
            env->ExceptionClear();
        }

        return true;
    }

    return false;
}