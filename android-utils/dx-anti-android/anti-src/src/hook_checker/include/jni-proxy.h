#ifndef _JNI_PROXY_H_
#define _JNI_PROXY_H_

#include "env-chain-default.h"
#include "env-chain.h"
#include <pthread.h>

class JavaVMProxy;
class JniEnvProxy : public JniEnvChain {
  public:
    struct Env0 {
        const struct JNINativeInterface *calltable; // reserved for jni call table, this must be the first one
        JniEnvChain *next;
        JavaVMProxy *owner;
        Env0(const struct JNINativeInterface *calltable, JniEnvChain *next, JavaVMProxy *owner) : calltable(calltable), next(next), owner(owner){};
    };
    const Env0 env0;
    JniEnvChainDefault *def;

    JNIEnv *GetProxyedEnv() { return (JNIEnv *)&env0; }

    JniEnvProxy(JavaVMProxy *owner, JniEnvChain *next);
};




class JavaVMProxy {

  private:
    pthread_key_t env_key;

  public:
    struct VM0 {
        const struct JNIInvokeInterface *const calltable;
        JavaVM *next;
        JavaVMProxy *const owner;
        VM0(const struct JNIInvokeInterface *ct, JavaVM *next, JavaVMProxy *owner) : calltable(ct), next(next), owner(owner) {}
    };

  private:
    VM0 vm0;

  public:
    void SetVM(JavaVM *next) {
        if (!next) {
            abort();
        }
        this->vm0.next = next;
    }
    JavaVMProxy();
    JavaVM *GetProxyedVM() { return (JavaVM *)&vm0; }
    virtual JniEnvChain *chain(JniEnvChain *next) { return next; };

    JNIEnv *proxy(JNIEnv *env);
};
JniEnvChain *newTraceJniEnv(JniEnvChain *next);

#endif
