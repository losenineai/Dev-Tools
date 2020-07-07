/*
 * jni_utils.h
 *
 *  Created on: 2015年8月19日
 *      Author: boyliang
 */

#include <jni.h>
#include <string>
#include <cstring>
#include "utils_anti_log.h"
#include "jni_utils.h"
#include "string_utils.h"

CheckerJavaVM DXAnti_AndroidRuntime::cvm ;
JavaVM *DXAnti_AndroidRuntime_V2::jvm_ = NULL;


JNIEnv *anti_get_jni_env() {

    JNIEnv *env = DXAnti_AndroidRuntime::getJNIEnv();

    if (env == NULL) {
        anti_err_log("The current process isn't a jvm thread!");
        exit(-1);
    }

    return env;

}

jobject anti_jni_convert_to_global_ref(JNIEnv *env, jobject obj) {
    jobject res = env->NewGlobalRef(obj);

//    jobjectRefType ref_type = env->GetObjectRefType(obj);
//    if (ref_type != JNIGlobalRefType) {
//        res = env->NewGlobalRef(obj);
//        env->DeleteLocalRef(obj);
//    }

    return res;
}

void anti_jni_delete_ref(JNIEnv *env, jobject obj) {
    jobjectRefType ref_type = obj != NULL ? env->GetObjectRefType(obj) : JNIInvalidRefType;

    if (ref_type == JNIGlobalRefType) {
        env->DeleteGlobalRef(obj);
    } else if (ref_type == JNILocalRefType) {
        env->DeleteLocalRef(obj);
    } else if (ref_type == JNIWeakGlobalRefType) {
        env->DeleteWeakGlobalRef(obj);
    }
}

string anti_jni_jstring_to_string(JNIEnv *env, const jstring in) {
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

jstring anti_jni_string_to_jstring(JNIEnv *env, const string &in) {
    return env->NewStringUTF(in.data());
}


void anti_jni_jbyteArray_to_cbytes(JNIEnv *env, const jbyteArray obj, uint8_t **cbytes,
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

string anti_jni_jbyteArray_to_string(JNIEnv *env, const jbyteArray in) {
    uint8_t *bytes = NULL;
    uint32_t bytes_size = 0;
    anti_jni_jbyteArray_to_cbytes(env, in, &bytes, &bytes_size);

    string out;
    if (bytes != NULL) {
        out.assign((const char *) bytes, bytes_size);
        delete_array(bytes);
    }

    return out;
}


bool anti_jni_CheckException(JNIEnv *env) {

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