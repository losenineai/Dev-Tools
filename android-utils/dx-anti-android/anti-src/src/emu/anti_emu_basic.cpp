//
// Created by amlloc on 18-7-11.
//

//#include <bits/strcasecmp.h>
#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include "anti_utils.h"

#include "jni_comm_utils.h"
#include "jni_utils.h"
#include "sim_utils.h"
#include "permission_utils.h"
#include "string_utils.h"
#include "dx_anti_stee.h"
#include "dx-libc/dx_libc.h"
/*
 * java.lang.ProcessBuild
 * java.lang.Process
 * java.io.File
 * java.io.InputStream
 * android.content.pm.PackageManager
 *
 * android.content.IntentFilter
 * android.content.Intent
 *
 * android.content.BroadcastReceiver
 */
STEE
bool anti_emu_basic_checkPhoneNumber(){

    string know_numbers[] = {
            "15555215554", "15555215556", "15555215558", "15555215560",
            "15555215562", "15555215564", "15555215566", "15555215568",
            "15555215570", "15555215572", "15555215574", "15555215576",
            "15555215578", "15555215580", "15555215582", "15555215584",
    };

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jstring jphoneNumber = getPhoneNumber();
    if (jphoneNumber == nullptr){
        anti_info_log("get phone number failed 1");
        return false;
    }
    string phoneNumber = anti_jni_jstring_to_string(env, jphoneNumber);
    if (phoneNumber == ""){
        anti_info_log("get phone number failed 2");
        anti_jni_delete_ref(env, jphoneNumber);
        return false;
    }

    for (string know_number :know_numbers)
    {
        if (phoneNumber.find(know_number) != string::npos){
            anti_info_log("check emulator phone number : %s", phoneNumber.c_str());
            anti_jni_delete_ref(env, jphoneNumber);
            return true;
        }
    }
    anti_jni_delete_ref(env, jphoneNumber);
    anti_info_log("this phone number is not emulator");
    return false;
}

STEE
bool anti_emu_basic_checkDeviceId(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    bool result = false;
    string known_device_ids[] = {
            "000000000000000", "012345678912345", "e21833235b6eef10"
    };
    jstring jimei = getDeviceId(0);
    string imei = anti_jni_jstring_to_string(env, jimei);

    for (string known_device_id : known_device_ids){
        if (imei.find(known_device_id) != string::npos){
            result = true;
            break;
        }
    }

    anti_jni_delete_ref(env, jimei);
    return result;
}

STEE
bool anti_emu_basic_checkImsi(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    string known_imsi_ids[] = {
            "310260000000000",
            "460072489419419"
    };

    jstring jimsi = getSubscriberid(0);
    string imsi = anti_jni_jstring_to_string(env, jimsi);
    anti_info_log("check imsi : %s", imsi.c_str());
    for(string known_imsi_id : known_imsi_ids){
        anti_info_log("known imsi id %s : ", known_imsi_id.c_str());
        if (imsi.find(known_imsi_id) != string::npos){
            return true;
        }
    }

    anti_jni_delete_ref(env, jimsi);
    return false;
}
//todo: some times can't get voiceMail
STEE
bool anti_emu_basic_checkVoiceMail(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    string known_voice_mail_numbers[] = {
            "15552175049"
    };

    jstring jvoiceNumber = getVoiceMailNumber();
    string voiceNumber = anti_jni_jstring_to_string(env, jvoiceNumber);
    anti_info_log("voiceNumber : %s", voiceNumber.c_str());
    for (string know_voice : known_voice_mail_numbers) {
        if (voiceNumber.find(know_voice) != string::npos) {
            anti_info_log("known imsi id %s : ", know_voice.c_str());
            return true;
        }
    }
    anti_jni_delete_ref(env, jvoiceNumber);
    return false;
}

