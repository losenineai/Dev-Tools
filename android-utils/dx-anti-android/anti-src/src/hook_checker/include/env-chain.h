#include "jni.h"

#ifndef __CHAIN_H__
#define __CHAIN_H__

class JniEnvChain {
  public:
    JniEnvChain *next;

  public:
    JniEnvChain(JniEnvChain *next) : next(next) {}
    JniEnvChain() : next(0) {}
    virtual ~JniEnvChain() {
        if (next) {
            delete next;
            next = 0;
        }
    };

    virtual jint GetVersion() { return (!next) ? 0 : next->GetVersion(); }
    virtual jclass DefineClass(const char *p0, jobject p1, const jbyte *p2, jsize p3) { return (!next) ? 0 : next->DefineClass(p0, p1, p2, p3); }
    virtual jclass FindClass(const char *p0) { return (!next) ? 0 : next->FindClass(p0); }
    virtual jmethodID FromReflectedMethod(jobject p0) { return (!next) ? 0 : next->FromReflectedMethod(p0); }
    virtual jfieldID FromReflectedField(jobject p0) { return (!next) ? 0 : next->FromReflectedField(p0); }
    virtual jobject ToReflectedMethod(jclass p0, jmethodID p1, jboolean p2) { return (!next) ? 0 : next->ToReflectedMethod(p0, p1, p2); }
    virtual jclass GetSuperclass(jclass p0) { return (!next) ? 0 : next->GetSuperclass(p0); }
    virtual jboolean IsAssignableFrom(jclass p0, jclass p1) { return (!next) ? 0 : next->IsAssignableFrom(p0, p1); }
    virtual jobject ToReflectedField(jclass p0, jfieldID p1, jboolean p2) { return (!next) ? 0 : next->ToReflectedField(p0, p1, p2); }
    virtual jint Throw(jthrowable p0) { return (!next) ? 0 : next->Throw(p0); }
    virtual jint ThrowNew(jclass p0, const char *p1) { return (!next) ? 0 : next->ThrowNew(p0, p1); }
    virtual jthrowable ExceptionOccurred() { return (!next) ? 0 : next->ExceptionOccurred(); }
    virtual void ExceptionDescribe() {
        if (next)
            next->ExceptionDescribe();
    }
    virtual void ExceptionClear() {
        if (next)
            next->ExceptionClear();
    }
    virtual void FatalError(const char *p0) {
        if (next)
            next->FatalError(p0);
    }
    virtual jint PushLocalFrame(jint p0) { return (!next) ? 0 : next->PushLocalFrame(p0); }
    virtual jobject PopLocalFrame(jobject p0) { return (!next) ? 0 : next->PopLocalFrame(p0); }
    virtual jobject NewGlobalRef(jobject p0) { return (!next) ? 0 : next->NewGlobalRef(p0); }
    virtual void DeleteGlobalRef(jobject p0) {
        if (next)
            next->DeleteGlobalRef(p0);
    }
    virtual void DeleteLocalRef(jobject p0) {
        if (next)
            next->DeleteLocalRef(p0);
    }
    virtual jboolean IsSameObject(jobject p0, jobject p1) { return (!next) ? 0 : next->IsSameObject(p0, p1); }
    virtual jobject NewLocalRef(jobject p0) { return (!next) ? 0 : next->NewLocalRef(p0); }
    virtual jint EnsureLocalCapacity(jint p0) { return (!next) ? 0 : next->EnsureLocalCapacity(p0); }
    virtual jobject AllocObject(jclass p0) { return (!next) ? 0 : next->AllocObject(p0); }
    virtual jobject NewObjectV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->NewObjectV(p0, p1, p2); }
    virtual jobject NewObjectA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->NewObjectA(p0, p1, p2); }
    virtual jclass GetObjectClass(jobject p0) { return (!next) ? 0 : next->GetObjectClass(p0); }
    virtual jboolean IsInstanceOf(jobject p0, jclass p1) { return (!next) ? 0 : next->IsInstanceOf(p0, p1); }
    virtual jmethodID GetMethodID(jclass p0, const char *p1, const char *p2) { return (!next) ? 0 : next->GetMethodID(p0, p1, p2); }
    virtual jobject CallObjectMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallObjectMethodV(p0, p1, p2); }
    virtual jobject CallObjectMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallObjectMethodA(p0, p1, p2); }
    virtual jboolean CallBooleanMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallBooleanMethodV(p0, p1, p2); }
    virtual jboolean CallBooleanMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallBooleanMethodA(p0, p1, p2); }
    virtual jbyte CallByteMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallByteMethodV(p0, p1, p2); }
    virtual jbyte CallByteMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallByteMethodA(p0, p1, p2); }
    virtual jchar CallCharMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallCharMethodV(p0, p1, p2); }
    virtual jchar CallCharMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallCharMethodA(p0, p1, p2); }
    virtual jshort CallShortMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallShortMethodV(p0, p1, p2); }
    virtual jshort CallShortMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallShortMethodA(p0, p1, p2); }
    virtual jint CallIntMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallIntMethodV(p0, p1, p2); }
    virtual jint CallIntMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallIntMethodA(p0, p1, p2); }
    virtual jlong CallLongMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallLongMethodV(p0, p1, p2); }
    virtual jlong CallLongMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallLongMethodA(p0, p1, p2); }
    virtual jfloat CallFloatMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallFloatMethodV(p0, p1, p2); }
    virtual jfloat CallFloatMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallFloatMethodA(p0, p1, p2); }
    virtual jdouble CallDoubleMethodV(jobject p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallDoubleMethodV(p0, p1, p2); }
    virtual jdouble CallDoubleMethodA(jobject p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallDoubleMethodA(p0, p1, p2); }
    virtual void CallVoidMethodV(jobject p0, jmethodID p1, va_list p2) {
        if (next)
            next->CallVoidMethodV(p0, p1, p2);
    }
    virtual void CallVoidMethodA(jobject p0, jmethodID p1, jvalue *p2) {
        if (next)
            next->CallVoidMethodA(p0, p1, p2);
    }
    virtual jobject CallNonvirtualObjectMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualObjectMethodV(p0, p1, p2, p3); }
    virtual jobject CallNonvirtualObjectMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualObjectMethodA(p0, p1, p2, p3); }
    virtual jboolean CallNonvirtualBooleanMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualBooleanMethodV(p0, p1, p2, p3); }
    virtual jboolean CallNonvirtualBooleanMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualBooleanMethodA(p0, p1, p2, p3); }
    virtual jbyte CallNonvirtualByteMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualByteMethodV(p0, p1, p2, p3); }
    virtual jbyte CallNonvirtualByteMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualByteMethodA(p0, p1, p2, p3); }
    virtual jchar CallNonvirtualCharMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualCharMethodV(p0, p1, p2, p3); }
    virtual jchar CallNonvirtualCharMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualCharMethodA(p0, p1, p2, p3); }
    virtual jshort CallNonvirtualShortMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualShortMethodV(p0, p1, p2, p3); }
    virtual jshort CallNonvirtualShortMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualShortMethodA(p0, p1, p2, p3); }
    virtual jint CallNonvirtualIntMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualIntMethodV(p0, p1, p2, p3); }
    virtual jint CallNonvirtualIntMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualIntMethodA(p0, p1, p2, p3); }
    virtual jlong CallNonvirtualLongMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualLongMethodV(p0, p1, p2, p3); }
    virtual jlong CallNonvirtualLongMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualLongMethodA(p0, p1, p2, p3); }
    virtual jfloat CallNonvirtualFloatMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualFloatMethodV(p0, p1, p2, p3); }
    virtual jfloat CallNonvirtualFloatMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualFloatMethodA(p0, p1, p2, p3); }
    virtual jdouble CallNonvirtualDoubleMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) { return (!next) ? 0 : next->CallNonvirtualDoubleMethodV(p0, p1, p2, p3); }
    virtual jdouble CallNonvirtualDoubleMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) { return (!next) ? 0 : next->CallNonvirtualDoubleMethodA(p0, p1, p2, p3); }
    virtual void CallNonvirtualVoidMethodV(jobject p0, jclass p1, jmethodID p2, va_list p3) {
        if (next)
            next->CallNonvirtualVoidMethodV(p0, p1, p2, p3);
    }
    virtual void CallNonvirtualVoidMethodA(jobject p0, jclass p1, jmethodID p2, jvalue *p3) {
        if (next)
            next->CallNonvirtualVoidMethodA(p0, p1, p2, p3);
    }
    virtual jfieldID GetFieldID(jclass p0, const char *p1, const char *p2) { return (!next) ? 0 : next->GetFieldID(p0, p1, p2); }
    virtual jobject GetObjectField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetObjectField(p0, p1); }
    virtual jboolean GetBooleanField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetBooleanField(p0, p1); }
    virtual jbyte GetByteField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetByteField(p0, p1); }
    virtual jchar GetCharField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetCharField(p0, p1); }
    virtual jshort GetShortField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetShortField(p0, p1); }
    virtual jint GetIntField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetIntField(p0, p1); }
    virtual jlong GetLongField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetLongField(p0, p1); }
    virtual jfloat GetFloatField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetFloatField(p0, p1); }
    virtual jdouble GetDoubleField(jobject p0, jfieldID p1) { return (!next) ? 0 : next->GetDoubleField(p0, p1); }
    virtual void SetObjectField(jobject p0, jfieldID p1, jobject p2) {
        if (next)
            next->SetObjectField(p0, p1, p2);
    }
    virtual void SetBooleanField(jobject p0, jfieldID p1, jboolean p2) {
        if (next)
            next->SetBooleanField(p0, p1, p2);
    }
    virtual void SetByteField(jobject p0, jfieldID p1, jbyte p2) {
        if (next)
            next->SetByteField(p0, p1, p2);
    }
    virtual void SetCharField(jobject p0, jfieldID p1, jchar p2) {
        if (next)
            next->SetCharField(p0, p1, p2);
    }
    virtual void SetShortField(jobject p0, jfieldID p1, jshort p2) {
        if (next)
            next->SetShortField(p0, p1, p2);
    }
    virtual void SetIntField(jobject p0, jfieldID p1, jint p2) {
        if (next)
            next->SetIntField(p0, p1, p2);
    }
    virtual void SetLongField(jobject p0, jfieldID p1, jlong p2) {
        if (next)
            next->SetLongField(p0, p1, p2);
    }
    virtual void SetFloatField(jobject p0, jfieldID p1, jfloat p2) {
        if (next)
            next->SetFloatField(p0, p1, p2);
    }
    virtual void SetDoubleField(jobject p0, jfieldID p1, jdouble p2) {
        if (next)
            next->SetDoubleField(p0, p1, p2);
    }
    virtual jmethodID GetStaticMethodID(jclass p0, const char *p1, const char *p2) { return (!next) ? 0 : next->GetStaticMethodID(p0, p1, p2); }
    virtual jobject CallStaticObjectMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticObjectMethodV(p0, p1, p2); }
    virtual jobject CallStaticObjectMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticObjectMethodA(p0, p1, p2); }
    virtual jboolean CallStaticBooleanMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticBooleanMethodV(p0, p1, p2); }
    virtual jboolean CallStaticBooleanMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticBooleanMethodA(p0, p1, p2); }
    virtual jbyte CallStaticByteMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticByteMethodV(p0, p1, p2); }
    virtual jbyte CallStaticByteMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticByteMethodA(p0, p1, p2); }
    virtual jchar CallStaticCharMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticCharMethodV(p0, p1, p2); }
    virtual jchar CallStaticCharMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticCharMethodA(p0, p1, p2); }
    virtual jshort CallStaticShortMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticShortMethodV(p0, p1, p2); }
    virtual jshort CallStaticShortMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticShortMethodA(p0, p1, p2); }
    virtual jint CallStaticIntMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticIntMethodV(p0, p1, p2); }
    virtual jint CallStaticIntMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticIntMethodA(p0, p1, p2); }
    virtual jlong CallStaticLongMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticLongMethodV(p0, p1, p2); }
    virtual jlong CallStaticLongMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticLongMethodA(p0, p1, p2); }
    virtual jfloat CallStaticFloatMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticFloatMethodV(p0, p1, p2); }
    virtual jfloat CallStaticFloatMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticFloatMethodA(p0, p1, p2); }
    virtual jdouble CallStaticDoubleMethodV(jclass p0, jmethodID p1, va_list p2) { return (!next) ? 0 : next->CallStaticDoubleMethodV(p0, p1, p2); }
    virtual jdouble CallStaticDoubleMethodA(jclass p0, jmethodID p1, jvalue *p2) { return (!next) ? 0 : next->CallStaticDoubleMethodA(p0, p1, p2); }
    virtual void CallStaticVoidMethodV(jclass p0, jmethodID p1, va_list p2) {
        if (next)
            next->CallStaticVoidMethodV(p0, p1, p2);
    }
    virtual void CallStaticVoidMethodA(jclass p0, jmethodID p1, jvalue *p2) {
        if (next)
            next->CallStaticVoidMethodA(p0, p1, p2);
    }
    virtual jfieldID GetStaticFieldID(jclass p0, const char *p1, const char *p2) { return (!next) ? 0 : next->GetStaticFieldID(p0, p1, p2); }
    virtual jobject GetStaticObjectField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticObjectField(p0, p1); }
    virtual jboolean GetStaticBooleanField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticBooleanField(p0, p1); }
    virtual jbyte GetStaticByteField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticByteField(p0, p1); }
    virtual jchar GetStaticCharField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticCharField(p0, p1); }
    virtual jshort GetStaticShortField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticShortField(p0, p1); }
    virtual jint GetStaticIntField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticIntField(p0, p1); }
    virtual jlong GetStaticLongField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticLongField(p0, p1); }
    virtual jfloat GetStaticFloatField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticFloatField(p0, p1); }
    virtual jdouble GetStaticDoubleField(jclass p0, jfieldID p1) { return (!next) ? 0 : next->GetStaticDoubleField(p0, p1); }
    virtual void SetStaticObjectField(jclass p0, jfieldID p1, jobject p2) {
        if (next)
            next->SetStaticObjectField(p0, p1, p2);
    }
    virtual void SetStaticBooleanField(jclass p0, jfieldID p1, jboolean p2) {
        if (next)
            next->SetStaticBooleanField(p0, p1, p2);
    }
    virtual void SetStaticByteField(jclass p0, jfieldID p1, jbyte p2) {
        if (next)
            next->SetStaticByteField(p0, p1, p2);
    }
    virtual void SetStaticCharField(jclass p0, jfieldID p1, jchar p2) {
        if (next)
            next->SetStaticCharField(p0, p1, p2);
    }
    virtual void SetStaticShortField(jclass p0, jfieldID p1, jshort p2) {
        if (next)
            next->SetStaticShortField(p0, p1, p2);
    }
    virtual void SetStaticIntField(jclass p0, jfieldID p1, jint p2) {
        if (next)
            next->SetStaticIntField(p0, p1, p2);
    }
    virtual void SetStaticLongField(jclass p0, jfieldID p1, jlong p2) {
        if (next)
            next->SetStaticLongField(p0, p1, p2);
    }
    virtual void SetStaticFloatField(jclass p0, jfieldID p1, jfloat p2) {
        if (next)
            next->SetStaticFloatField(p0, p1, p2);
    }
    virtual void SetStaticDoubleField(jclass p0, jfieldID p1, jdouble p2) {
        if (next)
            next->SetStaticDoubleField(p0, p1, p2);
    }
    virtual jstring NewString(const jchar *p0, jsize p1) { return (!next) ? 0 : next->NewString(p0, p1); }
    virtual jsize GetStringLength(jstring p0) { return (!next) ? 0 : next->GetStringLength(p0); }
    virtual const jchar *GetStringChars(jstring p0, jboolean *p1) { return (!next) ? 0 : next->GetStringChars(p0, p1); }
    virtual void ReleaseStringChars(jstring p0, const jchar *p1) {
        if (next)
            next->ReleaseStringChars(p0, p1);
    }
    virtual jstring NewStringUTF(const char *p0) { return (!next) ? 0 : next->NewStringUTF(p0); }
    virtual jsize GetStringUTFLength(jstring p0) { return (!next) ? 0 : next->GetStringUTFLength(p0); }
    virtual const char *GetStringUTFChars(jstring p0, jboolean *p1) { return (!next) ? 0 : next->GetStringUTFChars(p0, p1); }
    virtual void ReleaseStringUTFChars(jstring p0, const char *p1) {
        if (next)
            next->ReleaseStringUTFChars(p0, p1);
    }
    virtual jsize GetArrayLength(jarray p0) { return (!next) ? 0 : next->GetArrayLength(p0); }
    virtual jobjectArray NewObjectArray(jsize p0, jclass p1, jobject p2) { return (!next) ? 0 : next->NewObjectArray(p0, p1, p2); }
    virtual jobject GetObjectArrayElement(jobjectArray p0, jsize p1) { return (!next) ? 0 : next->GetObjectArrayElement(p0, p1); }
    virtual void SetObjectArrayElement(jobjectArray p0, jsize p1, jobject p2) {
        if (next)
            next->SetObjectArrayElement(p0, p1, p2);
    }
    virtual jbooleanArray NewBooleanArray(jsize p0) { return (!next) ? 0 : next->NewBooleanArray(p0); }
    virtual jbyteArray NewByteArray(jsize p0) { return (!next) ? 0 : next->NewByteArray(p0); }
    virtual jcharArray NewCharArray(jsize p0) { return (!next) ? 0 : next->NewCharArray(p0); }
    virtual jshortArray NewShortArray(jsize p0) { return (!next) ? 0 : next->NewShortArray(p0); }
    virtual jintArray NewIntArray(jsize p0) { return (!next) ? 0 : next->NewIntArray(p0); }
    virtual jlongArray NewLongArray(jsize p0) { return (!next) ? 0 : next->NewLongArray(p0); }
    virtual jfloatArray NewFloatArray(jsize p0) { return (!next) ? 0 : next->NewFloatArray(p0); }
    virtual jdoubleArray NewDoubleArray(jsize p0) { return (!next) ? 0 : next->NewDoubleArray(p0); }
    virtual jboolean *GetBooleanArrayElements(jbooleanArray p0, jboolean *p1) { return (!next) ? 0 : next->GetBooleanArrayElements(p0, p1); }
    virtual jbyte *GetByteArrayElements(jbyteArray p0, jboolean *p1) { return (!next) ? 0 : next->GetByteArrayElements(p0, p1); }
    virtual jchar *GetCharArrayElements(jcharArray p0, jboolean *p1) { return (!next) ? 0 : next->GetCharArrayElements(p0, p1); }
    virtual jshort *GetShortArrayElements(jshortArray p0, jboolean *p1) { return (!next) ? 0 : next->GetShortArrayElements(p0, p1); }
    virtual jint *GetIntArrayElements(jintArray p0, jboolean *p1) { return (!next) ? 0 : next->GetIntArrayElements(p0, p1); }
    virtual jlong *GetLongArrayElements(jlongArray p0, jboolean *p1) { return (!next) ? 0 : next->GetLongArrayElements(p0, p1); }
    virtual jfloat *GetFloatArrayElements(jfloatArray p0, jboolean *p1) { return (!next) ? 0 : next->GetFloatArrayElements(p0, p1); }
    virtual jdouble *GetDoubleArrayElements(jdoubleArray p0, jboolean *p1) { return (!next) ? 0 : next->GetDoubleArrayElements(p0, p1); }
    virtual void ReleaseBooleanArrayElements(jbooleanArray p0, jboolean *p1, jint p2) {
        if (next)
            next->ReleaseBooleanArrayElements(p0, p1, p2);
    }
    virtual void ReleaseByteArrayElements(jbyteArray p0, jbyte *p1, jint p2) {
        if (next)
            next->ReleaseByteArrayElements(p0, p1, p2);
    }
    virtual void ReleaseCharArrayElements(jcharArray p0, jchar *p1, jint p2) {
        if (next)
            next->ReleaseCharArrayElements(p0, p1, p2);
    }
    virtual void ReleaseShortArrayElements(jshortArray p0, jshort *p1, jint p2) {
        if (next)
            next->ReleaseShortArrayElements(p0, p1, p2);
    }
    virtual void ReleaseIntArrayElements(jintArray p0, jint *p1, jint p2) {
        if (next)
            next->ReleaseIntArrayElements(p0, p1, p2);
    }
    virtual void ReleaseLongArrayElements(jlongArray p0, jlong *p1, jint p2) {
        if (next)
            next->ReleaseLongArrayElements(p0, p1, p2);
    }
    virtual void ReleaseFloatArrayElements(jfloatArray p0, jfloat *p1, jint p2) {
        if (next)
            next->ReleaseFloatArrayElements(p0, p1, p2);
    }
    virtual void ReleaseDoubleArrayElements(jdoubleArray p0, jdouble *p1, jint p2) {
        if (next)
            next->ReleaseDoubleArrayElements(p0, p1, p2);
    }
    virtual void GetBooleanArrayRegion(jbooleanArray p0, jsize p1, jsize p2, jboolean *p3) {
        if (next)
            next->GetBooleanArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetByteArrayRegion(jbyteArray p0, jsize p1, jsize p2, jbyte *p3) {
        if (next)
            next->GetByteArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetCharArrayRegion(jcharArray p0, jsize p1, jsize p2, jchar *p3) {
        if (next)
            next->GetCharArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetShortArrayRegion(jshortArray p0, jsize p1, jsize p2, jshort *p3) {
        if (next)
            next->GetShortArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetIntArrayRegion(jintArray p0, jsize p1, jsize p2, jint *p3) {
        if (next)
            next->GetIntArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetLongArrayRegion(jlongArray p0, jsize p1, jsize p2, jlong *p3) {
        if (next)
            next->GetLongArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetFloatArrayRegion(jfloatArray p0, jsize p1, jsize p2, jfloat *p3) {
        if (next)
            next->GetFloatArrayRegion(p0, p1, p2, p3);
    }
    virtual void GetDoubleArrayRegion(jdoubleArray p0, jsize p1, jsize p2, jdouble *p3) {
        if (next)
            next->GetDoubleArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetBooleanArrayRegion(jbooleanArray p0, jsize p1, jsize p2, const jboolean *p3) {
        if (next)
            next->SetBooleanArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetByteArrayRegion(jbyteArray p0, jsize p1, jsize p2, const jbyte *p3) {
        if (next)
            next->SetByteArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetCharArrayRegion(jcharArray p0, jsize p1, jsize p2, const jchar *p3) {
        if (next)
            next->SetCharArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetShortArrayRegion(jshortArray p0, jsize p1, jsize p2, const jshort *p3) {
        if (next)
            next->SetShortArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetIntArrayRegion(jintArray p0, jsize p1, jsize p2, const jint *p3) {
        if (next)
            next->SetIntArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetLongArrayRegion(jlongArray p0, jsize p1, jsize p2, const jlong *p3) {
        if (next)
            next->SetLongArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetFloatArrayRegion(jfloatArray p0, jsize p1, jsize p2, const jfloat *p3) {
        if (next)
            next->SetFloatArrayRegion(p0, p1, p2, p3);
    }
    virtual void SetDoubleArrayRegion(jdoubleArray p0, jsize p1, jsize p2, const jdouble *p3) {
        if (next)
            next->SetDoubleArrayRegion(p0, p1, p2, p3);
    }
    virtual jint RegisterNatives(jclass p0, const JNINativeMethod *p1, jint p2) { return (!next) ? 0 : next->RegisterNatives(p0, p1, p2); }
    virtual jint UnregisterNatives(jclass p0) { return (!next) ? 0 : next->UnregisterNatives(p0); }
    virtual jint MonitorEnter(jobject p0) { return (!next) ? 0 : next->MonitorEnter(p0); }
    virtual jint MonitorExit(jobject p0) { return (!next) ? 0 : next->MonitorExit(p0); }
    virtual void GetStringRegion(jstring p0, jsize p1, jsize p2, jchar *p3) {
        if (next)
            next->GetStringRegion(p0, p1, p2, p3);
    }
    virtual void GetStringUTFRegion(jstring p0, jsize p1, jsize p2, char *p3) {
        if (next)
            next->GetStringUTFRegion(p0, p1, p2, p3);
    }
    virtual void *GetPrimitiveArrayCritical(jarray p0, jboolean *p1) { return (!next) ? 0 : next->GetPrimitiveArrayCritical(p0, p1); }
    virtual void ReleasePrimitiveArrayCritical(jarray p0, void *p1, jint p2) {
        if (next)
            next->ReleasePrimitiveArrayCritical(p0, p1, p2);
    }
    virtual const jchar *GetStringCritical(jstring p0, jboolean *p1) { return (!next) ? 0 : next->GetStringCritical(p0, p1); }
    virtual void ReleaseStringCritical(jstring p0, const jchar *p1) {
        if (next)
            next->ReleaseStringCritical(p0, p1);
    }
    virtual jweak NewWeakGlobalRef(jobject p0) { return (!next) ? 0 : next->NewWeakGlobalRef(p0); }
    virtual void DeleteWeakGlobalRef(jweak p0) {
        if (next)
            next->DeleteWeakGlobalRef(p0);
    }
    virtual jboolean ExceptionCheck() { return (!next) ? 0 : next->ExceptionCheck(); }
    virtual jobject NewDirectByteBuffer(void *p0, jlong p1) { return (!next) ? 0 : next->NewDirectByteBuffer(p0, p1); }
    virtual void *GetDirectBufferAddress(jobject p0) { return (!next) ? 0 : next->GetDirectBufferAddress(p0); }
    virtual jlong GetDirectBufferCapacity(jobject p0) { return (!next) ? 0 : next->GetDirectBufferCapacity(p0); }
    virtual jobjectRefType GetObjectRefType(jobject p0) { return (!next) ? (jobjectRefType)0 : next->GetObjectRefType(p0); }
};

#endif
