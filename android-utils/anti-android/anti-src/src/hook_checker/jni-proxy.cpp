
#include "include/jni-proxy.h"
#include "include/jni-mock-log.h"
#include "jni-proxy-cpp.inc"
#include <stdio.h>
#include "dx_anti_stee.h"

extern const struct JNIInvokeInterface gWrapperJNIInvokeInterface;

JniEnvProxy::JniEnvProxy(JavaVMProxy *owner, JniEnvChain *next) : JniEnvChain(next), env0((const struct JNINativeInterface *)&gWrapperJniNativeInterface, this, owner) {}

JavaVMProxy::JavaVMProxy() : vm0(&gWrapperJNIInvokeInterface, 0, this) {
    pthread_key_create(&env_key, [](void *a) -> void {
        JniEnvProxy *e = (JniEnvProxy *)a;
        if (e) {
            delete e;
        }
    });
}

STEE
JNIEnv *JavaVMProxy::proxy(JNIEnv *env) {

    JniEnvProxy *s = (JniEnvProxy *)pthread_getspecific(env_key);
    if (!s) {
        JniEnvChainDefault *def = new JniEnvChainDefault(env);
        s = new JniEnvProxy(this, chain(def));
        s->def = def;
        pthread_setspecific(env_key, s);
    }
    s->def->SetEnv(env);

    return s->GetProxyedEnv();
}




STEE
static jint wrapper_DestroyJavaVM(JavaVM *vm) { return ((JavaVMProxy::VM0 *)vm)->next->DestroyJavaVM(); }
STEE
static jint wrapper_AttachCurrentThread(JavaVM *vm, JNIEnv **p_env, void *thr_args) {
    auto result = ((JavaVMProxy::VM0 *)vm)->next->AttachCurrentThread(p_env, thr_args);
    if (result == JNI_OK) {
        *p_env = ((JavaVMProxy::VM0 *)vm)->owner->proxy(*p_env);
    }
    return result;
}
STEE
static jint wrapper_DetachCurrentThread(JavaVM *vm) { return ((JavaVMProxy::VM0 *)vm)->next->DetachCurrentThread(); }
STEE
static jint wrapper_GetEnv(JavaVM *vm, void **p_env, jint version) {
    auto result = ((JavaVMProxy::VM0 *)vm)->next->GetEnv(p_env, version);
    if (result == JNI_OK) {
        *p_env = ((JavaVMProxy::VM0 *)vm)->owner->proxy((JNIEnv *)*p_env);
    }
    return result;
}
STEE
static jint wrapper_AttachCurrentThreadAsDaemon(JavaVM *vm, JNIEnv **p_env, void *thr_args) {

    auto result = ((JavaVMProxy::VM0 *)vm)->next->AttachCurrentThreadAsDaemon(p_env, thr_args);
    if (result == JNI_OK) {
        *p_env = ((JavaVMProxy::VM0 *)vm)->owner->proxy(*p_env);
    }
    return result;
}

const struct JNIInvokeInterface gWrapperJNIInvokeInterface = {
    NULL, NULL, NULL, wrapper_DestroyJavaVM, wrapper_AttachCurrentThread, wrapper_DetachCurrentThread, wrapper_GetEnv, wrapper_AttachCurrentThreadAsDaemon};