STEE
bool anti_emu_basic_checkOperatorNameAndroid(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jstring joperatorName = (jstring)invokeTelephonyMethodRetObj("getNetworkOperatorName", "()Ljava/lang/String;");
    string operatorName = anti_jni_jstring_to_string(env, joperatorName);
    string strAndroid = "android";
//    anti_info_log("operatorName : ", operatorName.c_str());
    anti_jni_delete_ref(env, joperatorName);
    return strcasecmp(operatorName.c_str(), strAndroid.c_str()) == 0;

}

STEE
bool anti_emu_basic_checkIp(){

    string IP = "10.0.2.15";

    bool checkIp = false;

    if (checkPermissions("android.permission.INTERNET")){

        // FILE *pfile = NULL;
        char buf[0x1000] = {0};
        string strbuf = "";
        bool ret = false;

        const char *strRunNetcfg = "/system/bin/netcfg";
        // pfile = fopen(strRunNetcfg, "r");
        int fd = open(strRunNetcfg, O_RDONLY);

        // if(NULL == pfile){
        //     return ret;
        // }
        if(fd <= 0){
            return ret;
        }
        // if (fgets(buf, sizeof(buf), pfile)){
        //     strbuf = strbuf.append(buf, sizeof(buf));
        // }
        if(dx_read(fd, buf, sizeof(buf))>0){
            strbuf = strbuf.append(buf, sizeof(buf));
        }

        // fclose(pfile);
        dx_close(fd);
        
        //todo: hown  to release buf
        vector<string> lans;
        string split = "\n";
        if (strbuf == ""){
            anti_info_log("get IP Failed");
            return false;
        }
        anti_str_splitString(strbuf, lans, split);

        for(string lan : lans){

            if ((lan.find("wlan0") != string::npos
                 || lan.find("tunl0") != string::npos
                 || lan.find("eth0") != string::npos)
                 && (lan.find("IP") != string::npos)){
                checkIp = true;
                break;
            }
        }
    }

    anti_info_log("checkIp : %d", checkIp);
    return checkIp;
}


STEE
bool anti_emu_basic_checkEth0(){
    /*
     * Enumeration<E> sig : java.util.Enumeration; .hasMoreElements : ()Z  .nextElement : ()E
     * NetworkInterface sig : java.net.NetworkInterface; .getNetworkInterfaces : ()Enumeration<NetworkInterface>
     *
     */
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    bool checkEth0 = false;

    jclass jclxNetworkInterface = anti_jni_findClassByName(env, "java/net/NetworkInterface");
    jobject jnwInfEnt = anti_jni_callStaticMethodReturnObj(env, jclxNetworkInterface, "getNetworkInterfaces", "()Ljava/util/Enumeration;");
//    jstring jnwName = nullptr;
//    jobject jintf = nullptr;

    while (true){

        bool has = anti_jni_callBoolMethodByName(env, jnwInfEnt, "hasMoreElements", "()Z") == JNI_TRUE;

        if (has == false) {
            anti_info_log("has : false");
            break;
        }
        jobject jintf = anti_jni_callMethodByName(env, jnwInfEnt, "nextElement", "()Ljava/lang/Object;");
        jstring jnwName = (jstring)anti_jni_callMethodByName(env, jintf, "getName", "()Ljava/lang/String;");
        string nwName = anti_jni_jstring_to_string(env, jnwName);

        if (nwName == string("eth0")){
            checkEth0 = true;
            anti_jni_delete_ref(env, jintf);
            anti_jni_delete_ref(env, jnwName);
            break;
        }
        anti_jni_delete_ref(env, jintf);
        anti_jni_delete_ref(env, jnwName);

    }

    anti_jni_delete_ref(env, jclxNetworkInterface);
    anti_jni_delete_ref(env, jnwInfEnt);
    anti_info_log("checkEth0 : %d", checkEth0);
    return checkEth0;
}


