//
// Created by amlloc on 18-7-11.
//

#include <jni.h>
#include <cstring>
#include "jni_comm_utils.h"
#include "jni_utils.h"
#include "sim_utils.h"
#include "permission_utils.h"

using namespace std;

/*
 * Context sig: android/content/Context
 * NetworkInfo sig: android/net/NetworkInfo
 */

jboolean isSupportTelePhony(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jobject jpackageManager = anti_jni_callMethodByName(env,
                                                        jobjCxt,
                                                        "getPackageManager",
                                                        "()Landroid/content/pm/PackageManager;");

    jstring jparam = env->NewStringUTF("android.hardware.telephony");
    jboolean jret = anti_jni_callBoolMethodByName(env,
                                                  jpackageManager,
                                                  "hasSystemFeature",
                                                  "(Ljava/lang/String;)Z",
                                                  jparam);

    anti_jni_delete_ref(env, jpackageManager);
    anti_jni_delete_ref(env, jparam);

    return jret;
}

jobject invokeTelephonyMethodRetObj(const char *method, int param, const char *sig){

    if(!isSupportTelePhony())
    {
        return nullptr;
    }
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jstring jstr = env->NewStringUTF("phone");
    jobject jtelephonyManager = anti_jni_callMethodByName(env,
                                                          jobjCxt,
                                                          "getSystemService",
                                                          "(Ljava/lang/String;)Ljava/lang/Object;",
                                                          jstr);
    jobject jobjRet = anti_jni_callMethodByName(env, jtelephonyManager, method, sig, param);

    anti_jni_delete_ref(env, jtelephonyManager);
    anti_jni_delete_ref(env, jstr);

    return jobjRet;
}

jobject invokeTelephonyMethodRetObj(const char *method, const char *sig){
    if(!isSupportTelePhony())
    {
        return nullptr;
    }

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jstring jstr = env->NewStringUTF("phone");
    jobject jtelephonyManager = anti_jni_callMethodByName(env,
                                                          jobjCxt,
                                                          "getSystemService",
                                                          "(Ljava/lang/String;)Ljava/lang/Object;",
                                                          jstr);

    jobject jobjRet = anti_jni_callMethodByName(env, jtelephonyManager, method, sig);

    anti_jni_delete_ref(env, jstr);
    anti_jni_delete_ref(env, jtelephonyManager);

    return jobjRet;
}

jint invokeTelephonyMethodRetInt(const char *method, int param, const char *sig){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jstring jstr = env->NewStringUTF("phone");
    jobject jtelephonyManager = anti_jni_callMethodByName(env,
                                                          jobjCxt,
                                                          "getSystemService",
                                                          "(Ljava/lang/String;)Ljava/lang/Object;",
                                                          jstr);

    jint jret = anti_jni_callIntMethodByName(env, jtelephonyManager, method, sig, param);

    anti_jni_delete_ref(env, jstr);
    anti_jni_delete_ref(env, jtelephonyManager);

    return jret;
}

jint invokeTelephonyMethodRetInt(const char *method, const char *sig){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jstring jstr = env->NewStringUTF("phone");
    jobject jtelephonyManager = anti_jni_callMethodByName(env,
                                                          jobjCxt,
                                                          "getSystemService",
                                                          "(Ljava/lang/String;)Ljava/lang/Object;",
                                                          jstr);
    jclass jclaxx = anti_jni_findClassByObj(env, jtelephonyManager);
    jmethodID methodid = env->GetMethodID(jclaxx, method, sig);

    jint jret = env->CallIntMethod(jtelephonyManager, methodid);

    anti_jni_delete_ref(env, jstr);
    anti_jni_delete_ref(env, jtelephonyManager);
    anti_jni_delete_ref(env, jclaxx);

    return jret;
}


/* only be used in Android 5.0 +
 * It is unnecessary to implements jni code accounding to Java
 * because jni is unnecessary to import the SubsrciotionManager class
*/
jobject invokeSubscriptionManagerMethod(const char* method, int param, const char *sig){
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjCxt = anti_jni_getGlobalContext(env);
    jobject jobjRet = nullptr;

//    jclass jsubscriptionManagerClass =  env->FindClass("android/telephony/SubscriptionManager");
//    jmethodID getConstructorMethodId = env->GetMethodID(jsubscriptionManagerClass, "getConstructor", "Ljava/lang/Class<T>;");
//    jclass jcontestClass = env->FindClass("android/content/Context");

    jstring jstr = env->NewStringUTF("telephony_subscription_service");
    jobject jsubscriptionManager = anti_jni_callMethodByName(env,
                                                             jobjCxt,
                                                             "getSystemService",
                                                             "(Ljava/lang/String;)Ljava/lang/Object;",
                                                             jstr);
    jobjRet = anti_jni_callMethodByName(env, jsubscriptionManager, method, sig, param);

    anti_jni_delete_ref(env, jstr);
    anti_jni_delete_ref(env, jsubscriptionManager);

    return jobjRet;
}

