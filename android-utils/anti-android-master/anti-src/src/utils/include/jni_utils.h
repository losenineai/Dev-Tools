/*
 * jni_utils.h
 *
 *  Created on: 2015年8月19日
 *      Author: boyliang
 */

#ifndef JNI_UTILS_H_
#define JNI_UTILS_H_

#include <stdlib.h>
#include <string>
#include <exception>
#include <jni.h>
#include <map>

#include "utils_anti_log.h"
#include "anti_config.h"

using std::string;
using namespace std;

JNIEnv *anti_get_jni_env();

bool anti_jni_CheckException(JNIEnv *env);

jobject anti_jni_convert_to_global_ref(JNIEnv *env, jobject obj);

void anti_jni_delete_ref(JNIEnv *env, jobject obj);

string anti_jni_jstring_to_string(JNIEnv *env, const jstring in);

jstring anti_jni_string_to_jstring(JNIEnv *env, const string &in);

void anti_jni_jbyteArray_to_cbytes(JNIEnv *env, const jbyteArray obj, uint8_t **cbytes, uint32_t *psize);

string anti_jni_jbyteArray_to_string(JNIEnv *env, const jbyteArray in);

class DXAnti_AndroidRuntime {

private:
    static JavaVM *jvm_;

public:

    static JavaVM *getJavaVM() {
        return jvm_;
    }

    static JNIEnv *getJNIEnv() {
        JNIEnv *env = NULL;

        if (jvm_) {
            jvm_->GetEnv((void **) &env, JNI_VERSION_1_4);
        }

        return env;
    }

    static void init(JavaVM *jvm) {
        jvm_ = jvm;
    }

};

class DXAnti_JNIEnvGuard {

private:
    JavaVM *jvm_;
    JNIEnv *jni_;
    bool attach_;

public:
    DXAnti_JNIEnvGuard() : jvm_(DXAnti_AndroidRuntime::getJavaVM()), jni_(DXAnti_AndroidRuntime::getJNIEnv()),
                    attach_(false) {
        if (jvm_ == NULL) {
            anti_err_log("guard The current process isn't a jvm process.");
            exit(-1);
        }

        if (jni_ == NULL) {
            jvm_->AttachCurrentThread(&jni_, NULL);

            if (jni_ != NULL) {
                attach_ = true;
            }
        }
    }

    ~DXAnti_JNIEnvGuard() {
        if (jvm_ && jni_ && attach_) {
            jvm_->DetachCurrentThread();
            jni_ = NULL;
            jvm_ = NULL;
        }
    }

    inline JNIEnv *JNI() {
        return jni_;
    }

    inline JavaVM *JVM() {
        return jvm_;
    }
};



#endif /* JNI_UTILS_H_ */
