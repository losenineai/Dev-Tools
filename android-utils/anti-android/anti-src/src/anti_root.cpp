//
// Created by white on 2018/5/14.
//

#include <cstring>
#include <unistd.h>
#include <anti_utils.h>
#include <anti_crash_log_prefix.h>
#include "properties.h"
#include "jni_utils.h"
#include "file_utils.h"
#include "comm_utils.h"
#include "utils_anti_log.h"
#include "anti_root.h"
#include "dx_anti_stee.h"
#include "anti_result_handler.h"

int _root_crash_code;
/**
 * 通过检测ro.build.xxx
 * @return
 */
STEE
bool anti_root_detectByProps() {
    anti_info_log("======anti_root_detectByProps");
    bool result = false;

    const char *TEST_KEY = "test-keys";
    const string props[] = {"ro.build.tags"};
    char value[256];

    for (string p : props) {
        anti_property_get(p.c_str(), value, "none");
        if (dx_strstr(value, TEST_KEY)) {
            result = true;
            break;
        }
    }

    if (result) {
        _root_crash_code = ANTI_ROOT_ERR_BY_PROPS;
    }

    return result;
}

/**
 * 通过检测 /system/app/Superuser.apk
 * @return
 */
STEE
bool anti_root_detectByApk() {
    anti_info_log("==========anti_root_detectByApk");
    static const string APK_PATH = "/system/app/Superuser.apk";
    bool result = anti_file_exist(APK_PATH);

    if (result) {
        _root_crash_code = ANTI_ROOT_ERR_BY_APK;
    }

    return result;
}

/**
 * 通过检测 su 命令
 * @return
 */
STEE
bool anti_root_detectBySu() {
    anti_info_log("==============anti_root_detectBySu");
    static const string SU_PATHS[] = {"/su",
                                      "/sbin/su",
                                      "/su/bin/su",
                                      "/data/local/xbin/su",
                                      "/data/local/bin/su",
                                      "/data/local/su",
                                      "/vendor/bin/su",
                                      "/system/bin/.ext/.su",
                                      "/system/usr/we-need-root/su-backup",
                                      "/system/bin/su",
                                      "/system/bin/failsafe/su",
                                      "/system/bin/cufsdosck",
                                      "/system/bin/cufsmgr",
                                      "/system/bin/cufaevdd",
                                      "/system/bin/conbb",
                                      "/system/xbin/su",
                                      "/system/xbin/cufsdosck",
                                      "/system/xbin/cufsmgr",
                                      "/system/xbin/cufaevdd",
                                      "/system/xbin/conbb",
                                      "/system/sbin/su",
                                      "/system/sd/xbin/su"
    };

    bool result = false;
    for (string su : SU_PATHS) {
        if (anti_file_executable(su)) {
            result = true;
            break;
        }
    }

    if (result) {
        _root_crash_code = ANTI_ROOT_ERR_BY_SU;
    }
    return result;
}

/**
 * 通过检测 which 命令
 * @return
 */
STEE
bool anti_root_detectByWhich() {
    anti_info_log("============anti_root_detectByWhich");
    bool result = false;

    static const string WHICH_PATHS[] = {"/system/xbin/which",
                                        "/system/bin/which"};

    for (string which : WHICH_PATHS) {
        if (anti_file_exist(which)) {
            string cmd_result = anti_util_executeCMD(which + " su", "");
            result = (cmd_result.length() > 0);
            break;
        }
    }

    if (result) {
        _root_crash_code = ANTI_ROOT_ERR_BY_WHICH;
    }

    return result;
}


/**
 * 检测app应用列表名称。
 * @param args
 * @return
 */
STEE
bool anti_root_detect_ManagementApps(){
    static const string knownRootAppsPackages[] ={
            "com.noshufou.android.su",
            "com.noshufou.android.su.elite",
            "eu.chainfire.supersu",
            "com.koushikdutta.superuser",
            "com.thirdparty.superuser",
            "com.yellowes.su",
            "com.topjohnwu.magisk"
    };

    anti_info_log("=====debug_check_UserAMonkey=====");
    const char* command = "pm list package -3";

    dxFILE* file = dx_popen(command, "r");
    char buf[256] = {0};

    while(dx_fgets(buf, sizeof(buf), file)){
        const char* pkg_name = strstr(buf, ":");
        if(pkg_name == NULL){
            continue;
        }
        pkg_name += 1;
        int len = strlen(pkg_name) - 1;
        for(string rootAppPackage : knownRootAppsPackages){
            if(strncmp(rootAppPackage.c_str(), pkg_name, len) == 0){
                anti_info_log("======find know root app:%s, line buf:%s", rootAppPackage.c_str(), buf);
                return true;
            }
        }
    }
    return false;
}


/**
 *
 * @param args
 * @return
 */
STEE
bool anti_root_detect_RWPaths(){
    static const string pathsThatShouldNotBeWritable[] = {
            "/system",
            "/system/bin",
            "/system/sbin",
            "/system/xbin",
            "/vendor/bin",
            "/sbin/",
            "/etc/"
    };

    dxFILE* file = dx_popen("mount", "r");

    char buf[256] = {0};
    while(dx_fgets(buf, sizeof(buf), file)){
        for(string notBeWritablePath : pathsThatShouldNotBeWritable){
            if(strstr(buf, (" "+notBeWritablePath).c_str()) && strstr(buf, "rw,")){
                anti_info_log("======find paths that should not be writable:%s", buf);
                return true;
            }
        }
    }

    return false;
}

STEE
void *DXAnti_detect_root(void *args) {
    anti_info_log("============DXAnti_detect_root");
    if(anti_root_detect_RWPaths() || anti_root_detect_ManagementApps() || anti_root_detectByProps() || anti_root_detectByApk() || anti_root_detectBySu() ||
       anti_root_detectByWhich()) {

        // 统一anti结果处理
        AntiResult result = {AntiType::AT_ROOT, _root_crash_code};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    return NULL;
}

STEE
int DXAnti_detect_root_async() {
    pthread_t newthread;
    int ret = -1;

    dx_anti_pthread_create(ret, &newthread, NULL, DXAnti_detect_root, NULL);

    return ret;
}

STEE
void DXAnti_detect_root_entry() {
    DXAnti_detect_root_async();
}