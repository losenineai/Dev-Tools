/*
 * jni_utils.h
 *
 *  Created on: 2015年8月19日
 *      Author: boyliang
 */

#include <stdio.h>
#include <jni.h>
#include <string>
#include <stdlib.h>

#include "jni_utils.h"
#include "utils.h"

JavaVM *AndroidRuntime::jvm_ = NULL;
jobject AndroidRuntime::_jContext = NULL;
jclass sJNIHelper_class = NULL;

JNIEnv *get_jni_env() {

    JNIEnv *env = AndroidRuntime::getJNIEnv();

    if (env == NULL) {
        LOGE("The current process isn't a jvm thread!");
        exit(-1);
    }

    return env;
}

JNIEnv *jni_init() {
    static mutex lock;
    lock_guard<mutex> p(lock);

    JNIEnv *env = get_jni_env();

    if (sJNIHelper_class == NULL) {
        // a为JNIHelper的混淆
        // sJNIHelper_class = env->FindClass("com/dingxiang/mobile/risk/dx/a");
        sJNIHelper_class = env->FindClass(CUSTOMIZE_JNIHELPER);

        if (sJNIHelper_class == NULL) {
            LOGE("Can not find the JNIHelper class, jni_init fails");
            exit(-1);
        }

        sJNIHelper_class = reinterpret_cast<jclass>(convert_to_global_ref(env, sJNIHelper_class));
    }

    return env;
}

jobject convert_to_global_ref(JNIEnv *env, jobject obj) {
    jobject res = obj;
    jobjectRefType ref_type = env->GetObjectRefType(obj);
    if (ref_type != JNIGlobalRefType) {
        res = env->NewGlobalRef(obj);
        env->DeleteLocalRef(obj);
    }
    return res;
}

void delete_ref(JNIEnv *env, jobject obj) {
    jobjectRefType ref_type = obj != NULL ? env->GetObjectRefType(obj) : JNIInvalidRefType;

    if (ref_type == JNIGlobalRefType) {
        env->DeleteGlobalRef(obj);
    } else if (ref_type == JNILocalRefType) {
        env->DeleteLocalRef(obj);
    } else if (ref_type == JNIWeakGlobalRefType) {
        env->DeleteWeakGlobalRef(obj);
    }
}

jbyteArray cbytes_to_jbyteArray(JNIEnv *env, const uint8_t *input, uint32_t size) {
    jbyteArray res = env->NewByteArray(size);
    env->SetByteArrayRegion(res, 0, size, (jbyte *) input);
    return res;
}


string jstring_to_string(JNIEnv *env, const jstring in) {
    jboolean iscopy = JNI_FALSE;
    const char *c_str = in != NULL ? env->GetStringUTFChars(in, &iscopy) : NULL;

    string result = "";
    if (c_str != NULL) {
        result = string(c_str, strlen(c_str));
    }

    //设备指纹 bugfix 3月1日 有问题！！
    //env->ReleaseStringUTFChars(in, c_str);

    return result;
}

static void
jbyteArray_to_cbytes(JNIEnv *env, const jbyteArray obj, uint8_t **cbytes, uint32_t *psize) {

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

string jbyteArray_to_string(JNIEnv *env, const jbyteArray in) {
    uint8_t *bytes = NULL;
    uint32_t bytes_size = 0;
    jbyteArray_to_cbytes(env, in, &bytes, &bytes_size);

    string out;
    if (bytes != NULL) {
        out.assign((const char *) bytes, bytes_size);
        delete_array(bytes);
    }

    return out;
}

jstring string_to_jstring(JNIEnv *env, const string &in) {
    return env->NewStringUTF(in.data());
}


//jobject map_to_jmap(JNIEnv *env, map<string, string> cMap) {
//    jclass jMapClass = env->FindClass("java/util/HashMap");
//    jmethodID mid = env->GetMethodID(jMapClass, "<init>", "()V");
//    jmethodID putMethod = env->GetMethodID(jMapClass, "put",
//                                           "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
//    jobject jMap = env->NewObject(jMapClass, mid);
//    jclass strClass = env->FindClass("java/lang/String");
//    jmethodID cConstructorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
//    jstring encoding = env->NewStringUTF("utf-8");
//    std::map<string, string>::iterator it;
//    for (it = cMap.begin(); it != cMap.end(); it++) {
//        string key = it->first;
//        jbyteArray keyBytes = env->NewByteArray(strlen(key.c_str()));
//        env->SetByteArrayRegion(keyBytes, 0, strlen(key.c_str()), (jbyte *) key.c_str());
//        jstring jKey = (jstring) env->NewObject(strClass, cConstructorID, keyBytes, encoding);
//        string value = it->second;
//        jbyteArray valueBytes = env->NewByteArray(strlen(value.c_str()));
//        env->SetByteArrayRegion(valueBytes, 0, strlen(value.c_str()), (jbyte *) value.c_str());
//        jstring jValue = (jstring) env->NewObject(strClass, cConstructorID, valueBytes, encoding);
//        env->CallVoidMethod(jMap, putMethod, jKey, jValue);
//
//        delete_ref(env, keyBytes);
//        delete_ref(env, jKey);
//        delete_ref(env, valueBytes);
//        delete_ref(env, jValue);
//    }
//
//    delete_ref(env, jMapClass);
//    delete_ref(env, strClass);
//    delete_ref(env, encoding);
//
//    return jMap;
//}
map<string, string> jmap_to_map(JNIEnv *env, jobject jMap) {

    std::map<string, string> cMap;
    jclass jMapClass = env->FindClass("java/util/HashMap");
    jmethodID jKeySetMethodId = env->GetMethodID(jMapClass, "keySet", "()Ljava/util/Set;");
    jmethodID jGetMethodId = env->GetMethodID(jMapClass, "get",
                                              "(Ljava/lang/Object;)Ljava/lang/Object;");
    jobject jSetKey = env->CallObjectMethod(jMap, jKeySetMethodId);
    jclass jSetClass = env->FindClass("java/util/Set");
    jmethodID jToArrayMethodId = env->GetMethodID(jSetClass, "toArray", "()[Ljava/lang/Object;");
    jobjectArray jObjArray = (jobjectArray) env->CallObjectMethod(jSetKey, jToArrayMethodId);
    if (jObjArray == NULL) {
        LOGD("param is NULL");
    }
    jsize arraySize = env->GetArrayLength(jObjArray);
    int i = 0;
    for (i = 0; i < arraySize; i++) {
        jstring jKey = (jstring) env->GetObjectArrayElement(jObjArray, i);
        jstring jValue = (jstring) env->CallObjectMethod(jMap, jGetMethodId, jKey);
        string key = env->GetStringUTFChars(jKey, 0);
        string value = env->GetStringUTFChars(jValue, 0);
        cMap[key] = value;

        delete_ref(env, jKey);
        delete_ref(env, jValue);
    }

    delete_ref(env, jMapClass);
    delete_ref(env, jSetKey);
    delete_ref(env, jSetClass);
    delete_ref(env, jObjArray);

    return cMap;
}

bool jboolean_to_bool(jboolean value) {
    return value == JNI_TRUE;
}

bool CheckException(JNIEnv *env) {

    if (env->ExceptionCheck() == JNI_TRUE) {
        jthrowable exp = env->ExceptionOccurred();

        if (exp != NULL) {
            env->Throw(exp);
        }

        return true;
    }

    return false;
}