STEE
bool anti_emu_basic_checkBuildInfo(){
    /*
     * function requires:
     * string: startWith, toLowerCase, contains,
    */
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jstring jfingerprint = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "FINGERPRINT", "Ljava/lang/String;");
    jstring jmodel = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "MODEL", "Ljava/lang/String;");
    jstring jmanufacturer = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "MANUFACTURER", "Ljava/lang/String;");
    jstring jhardware = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "HARDWARE", "Ljava/lang/String;");
    jstring jproduct = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "PRODUCT", "Ljava/lang/String;");
    jstring jboard = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "BOARD", "Ljava/lang/String;");
    jstring jbootloader = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "BOOTLOADER", "Ljava/lang/String;");
    jstring jserial = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "SERIAL", "Ljava/lang/String;");
    jstring jbrand = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "BRAND", "Ljava/lang/String;");
    jstring jdevice = (jstring)anti_jni_getStaticObjectField(env, "android/os/Build", "DEVICE", "Ljava/lang/String;");

    string fingerprint = anti_jni_jstring_to_string(env, jfingerprint);

    string model = anti_jni_jstring_to_string(env, jmodel);
    string lowModel = anti_jni_jstring_to_string(env, jmodel);
    std::transform(lowModel.begin(), lowModel.end(), lowModel.begin(), ::tolower);

    string manufacturer = anti_jni_jstring_to_string(env, jmanufacturer);
    string hardware = anti_jni_jstring_to_string(env, jhardware);
    string product = anti_jni_jstring_to_string(env, jproduct);

    string board = anti_jni_jstring_to_string(env, jboard);
    string lowBoard = anti_jni_jstring_to_string(env, jboard);
    transform(lowBoard.begin(), lowBoard.end(), lowBoard.begin(), ::tolower);

    string bootloader = anti_jni_jstring_to_string(env, jbootloader);
    string serial = anti_jni_jstring_to_string(env, jserial);
    string brand = anti_jni_jstring_to_string(env, jbrand);
    string device = anti_jni_jstring_to_string(env, jdevice);

    bool result = ((0 == strncmp(fingerprint.c_str(), "generic", 7))
                   || model.find("google_sdk") != string::npos
                   || lowModel.find("droid4x") != string::npos
                   || model.find("Emulator") != string::npos
                   || model.find("Android SDK built for x86") != string::npos
                   || manufacturer.find("Genymotion") != string::npos
                   || hardware.find("goldfish") != string::npos
                   || hardware.find("vbox86") != string ::npos
                   || lowBoard.find("nox") != string::npos
                   || bootloader.find("nox") != string::npos
                   || hardware.find("nox") != string::npos
                   || product.find("nox") != string::npos
                   || serial.find("nox") != string::npos
                   || product.find("redfinger") != string::npos
                   || fingerprint.find("redfinger") != string::npos
                   || brand.find("redfinger") != string::npos
                    );

    if (result) {return true;}

    result |= (0 == dx_strncmp(brand.c_str(), "generic", 7)) && (0 == dx_strncmp(device.c_str(), "generic", 7));
    if (result) return true;

    string google_sdk = "google_sdk";
    result |= (google_sdk == product);
    anti_info_log("checkBuildInfo : %d", result);
    return result;
}

