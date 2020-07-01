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
#include <dx_anti_stee.h>

#include "jni-proxy.h"
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






class CheckCalleNativeJNIEnv : public JniEnvChain {
public:
    CheckCalleNativeJNIEnv(JniEnvChain *next) : JniEnvChain(next) {}

public:
    STEE
    jint RegisterNatives(jclass p0, const JNINativeMethod *p1, jint p2) override {
//        anti_info_log("call jni RegisterNatives");
        return next->RegisterNatives(p0, p1, p2);
    }

    STEE
    virtual jclass FindClass(const char *name) override
    {
//        anti_info_log("call jni FindClass");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        return (!next) ? 0 : next->FindClass(name);
    }

    // call type method
    STEE
    virtual void CallVoidMethodA(jobject obj, jmethodID methodID, jvalue *args) override
    {
//        anti_info_log("call jni CallVoidMethodA");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallVoidMethodA(obj, methodID, args);
    }

    STEE
    virtual void CallVoidMethodV(jobject obj, jmethodID methodID, va_list args) override
    {
//        anti_info_log("call jni CallVoidMethodV");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallVoidMethodV(obj, methodID, args);
    }

    STEE
    virtual void CallNonvirtualVoidMethodA(jobject obj, jclass clazz, jmethodID methodID, jvalue *args) override
    {
//        anti_info_log("call jni CallNonvirtualVoidMethodA");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallNonvirtualVoidMethodA(obj, clazz, methodID, args);
    }

    STEE
    virtual void CallNonvirtualVoidMethodV(jobject obj, jclass clazz, jmethodID methodID, va_list args) override
    {
//        anti_info_log("call jni CallNonvirtualVoidMethodV");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallNonvirtualVoidMethodV(obj, clazz, methodID, args);
    }

    STEE
    virtual void CallStaticVoidMethodA(jclass clazz, jmethodID methodID, jvalue *args) override
    {
//        anti_info_log("call jni CallStaticVoidMethodA");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallStaticVoidMethodA(clazz, methodID, args);
    }

    STEE
    virtual void CallStaticVoidMethodV(jclass clazz, jmethodID methodID, va_list args) override
    {
//        anti_info_log("call jni CallStaticVoidMethodV");
//        DXJavaHookChecker_CheckAll((JniEnvChain* )this);
        next->CallStaticVoidMethodV(clazz, methodID, args);
    }

#define DefineCallMethods(returnType, methodType) \
    STEE \
    virtual returnType Call##methodType##Method##A(jobject obj, jmethodID methodID, jvalue *args) override \
    { \
        return next->Call##methodType##Method##A(obj, methodID, args); \
    } \
    STEE \
    virtual returnType Call##methodType##Method##V(jobject obj, jmethodID methodID, va_list args) override \
    { \
        return next->Call##methodType##Method##V(obj, methodID, args); \
    } \
    STEE \
    virtual returnType CallNonvirtual##methodType##Method##A(jobject obj, jclass clazz, jmethodID methodID, jvalue *args) override \
    { \
        return next->CallNonvirtual##methodType##Method##A(obj, clazz, methodID, args); \
    } \
    STEE \
    virtual returnType CallNonvirtual##methodType##Method##V(jobject obj, jclass clazz, jmethodID methodID, va_list args) override \
    { \
        return next->CallNonvirtual##methodType##Method##V(obj, clazz, methodID, args); \
    } \
    STEE \
    virtual returnType CallStatic##methodType##Method##A(jclass clazz, jmethodID methodID, jvalue *args) override \
    { \
        return next->CallStatic##methodType##Method##A(clazz, methodID, args); \
    } \
    STEE \
    virtual returnType CallStatic##methodType##Method##V(jclass clazz, jmethodID methodID, va_list args) override \
    { \
        return next->CallStatic##methodType##Method##V(clazz, methodID, args); \
    }

    DefineCallMethods(jobject, Object);
    DefineCallMethods(jboolean, Boolean);
    DefineCallMethods(jbyte, Byte);
    DefineCallMethods(jchar, Char);
    DefineCallMethods(jshort, Short);
    DefineCallMethods(jint, Int);
    DefineCallMethods(jlong, Long);
    DefineCallMethods(jfloat, Float);
    DefineCallMethods(jdouble, Double);
};


class CheckerJavaVM : public JavaVMProxy {
public:
    STEE \
    virtual JniEnvChain *chain(JniEnvChain *next) override { return new CheckCalleNativeJNIEnv(next); };
};


class DXAnti_AndroidRuntime {

//private:
//    static JavaVM *jvm_;

public:
    static CheckerJavaVM cvm;

    static JavaVM *getJavaVM() {

        return cvm.GetProxyedVM();
    }

    static JNIEnv *getJNIEnv() {
        JNIEnv *env = NULL;

        if (getJavaVM()) {
            getJavaVM()->GetEnv((void **) &env, JNI_VERSION_1_4);
        }

        return env;
    }

    static void init(JavaVM *jvm) {
        cvm.SetVM(jvm);
//        jvm_ = jvm;
    }
};


class DXAnti_AndroidRuntime_V2 {

private:
    static JavaVM *jvm_;

public:

    static JavaVM *getJavaVM() {
        return jvm_;
    }

    static JNIEnv *getJNIEnv() {
        JNIEnv *env = NULL;

        if (getJavaVM()) {
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
