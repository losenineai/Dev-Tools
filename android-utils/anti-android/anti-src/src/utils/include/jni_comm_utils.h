//
// Created by white on 2018/5/14.
//

#include <jni.h>

#include "anti_config.h"

jclass anti_jni_findClassByName(JNIEnv *env, const char *clsName);

jmethodID anti_jni_getStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);

jmethodID anti_jni_getMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);

jclass anti_jni_findClassByObj(JNIEnv *env, jobject obj);

jmethodID anti_jni_getMethodIDByObj(JNIEnv *env, jobject obj, const char *name, const char *sig);

jobject anti_jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, jmethodID methodID,
                                           va_list args);

jobject anti_jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, const char *name,
                                           const char *sig, ...);

jboolean anti_jni_callStaticMethodReturnBool(JNIEnv *env, jclass clazz, const char *name,
                                        const char *sig, ...);

jobject anti_jni_callMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...);

void anti_jni_callVoidMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                   ...);

void anti_jni_callVoidMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);

jobject anti_jni_callMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);

int anti_jni_deleteGlobalRef(JNIEnv *env, jobject globalRef);

jobject anti_jni_getGlobalContext(JNIEnv *env);

jobject anti_jni_getObjectField(JNIEnv *env, jobject obj, const char *name, const char *sig);

jint anti_jni_getIntField(JNIEnv *env, jobject obj, const char *name, const char *sig);

jobject anti_jni_getStaticObjectField(JNIEnv *env, const char *clasName, const char *name,
                                      const char *sig);

jint anti_jni_callStaticMethodReturnInt(JNIEnv *env, jclass clazz , const char *name, const char *sig, ...);

jboolean anti_jni_callBoolMethodById(JNIEnv* env, jobject obj,jmethodID methodID,va_list args);

jboolean anti_jni_callBoolMethodByName(JNIEnv *env, jobject clazz, const char *name,
                                       const char *sig, ...);

jint anti_jni_getStaticFieldRetIntByName(JNIEnv *env, const char *clsname, const char *name, const char* sig);

jint anti_jni_callIntMethodById(JNIEnv* env, jobject obj,jmethodID methodID,va_list args);

jint anti_jni_callIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);

jobject anti_jni_NewObject(JNIEnv *env, const char *clsname,  const char *sig, ...);


jint anti_jni_get_sdk_int();

void anti_jni_setContext(JNIEnv *env, jobject jContext);