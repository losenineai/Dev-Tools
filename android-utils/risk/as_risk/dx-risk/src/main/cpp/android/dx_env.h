#include <jni.h>
#include "android_prefix.h"

jclass DXFindClass(JNIEnv *env, const char *clsName);
jmethodID DXGetStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);
jmethodID DXGetMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);
jclass DXGetObjectClass(JNIEnv *env, jobject obj);
int DXDeleteGlobalRef(JNIEnv *env, jobject globalRef);
jmethodID DXGetObjectMethodID(JNIEnv *env, jobject obj, const char *name, const char *sig);
jboolean DXCallBooleanMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);
jboolean DXCallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
jint DXCallIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);
jint DXCallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
jobject DXCallStaticObjectMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
jobject DXCallStaticObjectMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...);
jobject DXCallObjectMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);
jobject DXCallObjectMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
int DXCallStaticVoidMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...);
int DXCallStaticVoidMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
int DXCallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
int DXCallVoidMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);
jboolean DXCallStaticBooleanMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
jboolean DXCallStaticBooleanMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...);
jint DXCallStaticIntMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
jint DXCallStaticIntMethodByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...);
jobject DXNewObjectV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
jobject DXNewObjectByName(JNIEnv *env, jclass clazz, const char *name, const char *sig, ...);
jobject DXToReflectedMethod(JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic);
jobject DXGetGlobalContext(JNIEnv *env);

bool DXCheckException(JNIEnv *env);
jobject DXGetStaticObjectField(JNIEnv *env, const char *clasName, const char *name, const char *sig);



// class DXEnv {
// public:
//     DXEnv(JNIEnv* env);
//     virtual ~DXEnv();

// public:
//     JNIEnv* _env;
// }