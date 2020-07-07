//
// Created by white on 2018/6/8.
//

#include "anti_pkg_signature.h"


#include <stdlib.h>
#include <string>
#include <map>
#include <unistd.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <fcntl.h>
#include "utils/include/jni_utils.h"
#include "utils/include/jni_comm_utils.h"
#include "utils/include/string_utils.h"
#include "dx_anti_stee.h"
#include "pkcs7.h"


#define DEBUG_CERT_MD5 "85d8f868f3149f74b10693b4bbf1113d"


STEE
vector<string> get_signature_md5_list_by_java() {
    anti_info_log("get_signature_md5_list_by_java");

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    // get PackageManager obj
    jobject jo_context = anti_jni_getGlobalContext(env);
    jobject jo_pkgManger = anti_jni_callMethodByName(env, jo_context,
                                                    "getPackageManager",
                                                    "()Landroid/content/pm/PackageManager;");

    // invoke Conetxt.getPackageName
    jstring js_pkgName = (jstring) anti_jni_callMethodByName(env, jo_context, "getPackageName", "()Ljava/lang/String;");

    // get PackageInfo obj
    jobject jo_packageInfo = anti_jni_callMethodByName(env, jo_pkgManger,
                                                      "getPackageInfo",
                                                      "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;", js_pkgName, 64);


    // get PackageInfo.signature field value
    jobjectArray joa_sigs = (jobjectArray) anti_jni_getObjectField(env, jo_packageInfo, "signatures",
                                                            "[Landroid/content/pm/Signature;");
    anti_info_log("get_signature_md5_list_by_java joa_sigs %d", joa_sigs != nullptr);


    vector<string> sigVect;

    int size = env->GetArrayLength(joa_sigs);

    for (int i = 0; i < size; i++) {
        jobject jo_signature = (jobject) env->GetObjectArrayElement(joa_sigs, i);
        // invoke Signature.toByteArray
        jbyteArray jba_sig = (jbyteArray)anti_jni_callMethodByName(env, jo_signature, "toByteArray", "()[B");

        string sig = anti_jni_jbyteArray_to_string(env, jba_sig);
        string sig_md5 = anti_str_MD5(sig);
        sig_md5 = anti_str_MD5("dx" + sig_md5 + "inc");
        sigVect.push_back(sig_md5);
        anti_jni_delete_ref(env, jba_sig);
    }

    anti_info_log("get_signature_md5_list_by_java sigVect size %d", sigVect.size());

    anti_jni_delete_ref(env, jo_pkgManger);
    anti_jni_delete_ref(env, js_pkgName);
    anti_jni_delete_ref(env, jo_packageInfo);
    anti_jni_delete_ref(env, joa_sigs);

    return sigVect;
}


STEE
bool anti_do_verify_signature_by_java(const string &inputSign) {
    anti_info_log("anti_do_verify_signature_by_java");
    if (inputSign.length() <= 0) {
        return false;
    }

    bool result = false;

    vector<string> sigVec = get_signature_md5_list_by_java();

    if (sigVec.size() > 0) {
        for (auto &s : sigVec) {
            anti_info_log("anti_do_verify_signature_by_java: %s - %s", s.c_str(), inputSign.c_str());
            if (inputSign == s) {
                result = true;
                break;
            }
        }
    }

    anti_info_log("anti_do_verify_signature_by_java result:%d", result);
    return result;
}

