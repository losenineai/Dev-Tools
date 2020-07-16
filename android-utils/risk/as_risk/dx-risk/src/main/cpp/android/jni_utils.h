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
#include <mutex>

#include "DXLog.h"

using std::string;
using namespace std;

#define GetStaticMethod(meth, sig) \
        static jmethodID meth##_method = env->GetStaticMethodID(sJNIHelper_class, meth, sig);       \
            if(meth##_method == NULL){                                                              \
                LOGE("Can't find the " meth " method from JNIHelper.");                          \
                exit(-1);                                                                           \
            }                                                                                       \

#define GetStaticMethod_t(meth, sig) \
        static jmethodID meth##_method = env->GetStaticMethodID(sJNIHelper_class, #meth, sig);      \
            if(meth##_method == NULL){                                                              \
                LOGE("Can't find the " #meth " method from JNIHelper.");                         \
                exit(-1);                                                                           \
            }

#define CallStaticMethod(excret, rettype, meth, ...) \
    env->CallStatic##rettype##Method(sJNIHelper_class, meth##_method, ##__VA_ARGS__);\
    if(CheckException(env)) return excret;

#define CallStaticMethod2(rettype, meth, ...) \
    env->CallStatic##rettype##Method(sJNIHelper_class, meth##_method, ##__VA_ARGS__);\
    if(CheckException(env)) return;

bool CheckException(JNIEnv *env);

typedef void jvoid;
extern jclass sJNIHelper_class;
JNIEnv *get_jni_env();

JNIEnv *jni_init();

jobject convert_to_global_ref(JNIEnv *env, jobject obj);

void delete_ref(JNIEnv *env, jobject obj);

jbyteArray cbytes_to_jbyteArray(JNIEnv *env, const uint8_t *input, uint32_t size);

string jstring_to_string(JNIEnv *env, const jstring in);

string jbyteArray_to_string(JNIEnv *env, const jbyteArray in);

jstring string_to_jstring(JNIEnv *env, const string &in);

jobject map_to_jmap(JNIEnv *env, map<string, string> cMap);

map<string, string>  jmap_to_map(JNIEnv *env, jobject jMap);

bool jboolean_to_bool(jboolean value);

class AndroidRuntime {

private:
    static JavaVM *jvm_;
    static jobject _jContext;

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

    static void initContext(jobject jContext) {
        _jContext = jContext;
    }

    static jobject getContext() {
        return _jContext;
    }
};

class JNIEnvGuard {

private:
    JavaVM *jvm_;
    JNIEnv *jni_;
    bool attach_;

public:
    JNIEnvGuard() : jvm_(AndroidRuntime::getJavaVM()), jni_(AndroidRuntime::getJNIEnv()),
                    attach_(false) {

        if (jvm_ == NULL) {
            LOGE("The current process is't a jvm process.");
            exit(-1);
        }

        if (jni_ == NULL) {
            jvm_->AttachCurrentThread(&jni_, NULL);

            if (jni_ != NULL) {
                attach_ = true;
            }
        }
    }

    ~JNIEnvGuard() {
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
