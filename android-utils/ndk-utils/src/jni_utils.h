//
// Created by white on 2020/7/24.
//


#include <jni.h>
#include <string>
#include "log.h"

using std::string;
using namespace std;


class JniUtils {

private:
    static JavaVM *m_jvm;
JNIEnv *m_env;
    bool m_attach;

public:

    jobject NewGlobalRef(jobject obj);
    void DeleteRef(jobject obj);
    string Jstring2string(const jstring in);
    jstring String2jstring(const string &in);
    void JbyteArray2cbytes(const jbyteArray obj, uint8_t **cbytes, uint32_t *psize);
    string JbyteArray2string(const jbyteArray in);
    jobjectArray StrArray2objectArray(string strArray[], int len);

    static void Init(JavaVM* jvm) {
        m_jvm = jvm;
    }

    JniUtils() : m_env(NULL),
                 m_attach(false) {
        if (m_jvm == NULL) {
            LOGE("JniUtils The current process isn't a jvm process.");
            return;
        }

        m_jvm->GetEnv((void **) &m_env, JNI_VERSION_1_4);

        if (m_env == NULL) {
            m_jvm->AttachCurrentThread(&m_env, NULL);

            if (m_env != NULL) {
                m_attach = true;
            }
        }
    }

     ~JniUtils() {
        if (m_jvm && m_env && m_attach) {
            m_jvm->DetachCurrentThread();
            m_env = NULL;
            m_jvm = NULL;
        }
    }


private:
    bool CheckException();

   
};