STEE
bool anti_emu_basic_checkPackageName(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jcontext = anti_jni_getGlobalContext(env);

    string PKG_NAME[] = {
            "com.google.android.launcher.layouts.genymotion",
            "com.bluestacks",
            "org.appanalysis",
            "com.bignox.app"
    };

    jobject jpackageManager = anti_jni_callMethodByName(env, jcontext,
                                                        "getPackageManager",
                                                        "()Landroid/content/pm/PackageManager;");
    bool result = false;
    jstring jpkgName = nullptr;
    jobject jtryIntent = nullptr;
    jobject jresolveInfos = nullptr;

    for (string pkgName : PKG_NAME){
        jpkgName = env->NewStringUTF(pkgName.c_str());
        jresolveInfos = anti_jni_callMethodByName(env, jpackageManager,
                                               "getLaunchIntentForPackage",
                                               "(Ljava/lang/String;)Landroid/content/Intent;",
                                               jpkgName);


        if(jtryIntent != nullptr){

            jobject jresolveInfos = anti_jni_callMethodByName(env,
                                                              jpackageManager,
                                                              "queryIntentActivities",
                                                              "(Landroid/content/Intent;I)Ljava/utils/List;",
                                                              jtryIntent,
                                                              65536);// PackageManager.MATCH_DEFAULT_ONLY
            jboolean jisEmpy = anti_jni_callBoolMethodByName(env, jresolveInfos, "isEmpty", "()Z");


            if (jisEmpy == JNI_FALSE){
                LOGLOCATION
                result = true;
            }
        }
    }

    anti_jni_delete_ref(env, jpkgName);
    anti_jni_delete_ref(env, jtryIntent);
    anti_jni_delete_ref(env, jresolveInfos);
    anti_jni_delete_ref(env, jpackageManager);

    anti_info_log("anti_emu_basic_checkPackageName result : %d", result);

    return result;

}

STEE
bool anti_emu_basic_checkBattery(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jcontext = anti_jni_getGlobalContext(env);

    jstring jparam = env->NewStringUTF("android.intent.action.BATTERY_CHANGED");
    jobject jintentFilter = anti_jni_NewObject(env, "android/content/IntentFilter", "(Ljava/lang/String;)V",jparam );

    bool result = false;

    //todo:how to pass the parameter when parameter is NULL in Java ??
    jobject jbatteryStatusIntent = anti_jni_callMethodByName(env,
                                                             jcontext,
                                                             "registerReceiver",
                                                             "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;",
                                                             nullptr,
                                                             jintentFilter);

    const char* strVoltage = "voltage";
    const char* strTemperature = "temperature";
    jstring jstrVoltage = env->NewStringUTF(strVoltage);
    jstring jstrtemperature = env->NewStringUTF(strTemperature);

    jint jvoltage = anti_jni_callIntMethodByName(env, jbatteryStatusIntent, "getIntExtra", "(Ljava/lang/String;I)I", jstrVoltage, 99999);
    jint jtemperature = anti_jni_callIntMethodByName(env, jbatteryStatusIntent, "getIntExtra", "(Ljava/lang/String;I)I", jstrtemperature, 99999);

    anti_info_log("voltage : %d", jvoltage);
    anti_info_log("temperature : %d", jtemperature);

    if ((jvoltage == 0 && jtemperature == 0) || ((jvoltage == 10000) && jtemperature == 0)){

        //这是通过电池的伏数和温度来判断是真机还是模拟器
        result = true;


    }
    anti_jni_delete_ref(env, jparam);
    anti_jni_delete_ref(env, jintentFilter);
    anti_jni_delete_ref(env, jstrVoltage);
    anti_jni_delete_ref(env, jbatteryStatusIntent);
    anti_jni_delete_ref(env, jstrtemperature);

    return result;
}

STEE
bool anti_emu_basic_checkTaint(){
//todo: how is it work? and this code may be false, please fix me
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jclass jc_taint = anti_jni_findClassByName(env, "dalvik/system/Taint");
    return jc_taint != nullptr;
}

STEE
bool anti_emu_basic_checkEmuBasicAll(){
    bool result = anti_emu_basic_checkPhoneNumber() ||  //ok
                  anti_emu_basic_checkDeviceId() ||     //ok
                  anti_emu_basic_checkImsi() ||         //ok
                  anti_emu_basic_checkVoiceMail() ||
                  anti_emu_basic_checkOperatorNameAndroid() ||
                  anti_emu_basic_checkIp() ||           //ok, but needed to be tested
                  anti_emu_basic_checkEth0() ||         //todo: misjudge
                  anti_emu_basic_checkBuildInfo() ||    //ok
                  anti_emu_basic_checkPackageName() ||   //ok
                  anti_emu_basic_checkBattery() ||      //ok
                  anti_emu_basic_checkTaint();          //ok, but needed to be tested
    return result;

}

