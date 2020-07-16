//
// Created by white on 2018/12/14.
//

#ifndef AS_RISK_STORAGE_JNI_H
#define AS_RISK_STORAGE_JNI_H


#include <string>
using namespace std;


/*
 * 对SharedPreferences进行存储
 * 原型：public static boolean PutInSharedPreferences(String key, String value)
 * 存储文件名为 .sf.db
 *
 */
bool jni_putInSharedPreferences(const string &key, const string &value, const string &dbName);

/*
 * 对SharedPreferences进行读取
 * 原型： public static String GetInSharedPreferences(String key)
 */
string jni_getInSharedPreferences(const string &key, const string &dbName);

/*
 * 对SharedPreferences进行删除指定key
 * 原型： public static String GetInSharedPreferences(String key)
 * 存储文件名为 .sf.db
 */
void jni_removeInSharedPreferences(const string &key, const string &dbName);

/*
 * 对Setting进行存储
 * 原型public static boolean PutInSetting(String key, String value)
 */
bool jni_putInSetting(const string &key, const string &value);

/*
 * 对Setting进行获取
 * 原型public static boolean GetInSetting(String key, String value)
 */
string jni_getInSetting(const string &key);

/*
 * 对Setting进行获取
 * 原型public static void RemoveInSetting(String key)
 */
void jni_removeInSetting(const string &key);

bool jni_putInSQLite(const string &key, const string &value);

string jni_getInSQLite(const string &key);

bool jni_removeInSQLite(const string &key);

bool jni_getWRPermission();

string jni_getAssetsFileData(const string &fileName);

string jni_getFileFromDir(const string &path, const string &end, const string &filter);

#endif //AS_RISK_STORAGE_JNI_H
