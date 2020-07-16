//
// Created by white on 2018/12/14.
//

#include "storage_jni.h"
#include <jni.h>
#include <stdlib.h>
#include <string>
#include <map>

#include "pb_prefix.h"

#include "jni_utils.h"
#include "DXPlatform.h"
#include "DXRiskInter.h"
#include "dx_env.h"

/*
 * 对SharedPreferences进行存储
 * 原型：public static boolean PutInSharedPreferences(String key, String value)
 * 存储文件名为 .sf.db
 *
 */

bool jni_putInSharedPreferences(const string &key, const string &value, const string &dbName){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sa, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");

    jstring jdbName = string_to_jstring(env, dbName);
    jstring jkey = string_to_jstring(env, key);
    jstring jvalue = string_to_jstring(env, value);
    jboolean jput_sharedpreference_statu = CallStaticMethod(false, Boolean , sa, jkey, jvalue, jdbName);

    delete_ref(env, jkey);
    delete_ref(env, jvalue);
    delete_ref(env, jdbName);

    bool result = jboolean_to_bool(jput_sharedpreference_statu);

    return result;
}

/*
 * 对SharedPreferences进行读取
 * 原型： public static String GetInSharedPreferences(String key)
 */
 string jni_getInSharedPreferences(const string &key, const string &dbName){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sb, "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;")

    jstring jkey = string_to_jstring(env, key);
    jstring jdbName = string_to_jstring(env, dbName);

    jstring jvalue = (jstring)CallStaticMethod("", Object, sb, jkey, jdbName);
    string result = jstring_to_string(env, jvalue);
    delete_ref(env, jkey);
    delete_ref(env, jvalue);
    delete_ref(env, jdbName);
    return result;
}

/*
 * 对SharedPreferences进行删除指定key
 * 原型： public static String GetInSharedPreferences(String key)
 * 存储文件名为 .sf.db
 */

void jni_removeInSharedPreferences(const string &key, const string &dbName){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sc, "(Ljava/lang/String;Ljava/lang/String;)V");
    jstring jkey = string_to_jstring(env, key);
    jstring jdbName = string_to_jstring(env, dbName);
    CallStaticMethod2(Void, sc, jkey, jdbName);

    delete_ref(env, jkey);
    delete_ref(env, jdbName);

}

/*
 * 对Setting进行存储
 * 原型public static boolean PutInSetting(String key, String value)
 */

bool jni_putInSetting(const string &key, const string &value){

    JNIEnvGuard guard;
    JNIEnv  *env = jni_init();

    GetStaticMethod_t(sd, "(Ljava/lang/String;Ljava/lang/String;)Z");
    jstring  jkey = string_to_jstring(env, key);
    jstring jvalue = string_to_jstring(env, value);

    jboolean result = CallStaticMethod(false, Boolean, sd, jkey, jvalue);

    delete_ref(env, jkey);
    delete_ref(env, jvalue);

    return result;
}

/*
 * 对Setting进行获取
 * 原型public static boolean GetInSetting(String key, String value)
 */

string jni_getInSetting(const string &key){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sf, "(Ljava/lang/String;)Ljava/lang/String;");
    jstring jkey = string_to_jstring(env, key);

    jstring jresult = (jstring)CallStaticMethod("", Object, sf, jkey);
    string result = jstring_to_string(env, jresult);

    delete_ref(env, jkey);
    delete_ref(env, jresult);

    return result;

}

/*
 * 对Setting进行获取
 * 原型public static void RemoveInSetting(String key)
 */

void jni_removeInSetting(const string &key) {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(se, "(Ljava/lang/String;)V");
    jstring jkey = string_to_jstring(env, key);
    CallStaticMethod2(Void, se, jkey);

    delete_ref(env, jkey);

}


bool jni_putInSQLite(const string &key, const string &value){
    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sg, "(Ljava/lang/String;Ljava/lang/String;)Z");

    jstring jkey = string_to_jstring(env, key);
    jstring jvalue = string_to_jstring(env, value);

    jboolean result = CallStaticMethod(false, Boolean, sg, jkey, jvalue);

    delete_ref(env, jkey);
    delete_ref(env, jvalue);
    return result;

}


string jni_getInSQLite(const string &key){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(sh, "(Ljava/lang/String;)Ljava/lang/String;");
    jstring jkey = string_to_jstring(env, key);

    jstring jresult = (jstring)CallStaticMethod("", Object, sh, jkey);
    string result = jstring_to_string(env, jresult);

    delete_ref(env, jkey);
    delete_ref(env, jresult);

    return result;

}


bool jni_removeInSQLite(const string &key){

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(si, "(Ljava/lang/String;)Z");
    jstring jkey = string_to_jstring(env, key);

    jboolean result = CallStaticMethod(false, Boolean, si, jkey);

    delete_ref(env, jkey);
    return result;
}


bool jni_getWRPermission() {
    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    GetStaticMethod_t(d, "(I)Z");
    // d(2)=checkWRPermission
    jboolean hasPermission = CallStaticMethod(false, Boolean , d, 2);
    return hasPermission;
}



string jni_getAssetsFileData(const string &fileName) {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    string result;
    GetStaticMethod_t(sj, "(Ljava/lang/String;)[B")

    jstring jfileName = string_to_jstring(env, fileName);
    jbyteArray jdata = (jbyteArray) CallStaticMethod("", Object, sj,
                                                     jfileName);

    result = jbyteArray_to_string(env, jdata);

    delete_ref(env, jfileName);
    delete_ref(env, jdata);

    return result;

}

string jni_getFileFromDir(const string &path, const string &end, const string &filter) {

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    string result;
    // getFileFromDir
    GetStaticMethod_t(sk, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

    jstring jpath = string_to_jstring(env, path);
    jstring jend = string_to_jstring(env, end);
    jstring jfilter = string_to_jstring(env, end);

    jstring jfileName = (jstring) CallStaticMethod(NULL, Object, sk, jpath, jend,
                                                   jfilter);

    result = jstring_to_string(env, jfileName);

    delete_ref(env, jpath);
    delete_ref(env, jend);
    delete_ref(env, jfilter);
    delete_ref(env, jfileName);

    return result;
}