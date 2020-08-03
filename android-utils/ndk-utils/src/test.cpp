#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/types.h>
#include "log.h"
#include "jni_utils.h"

#define STRING "Ljava/lang/String;"
#define TEST_CLASS "com/gamesecdemo/test/JniUtilsTest"


void* test_staticJni(void *args) {
    LOGD("[test_staticJni] start tid:%d", gettid());

    JniUtils jni;

    jstring js_input = jni.String2jstring("str from jni");

    // set static field
    jni.SetStaticObjectField(TEST_CLASS, "sStr", STRING, js_input);
    jni.SetStaticIntField(TEST_CLASS, "sInt", "I", 9999);
    jni.SetStaticBooleanField(TEST_CLASS, "sBool", "Z", true);
    jni.SetStaticFloatField(TEST_CLASS, "sFloat", "F", 99.99F);
    jni.SetStaticDoubleField(TEST_CLASS, "sDouble", "D", 88.88F);
    LOGD("[setStaticField] success");

    // get static field
    jstring js_sStr = (jstring)jni.GetStaticObjectField(TEST_CLASS, "sStr", STRING);
    LOGD("[getStaticField] js_sStr:%s", jni.Jstring2string(js_sStr).c_str());
    jni.DeleteRef(js_sStr);

    jint j_sInt = jni.GetStaticIntField(TEST_CLASS, "sInt", "I");
    LOGD("[getStaticField] j_sInt:%d", j_sInt);

    jboolean j_sBool = jni.GetStaticBooleanField(TEST_CLASS, "sBool", "Z");
    LOGD("[getStaticField] j_sBool:%d", j_sBool);

    jfloat j_sFloat = jni.GetStaticFloatField(TEST_CLASS, "sFloat", "F");
    LOGD("[getStaticField] j_sFloat:%2.2f", j_sFloat);

    jdouble j_sDouble = jni.GetStaticDoubleField(TEST_CLASS, "sDouble", "D");
    LOGD("[getStaticField] j_sDouble:%2.2f", j_sDouble);

    // call statc method
    jstring js_input1 = jni.String2jstring("setStaticStr by jni");
    jni.CallStaticVoidMethod(TEST_CLASS, "setStaticStr", "(" STRING ")V", js_input1);
    jni.DeleteRef(js_input1);
    jstring j_callStr = (jstring)jni.CallStaticObjectMethod(TEST_CLASS, "getStaticStr", "()" STRING);
    LOGD("[CallStaticMeth] j_callStr:%s", jni.Jstring2string(j_callStr).c_str());
    jni.DeleteRef(j_callStr);

    jint j_callInt = jni.CallStaticIntMethod(TEST_CLASS, "getStaticInt", "()I");
    LOGD("[CallStaticMeth] j_sInt:%d", j_callInt);

    jboolean j_callBool = jni.CallStaticBooleanMethod(TEST_CLASS, "getStaticBoolean", "()Z");
    LOGD("[CallStaticMeth] j_callBool:%d", j_callBool);

    jfloat j_callFloat = jni.CallStaticFloatMethod(TEST_CLASS, "getStaticFloat", "()F");
    LOGD("[CallStaticMeth] j_callFloat:%2.2f", j_callFloat);

    jdouble j_callDouble = jni.CallStaticDoubleMethod(TEST_CLASS, "getStaticDouble", "()D");
    LOGD("[CallStaticMeth] j_callDouble:%2.2f", j_callDouble);

    jni.DeleteRef(js_input);
    LOGD("[test_staticJni] end");

    return NULL;
}

void* test_Jni(void *args) {
    LOGD("[test_Jni] start tid:%d", gettid());

    JniUtils jni;

    jstring js_input = jni.String2jstring("str from jni");

    // getInstance
    jobject jo_instance = jni.CallStaticObjectMethod(TEST_CLASS, "getInstance", "()L" TEST_CLASS ";");
    LOGD("[CallStaticMeth] jo_instance:%p", jo_instance);

    // set field
    jni.SetObjectField(jo_instance, "mStr", STRING, js_input);
    jni.SetIntField(jo_instance, "mInt", "I", 9999);
    jni.SetBooleanField(jo_instance, "mBool", "Z", true);
    jni.SetFloatField(jo_instance, "mFloat", "F", 99.99F);
    jni.SetDoubleField(jo_instance, "mDouble", "D", 88.88F);
    LOGD("[setField] success");

    // get field
    jstring js_mStr = (jstring)jni.GetObjectField(jo_instance, "mStr", STRING);
    LOGD("[getField] js_mStr:%s", jni.Jstring2string(js_mStr).c_str());
    jni.DeleteRef(js_mStr);

    jint j_mInt = jni.GetIntField(jo_instance, "mInt", "I");
    LOGD("[getField] j_mInt:%d", j_mInt);

    jboolean j_mBool = jni.GetBooleanField(jo_instance, "mBool", "Z");
    LOGD("[getField] j_mBool:%d", j_mBool);

    jfloat j_mFloat = jni.GetFloatField(jo_instance, "mFloat", "F");
    LOGD("[getField] j_mFloat:%2.2f", j_mFloat);

    jdouble j_mDouble = jni.GetDoubleField(jo_instance, "mDouble", "D");
    LOGD("[getField] j_mDouble:%2.2f", j_mDouble);

    // call method
    jstring js_input1 = jni.String2jstring("setStaticStr by jni");
    jni.CallVoidMethod(jo_instance, "setStr", "(" STRING ")V", js_input1);
    jni.DeleteRef(js_input1);
    jstring j_callmStr = (jstring)jni.CallObjectMethod(jo_instance, "getStr", "()" STRING);
    LOGD("[CallMeth] j_callmStr:%s", jni.Jstring2string(j_callmStr).c_str());
    jni.DeleteRef(j_callmStr);

    jint j_callmInt = jni.CallIntMethod(jo_instance, "getInt", "()I");
    LOGD("[CallMeth] j_mInt:%d", j_callmInt);

    jboolean j_callmBool = jni.CallBooleanMethod(jo_instance, "getBoolean", "()Z");
    LOGD("[CallMeth] j_callmBool:%d", j_callmBool);

    jfloat j_callmFloat = jni.CallFloatMethod(jo_instance, "getFloat", "()F");
    LOGD("[CallMeth] j_callmFloat:%2.2f", j_callmFloat);

    jdouble j_callmDouble = jni.CallDoubleMethod(jo_instance, "getDouble", "()D");
    LOGD("[CallMeth] j_callmDouble:%2.2f", j_callmDouble);


    jni.DeleteRef(js_input);
    LOGD("[test_Jni] end");
    return NULL;
}

void test_by_thread() {
    pthread_t pt;
    pthread_create(&pt, NULL, test_staticJni, NULL);
    pthread_detach(pt);


    pthread_t pt1;
    pthread_create(&pt1, NULL, test_Jni, NULL);
    pthread_detach(pt1);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnLoad =======start");
    jint result = -1;

    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    JniUtils::Init(vm, TEST_CLASS);

    // java线程测试
    test_staticJni(NULL);
    test_Jni(NULL);

    // native线程测试
    test_by_thread();

    LOGD("JNI_OnLoad =======end");
    return JNI_VERSION_1_4;
}