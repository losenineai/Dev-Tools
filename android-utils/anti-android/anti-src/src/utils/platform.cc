//
// Created by boyliang on 2017/5/26.
//

#include <jni.h>
#include <string>
#include <include/anti_utils.h>
#include "include/jni_utils.h"
#include "include/file_utils.h"
#include "include/platform.h"
#include "include/jni_comm_utils.h"

using std::string;

/**
 * 获取应用包名
 * @return
 */
string anti_platform_getPkgName() {

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject context = anti_jni_getGlobalContext(env);
    jstring j_pkg_name = (jstring) anti_jni_callMethodByName(env, context,
                                                             "getPackageName",
                                                             "()Ljava/lang/String;");
    string pkg_name = anti_jni_jstring_to_string(env, j_pkg_name);
    anti_jni_delete_ref(env, j_pkg_name);

    return pkg_name;
}

/**
 * 获取当前可读可写的工作目录
 * @param path
 * @return
 */
string anti_platform_getWorkDir() {

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject j_context = anti_jni_getGlobalContext(env);
    // invoke java method Context.getFilesDir
    jobject j_file = anti_jni_callMethodByName(env, j_context,
                                               "getFilesDir",
                                               "()Ljava/io/File;");

    if (j_file == nullptr) {
        return string("");
    }

    // invoke java method File.getAbsolutePath
    jstring j_path = (jstring) anti_jni_callMethodByName(env, j_file,
                                                         "getAbsolutePath",
                                                         "()Ljava/lang/String;");


    string base_dir = anti_jni_jstring_to_string(env, j_path);
    anti_jni_delete_ref(env, j_path);

    string work_dir = base_dir + "/stee";
    anti_file_makeDir(work_dir);

    return work_dir;
}


string anti_platform_getResPath(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject j_context = anti_jni_getGlobalContext(env);

    jstring j_resPath = (jstring) anti_jni_callMethodByName(env, j_context,
                       "getPackageResourcePath", "()Ljava/lang/String;");

    string res_path = anti_jni_jstring_to_string(env, j_resPath);

    anti_jni_delete_ref(env, j_resPath);

    return res_path;
}


bool anti_platform_isDebuggerConnected(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jclass j_debug = anti_jni_findClassByName(env, "android/os/Debug");
    jboolean j_result = anti_jni_callStaticMethodReturnBool(env, j_debug,
                                                           "isDebuggerConnected", "()Z");
    anti_jni_delete_ref(env, j_debug);

    return j_result == JNI_TRUE;
}
