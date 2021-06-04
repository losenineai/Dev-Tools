//
// Created by white on 2018/5/14.
//

#include <cstring>
#include <unistd.h>
#include <anti_utils.h>
#include <anti_crash_log_prefix.h>
#include "properties.h"
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

    bool result = false;

    const char *TEST_KEY = "test-keys";
    const string props[] = {"ro.build.tags"};
    char value[256];

    for (string p : props) {
        anti_property_get(p.c_str(), value, "none");
        if (strstr(value, TEST_KEY)) {
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

    static const string SU_PATHS[] = {"/su",
                                      "/sbin/su",
                                      "/su/bin/su",
                                      "/system/bin/su",
                                      "/system/xbin/su",
                                      "/data/local/xbin/su",
                                      "/data/local/bin/su",
                                      "/system/sbin/su",
                                      "/system/sd/xbin/su",
                                      "/system/bin/failsafe/su",
                                      "/data/local/su",
                                      "/vendor/bin/su"};

    bool result = false;
    for (string su : SU_PATHS) {
        if (anti_file_exist(su)) {
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

STEE
void *DXAnti_detect_root(void *args) {

    if(anti_root_detectByProps() || anti_root_detectByApk() || anti_root_detectBySu() ||
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

    dx_pthread_create(ret, &newthread, NULL, DXAnti_detect_root, NULL);

    return ret;
}

STEE
void DXAnti_detect_root_entry() {
    DXAnti_detect_root_async();
}