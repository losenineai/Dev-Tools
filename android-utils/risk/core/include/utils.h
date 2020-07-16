//
// Created by boyliang on 2017/5/22.
//

#ifndef DEVICEFINGERPRINT_UTILS_H
#define DEVICEFINGERPRINT_UTILS_H

#include <string>
#include <vector>
#include <stdint.h>
#include <errno.h>
#include <zlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include "DXLog.h"
#include "thirdpart/zip/zip.h"
#include "thirdpart/zip/unzip.h"
#include "thirdpart/zip/ioapi_mem.h"
#include "stee_base.pb-c.h"
#include "stee.h"
#include "json11.h"
#include "core_prefix.h"
#include "pb/stee_riskmgr.pb-c.h"
#include "pb/pb_prefix.h"

using namespace std;
using namespace json11;

//常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}
#define delete_not_array(x) if((x)){delete (x);(x) = NULL;}
#define errmsg strerror(errno)
#define synchronized(lock) lock_guard<recursive_mutex> __lg__(lock)

/**
 * 加解密工具类
 */
class CryptUtils {

private:
    CryptUtils() {};

public:

    /*
     * rc4加密
     */
    static string RC4_Encrypt(const string &key, const string &src);

    /*
     * rc4
     */
    static string RC4_Decrypt(const string &key, const string &src);

    /**
     * AES_MINI加密
     */
    static string AES_Encrypt(const string &key, const string &src);

    /**
     * AES_MINI解密
     */
    static string AES_Decrypt(const string &key, const string &src);
    /**
     * XXTea加密
     */
    static string XXTeaEncrypt(const string &key, const string &src);

    /**
     * XXTea解密
     */
    static string XXTeaDecrypt(const string &key, const string &src);

    /**
     * Base64编码加密
     */
    static string Base64Encode(const string &src);

    /**
     * Base64编码解密
     */
    static string Base64Decode(const string &src);

    /**
     * MD5运算
     */
    static string MD5(const string &src);

    /**
     * SHA1运算
     */
    static string SHA1(const string &src);

    /**
     * OpenSSL中hash算法
     * @param str
     * @return
     */
    static unsigned int StrHash(const char *str);
};

/**
 * Json工具类
 */
class JSonUtils {

private:
    JSonUtils();

public:
    static string JSon2string(Json &json) {
        return json.dump();
    }

    static Json string2JSon(string json_str) {
        string error;
        Json json = Json::parse(json_str, error);
        if (!error.empty()) {
            return json;
        } else {
            return json;
        }
    }

};

#define DXRISK_REQUEST_NETWORK_ERR            -1001
#define DXRISK_REQUEST_DECRYPT_ERR            -1002
#define DXRISK_REQUEST_UNCOMPRESS_ERR         -1003
#define DXRISK_REQUEST_RESPONSE_EMPTY_ERR     -1004
#define DXRISK_REQUEST_DATA_PARSE_ERR         -1005
#define DXRISK_REQUEST_DIRTY_DATA_ERR         -1006
#define DXRISK_CONST_ID_EMPTY                 -1007
#define DXRISK_REQUEST_LIMIT                  -1008 // 服务器限流

class OthersUtils {

private:
    OthersUtils();

public:

    enum class HWEERR {
        NOERROR,
        NETWORK_ERR,
        DECRYPT_ERR,
        UNCOMPREESS_ERR
    };

    enum class OS_BIT {
        W32,
        W64
    };

    /**
     * 加压缩和加密的HttpPost请求
     * @param url
     * @param data
     * @param respone
     * @param appkey
     * @return -1 表示网络不通或者网络超时；-2 表示解密失败；-3表示解压失败
     */
    static HWEERR HttpWithEncrypt(const string &url, const string &data, string &respone, const string &appId, const bool isSaas);

    static void AsyncHttpWithEncrypt(const string &url, const string &data, string &respone, const string &appId, const bool isSaas);

    static int64_t CurrentTimes();

    static int64_t CurrentTimesBySec();

