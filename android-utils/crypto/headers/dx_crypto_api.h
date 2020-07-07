//
// Created by white on 2018/8/28.
//

#ifndef AS_WHITEBOX_DX_CRYPTO_MAIN_H
#define AS_WHITEBOX_DX_CRYPTO_MAIN_H



#include <string>
#include <vector>
#include "ioapi_mem.h"
#include "unzip.h"
#include "zip.h"
#include "json11.h"
#include "pkcs7.h"
#include "dx_utils_crypto_prefix.h"



using namespace std;
using namespace json11;

//#ifdef __cplusplus
//extern "C" {
//#endif


/**
 * 加解密工具类
 */
class CryptUtils {

private:
    CryptUtils() {};

public:

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
     * 带Key的MD5运算
     */
    static string MD5WithKey(const string &src, const string &key);

    /**
     * SHA1运算
     */
    static string SHA1(const string &src);

    /**
     * 带Key的SHA1运算
     */
    static string SHA1WithKey(const string &src, const string &key);

    /**
     * SHA256运算
     * @param src
     * @return
     */
    static string SHA256(const string &src);

    static string SHA256WithKey(const string &src, const string &key);

    /**
     * rc4 加密
     * @param key
     * @param input
     * @return
     */
    static string RC4(const string &key, const string &input);

    /**
     * Hex String to char *
     * 返回数据的个数
     */
    static int HexStringToChar(const string &str, unsigned char *out);

    static string CharToHexString(unsigned char *pSrc, int nL);

    /**
     * aes ecb 加密
     * @param key
     * @param src
     * @return
     */
    static string AesEcbEncrypt(const string &key, const string &input);


    /**
     * aes ecb 解密
     * @param key
     * @param src
     * @return
     */
    static string AesEcbDecrypt(const string &key, const string &input);

    /**
     * aes cbc 加密
     * @param key
     * @param src
     * @return
     */
    static string AesCbcEncrypt(const string &key, const string &iv, const string &input);


    /**
     * aes cbc 解密
     * @param key
     * @param src
     * @return
     */
    static string AesCbcDecrypt(const string &key, const string &iv, const string &input);



    /**
     * 白盒aes 加密
     * @param key
     * @param input
     * @return
     */
    static string WBAesEncrypt(const string &key, const string &input);

    /**
     * 白盒aes 解密
     * @param key
     * @param input
     * @return
     */
    static string WBAesDecrypt(const string &key, const string &input);

    /**
     * sm3 加密
     * @param key
     * @param input
     * @return
     */
    static string SM3Encrypt(const string &key, const string &input);


    /**
     * sm4 加密
     * @param key
     * @param input
     * @return
     */
    static string SM4Encrypt(const string &key, const string &input);

    /**
     * sm4 解密
     * @param key
     * @param input
     * @return
     */
    static string SM4Decrypt(const string &key, const string &input);


    /**
     * MT马涛 加密
     * @param key
     * @param input
     * @return
     */
    static string MTKeyEncrypt(const string &key, const string &input);

    /**
     * MT马涛 解密
     * @param key
     * @param input
     * @return
     */
    static string MTKeyDecrypt(const string &key, const string &input);


    static bool isValidKeyByAES(const string &key);

    static bool isValidKeyByWBAES(const string &key);

    static bool isValidKeyBySM4(const string &key);

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
    const string &GetFileData(const string &path);

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


//#ifdef __cplusplus
//}
//#endif

#endif //AS_WHITEBOX_DX_CRYPTO_MAIN_H
