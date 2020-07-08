#ifndef DX_HOOKPROCS_
#define DX_HOOKPROCS_
#include <jni.h>

jbyteArray getAddr(JNIEnv *env, jclass);
void initNative(JNIEnv* env, jobject, jstring sourceDirJs, jstring extraDirJs, jstring releaseDirJs, jstring pkgName, jint encSize);
int initDexOptDexLoad(JNIEnv* env, jobject jobj, jint payloadType, jint encLen);
int initNomalDexLoad(JNIEnv* env, jobject jobj, jint payloadType, jint encLen);
void cleanup(JNIEnv *env, jclass _);
#endif