jobject invokeStaticSubscriptionManagerMethod(const char* method, int param, const char *sig){
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jobjRet = nullptr;

    jclass jSubscriptionManagerClxx = env->FindClass("android/telephony/SubscriptionManager");

    jobjRet = anti_jni_callStaticMethodReturnObj(env, jSubscriptionManagerClxx, method, sig, param);

    anti_jni_delete_ref(env, jSubscriptionManagerClxx);

    return jobjRet;
}



jboolean getTelephonyStateBySloIdx(int slotIdx){
    jboolean jisReady = JNI_FALSE;

    jint jsimState;
    //android.os.Build
    jint jversion_sdk_int = anti_jni_get_sdk_int();

    if(jversion_sdk_int < 21){

        jsimState = (slotIdx == 0) ? invokeTelephonyMethodRetInt("getSimState", "()I"): SIM_STATE_UNKNOWN;

    } else if(jversion_sdk_int < 26){

        jsimState = invokeTelephonyMethodRetInt("getSimState", slotIdx, "(I)I");

    } else{

        jsimState = invokeTelephonyMethodRetInt("getSimState", "()I");

    }

    if(SIM_STATE_ABSENT != jsimState  &&  SIM_STATE_UNKNOWN != jsimState) jisReady = JNI_TRUE;

    return jisReady;
}

jint getSubidBySlotId(int slotIdx){
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject jobjRet = nullptr;

    jobjRet = invokeStaticSubscriptionManagerMethod("getSubId", slotIdx, "(I)[I");

    jintArray jintArrayRet = (jintArray)jobjRet;
    //jsize size = env->GetArrayLength(jintArrayRet);
    jint* jpIntRet = env->GetIntArrayElements(jintArrayRet, JNI_FALSE);


    if (anti_jni_CheckException(env)){
        anti_err_log("change LongArray to elements failed");
    }

    jlong jlongRet = (jlong)jpIntRet[0];
    //release jint pointer
    //todo : how to release the Array and pointer in the right way?
    env->ReleaseIntArrayElements(jintArrayRet, jpIntRet, JNI_ABORT);
    anti_jni_delete_ref(env, jintArrayRet);
    return jlongRet;
}

jstring getSubscriberid(int slotIdx){
    jint jversion_sdk_int = anti_jni_get_sdk_int();
    jstring jsubcriberid = nullptr;

    if (!checkPermissions(READ_PHONE_STATE)) return nullptr;

    if(getTelephonyStateBySloIdx(slotIdx)){
        if (jversion_sdk_int < 21)
            jsubcriberid = (jstring)invokeTelephonyMethodRetObj("getSubscriberId",
                                                                "()Ljava/lang/String;");
        else
            jsubcriberid = (jstring)invokeTelephonyMethodRetObj("getSubscriberId",
                                                                (int)getSubidBySlotId(slotIdx),
                                                                "(I)Ljava/lang/String;");
    }
    return jsubcriberid;

}

jstring getDeviceId(int slotIdx){
    if (!checkPermissions(READ_PHONE_STATE)) return nullptr;


    jstring jdeviceId;
    jint jversion_sdk_int = anti_jni_get_sdk_int();

    if (jversion_sdk_int < 21){

        jdeviceId = (slotIdx == 0) ? (jstring) invokeTelephonyMethodRetObj("getDeviceId", "()Ljava/lang/String;") : nullptr;

    } else if(jversion_sdk_int  < 26){

        jdeviceId = (jstring)invokeTelephonyMethodRetObj("getDeviceId",
                                                         slotIdx,
                                                         "(I)Ljava/lang/String;");

    } else{

        jdeviceId = (jstring)invokeTelephonyMethodRetObj("getImei",
                                                         slotIdx,
                                                         "(I)Ljava/lang/String;");
    }
    return jdeviceId;
}

jstring getSimSerialNumber(int slotIdx){

    if (!checkPermissions(READ_PHONE_STATE)) {
        return nullptr;
    }
    jstring jsimSerialNumber = nullptr;
    jint jversion_sdk_int = anti_jni_get_sdk_int();

    if(getTelephonyStateBySloIdx(slotIdx)){

        jsimSerialNumber = (jversion_sdk_int  < 21) ?
                           (jstring) invokeTelephonyMethodRetObj("getSimSerialNumber",
                                                                 "()Ljava/lang/String;") :
                           (jstring) invokeTelephonyMethodRetObj("getSimSerialNumber",
                                                                 getSubidBySlotId(slotIdx),
                                                                 "(I)Ljava/lang/String;");

    }

    return jsimSerialNumber;
}

jstring getPhoneNumber(){

    if(!isSupportTelePhony()
       || !checkPermissions(ACCESS_NETWORK_STATE)
       || !checkPermissions( READ_SMS)
       ||!checkPermissions(READ_PHONE_STATE))
    {return nullptr;}

    jobject jobjRet = invokeTelephonyMethodRetObj("getLine1Number", "()Ljava/lang/String;");
    return (jstring)jobjRet;

}

