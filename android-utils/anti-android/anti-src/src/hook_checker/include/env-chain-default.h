#include "env-chain.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef __CHAIN_ENV_H__
#define __CHAIN_ENV_H__

class JniEnvChainDefault : public JniEnvChain {
  private:
    JNIEnv *env;

  public:
    void SetEnv(JNIEnv *env) {
        if (env == NULL) {
            abort();
        }
        this->env = env;
    }

    // matao add
    JNIEnv* GetEnv(void) {
        return this->env;
    }

    JniEnvChainDefault(JNIEnv *env) : JniEnvChain(0), env(env) {
        if (env == NULL) {
            abort();
        }
    }
    virtual jint GetVersion() override { return (!env) ? 0 : env->GetVersion(); }
    virtual jclass DefineClass(const char *p0, jobject p1, const jbyte *p2, jsize p3) override { return (!env) ? 0 : env->DefineClass(p0, p1, p2, p3); }
    virtual jclass FindClass(const char *p0) override { return (!env) ? 0 : env->FindClass(p0); }
    virtual jmethodID FromReflectedMethod(jobject p0) override { return (!env) ? 0 : env->FromReflectedMethod(p0); }
    virtual jfieldID FromReflectedField(jobject p0) override { return (!env) ? 0 : env->FromReflectedField(p0); }
    virtual jobject ToReflectedMethod(jclass p0, jmethodID p1, jboolean p2) override { return (!env) ? 0 : env->ToReflectedMethod(p0, p1, p2); }
    virtual jclass GetSuperclass(jclass p0) override { return (!env) ? 0 : env->GetSuperclass(p0); }
    virtual jboolean IsAssignableFrom(jclass p0, jclass p1) override { return (!env) ? 0 : env->IsAssignableFrom(p0, p1); }
    virtual jobject ToReflectedField(jclass p0, jfieldID p1, jboolean p2) override { return (!env) ? 0 : env->ToReflectedField(p0, p1, p2); }
    virtual jint Throw(jthrowable p0) override { return (!env) ? 0 : env->Throw(p0); }
    virtual jint ThrowNew(jclass p0, const char *p1) override { return (!env) ? 0 : env->ThrowNew(p0, p1); }
    virtual jthrowable ExceptionOccurred() override { return (!env) ? 0 : env->ExceptionOccurred(); }
    virtual void ExceptionDescribe() override {
        if (env)
            env->ExceptionDescribe();
    }
    virtual void ExceptionClear() override {
        if (env)
            env->ExceptionClear();
    }
    virtual void FatalError(const char *p0) override {
        if (env)
            env->FatalError(p0);
    }
    virtual jint PushLocalFrame(jint p0) override { return (!env) ? 0 : env->PushLocalFrame(p0); }
    virtual jobject PopLocalFrame(jobject p0) override { return (!env) ? 0 : env->PopLocalFrame(p0); }
    virtual jobject NewGlobalRef(jobject p0) override { return (!env) ? 0 : env->NewGlobalRef(p0); }
    virtual void DeleteGlobalRef(jobject p0) override {
        if (env)
            env->DeleteGlobalRef(p0);
    }
    virtual void DeleteLocalRef(jobject p0) override {
        if (env)
            env->DeleteLocalRef(p0);
    }
    virtual jboolean IsSameObject(jobject p0, jobject p1) override { return (!env) ? 0 : env->IsSameObject(p0, p1); }
    virtual jobject NewLocalRef(jobject p0) override { return (!env) ? 0 : env->NewLocalRef(p0); }
    virtual jint EnsureLocalCapacity(jint p0) override { return (!env) ? 0 : env->EnsureLocalCapacity(p0); }
    virtual jobject AllocObject(jclass p0) override { return (!env) ? 0 : env->AllocObject(p0); }
    virtual jobject NewObjectV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->NewObjectV(p0, p1, p2); }
    virtual jobject NewObjectA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->NewObjectA(p0, p1, p2); }
    virtual jclass GetObjectClass(jobject p0) override { return (!env) ? 0 : env->GetObjectClass(p0); }
    virtual jboolean IsInstanceOf(jobject p0, jclass p1) override { return (!env) ? 0 : env->IsInstanceOf(p0, p1); }
    virtual jmethodID GetMethodID(jclass p0, const char *p1, const char *p2) override { return (!env) ? 0 : env->GetMethodID(p0, p1, p2); }
    virtual jobject CallObjectMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallObjectMethodV(p0, p1, p2); }
    virtual jobject CallObjectMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallObjectMethodA(p0, p1, p2); }
    virtual jboolean CallBooleanMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallBooleanMethodV(p0, p1, p2); }
    virtual jboolean CallBooleanMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallBooleanMethodA(p0, p1, p2); }
    virtual jbyte CallByteMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallByteMethodV(p0, p1, p2); }
    virtual jbyte CallByteMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallByteMethodA(p0, p1, p2); }
    virtual jchar CallCharMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallCharMethodV(p0, p1, p2); }
    virtual jchar CallCharMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallCharMethodA(p0, p1, p2); }
    virtual jshort CallShortMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallShortMethodV(p0, p1, p2); }
    virtual jshort CallShortMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallShortMethodA(p0, p1, p2); }
    virtual jint CallIntMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallIntMethodV(p0, p1, p2); }
    virtual jint CallIntMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallIntMethodA(p0, p1, p2); }
    virtual jlong CallLongMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallLongMethodV(p0, p1, p2); }
    virtual jlong CallLongMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallLongMethodA(p0, p1, p2); }
    virtual jfloat CallFloatMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallFloatMethodV(p0, p1, p2); }
    virtual jfloat CallFloatMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallFloatMethodA(p0, p1, p2); }
    virtual jdouble CallDoubleMethodV(jobject p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallDoubleMethodV(p0, p1, p2); }
    virtual jdouble CallDoubleMethodA(jobject p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallDoubleMethodA(p0, p1, p2); }
    virtual void CallVoidMethodV(jobject p0, jmethodID p1, va_list p2) override {
        if (env)
            env->CallVoidMethodV(p0, p1, p2);
    }
    virtual void CallVoidMethodA(jobject p0, jmethodID p1, jvalue *p2) override {
        if (env)
            env->CallVoidMethodA(p0, p1, p2);
    }
    virtual jobject CallNonvirtualObjectMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualObjectMethodV(p0, p1, p2, p3); }
    virtual jobject CallNonvirtualObjectMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualObjectMethodA(p0, p1, p2, p3); }
    virtual jboolean CallNonvirtualBooleanMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualBooleanMethodV(p0, p1, p2, p3); }
    virtual jboolean CallNonvirtualBooleanMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualBooleanMethodA(p0, p1, p2, p3); }
    virtual jbyte CallNonvirtualByteMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualByteMethodV(p0, p1, p2, p3); }
    virtual jbyte CallNonvirtualByteMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualByteMethodA(p0, p1, p2, p3); }
    virtual jchar CallNonvirtualCharMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualCharMethodV(p0, p1, p2, p3); }
    virtual jchar CallNonvirtualCharMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualCharMethodA(p0, p1, p2, p3); }
    virtual jshort CallNonvirtualShortMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualShortMethodV(p0, p1, p2, p3); }
    virtual jshort CallNonvirtualShortMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualShortMethodA(p0, p1, p2, p3); }
    virtual jint CallNonvirtualIntMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualIntMethodV(p0, p1, p2, p3); }
    virtual jint CallNonvirtualIntMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualIntMethodA(p0, p1, p2, p3); }
    virtual jlong CallNonvirtualLongMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualLongMethodV(p0, p1, p2, p3); }
    virtual jlong CallNonvirtualLongMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualLongMethodA(p0, p1, p2, p3); }
    virtual jfloat CallNonvirtualFloatMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualFloatMethodV(p0, p1, p2, p3); }
    virtual jfloat CallNonvirtualFloatMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualFloatMethodA(p0, p1, p2, p3); }
    virtual jdouble CallNonvirtualDoubleMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override { return (!env) ? 0 : env->CallNonvirtualDoubleMethodV(p0, p1, p2, p3); }
    virtual jdouble CallNonvirtualDoubleMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override { return (!env) ? 0 : env->CallNonvirtualDoubleMethodA(p0, p1, p2, p3); }
    virtual void CallNonvirtualVoidMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) override {
        if (env)
            env->CallNonvirtualVoidMethodV(p0, p1, p2, p3);
    }
    virtual void CallNonvirtualVoidMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) override {
        if (env)
            env->CallNonvirtualVoidMethodA(p0, p1, p2, p3);
    }
    virtual jfieldID GetFieldID(jclass p0, const char *p1, const char *p2) override { return (!env) ? 0 : env->GetFieldID(p0, p1, p2); }
    virtual jobject GetObjectField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetObjectField(p0, p1); }
    virtual jboolean GetBooleanField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetBooleanField(p0, p1); }
    virtual jbyte GetByteField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetByteField(p0, p1); }
    virtual jchar GetCharField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetCharField(p0, p1); }
    virtual jshort GetShortField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetShortField(p0, p1); }
    virtual jint GetIntField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetIntField(p0, p1); }
    virtual jlong GetLongField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetLongField(p0, p1); }
    virtual jfloat GetFloatField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetFloatField(p0, p1); }
    virtual jdouble GetDoubleField(jobject p0, jfieldID p1) override { return (!env) ? 0 : env->GetDoubleField(p0, p1); }
    virtual void SetObjectField(jobject p0, jfieldID p1, jobject p2) override {
        if (env)
            env->SetObjectField(p0, p1, p2);
    }
    virtual void SetBooleanField(jobject p0, jfieldID p1, jboolean p2) override {
        if (env)
            env->SetBooleanField(p0, p1, p2);
    }
    virtual void SetByteField(jobject p0, jfieldID p1, jbyte p2) override {
        if (env)
            env->SetByteField(p0, p1, p2);
    }
    virtual void SetCharField(jobject p0, jfieldID p1, jchar p2) override {
        if (env)
            env->SetCharField(p0, p1, p2);
    }
    virtual void SetShortField(jobject p0, jfieldID p1, jshort p2) override {
        if (env)
            env->SetShortField(p0, p1, p2);
    }
    virtual void SetIntField(jobject p0, jfieldID p1, jint p2) override {
        if (env)
            env->SetIntField(p0, p1, p2);
    }
    virtual void SetLongField(jobject p0, jfieldID p1, jlong p2) override {
        if (env)
            env->SetLongField(p0, p1, p2);
    }
    virtual void SetFloatField(jobject p0, jfieldID p1, jfloat p2) override {
        if (env)
            env->SetFloatField(p0, p1, p2);
    }
    virtual void SetDoubleField(jobject p0, jfieldID p1, jdouble p2) override {
        if (env)
            env->SetDoubleField(p0, p1, p2);
    }
    virtual jmethodID GetStaticMethodID(jclass p0, const char *p1, const char *p2) override { return (!env) ? 0 : env->GetStaticMethodID(p0, p1, p2); }
    virtual jobject CallStaticObjectMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticObjectMethodV(p0, p1, p2); }
    virtual jobject CallStaticObjectMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticObjectMethodA(p0, p1, p2); }
    virtual jboolean CallStaticBooleanMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticBooleanMethodV(p0, p1, p2); }
    virtual jboolean CallStaticBooleanMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticBooleanMethodA(p0, p1, p2); }
    virtual jbyte CallStaticByteMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticByteMethodV(p0, p1, p2); }
    virtual jbyte CallStaticByteMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticByteMethodA(p0, p1, p2); }
    virtual jchar CallStaticCharMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticCharMethodV(p0, p1, p2); }
    virtual jchar CallStaticCharMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticCharMethodA(p0, p1, p2); }
    virtual jshort CallStaticShortMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticShortMethodV(p0, p1, p2); }
    virtual jshort CallStaticShortMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticShortMethodA(p0, p1, p2); }
    virtual jint CallStaticIntMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticIntMethodV(p0, p1, p2); }
    virtual jint CallStaticIntMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticIntMethodA(p0, p1, p2); }
    virtual jlong CallStaticLongMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticLongMethodV(p0, p1, p2); }
    virtual jlong CallStaticLongMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticLongMethodA(p0, p1, p2); }
    virtual jfloat CallStaticFloatMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticFloatMethodV(p0, p1, p2); }
    virtual jfloat CallStaticFloatMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticFloatMethodA(p0, p1, p2); }
    virtual jdouble CallStaticDoubleMethodV(jclass p0, jmethodID p1, va_list p2) override { return (!env) ? 0 : env->CallStaticDoubleMethodV(p0, p1, p2); }
    virtual jdouble CallStaticDoubleMethodA(jclass p0, jmethodID p1, jvalue *p2) override { return (!env) ? 0 : env->CallStaticDoubleMethodA(p0, p1, p2); }
    virtual void CallStaticVoidMethodV(jclass p0, jmethodID p1, va_list p2) override {
        if (env)
            env->CallStaticVoidMethodV(p0, p1, p2);
    }
    virtual void CallStaticVoidMethodA(jclass p0, jmethodID p1, jvalue *p2) override {
        if (env)
            env->CallStaticVoidMethodA(p0, p1, p2);
    }
    virtual jfieldID GetStaticFieldID(jclass p0, const char *p1, const char *p2) override { return (!env) ? 0 : env->GetStaticFieldID(p0, p1, p2); }
    virtual jobject GetStaticObjectField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticObjectField(p0, p1); }
    virtual jboolean GetStaticBooleanField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticBooleanField(p0, p1); }
    virtual jbyte GetStaticByteField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticByteField(p0, p1); }
    virtual jchar GetStaticCharField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticCharField(p0, p1); }
    virtual jshort GetStaticShortField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticShortField(p0, p1); }
    virtual jint GetStaticIntField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticIntField(p0, p1); }
    virtual jlong GetStaticLongField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticLongField(p0, p1); }
    virtual jfloat GetStaticFloatField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticFloatField(p0, p1); }
    virtual jdouble GetStaticDoubleField(jclass p0, jfieldID p1) override { return (!env) ? 0 : env->GetStaticDoubleField(p0, p1); }
    virtual void SetStaticObjectField(jclass p0, jfieldID p1, jobject p2) override {
        if (env)
            env->SetStaticObjectField(p0, p1, p2);
    }
    virtual void SetStaticBooleanField(jclass p0, jfieldID p1, jboolean p2) override {
        if (env)
            env->SetStaticBooleanField(p0, p1, p2);
    }
    virtual void SetStaticByteField(jclass p0, jfieldID p1, jbyte p2) override {
        if (env)
            env->SetStaticByteField(p0, p1, p2);
    }
    virtual void SetStaticCharField(jclass p0, jfieldID p1, jchar p2) override {
        if (env)
            env->SetStaticCharField(p0, p1, p2);
    }
    virtual void SetStaticShortField(jclass p0, jfieldID p1, jshort p2) override {
        if (env)
            env->SetStaticShortField(p0, p1, p2);
    }
    virtual void SetStaticIntField(jclass p0, jfieldID p1, jint p2) override {
        if (env)
            env->SetStaticIntField(p0, p1, p2);
    }
    virtual void SetStaticLongField(jclass p0, jfieldID p1, jlong p2) override {
        if (env)
            env->SetStaticLongField(p0, p1, p2);
    }
    virtual void SetStaticFloatField(jclass p0, jfieldID p1, jfloat p2) override {
        if (env)
            env->SetStaticFloatField(p0, p1, p2);
    }
    virtual void SetStaticDoubleField(jclass p0, jfieldID p1, jdouble p2) override {
        if (env)
            env->SetStaticDoubleField(p0, p1, p2);
    }
    virtual jstring NewString(const jchar *p0, jsize p1) override { return (!env) ? 0 : env->NewString(p0, p1); }
    virtual jsize GetStringLength(jstring p0) override { return (!env) ? 0 : env->GetStringLength(p0); }
    virtual const jchar *GetStringChars(jstring p0, jboolean *p1) override { return (!env) ? 0 : env->GetStringChars(p0, p1); }
    virtual void ReleaseStringChars(jstring p0, const jchar *p1) override {
        if (env)
            env->ReleaseStringChars(p0, p1);
    }
    virtual jstring NewStringUTF(const char *p0) override { return (!env) ? 0 : env->NewStringUTF(p0); }
    virtual jsize GetStringUTFLength(jstring p0) override { return (!env) ? 0 : env->GetStringUTFLength(p0); }
    virtual const char *GetStringUTFChars(jstring p0, jboolean *p1) override { return (!env) ? 0 : env->GetStringUTFChars(p0, p1); }
    virtual void ReleaseStringUTFChars(jstring p0, const char *p1) override {
        if (env)
            env->ReleaseStringUTFChars(p0, p1);
    }
    virtual jsize GetArrayLength(jarray p0) override { return (!env) ? 0 : env->GetArrayLength(p0); }
    virtual jobjectArray NewObjectArray(jsize p0, jclass p1, jobject p2) override { return (!env) ? 0 : env->NewObjectArray(p0, p1, p2); }
    virtual jobject GetObjectArrayElement(jobjectArray p0, jsize p1) override { return (!env) ? 0 : env->GetObjectArrayElement(p0, p1); }
    virtual void SetObjectArrayElement(jobjectArray p0, jsize p1, jobject p2) override {
        if (env)
            env->SetObjectArrayElement(p0, p1, p2);
    }
    virtual jbooleanArray NewBooleanArray(jsize p0) override { return (!env) ? 0 : env->NewBooleanArray(p0); }
    virtual jbyteArray NewByteArray(jsize p0) override { return (!env) ? 0 : env->NewByteArray(p0); }
    virtual jcharArray NewCharArray(jsize p0) override { return (!env) ? 0 : env->NewCharArray(p0); }
    virtual jshortArray NewShortArray(jsize p0) override { return (!env) ? 0 : env->NewShortArray(p0); }
    virtual jintArray NewIntArray(jsize p0) override { return (!env) ? 0 : env->NewIntArray(p0); }
    virtual jlongArray NewLongArray(jsize p0) override { return (!env) ? 0 : env->NewLongArray(p0); }
    virtual jfloatArray NewFloatArray(jsize p0) override { return (!env) ? 0 : env->NewFloatArray(p0); }
    virtual jdoubleArray NewDoubleArray(jsize p0) override { return (!env) ? 0 : env->NewDoubleArray(p0); }
    virtual jboolean *GetBooleanArrayElements(jbooleanArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetBooleanArrayElements(p0, p1); }
    virtual jbyte *GetByteArrayElements(jbyteArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetByteArrayElements(p0, p1); }
    virtual jchar *GetCharArrayElements(jcharArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetCharArrayElements(p0, p1); }
    virtual jshort *GetShortArrayElements(jshortArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetShortArrayElements(p0, p1); }
    virtual jint *GetIntArrayElements(jintArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetIntArrayElements(p0, p1); }
    virtual jlong *GetLongArrayElements(jlongArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetLongArrayElements(p0, p1); }
    virtual jfloat *GetFloatArrayElements(jfloatArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetFloatArrayElements(p0, p1); }
    virtual jdouble *GetDoubleArrayElements(jdoubleArray p0, jboolean *p1) override { return (!env) ? 0 : env->GetDoubleArrayElements(p0, p1); }
    virtual void ReleaseBooleanArrayElements(jbooleanArray p0, jboolean *p1, jint p2) override {
        if (env)
            env->ReleaseBooleanArrayElements(p0, p1, p2);
    }
    virtual void ReleaseByteArrayElements(jbyteArray p0, jbyte *p1, jint p2) override {
        if (env)
            env->ReleaseByteArrayElements(p0, p1, p2);
    }
    virtual void ReleaseCharArrayElements(jcharArray p0, jchar *p1, jint p2) override {
        if (env)
            env->ReleaseCharArrayElements(p0, p1, p2);
    }
    virtual void ReleaseShortArrayElements(jshortArray p0, jshort *p1, jint p2) override {
        if (env)
            env->ReleaseShortArrayElements(p0, p1, p2);
    }
    virtual void ReleaseIntArrayElements(jintArray p0, jint *p1, jint p2) override {
        if (env)
            env->ReleaseIntArrayElements(p0, p1, p2);
    }
    virtual void ReleaseLongArrayElements(jlongArray p0, jlong *p1, jint p2) override {
        if (env)
            env->ReleaseLongArrayElements(p0, p1, p2);
    }
    virtual void ReleaseFloatArrayElements(jfloatArray p0, jfloat *p1, jint p2) override {
        if (env)
            env->ReleaseFloatArrayElements(p0, p1, p2);
    }
    virtual void ReleaseDoubleArrayElements(jdoubleArray p0, jdouble *p1, jint p2) override {
        if (env)
            env->ReleaseDoubleArrayElements(p0, p1, p2);
    }
    virtual void GetBooleanArrayRegion(jbooleanArray p0, jsize p1, jsize p2, jboolean *p3) override {
        if (env)
            env->GetBooleanArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetByteArrayRegion(jbyteArray p0, jsize p1, jsize p2, jbyte *p3) override {
        if (env)
            env->GetByteArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetCharArrayRegion(jcharArray p0, jsize p1, jsize p2, jchar *p3) override {
        if (env)
            env->GetCharArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetShortArrayRegion(jshortArray p0, jsize p1, jsize p2, jshort *p3) override {
        if (env)
            env->GetShortArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetIntArrayRegion(jintArray p0, jsize p1, jsize p2, jint *p3) override {
        if (env)
            env->GetIntArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetLongArrayRegion(jlongArray p0, jsize p1, jsize p2, jlong *p3) override {
        if (env)
            env->GetLongArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetFloatArrayRegion(jfloatArray p0, jsize p1, jsize p2, jfloat *p3) override {
        if (env)
            env->GetFloatArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetDoubleArrayRegion(jdoubleArray p0, jsize p1, jsize p2, jdouble *p3) override {
        if (env)
            env->GetDoubleArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetBooleanArrayRegion(jbooleanArray p0, jsize p1, jsize p2, const jboolean *p3) override {
        if (env)
            env->SetBooleanArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetByteArrayRegion(jbyteArray p0, jsize p1, jsize p2, const jbyte *p3) override {
        if (env)
            env->SetByteArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetCharArrayRegion(jcharArray p0, jsize p1, jsize p2, const jchar *p3) override {
        if (env)
            env->SetCharArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetShortArrayRegion(jshortArray p0, jsize p1, jsize p2, const jshort *p3) override {
        if (env)
            env->SetShortArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetIntArrayRegion(jintArray p0, jsize p1, jsize p2, const jint *p3) override {
        if (env)
            env->SetIntArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetLongArrayRegion(jlongArray p0, jsize p1, jsize p2, const jlong *p3) override {
        if (env)
            env->SetLongArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetFloatArrayRegion(jfloatArray p0, jsize p1, jsize p2, const jfloat *p3) override {
        if (env)
            env->SetFloatArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetDoubleArrayRegion(jdoubleArray p0, jsize p1, jsize p2, const jdouble *p3) override {
        if (env)
            env->SetDoubleArrayRegion(p0, p1, p2, p3);
    }
    virtual jint RegisterNatives(jclass p0, const JNINativeMethod *p1, jint p2) override { return (!env) ? 0 : env->RegisterNatives(p0, p1, p2); }
    virtual jint UnregisterNatives(jclass p0) override { return (!env) ? 0 : env->UnregisterNatives(p0); }
    virtual jint MonitorEnter(jobject p0) override { return (!env) ? 0 : env->MonitorEnter(p0); }
    virtual jint MonitorExit(jobject p0) override { return (!env) ? 0 : env->MonitorExit(p0); }
    virtual void GetStringRegion(jstring p0, jsize p1, jsize p2, jchar *p3) override {
        if (env)
            env->GetStringRegion(p0, p1, p2, p3);
    }
    virtual void GetStringUTFRegion(jstring p0, jsize p1, jsize p2, char *p3) override {
        if (env)
            env->GetStringUTFRegion(p0, p1, p2, p3);
    }
    virtual void *GetPrimitiveArrayCritical(jarray p0, jboolean *p1) override { return (!env) ? 0 : env->GetPrimitiveArrayCritical(p0, p1); }
    virtual void ReleasePrimitiveArrayCritical(jarray p0, void *p1, jint p2) override {
        if (env)
            env->ReleasePrimitiveArrayCritical(p0, p1, p2);
    }
    virtual const jchar *GetStringCritical(jstring p0, jboolean *p1) override { return (!env) ? 0 : env->GetStringCritical(p0, p1); }
    virtual void ReleaseStringCritical(jstring p0, const jchar *p1) override {
        if (env)
            env->ReleaseStringCritical(p0, p1);
    }
    virtual jweak NewWeakGlobalRef(jobject p0) override { return (!env) ? 0 : env->NewWeakGlobalRef(p0); }
    virtual void DeleteWeakGlobalRef(jweak p0) override {
        if (env)
            env->DeleteWeakGlobalRef(p0);
    }
    virtual jboolean ExceptionCheck() override { return (!env) ? 0 : env->ExceptionCheck(); }
    virtual jobject NewDirectByteBuffer(void *p0, jlong p1) override { return (!env) ? 0 : env->NewDirectByteBuffer(p0, p1); }
    virtual void *GetDirectBufferAddress(jobject p0) override { return (!env) ? 0 : env->GetDirectBufferAddress(p0); }
    virtual jlong GetDirectBufferCapacity(jobject p0) override { return (!env) ? 0 : env->GetDirectBufferCapacity(p0); }
    virtual jobjectRefType GetObjectRefType(jobject p0) override { return (!env) ? (jobjectRefType)0 : env->GetObjectRefType(p0); }
};
#endif
