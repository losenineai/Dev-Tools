//
// Created by blue on 2017/11/27.
//

#include "storage_utils.h"
#include "DXPlatform.h"
#include "DXLog.h"
#include "utils.h"
#include "jni_utils.h"
#include "storage_jni.h"
#include <mutex>

//表示内存路径的数量
#define APP_LOCATION_COUNT 3

//表示内置sdcard路径的数量
#define SDCARD_LOCATION_COUNT 20

//表示总的路径的总数量
#define ALL_LOCATION_COUNT APP_LOCATION_COUNT + SDCARD_LOCATION_COUNT

//表示Java层存储的采集点数量
#define JAVA_LOCATION_COUNT 3

// 加密方式
#define CRYPT_XXTEA 0
#define CRYPT_AES 1
#define CRYPT_RC4 2

//存储媒介文件类型
#define DB_TYPE 0
#define MP3_TYPE 1
#define JPG_TYPE 2

#define STRING_NULL ""

mutex PG_lock;

/****************************** public static ************************************/

int StorageUtils::GLOBAL = 0;
int StorageUtils::LOCAL = 1;


StorageUtils* StorageUtils::GetInstance() {
    static StorageUtils sInstance;
    return &sInstance;
}

bool StorageUtils::Put(const string &key, const string &value, const int scope) {
    return StorageUtils::GetInstance()->Put_Internal(key, value, scope);
}

string StorageUtils::Get(const string &key, const int scope) {
    string devValue = "";
    return StorageUtils::Get(key, scope, devValue);
}

string StorageUtils::Get(const string &key, const int scope, const string &defValue) {
    return StorageUtils::GetInstance()->Get_Internal(key, scope, defValue);
}

bool StorageUtils::Remove(const string &key, const int scope) {
    return StorageUtils::GetInstance()->Remove_Internal(key, scope);
}

/****************************** private ************************************/

StorageUtils::StorageUtils() :mPLocationList(nullptr),
                              mLocationsCount(0) {

    InitLocationList();
}

StorageUtils::~StorageUtils() {
    delete_array(mPLocationList);
}