jstring getVoiceMailNumber(){

    if (!isSupportTelePhony() || !checkPermissions(READ_PHONE_STATE)) {return nullptr;}

    jstring jvoiceMailNumber = (jstring)invokeTelephonyMethodRetObj("getVoiceMailNumber",
                                                                    "()Ljava/lang/String;");
    return jvoiceMailNumber;

}
jint getSimCardCount(){
    jint jphoneCount = -1;
    jint jversion_sdk_int = anti_jni_get_sdk_int();

    if (jversion_sdk_int >= 21){
        jphoneCount = invokeTelephonyMethodRetInt("getPhoneCount", "()I");
    }
    else{
        anti_err_log("The SystemVersion is too low to check this device's SIM mode!");
    }

    return jphoneCount;
}


jint getNetworkClassByType(int networkType){
    switch (networkType){
        case NETWORK_TYPE_UNAVAILABLE:
            return NETWORK_CLASS_UNAVAILABLE;
        case NETWORK_TYPE_WIFI:
            return NETWORK_CLASS_WIFI;
        case NETWORK_TYPE_GPRS:
        case NETWORK_TYPE_EDGE:
        case NETWORK_TYPE_CDMA:
        case NETWORK_TYPE_1xRTT:
        case NETWORK_TYPE_IDEN:
            return NETWORK_CLASS_2_G;
        case NETWORK_TYPE_UMTS:
        case NETWORK_TYPE_EVDO_0:
        case NETWORK_TYPE_EVDO_A:
        case NETWORK_TYPE_HSDPA:
        case NETWORK_TYPE_HSUPA:
        case NETWORK_TYPE_HSPA:
        case NETWORK_TYPE_EVDO_B:
        case NETWORK_TYPE_EHRPD:
        case NETWORK_TYPE_HSPAP:
            return NETWORK_CLASS_3_G;
        case NETWORK_TYPE_LTE:
            return NETWORK_CLASS_4_G;
        default:
            return NETWORK_CLASS_UNKNOWN;
    }
}

jint getNetworkClass(int slotId){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jcontext = anti_jni_getGlobalContext(env);
    jint jversion_sdk_int = anti_jni_get_sdk_int();

    if(!getTelephonyStateBySloIdx(slotId)) {return NETWORK_CLASS_UNAVAILABLE;}

    jint jnetworkType = NETWORK_TYPE_UNKNOWN;

    if (!checkPermissions(ACCESS_NETWORK_STATE)) {return NETWORK_TYPE_UNKNOWN;}


    jobject japplicationContext = anti_jni_callMethodByName(env,
                                                            jcontext,
                                                            "getApplicationContext",
                                                            "()Landroid/content/Context;");
    jstring jparam = env->NewStringUTF("connectivity");
    jobject jconnectivityService = anti_jni_callMethodByName(env, japplicationContext,
                                                             "getSystemService",
                                                             "(Landroid/content/Context;)Ljava/lang/Object;",
                                                             jparam);
    jobject jnetwork = anti_jni_callMethodByName(env,
                                                 jconnectivityService,
                                                 "getActiveNetworkInfo",
                                                 "()Landroid/net/NetworkInfo;");

    jboolean jnwIsAvailable = anti_jni_callBoolMethodByName(env,
                                                            jnetwork,
                                                            "isAvailable",
                                                            "()B");
    jboolean jnwIsConnected = anti_jni_callBoolMethodByName(env,
                                                            jnetwork,
                                                            "isConnected",
                                                            "()B");

    if (jnetwork != nullptr && jnwIsAvailable && jnwIsConnected){
        jint jtype = anti_jni_callIntMethodByName(env,
                                                  jnetwork,
                                                  "getType",
                                                  "()I");

        if (jtype == 1){
            jnetworkType = NETWORK_TYPE_WIFI;
        }
        else if(jtype == 0){

            if (jversion_sdk_int >= 21){
                jnetworkType = invokeTelephonyMethodRetInt("getDataNetworkType",
                                                           (int)getSubidBySlotId(slotId),
                                                           "(I)I" );
            } else{
                jnetworkType = invokeTelephonyMethodRetInt("getNetworkType",
                                                           "()I");
            }

        }
    } else{
        jnetworkType =  -1;
    }

    anti_jni_delete_ref(env, jparam);
    anti_jni_delete_ref(env,jconnectivityService);
    anti_jni_delete_ref(env, jnetwork);
    return getNetworkClassByType(jnetworkType);
}
jstring getCurrentNetworkType(int slotId){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    if(checkPermissions(ACCESS_NETWORK_STATE)){
        anti_err_log("ACCESS_NETWORK_STATE deny");
        return nullptr;
    }

    jint networkClass = getNetworkClass(slotId);

    string type = "unknown";
    switch (networkClass){
        case NETWORK_CLASS_UNAVAILABLE:
            type = "unavailable";
            break;
        case NETWORK_CLASS_WIFI:
            type = "Wi-Fi";
            break;
        case NETWORK_CLASS_2_G:
            type = "2G";
            break;
        case NETWORK_CLASS_3_G:
            type = "3G";
            break;
        case NETWORK_CLASS_4_G:
            type = "4G";
            break;
            //不明网络类型
        case NETWORK_CLASS_UNKNOWN:
            type = "unknown";
            break;
    }
    return env->NewStringUTF(type.c_str());
}