    static Timestamp CurrentTimesstamp();

    static string
    CreateSTEERequestRaw(STEEDataType type, const string &appId, const ProtobufCBinaryData &data);

    static string
    CreateSTEERequestRaw(STEEDataType type, const string &appId, const map<string, string> &data_map);

#ifndef __APPLE__

    static string ExecuteCMD(const string &cmd, const string &filter = "");

#endif

    static inline OS_BIT GetOSBit() {

#if defined(__i386__) || defined(__arm__)
        return OS_BIT::W32;
#else
        return OS_BIT::W64;
#endif
    }

    static inline uint64_t CreateRandom(int base){
        srand(OthersUtils::CurrentTimes());
        return (uint64_t)rand() % base;
    }

    static inline void SleepMS(uint64_t ms){
        usleep(ms * 1000);
    }

    static inline void SleepSEC(uint32_t sec){
        SleepMS(sec * 1000);
    }
    //产生指定数量的不重复随机数
    static void SetRandom(int a[], int count, int max);

    //产生一个随机数
    static int Rand(void);

    //设置种子
    static void SRand(uint32_t seed);

    static uint32_t nextSeed;
private:

};
/**
 * 文件工具类
 */
class FileUtils {

private:
    FileUtils() {};

public:
    static bool Exists(const string &name);

    static bool CanOpen(const string &name);

    static string ReadAll(const string &name, unsigned long limitsize = 1024 * 1024 * 32);

    static vector<string> ReadLines(const string &name);

    static string ReadLine(const string &name);

    static void WriteAll(const string &name, const string &data, bool append = false);

    static void DeleteIfExist(const string &name);

    static bool MakeDir(const string &name, int mode = 0755);

    static vector<string> List(const string &dir, const vector<string> &filters = vector<string>());
};

/**
 * 字符串工具类
 */
class StringUtils {

private:
    StringUtils() { }

public:

    static void   SplitString(const string &s, vector<string> &v, const string &c);

    static string Format(const char *fmt, ...);

    static string ToHexString(const string &input);

    static char*  Int64ToChar(int64_t value);

    static string Int64ToString(int64_t value);

    static string FindString(string *strArray, int sum);

    static string Trim(string &input);

    static char*  StringToChar(const string& s);
};

#define ZMODE_ZIPPER    0
#define ZMODE_UNZIPPER  1

/**
 * Zip文件处理，目前只提供解压相关功能, 带有缓存功能
 */
class ZipFile {

private:
    int zipMode_;
    unzFile unzFile_;
    zipFile zipFile_;
    const string password_;
    map<string, string> nameDataMap_;
    ourmemory_t *pZipData_;
    bool closeState_;

private:
    ZipFile(int mode, void *handler, const string &password, ourmemory_t *pdata);

public:

    /**
     * 简化方法，zip压缩
     * @param input
     * @return
     */
    static string Compress(const string &input);

    /**
     * 简化方法，unzip解压
     * @param input
     * @return
     */
    static string UnCompress(const string &input);

public:

    virtual ~ZipFile();

    /**
     * 从文件中读取un_zipper
     */
    static ZipFile *CreateUZipperFromFile(const string &path, const string &password = "");

    /**
     * 从内存中读取unzipper
     */
    static ZipFile *CreateUZipperFromData(const string &rawdata, const string &password = "");

    /**
     * 在内存中创建zipper
     */
    static ZipFile *CreateZipperFromData(const string &password = "");

    /**
     * 判断文件是否存在
     */
    bool CheckIsExist(const string &path);

    /**
     * 根据路径获取文件，如果路径指向文件夹或者文件不存在，返回内容为空
     */
    const void GetFileData(const string &path, string &data);

    /**
     * 获取文件内容到Zip文件中
     * @param name
     * @param data
     */
    void AddFileData(const string &name, const string &data);

    void Close();

    string GetZipFileData();

    vector<string> TraversalZipFile();
};




#endif //DEVICEFINGERPRINT_UTILS_H
