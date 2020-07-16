//
// Created by boyliang on 2017/5/26.
//

#include <jni.h>
#include <string>
#include <map>

#include "jni_const.h"
#include "pb_prefix.h"

#include "jni_utils.h"
#include "DXPlatform.h"
#include "DXRiskInter.h"
#include "dx_env.h"
#include "storage_utils.h"

using std::string;

STEE
string sync_http_request(const int type, const string &url, const string &data, const bool isSaas, const string &contentType) {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    //convert args to java-args
    jint arg1 = type;
    jstring arg2 = string_to_jstring(env, url);
    jbyteArray arg3 = cbytes_to_jbyteArray(env, (uint8_t *) data.data(), data.size());
    jboolean arg4 = (jboolean) isSaas;
    jstring arg5 = string_to_jstring(env, contentType);

    //call the method
    string result;

    // 原型：a 类中 private static byte[] b(int type, String url, byte[] data, boolean bSaas, String contentType)
    jbyteArray java_result = (jbyteArray) DXCallStaticObjectMethodByName(env, sJNIHelper_class,
                  FUNC_NETWORK, FUNC_NETWORK_SIGN, FUNC_NETWORK_SYNE, arg1, arg2, arg3, arg4, arg5);

    if (java_result != NULL) {
        result = jbyteArray_to_string(env, java_result);

        delete_ref(env, java_result);
    }

    //delete refs
    delete_ref(env, arg2);
    delete_ref(env, arg3);
    delete_ref(env, arg5);

    return result;
}

STEE
void async_http_request(const int type, const string &url, const string &data, const bool isSaas, const string &contentType) {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    //convert args to java-args
    jint arg1 = type;
    jstring arg2 = string_to_jstring(env, url);
    jbyteArray arg3 = cbytes_to_jbyteArray(env, (uint8_t *) data.data(), data.size());
    jboolean arg4 = (jboolean) isSaas;
    jstring arg5 = string_to_jstring(env, contentType);

    // 原型：a 类中 private static void c(int type, String url, byte[] data, boolean bSaas, String contentType)
    DXCallStaticObjectMethodByName(env, sJNIHelper_class,
                                 FUNC_NETWORK, FUNC_NETWORK_SIGN, FUNC_NETWORK_ASYNE,
                                 arg1, arg2, arg3, arg4, arg5);

    //delete refs
    delete_ref(env, arg2);
    delete_ref(env, arg3);
    delete_ref(env, arg5);
}

/**
 * 获取app版本
 * @return
 */
string get_app_version() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring j_app_version = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_APP_VERSION);
    string app_version = jstring_to_string(env, j_app_version);
    delete_ref(env, j_app_version);

    return app_version;
}

/**
 * 获取app版本号
 * @return
 */
int get_app_version_code() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_INT, FUNC_GET_INT_SIGN);
    jint app_version_code = CallStaticMethod(-1, Int, FUNC_GET_INT, GET_APP_VERSION_CODE);

    return app_version_code;
}

/**
 * 获取系统版本
 * getOSVersion
 * @return
 */
string get_os_version() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring j_os_version = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_OS_VERSION);
    string os_version = jstring_to_string(env, j_os_version);
    delete_ref(env, j_os_version);

    return os_version;
}

/**
 * 获取应用包名
 * @return
 */
string get_pkg_name() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring j_pkg_name = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_PKG_NAME);
    string pkg_name = jstring_to_string(env, j_pkg_name);
    delete_ref(env, j_pkg_name);

    return pkg_name;
}

STEERequestHeader_OSArch get_os_arch() {

#if defined(__i386__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__X86;
#elif defined(__x86_64__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__X86_64;
#elif defined(__arm__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__ARM;
#elif defined(__aarch64__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__ARM64;
#else
# error Unsupported architecture
#endif

}

STEERequestHeader_OSType get_os_type() {
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSTYPE__Android;
}

string get_dx_risk_sdk_version() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring j_version_name = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_SDK_VERSION);
    string version_name = jstring_to_string(env, j_version_name);
    delete_ref(env, j_version_name);

    return version_name;
}

string get_property(const string &appId, const string &key, const int scope) {
    string final_key = appId + ":" + key;
    return StorageUtils::Get(final_key, scope);
}

void set_property(const string &appId, const string &key, const string &value, const int scope) {
    string final_key = appId + ":" + key;
    StorageUtils::Put(final_key, value, scope);
}

extern int dx_risk_init_android_impl(DXRiskInter &sp);
extern int dx_risk_check_android_impl(DXEnvCheck &sp);


int dx_risk_check_init(DXEnvCheck &sp){
    dx_risk_check_android_impl(sp);
    return 0;
}


int dx_risk_init(DXRiskInter &sp){
    dx_risk_init_android_impl(sp);
    return 0;
}


/**
 * 获取当前可读可写的工作目录
 * @param path
 * @return
 */
string get_work_dir() {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring java_dir = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_WORK_DIR);
    string dir = jstring_to_string(env, java_dir);
    delete_ref(env, java_dir);

    return dir;
}

/**
 * 获取内置sdcard路径
 * @param path
 * @return
 */
string get_sdcard_dir(){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    //原型:public String getSdcardDir()

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring jsdcard_dir = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_SDCARD_DIR);
    string sd_card = jstring_to_string(env, jsdcard_dir);
    delete_ref(env, jsdcard_dir);

    return sd_card;
}

/**
 * 获取外置sdcard路径
 * @param path
 * @return
 */
string get_exsdcard_dir(){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring jexsdcard_dir = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_EXSDCARD_DIR);
    string exsd_card = jstring_to_string(env, jexsdcard_dir);
    delete_ref(env, jexsdcard_dir);

    return exsd_card;
}

/*
 * 获取sdcard是否存在的状态
 * 原型： private static int getSdcardStatus()
 * @param path
 * @return
 */
int get_sdcard_status(){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_INT, FUNC_GET_INT_SIGN);
    jint statu = CallStaticMethod(-1, Int, FUNC_GET_INT, GET_SDCARD_STATUS);

    return statu;
}


/**
 * 获取AppList
 * @return
 */
string get_app_list() {
    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod(FUNC_GET_STR, FUNC_GET_STR_SIGN);
    jstring j_app_list = (jstring) CallStaticMethod("", Object, FUNC_GET_STR, GET_APP_LIST);
    string app_list = jstring_to_string(env, j_app_list);
    delete_ref(env, j_app_list);

    return app_list;
}


