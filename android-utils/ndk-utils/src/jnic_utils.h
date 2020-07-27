//
// Created by white on 2018/5/14.
//

#include <jni.h>
#include <string>
#include "log.h"

using std::string;
using namespace std;



jobject jni_getGlobalContext(JNIEnv *env);

void jni_setGlobalContext(JNIEnv *env, jobject jContext);

jclass jni_findClassByName(JNIEnv *env, const char *clsName);

jmethodID jni_getStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);

jmethodID jni_getMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig);

jclass jni_findClassByObj(JNIEnv *env, jobject obj);

jmethodID jni_getMethodIDByObj(JNIEnv *env, jobject obj, const char *name, const char *sig);

jobject jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, jmethodID methodID,
                                           va_list args);

jobject jni_callStaticMethodReturnObj(JNIEnv *env, jclass clazz, const char *name,
                                           const char *sig, ...);

jboolean jni_callStaticMethodReturnBool(JNIEnv *env, jclass clazz, const char *name,
                                        const char *sig, ...);

jobject jni_callMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig,
                                  ...);

jobject jni_callMethodById(JNIEnv *env, jobject obj, jmethodID methodID, va_list args);

jobject jni_getObjectField(JNIEnv *env, jobject obj, const char *name, const char *sig);

jobject jni_getStaticObjectField(JNIEnv *env, const char *clasName, const char *name,
                                      const char *sig);

jint jni_callStaticMethodReturnInt(JNIEnv *env, jclass clazz , const char *name, const char *sig, ...);

jboolean jni_callBoolMethodById(JNIEnv* env, jobject obj,jmethodID methodID,va_list args);

jboolean jni_callBoolMethodByName(JNIEnv *env, jobject clazz, const char *name,
                                       const char *sig, ...);

jint jni_getStaticFieldRetIntByName(JNIEnv *env, const char *clsname, const char *name, const char* sig);

jint jni_callIntMethodById(JNIEnv* env, jobject obj,jmethodID methodID,va_list args);

jint jni_callIntMethodByName(JNIEnv *env, jobject obj, const char *name, const char *sig, ...);

jobject jni_newObject(JNIEnv *env, const char *clsname,  const char *sig, ...);


int jni_getOsVersion();

JNIEnv *jni_getEnv();

bool jni_checkException(JNIEnv *env);

jobject jni_toGlobalRef(JNIEnv *env, jobject obj);

void jni_deleteObj(JNIEnv *env, jobject obj);

string jni_jstring2string(JNIEnv *env, const jstring in);

jstring jni_string2jstring(JNIEnv *env, const string &in);

void jni_jbyteArray2cbytes(JNIEnv *env, const jbyteArray obj, uint8_t **cbytes, uint32_t *psize);

string jni_jbyteArray2string(JNIEnv *env, const jbyteArray in);




class JNIUtils_AndroidRuntime {

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




class JNIUtils_JNIEnvGuard {

private:
    JavaVM *jvm_;
    JNIEnv *jni_;
    bool attach_;

public:
    JNIUtils_JNIEnvGuard() : jvm_(JNIUtils_AndroidRuntime::getJavaVM()), jni_(JNIUtils_AndroidRuntime::getJNIEnv()),
                    attach_(false) {
        if (jvm_ == NULL) {
            LOGE("guard The current process isn't a jvm process.");
            exit(-1);
        }

        if (jni_ == NULL) {
            jvm_->AttachCurrentThread(&jni_, NULL);

            if (jni_ != NULL) {
                attach_ = true;
            }
        }
    }

    ~JNIUtils_JNIEnvGuard() {
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