void StorageUtils::InitLocationList() {

    if(mPLocationList != nullptr)
        return;

    string packageName  = get_pkg_name();
    bool bSDCardValid   = (get_sdcard_status() == 1);

    mLocationsCount = bSDCardValid ? ALL_LOCATION_COUNT : APP_LOCATION_COUNT;
    mPLocationList  = (LocationInfo**)malloc(sizeof(LocationInfo*) * mLocationsCount);

    mPLocationList[0]  = new LocationInfo{"/data/data/" + packageName + "/cache",  ".cache_tmpXX.db",          "i6xbvViAD15xR8MstsLsTqeev9RqfHfP",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[1]  = new LocationInfo{"/data/data/" + packageName + "/files",  ".files.db",                "F7IeihPEkfkY2N2WjWENteeyLTL1o4ky",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[2]  = new LocationInfo{"/data/data/" + packageName + "/databases", ".android.db",           "U2FsdGVkX19GjRxpr5g0P9cg6S9wMDBw0fUM=",    CRYPT_RC4,   true, DB_TYPE};

    if(!bSDCardValid)
        return;

    string sdcardPath  = get_sdcard_dir();

    mPLocationList[3]  = new LocationInfo{sdcardPath + "/Movies/.xc",               ".xc.db",                  "eq0sdXA3YfVjm58mETvWIJCNl80omkmk",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[4]  = new LocationInfo{sdcardPath + "/DCIM/.yz",                 ".yz.db",                  "NqfIdgalM5JfXtbY",                         CRYPT_RC4,   true, DB_TYPE};
    mPLocationList[5]  = new LocationInfo{sdcardPath + "/Android/.vy",              ".vy.db",                  "ti4s2YFHgEaLs5NsHF7pvyBO7YTKj80t",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[6]  = new LocationInfo{sdcardPath + "/.ufs",                     ".ufs.db",                 "BJKFIzcabqKFPiStivgKqNrEKXDo1sdQ",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[7]  = new LocationInfo{sdcardPath + "/Music",                    ".music.db",               "Rpvt2yvzBhY2rAICOejSKnwwyT2JZLs4",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[8]  = new LocationInfo{sdcardPath + "/Android/data",             ".dxData.db",              "M1zriyGKKuDBlAA9QaCn5JCnZ8xJkBdk",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[9]  = new LocationInfo{sdcardPath + "/Movies",                   ".movesD.db",              "uAFs9WGPwK5zs0yjHiS620010EIHnBv4",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[10] = new LocationInfo{sdcardPath + "/data",                     ".dataX.db",               "irrGpl1hCOPtmNOVe0KinqLPguVkp6Eo",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[11] = new LocationInfo{sdcardPath + "/alipay",                   ".alipayX.db",             "PrWgtdFlFGpFL7dGFbSKv7IIjK71f8u7",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[12] = new LocationInfo{sdcardPath + "/tencent/QQfile_recv",      ".QQ.db",                  "SeDqOKC4DB5r40LYe9YJHAOBJlzqh6cz",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[13] = new LocationInfo{sdcardPath + "/Android",                  ".android.db",             "hi45DwaddHLqkQpN",                         CRYPT_RC4,   true, DB_TYPE};
    mPLocationList[14] = new LocationInfo{sdcardPath + "/Pictures",                 ".ptcs.db",                "UyOoxcRIbSGjIMhWsBPJ0Hpfbni1d1tE",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[15] = new LocationInfo{sdcardPath + "/Alarms",                   ".alas.db",                "9c7V8BZLun0GQXO8Td8pwkjf9n1rJlb6",         CRYPT_AES,   true, DB_TYPE};
    mPLocationList[16] = new LocationInfo{sdcardPath + "/WChat",                    ".wchatX.db",              "qdFUkOoyPEM6fAgYsZ2zkDudgVaxHbRs",         CRYPT_XXTEA, true, DB_TYPE};
    mPLocationList[17] = new LocationInfo{sdcardPath + "/DCIM/Camera",              ".camera.jpg.bgm",         "cat51vkUQP0N17Wu",                         CRYPT_RC4,   true, JPG_TYPE};
    mPLocationList[18] = new LocationInfo{sdcardPath + "/tencent/QQfile_recv",      ".qqfile_recv.jpg.bgm",    "yoGC3Pcy93YR8SlLhtM8x7G6yibXFl1q",         CRYPT_AES,   true, JPG_TYPE};
    mPLocationList[19] = new LocationInfo{sdcardPath + "/tencent/QQ_Images",        ".qq_images.jpg.bgm",      "JP3xFqdRxxxAsgdGZRs9SGoqD0aBHcea",         CRYPT_XXTEA, true, JPG_TYPE};
    mPLocationList[20] = new LocationInfo{sdcardPath + "/sina/weibo/weibo",         ".weibo.jpg.bgm",          "R9EG68mpuBVse2bsJCWkOaomp8NFQmjb",         CRYPT_AES,   true, JPG_TYPE};
    mPLocationList[21] = new LocationInfo{sdcardPath + "/qqmusic/song",             ".qqmusic.mp3.bgm",        "DQ14vcEnZSjG5VkjpfK3otXcnantRXCR",         CRYPT_AES,   true, MP3_TYPE};
    mPLocationList[22] = new LocationInfo{sdcardPath + "/Netease/cloudmusic/Music", ".neteasemusic.mp3.bgm",   "a6He18hOengBdjdBAtFBmsH2xB3MwrpU",         CRYPT_AES,   true, MP3_TYPE};

//    for(int i = 0; i < mLocationsCount; i++) {
//        err_log("cur path[%02d]: %s", i, mPLocationList[i]->path.c_str());
//    }

}

bool
StorageUtils::Put_Internal(const string &key, const string &value, const int scope) {

    // this lock is the same as
    lock_guard<mutex> g(PG_lock);

    string packageName = get_pkg_name();

    packageName = scope == GLOBAL ? "global" : packageName;

    for (int i = 0; i < mLocationsCount; i++) {
        PutLocalData(packageName, key, value, mPLocationList[i]);
    }
    //调用Java 系统api进行存储
    PutInJavaApi(packageName, key, value);

    //重置sd卡标记
//    PutSDCardFlag();

    return true;
}

string StorageUtils::Get_Internal(const string &key, const int scope, const string &defValue) {

    lock_guard<mutex> g(PG_lock);

    //当不存在sdcard时，存储的pathlist集合的index下限为4（因为非sdcard的path有4个）
    string packageName = get_pkg_name();

    //count ： 总共的踩点数
    int count = mLocationsCount + JAVA_LOCATION_COUNT;

    string results[count];

    packageName = scope == GLOBAL ? "global" : packageName;

    for (int i = 0; i < mLocationsCount; i++) {

        string strResult = GetLocalData(packageName, key, mPLocationList[i]);
        results[i] = strResult;
    }

    //todo:这里较大改动,以前是JavaApi的数据将前三个数据result覆盖了
    GetInJavaApi(packageName, key, &results[mLocationsCount]);

    string result = StringUtils::FindString(results, count);
    return result;
}

bool StorageUtils::Remove_Internal(const string &key, const int scope) {

    lock_guard<mutex> g(PG_lock);

    //当不存在sdcard时，存储的pathlist集合的index下限为4（因为非sdcard的path有4个）
    string packageName = get_pkg_name();

    packageName = scope == GLOBAL ? "global" : packageName;

    for (int i = 0; i < mLocationsCount; i++) {
        RemoveLocalData(packageName, key, mPLocationList[i]);
    }

    //****调用Java 系统api进行删除****//
    RemoveInJavaApi(packageName, key);
    return true;
}

//有密钥作为参数
bool
StorageUtils::PutLocalData(const string &packageName, const string &key, const string &value,
                           LocationInfo *pLocation) {

    string path = pLocation->path + "/" + pLocation->fileName;

    if (pLocation->bWriteWhenNotExist && !FileUtils::Exists(pLocation->path)) {
        if (!FileUtils::MakeDir(pLocation->path))
            return false;
    } else if (!pLocation->bWriteWhenNotExist) {
        if (!FileUtils::Exists(pLocation->path) || !FileUtils::CanOpen(path))
            return false;
    }

    switch (pLocation->fileType) {

        case DB_TYPE: {

            string strData = FileUtils::ReadAll(path);

            string removeSigData = RemoveMD5(strData);
            string strDecryptCode = Decode(pLocation->cryptKey, removeSigData, pLocation->cryptMethod);
            string strValue = JsonHelper::PutLocalJson(strDecryptCode, packageName, key, value);
            string reEncryptData = Encode(pLocation->cryptKey, strValue, pLocation->cryptMethod);
            string sigData = AddMD5(reEncryptData);
            FileUtils::WriteAll(path, sigData);

            break;
        }
        case MP3_TYPE: {

            //返回指定文件夹内其中一个符合条件的文件路径
            string mp3InMobilePath = jni_getFileFromDir(pLocation->path.c_str(), "mp3", pLocation->fileName.c_str());
            //如果指定文件夹内不存在mp3文件，则立即返回false
            if ("" == mp3InMobilePath) {
                return false;
            }

            //检查该文件是否存在是否可打开，如果不存在则创建该文件，再检查该文件是否符合存储要求
            if (!MP3Parser::checkMp3(path)) {

                //调用Java层代码获取MP3数据,并存入后缀名为".bgm"文件中，作为存储Json数据的载体
                string realFileData = FileUtils::ReadAll(mp3InMobilePath);
                FileUtils::WriteAll(path, realFileData);
                MP3Parser::insertTag(path);

            }

            string mp3DataWithSig = MP3Parser::readJsonFromEnd(path);

//            if(!CheckMD5(mp3DataWithSig)){
////                return false;
//            }

            string mp3Data = RemoveMD5(mp3DataWithSig);
            string mp3DecryptCode = Decode(pLocation->cryptKey, mp3Data, pLocation->cryptMethod);
            string mp3Value = JsonHelper::PutLocalJson(mp3DecryptCode, packageName, key, value);
            string reMp3Encrypt = Encode(pLocation->cryptKey, mp3Value, pLocation->cryptMethod);

            //前面64个字符为签名字符串
            string sigData = AddMD5(reMp3Encrypt);
            MP3Parser::writeJsontoEnd(path, sigData);

            break;
        }

        case JPG_TYPE: {

            //返回指定文件夹内其中一个符合条件的文件路径
            string jpgInMobilPath = jni_getFileFromDir(pLocation->path, "jpg", pLocation->fileName);

            //如果指定文件夹内不存在合法文件，则立即返回false
            if ("" == jpgInMobilPath){

                return false;

            }

            //检查该文件是否存在是否可打开，如果不存在则创建该文件，再检查该文件是否符合存储要求
            if (!JPGParser::checkJpg(path)) {

                //调用Java层代码获取jpg数据,并存入后缀名为".bgm"文件中，作为存储Json数据的载体
                string realFileData = FileUtils::ReadAll(jpgInMobilPath);
                FileUtils::WriteAll(path, realFileData);
                JPGParser::insertTag(path);
            }

            string jpgDataWithSig = JPGParser::readJsonFromEnd(path);

            string jpgData = RemoveMD5(jpgDataWithSig);
            string jpgDecryptCode = Decode(pLocation->cryptKey, jpgData, pLocation->cryptMethod);

            string jpgValue = JsonHelper::PutLocalJson(jpgDecryptCode, packageName, key, value);
            string reJpgEncrypt = Encode(pLocation->cryptKey, jpgValue, pLocation->cryptMethod);
            string sigData = AddMD5(reJpgEncrypt);
            JPGParser::writeJsontoEnd(path, sigData);

            break;
        }
        default:
            break;
    }

    return true;
}

//有密钥作为参数
string StorageUtils::GetLocalData(const string &packageName, const string &key, LocationInfo *pLocation) {

    string path = pLocation->path + "/" + pLocation->fileName;

    if (!FileUtils::Exists(pLocation->path) || !FileUtils::CanOpen(path))
        return "";

    map<string, map<string, string> > localMap;
    string encryptDataWithSig;
    string encryptData;
    string decryptData;

    switch (pLocation->fileType) {

        case DB_TYPE: {
            encryptDataWithSig = FileUtils::ReadAll(path);

            //检查md5值，如果不符合，则返回STRINGNULL
            if (!CheckMD5(encryptDataWithSig)){
                return STRING_NULL;
            }

            encryptData = RemoveMD5(encryptDataWithSig);
            decryptData = Decode(pLocation->cryptKey, encryptData, pLocation->cryptMethod);

            break;
        }

        case MP3_TYPE: { //获取隐藏在MP3中的Json信息

            encryptDataWithSig = MP3Parser::readJsonFromEnd(path);

            //检查md5值，如果不符合，则返回STRINGNULL
            if(!CheckMD5(encryptDataWithSig)){
                return STRING_NULL;
            }

            encryptData = RemoveMD5(encryptDataWithSig);
            decryptData = Decode(pLocation->cryptKey, encryptData, pLocation->cryptMethod);

            break;
        }

        case JPG_TYPE: {

            encryptDataWithSig = JPGParser::readJsonFromEnd(path);

            //检查md5值，如果不符合，则返回STRINGNULL
            if(!CheckMD5(encryptDataWithSig)){
                return STRING_NULL;
            }
            encryptData = RemoveMD5(encryptDataWithSig);
            decryptData = Decode(pLocation->cryptKey, encryptData, pLocation->cryptMethod);

            break;
        }
        default:
            break;
    }

    return JsonHelper::GetLocalJson(decryptData.data(), packageName, key);
}

bool StorageUtils::RemoveLocalData(const string &packageName, const string &key,
                                   LocationInfo *pLocation) {

    string path = pLocation->path + "/" + pLocation->fileName;

    if (!FileUtils::Exists(pLocation->path) || !FileUtils::CanOpen(path))
        return false;

    switch (pLocation->fileType) {

        case DB_TYPE: {

            string encryptDataWithSig = FileUtils::ReadAll(path);

//            if(!CheckMD5(encryptDataWithSig)){
////                return false;
//            }
            string encryptData = RemoveMD5(encryptDataWithSig);
            string decryptData = Decode(pLocation->cryptKey, encryptData, pLocation->cryptMethod);

            string json = JsonHelper::RemoveLocalJson(decryptData, packageName, key);

            if ("" == json) {
                return false;
            }
            string reEncryptData = Encode(pLocation->cryptKey, json, pLocation->cryptMethod);
            string reEncryptDataWithSig = AddMD5(reEncryptData);
            FileUtils::WriteAll(path, reEncryptDataWithSig);
            break;
        }
        case MP3_TYPE: {

            if (!MP3Parser::checkMp3(path)) {
                return false;
            }

            string mp3DataWithSig = MP3Parser::readJsonFromEnd(path);
//            if(!CheckMD5(mp3DataWithSig)){
////                return false;
//            }
            string mp3Data = RemoveMD5(mp3DataWithSig);
            string mp3DecryptCode = Decode(pLocation->cryptKey, mp3Data, pLocation->cryptMethod);
            string mp3Value = JsonHelper::RemoveLocalJson(mp3DecryptCode, packageName, key);

            if ("" == mp3Value)
                return false;

            string reMp3Encrypt = Encode(pLocation->cryptKey, mp3Value, pLocation->cryptMethod);
            string reMp3EncryptWithSig = AddMD5(reMp3Encrypt);
            MP3Parser::writeJsontoEnd(path, reMp3EncryptWithSig);
            break;
        }
        case JPG_TYPE: {

            if (!JPGParser::checkJpg(path)) {
                return false;
            }

            string jpgDataWithSig = JPGParser::readJsonFromEnd(path);
//            if(!CheckMD5(jpgDataWithSig)){
////                return false;
//            }
            string jpgData = RemoveMD5(jpgDataWithSig);
            string jpgDecryptCode = Decode(pLocation->cryptKey, jpgData, pLocation->cryptMethod);
            string jpgValue = JsonHelper::RemoveLocalJson(jpgDecryptCode, packageName, key);

            if ("" == jpgValue)
                return false;

            string reJpgEncrypt = Encode(pLocation->cryptKey, jpgValue, pLocation->cryptMethod);
            string reJpgEncryptWithSig = AddMD5(reJpgEncrypt);
            JPGParser::writeJsontoEnd(path, reJpgEncryptWithSig);
            break;

        }
        default:
            break;
    }

    return true;
}

//
//void b::PutSDCardFlag() {
//
//    int64_t random = OthersUtils::CreateRandom(9999999);
//    char tmp[12] = {0};
//    sprintf(tmp, "%lld", random);
//    string sRandom = tmp;
//
//    // 写入sp
//    string pkgName = get_pkg_name();
//    PutInSharedPreferences(pkgName, "sdFlag", sRandom, ".sd.db");
//
//    // 写入sd卡c
//    int sdCardStatus = get_sdcard_status();
//
//    if(sdCardStatus & 1 && GetWRPermission()) {
//
//        string sdcardPath = get_sdcard_dir();
//        string path = sdcardPath + "/.dsys";
//        string sdEncryptValue = Encode("9CABH6CCP7GTAq75j94EzX7s6eBbKgPF", sRandom, CRYPT_XXTEA);
//        string sdBase64SdEncryptValue = CryptUtils::Base64Encode(sdEncryptValue);
//        FileUtils::WriteAll(path, sdBase64SdEncryptValue);
//    }
//}
//
//
//bool b::IsSDCardChanged() {
//
//    int sdCardStatus = get_sdcard_status();
//
//    if((sdCardStatus & 1) && GetWRPermission()) {
//
//        // 读取sp
//        string pkgName = get_pkg_name();
//        string spFlag = GetInSharedPreferences(pkgName, "sdFlag", ".sd.db");
//
//        // 读取sd卡
//        string sdcardPath = get_sdcard_dir();
//        string path = sdcardPath + "/.dsys";
//        string sdFlag = FileUtils::ReadAll(path);
//        string sdBase64SdDecryptValue = CryptUtils::Base64Decode(sdFlag);
//        string sdDecryptValue = Decode("9CABH6CCP7GTAq75j94EzX7s6eBbKgPF", sdBase64SdDecryptValue, CRYPT_XXTEA);
//
//        return spFlag != sdDecryptValue;
//    }
//
//    return true;
//}


string StorageUtils::Encode(const string &cryptKey, const string &data, const int &cryptIndex) {

    string result;

    switch(cryptIndex) {
        case CRYPT_XXTEA:
            result = CryptUtils::XXTeaEncrypt(cryptKey, data);

            break;
        case CRYPT_AES:
            result = CryptUtils::AES_Encrypt(cryptKey, data);

            break;
        case CRYPT_RC4:
            result = CryptUtils::RC4_Encrypt(cryptKey, data);
            break;
    }

    return result;
}

string StorageUtils::Decode(const string &cryptKey, const string &data, const int &cryptIndex) {

    string result;

    switch(cryptIndex) {
        case CRYPT_XXTEA:
            result = CryptUtils::XXTeaDecrypt(cryptKey, data);
            break;
        case CRYPT_AES:
            result = CryptUtils::AES_Decrypt(cryptKey, data);
            break;
        case CRYPT_RC4:
            result = CryptUtils::RC4_Decrypt(cryptKey, data);
            break;
        default:
            break;
    }

    return result;
}

bool StorageUtils::PutInSharedPreferences(const string &packageName,const string &key, const string &value, const string &dbName) {

    string spfKey = packageName + "-" + key;
    string encryptSpfKey = Encode("Nn7BwDvtNPk3iDWJKEYI2BGqFKAnveU2", spfKey,CRYPT_XXTEA);
    string encryptValue = Encode("s1sgUUuU2lRoB2Jv30Boj7PVmoUmMz8f", value, CRYPT_XXTEA);
    string base64EncryptSpfKey = CryptUtils::Base64Encode(encryptSpfKey);
    string base64EncryptValue = CryptUtils::Base64Encode(encryptValue);

    return jni_putInSharedPreferences(base64EncryptSpfKey, base64EncryptValue, dbName);
}

string StorageUtils::GetInSharedPreferences(const string &packageName, const string &key, const string &dbName) {

    string spfKey = packageName + "-" + key;
    string encryptSpfKey = Encode("Nn7BwDvtNPk3iDWJKEYI2BGqFKAnveU2", spfKey, CRYPT_XXTEA);
    string base64EncrypySpfKey = CryptUtils::Base64Encode(encryptSpfKey);

    string base64EncryptSpfValue = jni_getInSharedPreferences(base64EncrypySpfKey, dbName);
    string encryptSpfValue = CryptUtils::Base64Decode(base64EncryptSpfValue);
    string result = Decode("s1sgUUuU2lRoB2Jv30Boj7PVmoUmMz8f", encryptSpfValue, CRYPT_XXTEA);

    return result;
}

bool StorageUtils::RemoveInSharedPreferences(const string &packageName, const string &key, const string &dbName){

    string spfKey = packageName + "-" + key;
    string encryptSpfKey = Encode("Nn7BwDvtNPk3iDWJKEYI2BGqFKAnveU2", spfKey, CRYPT_XXTEA);
    string base64EncrypySpfKey = CryptUtils::Base64Encode(encryptSpfKey);

    jni_removeInSharedPreferences(base64EncrypySpfKey, dbName);

    return true;
}

bool StorageUtils::PutInSetting(const string &packageName, const string &key, const string &value) {

    string settingKey = packageName + "-" + key;
    string encryptSettingKey = Encode("JAkacqbOvvq6UYzKWxLFgEvrTBk4yire", settingKey,CRYPT_XXTEA);
    string encryptValue = Encode("5FnXFrUOW4fkWMM5ldkM5krbjbE3oCkh", value, CRYPT_XXTEA);
    string base64EncryptSpfKey = CryptUtils::Base64Encode(encryptSettingKey);
    string base64EncryptValue = CryptUtils::Base64Encode(encryptValue);

    bool result  = jni_putInSetting(base64EncryptSpfKey, base64EncryptValue);

    return result;
}

string StorageUtils::GetInSetting(const string &packageName, const string &key) {

    string settingKey = packageName + "-" + key;
    string encryptSettingKey = Encode("JAkacqbOvvq6UYzKWxLFgEvrTBk4yire", settingKey, CRYPT_XXTEA);
    string base64EncrypySettingKey = CryptUtils::Base64Encode(encryptSettingKey);

    string base64EncrypySettingValue = jni_getInSetting(base64EncrypySettingKey);
    string encrypSettingValue = CryptUtils::Base64Decode(base64EncrypySettingValue);
    string settingValue = Decode("5FnXFrUOW4fkWMM5ldkM5krbjbE3oCkh", encrypSettingValue, CRYPT_XXTEA);

    return settingValue;
}

bool StorageUtils::RemoveInSetting(const string &packageName, const string &key) {

    string settingKey = packageName + "-" + key;
    string encryptSettingKey = Encode("JAkacqbOvvq6UYzKWxLFgEvrTBk4yire", settingKey, CRYPT_XXTEA);
    string base64EncrypySettingKey = CryptUtils::Base64Encode(encryptSettingKey);

    jni_removeInSetting(base64EncrypySettingKey);

    return true;
}

bool StorageUtils::PutInSQLite(const string &packageName, const string &key, const string &value) {

    string SQLiteKey = packageName + "-" + key;
    string encryptSQLiteKey = Encode("YlbdPJz6TK9FomWdpal4rXnoJ70MlnxW", SQLiteKey,CRYPT_XXTEA);
    string encryptValue = Encode("CtUiU8ToSPL7pbI2RqERfmHUfqimTVQK", value, CRYPT_XXTEA);
    string base64EncryptSQLiteKey = CryptUtils::Base64Encode(encryptSQLiteKey);
    string base64EncryptValue = CryptUtils::Base64Encode(encryptValue);

    return jni_putInSQLite(base64EncryptSQLiteKey, base64EncryptValue);
}

string StorageUtils::GetInSQLite(const string &packageName, const string &key){

    string SQLiteKey = packageName + "-" + key;
    string encryptSQLiteKey = Encode("YlbdPJz6TK9FomWdpal4rXnoJ70MlnxW", SQLiteKey, CRYPT_XXTEA);
    string base64EncrypySQLiteKey = CryptUtils::Base64Encode(encryptSQLiteKey);

    string base64EncrypySQLiteValue = jni_getInSQLite(base64EncrypySQLiteKey);
    string encrypSQLiteValue = CryptUtils::Base64Decode(base64EncrypySQLiteValue);

    return Decode("CtUiU8ToSPL7pbI2RqERfmHUfqimTVQK", encrypSQLiteValue, CRYPT_XXTEA);
}

bool StorageUtils::RemoveInSQLite(const string &packageName, const string &key) {

    string SQLiteKey = packageName + "-" + key;
    string encryptSQLiteKey = Encode("YlbdPJz6TK9FomWdpal4rXnoJ70MlnxW", SQLiteKey, CRYPT_XXTEA);
    string base64EncrypySQLiteKey = CryptUtils::Base64Encode(encryptSQLiteKey);

    return jni_removeInSQLite(base64EncrypySQLiteKey);
}

bool StorageUtils::PutInJavaApi(const string &packageName, const string &key, const string &value) {

    string dbName = ".sf.db";

    return PutInSharedPreferences(packageName, key, value, dbName) &
           PutInSetting(packageName, key, value) &
           PutInSQLite(packageName, key, value);
}


bool StorageUtils::RemoveInJavaApi(const string &packageName, const string &key) {

    string dbName = ".sf.db";

    return RemoveInSharedPreferences(packageName, key, dbName) & RemoveInSQLite(packageName, key) & RemoveInSetting(packageName, key);
}


bool StorageUtils::GetInJavaApi(const string &packageName, const string &key, string *result) {

    string dbName = ".sf.db";
    result[0] = GetInSetting(packageName, key);
    result[1] = GetInSQLite(packageName, key);
    result[2] = GetInSharedPreferences(packageName, key, dbName);

    return true;
}

string
JsonHelper::GetLocalJson(const string &jsonStr, const string &packageName, const string &key) {

    string err;
    Json jsonRoot = json11::Json::parse(jsonStr, err);

    if (!err.empty()) {
        return "";
    }

    map<string, Json> appsMap;
    map<string, Json> keyMap;

    appsMap = jsonRoot.object_items();

    if (appsMap.find(packageName) == appsMap.end())
        return "";

    keyMap = appsMap[packageName].object_items();

    if (keyMap.find(key) == keyMap.end())
        return "";

    return keyMap[key].string_value();
}

string JsonHelper::PutLocalJson(const string &jsonStr, const string &packageName,
                                const string &key, const string &value) {
    string err;
    map<string, Json> appsMap; // packageName -> tokenType
    map<string, Json> keyMap;// tokenType -> (Json)tokenValue
    Json valJson(value);

    Json root = json11::Json::parse(jsonStr, err);

    if (err.empty()) {

        appsMap = root.object_items();

        //packageName不存在
        if (appsMap.find(packageName) == appsMap.end()) {

            keyMap.insert(make_pair(key, value));

            Json json_keyMap(keyMap);
            appsMap.insert(make_pair(packageName, json_keyMap));

        } else {//package存在

            keyMap = appsMap[packageName].object_items();

            //tokenType存在，则覆盖
            if (keyMap.find(key) != keyMap.end()) {

                keyMap[key] = valJson;
                //tokenType不存在，则插入
            } else {

                keyMap.insert(make_pair(key, valJson));

            }

            Json json_keyMapMap(keyMap);
            appsMap[packageName] = json_keyMapMap;
        }

        Json json_appsMap(appsMap);

        return json_appsMap.dump();

    } else {//此处如果解析出错，则清空并重新写入了Locals


        string json_str = root.dump();
        //info_log(err);
        map<string, Json> rootMap;
        Json json_Null("");

        keyMap.insert(make_pair(key, valJson));
        Json json_tokenMap(keyMap);

        appsMap.insert(make_pair(packageName, json_tokenMap));
        Json json_appsMap(appsMap);

        return json_appsMap.dump();
    }
}

string JsonHelper::RemoveLocalJson(const string &jsonStr, const string &packageName,
                                   const string &key) {
    string err;
    Json root = json11::Json::parse(jsonStr, err);

    if (!err.empty())
        return jsonStr;

    map<string, Json> appMap;
    map<string, Json> keyMap;

    appMap = root.object_items();

    if (appMap.find(packageName) == appMap.end())
        return jsonStr;

    keyMap = appMap[packageName].object_items();

    if (keyMap.find(key) == keyMap.end())
        return jsonStr;

    keyMap.erase(key);

    if(keyMap.size() == 0)
        appMap.erase(packageName);
    else
        appMap[packageName] = keyMap;

    Json json_appsMap(appMap);
    return json_appsMap.dump();
}

bool MP3Parser::writeJsontoEnd(const string &filePath, const string &data) {

    string fileData;
    if (!FileUtils::Exists(filePath)) {
        return false;
    }

    fileData = FileUtils::ReadAll(filePath);
    unsigned long pos = MP3Parser::findTAGPos(fileData);

    //不存在TAG段
    if (pos == -1) {
        return false;
    }

    string dataEnd = fileData.substr(0, pos + 3);
    string apendData = dataEnd.append(data).append("=ADC");
    FileUtils::WriteAll(filePath, apendData);

    return true;
}

//返回mp3文件TAG字段的下标
unsigned long MP3Parser::findTAGPos(const string &data) {

    unsigned long pos = data.rfind("TAG", (int) data.length());

    return pos;
}

string MP3Parser::readJsonFromEnd(const string &filePath) {

    string fileData;
    unsigned long fileSize;
    if (!FileUtils::Exists(filePath)) {
        return "";
    }

    fileData = FileUtils::ReadAll(filePath);
    fileSize = fileData.length();

    unsigned long pos = MP3Parser::findTAGPos(fileData);

    //不存在TAG段
    if (pos == -1) {
        return "";
    }

    string strTAG = fileData.substr(pos, 3);
    unsigned long jsonLength = fileSize - pos - 7;
    string result = fileData.substr(pos + 3, jsonLength);
    return result;
}

//检查MP3文件是否有从文件末尾插入的标志末尾TAG: "=ADC",用于标识此文件被存储uuid
bool MP3Parser::checkMp3(const string &filePath) {

    bool result = false;

    if (!FileUtils::Exists(filePath) && !FileUtils::CanOpen(filePath)) {
        result = false;
    }

    FILE *fp = fopen(filePath.c_str(), "r");

    if (fp) {
        char tagData[5] = { 0 };

        fseek(fp, -4, SEEK_END);
        fread(tagData, sizeof(char), 4, fp);

        tagData[4] = 0;

        result = !strcmp(tagData, "=ADC");

        fclose(fp);
    }

    return result;
}

bool MP3Parser::insertTag(const string &filePath) {

    if (!FileUtils::Exists(filePath)) {
        return false;
    }

    if (MP3Parser::checkMp3(filePath)) {
        return true;
    }

    FileUtils::WriteAll(filePath, "TAG=ADC", true);

    return true;
}

bool JPGParser::writeJsontoEnd(const string &filePath, const string &data) {

    string fileData;
    if (!FileUtils::Exists(filePath)) {
        return false;
    }

    fileData = FileUtils::ReadAll(filePath);
    unsigned long pos = MP3Parser::findTAGPos(fileData);

    //不存在TAG段
    if (pos == -1) {
        return false;
    }

    string dataEnd = fileData.substr(0, pos + 3);
    string apendData = dataEnd.append(data).append("=ADC");
    FileUtils::WriteAll(filePath, apendData);

    return true;
}

string JPGParser::readJsonFromEnd(const string &filePath) {

    string fileData;
    unsigned long fileSize;
    if (!FileUtils::Exists(filePath)) {
        return "";
    }

    fileData = FileUtils::ReadAll(filePath);
    fileSize = fileData.length();

    unsigned long pos = JPGParser::findTAGPos(fileData);

    //不存在TAG段
    if (pos == -1) {
        return "";
    }

    string strTAG = fileData.substr(pos, 3);
    unsigned long jsonLength = fileSize - pos - 7;
    string result = fileData.substr(pos + 3, jsonLength);
    return result;
}

unsigned long JPGParser::findTAGPos(const string &data) {

    unsigned long pos = data.rfind("TAG", (int) data.length());

    if (pos == -1) {

    }

    return pos;
}

bool JPGParser::checkJpg(const string &filePath) {

    bool result = false;

    if (!FileUtils::Exists(filePath) && !FileUtils::CanOpen(filePath)) {
        result = false;
    }

    FILE *fp = fopen(filePath.c_str(), "r");

    if (fp) {
        char tagData[5] = { 0 };

        fseek(fp, -4, SEEK_END);
        fread(tagData, sizeof(char), 4, fp);

        tagData[4] = 0;

        result = !strcmp(tagData, "=APC");

        fclose(fp);
    }

    return result;
}

bool JPGParser::insertTag(const string &filePath) {

    if (JPGParser::checkJpg(filePath)) {
        return true;
    }

    FileUtils::WriteAll(filePath, "TAG=APC", true);

    return true;
}


/*
 * @param :输入必须是未加签的数据
 * @return : 新加签的数据
 */
string StorageUtils::AddMD5(const string &data) {

    string strMD5 = CryptUtils::MD5(data);
    string ret = strMD5 + data;
    return ret;

}
/*
 * @param :输入必须是已加签的数据
 * @return : 去除掉签的数据
 */
string StorageUtils::RemoveMD5(const string &data) {

    if (data.length() < 32){
        return STRING_NULL;
    }

    string removeMD5 = data.substr(32);
    return removeMD5;
}
/*
 * @param :输入必须是已加签的数据
 * @return : 校验签与数据,true为签和数据一致
 */
bool StorageUtils::CheckMD5(const string &data) {

    if (data.length() < 32){
        return false;
    }
    string MD5Data = data.substr(0, 32);
    string realData = data.substr(32);
    string realDataMd5 = CryptUtils::MD5(realData);

    bool result = realDataMd5 == MD5Data;
    return result;
}
