//
// Created by white on 2020/7/24.
//


#include "log.h"
#include "jni_utils.h"
#include <string>
#include <exception>


JavaVM *JniUtils::m_jvm = NULL;



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
    jobjectRefType ref_type = obj != NULL ? env->GetObjectRefType(obj) : JNIInvalidRefType;

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
    const char *c_str = in != NULL ? env->GetStringUTFChars(in, &iscopy) : NULL;

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

string JniUtils::JbyteArray2string(const jbyteArray in) {
    uint8_t *bytes = NULL;
    uint32_t bytes_size = 0;
    JbyteArray2cbytes(in, &bytes, &bytes_size);

    string out;
    if (bytes != NULL) {
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
        if (exp != NULL) {
//            LOGE("occurred jni exception!!!");
//            env->Throw(exp);
            env->ExceptionClear();
        }

        return true;
    }

    return false;
}