STEE
bool anti_do_verify_signature_by_c(const string &inputSign) {
    anti_info_log("anti_do_verify_signature_by_c");
    if (inputSign.length() <= 0) {
        return false;
    }

    bool result = false;

    // 获取已安装的apk路径
    auto getApkPath = []() {
        DXAnti_JNIEnvGuard guard;
        JNIEnv *env = anti_get_jni_env();

        jobject j_context = anti_jni_getGlobalContext(env);
        jstring j_apkPath = (jstring) anti_jni_callMethodByName(env, j_context, "getPackageResourcePath", "()Ljava/lang/String;");
        string apkPath = anti_jni_jstring_to_string(env, j_apkPath);
        anti_info_log("anti_do_verify_signature_by_c pkgPath:%s", apkPath.c_str());
        anti_jni_delete_ref(env, j_apkPath);

        return apkPath;
    };

    // 转小写
    auto toLower = [](char *arr) {

        int index = 0;
        while (arr[index] != '\0') {
            if (arr[index] >= 'A' && arr[index] <= 'Z')
                arr[index] = tolower(arr[index]);

            index++;
        }
    };

    string apkPath = getApkPath();

    char apkPath_c[apkPath.length() * sizeof(char) + 1];
    dx_strcpy(apkPath_c, apkPath.data());

    pkcs7 test;
    if (test.open_file(apkPath_c)) {
        int count = test.get_count();

        // 解析apk，有签名
        if (count > 0) {
            vector<string> sigs;

            char md5CharArr[33] = {0};
            for (int i = 0; i < count; i++) {
                test.get_MD5_hex(md5CharArr, i);
                toLower(md5CharArr);
                string sig_md5 = anti_str_MD5("dx" + string(md5CharArr) + "inc");
                sigs.push_back(sig_md5);
            }

            for (auto &s : sigs) {
                anti_info_log("anti_do_verify_signature_by_c: %s - %s", s.c_str(), inputSign.c_str());
                if (inputSign == s) {
                    result = true;
                    break;
                }
            }
        }
    }


    anti_info_log("check_license_validity_from_c result:%d", result);

    return result;
}




STEE
bool DXAnti_verify_pkg(const string &pkg){
    anti_info_log("DXAnti_detect_pkg pkg:%s", pkg.c_str());
    if (pkg.length() <= 0) {
        return false;
    }

    bool result = false;

    // java方式检查
    auto check_by_java = [&]() {
        DXAnti_JNIEnvGuard guard;
        JNIEnv *env = anti_get_jni_env();

        jobject context = anti_jni_getGlobalContext(env);
        jstring j_pkgName = (jstring) anti_jni_callMethodByName(env, context, "getPackageName",
                                                                "()Ljava/lang/String;");
        string curPkgName = anti_jni_jstring_to_string(env, j_pkgName);
        anti_jni_delete_ref(env, j_pkgName);

        anti_info_log("check_by_java curPkg:%s", curPkgName.c_str());

        bool result = pkg == curPkgName;
        anti_info_log("check_by_java result %d", result);
        return result;
    };

    // cmdline方式检查
    auto check_by_cmdline = [&]() {
        bool result = true;
        pid_t pid = getpid();
        char path[64] = {0};
        dx_sprintf(path, "/proc/%d/cmdline", pid);

        // FILE *cmdline = fopen(path, "r");
        int cmd_fd = dx_open(path, O_RDONLY);

        if (cmd_fd > 0) {
            char application_id[64] = {0};
            dx_read(cmd_fd, application_id, sizeof(application_id));
            // fread(application_id, sizeof(application_id), 1, cmdline);
            // fclose(cmdline);
            dx_close(cmd_fd);
            string appId = string(application_id);
            int colonPos = appId.find(':', 0);

            if(colonPos != string::npos) {
                appId = appId.substr(0, colonPos);
            }

            result = pkg == appId;
        }


        anti_info_log("check_by_cmdline result %d", result);
        return result;
    };

    result = check_by_java() && check_by_cmdline();

    anti_info_log("DXAnti_detect_pkg result: %d", result);

    return result;
}



STEE
bool DXAnti_verify_signature(const string &sign){
    anti_info_log("DXAnti_detect_signature sign:%s", sign.c_str());

    // TODO for developer debug
    if (anti_do_verify_signature_by_java(DEBUG_CERT_MD5) && anti_do_verify_signature_by_c(DEBUG_CERT_MD5)) {
        return true;
    }

    return anti_do_verify_signature_by_java(sign) && anti_do_verify_signature_by_c(sign);
}
