//
// Created by blue on 2017/11/27.
//

#ifndef DXRISK_STORAGE_UTILS_H
#define DXRISK_STORAGE_UTILS_H

#include <string>
#include <vector>

using namespace std;

struct LocationInfo {
    string path;
    string fileName;
    string cryptKey;
    int cryptMethod;
    bool bWriteWhenNotExist;
    int fileType;
};

class StorageUtils {

public:

    static int GLOBAL;
    static int LOCAL;

    static StorageUtils* GetInstance();

    static bool Put(const string &key, const string &value, const int scope);

    static string Get(const string &key, const int scope);

    static string Get(const string &key, const int scope, const string &defValue);

    static bool Remove(const string &key, const int scope);

private:

    StorageUtils();

    ~StorageUtils();

    bool   Put_Internal(const string &key, const string &value, const int scope);

    string Get_Internal(const string &key, const int scope, const string &defValue);

    bool   Remove_Internal(const string &key, const int scope);

    /** local **/

    bool PutLocalData(const string &packageName, const string &key, const string &value,
                      LocationInfo *pLocation);

    string GetLocalData(const string &packageName, const string &key, LocationInfo *pLocation);

    bool RemoveLocalData(const string &packageName, const string &key, LocationInfo *pLocation);

//    /** 检测内置SD卡相关 **/
//
//    void PutSDCardFlag();
//
//    bool IsSDCardChanged();

    /** crypt **/

    string Encode(const string &cryptKey, const string &data, const int &cryptIndex);

    string Decode(const string &cryptKey, const string &data, const int &cryptIndex);

    string AddMD5(const string &data);

    string RemoveMD5(const string &data);

    bool CheckMD5(const string &data);

    /**调用Java 系统api进行存储**/

    bool PutInSharedPreferences(const string &packageName,const string &key, const string &value, const string &dbName);

    string GetInSharedPreferences(const string &packageName, const string &key, const string &dbName);

    bool RemoveInSharedPreferences(const string &packageName, const string &key, const string &dbName);

    bool PutInSetting(const string &packageName, const string &key, const string &value);

    string GetInSetting(const string &packageName, const string &key);

    bool RemoveInSetting(const string &packageName, const string &key);

    bool PutInSQLite(const string &packageName, const string &key, const string &value);

    string GetInSQLite(const string &packageName, const string &key);

    bool RemoveInSQLite(const string &packageName, const string &key);

    bool PutInJavaApi(const string &packageName, const string &key, const string &value);

    bool RemoveInJavaApi(const string &packageName, const string &key);

    bool GetInJavaApi(const string &packageName, const string &key, string* result);

    /**Java 系统api存储实现**/

    bool PutInSharedPreferencesJava(const string &key, const string &value, const string &dbName);

    string GetInSharedPreferencesJava(const string &key, const string &dbName);

    void RemoveInSharedPreferencesJava(const string &key, const string &dbName);

    bool PutInSettingJava(const string &key, const string &value);

    string GetInSettingJava(const string &key);

    void RemoveInSettingJava(const string &key);

    bool PutInSQLiteJava(const string &key, const string &value);

    string GetInSQLiteJava(const string &key);

    bool RemoveInSQLiteJava(const string &key);

    LocationInfo **mPLocationList;
    int mLocationsCount;
    void InitLocationList();

    /** OtherTools **/

    /** 获得assets文件夹下指定文件数据*/
    string GetAssetsFileData(const string &fileName);

    /** 获取指定路径下指定后缀名文件集合中的某个文件路径**/
    string GetFileFromDir(const string &path, const string &end, const string &filter);

};

/**Json 存储相关**/
class JsonHelper {

public:

    static string PutLocalJson(const string &jsonStr, const string &packageName,
                               const string &key, const string &value);

    static string GetLocalJson(const string &jsonStr, const string &packageName, const string &key);

    static string RemoveLocalJson(const string &strValue, const string &packageName,
                                  const string &tokenType);


};

/**mp3文件改写相关**/
class MP3Parser
{
public:

    static bool writeJsontoEnd(const string &filePath, const string &data);

    static string readJsonFromEnd(const string &filePath);

    static unsigned long findTAGPos(const string &data);

    static bool checkMp3(const string &filePath);

    static bool insertTag(const string &filePath);
};

/**jpg文件改写相关 **/
class JPGParser{

public:

    static bool writeJsontoEnd(const string &filePath, const string &data);

    static string readJsonFromEnd(const string &filePath);

    static unsigned long findTAGPos(const string &data);

    static bool checkJpg(const string &filePath);

    static bool insertTag(const string &filePath);

};

#endif //DXRISK_STORAGE_UTILS_H
