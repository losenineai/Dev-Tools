//
// Created by white on 2018/8/28.
//

#include "dx_crypto_api.h"
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <zlib.h>
#include <dirent.h>
#include <bitset>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "zip.h"
#include "hmac_sha1.h"
#include "hmac_md5.h"
#include "md5.h"
#include "base64.h"
#include "xxtea.h"
#include "sm3.h"
#include "hmac_sm3.h"
#include "sm4.h"
#include "wbox.h"
#include "sha2_mini.h"
#include "hmac_sha256.h"
#include "aes_mini.h"
#include "aes.h"
#include "rc4.h"
#include "common.h"
#include "mt_kp.h"
#include "dx_utils_crypto_stee.h"


using namespace std;
#define MAX_FILENAME 1024


//常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}
#define delete_not_array(x) if((x)){delete (x);(x) = NULL;}
#define errmsg strerror(errno)
#define synchronized(lock) lock_guard<recursive_mutex> __lg__(lock)

const string STRING_NULL = "";



STEE
string CryptUtils::MD5(const string &src) {

    uint8_t signature[16];

    md5((const unsigned char *) src.data(), src.size(), signature);

    string result((char *) signature, 16);

    return CryptUtils::CharToHexString((unsigned char *)result.c_str(), result.size());
}

STEE
string CryptUtils::MD5WithKey(const string &src, const string &key) {
    uint8_t signature_hmac[16];

    hmac_md5((const unsigned char *) key.data(), key.size(), (const unsigned char *) src.data(),
             src.size(), signature_hmac);

    string result_hmac((char *) signature_hmac, 16);

    return CryptUtils::CharToHexString((unsigned char *)result_hmac.data(), result_hmac.size());
}

STEE
string CryptUtils::SHA1(const string &src) {
    uint8_t sha1_result[20];

    sha1(sha1_result, src.data(), src.size() * 8);
    string result_str((char *) sha1_result, 20);

    return CryptUtils::CharToHexString((unsigned char *)result_str.data(), result_str.size());
}

STEE
string CryptUtils::SHA1WithKey(const string &src, const string &key) {
    uint8_t hmac_sha1_result[20];
    hmac_sha1(hmac_sha1_result, key.data(), key.size() * 8, src.data(), src.size() * 8);
    string hmac_result_str((char *) hmac_sha1_result, 20);

    return CryptUtils::CharToHexString((unsigned char *)hmac_result_str.data(), hmac_result_str.size());
}

string CryptUtils::SHA256(const string &src) {
    uint8_t buf[SHA256MINI_HASHLEN];
    SHA256Mini(src.data(), src.size(), buf);
    string input((char *) buf, SHA256MINI_HASHLEN);

    return CryptUtils::CharToHexString((unsigned char *)input.data(), input.size());
}

STEE
string CryptUtils::SHA256WithKey(const string &src, const string &key) {
    uint8_t buf[SHA256MINI_HASHLEN];

    hmac_sha256(buf, (const uint8_t *) src.data(), src.size(), (const uint8_t *) key.data(),
                key.size());

    string input((char *) buf, SHA256MINI_HASHLEN);
    string result = CryptUtils::CharToHexString((unsigned char *)input.data(), input.size());

    return result;
}

STEE
string CryptUtils::Base64Encode(const string &input) {
    size_t encode_size = BASE64_ENCODE_OUT_SIZE(input.size());
    char *base64_encode_buf = new char[encode_size];
    memset(base64_encode_buf, 0, encode_size);

    string output;
    base64_encode_imp((const uint8_t *) input.data(), input.size(), base64_encode_buf);
    output.assign(base64_encode_buf, encode_size);
    delete_array(base64_encode_buf);

    return output;
}

STEE
string CryptUtils::Base64Decode(const string &input) {
    size_t decode_size = BASE64_DECODE_OUT_SIZE(input.size());

    for (int i = input.size() - 1; i > -1; i--) {
        if (input[i] == '=') {
            decode_size--;
        } else {
            break;
        }
    }

    unsigned char *base64_decode_buf = new unsigned char[decode_size];
    memset(base64_decode_buf, 0, decode_size);

    string output;
    int res_code = base64_decode_imp(input.data(), input.size(), base64_decode_buf);
    if (res_code == BASE64_OK) {
        output.assign((char *) base64_decode_buf, decode_size);
    }

    delete_array(base64_decode_buf);
    return output;
}

STEE
string CryptUtils::XXTeaEncrypt(const string &key, const string &src) {
    size_t sz;
    void *result = NULL;

    string output;
    result = xxtea_encrypt_in(src.data(), src.size(), key.data(), &sz);
    if (result != NULL) {
        output.assign((char *) result, sz);
        free(result);
    }

    return output;
}

STEE
string CryptUtils::XXTeaDecrypt(const string &key, const string &input) {
    size_t sz;
    void *result = NULL;

    string output;
    result = xxtea_decrypt_in(input.data(), input.size(), key.data(), &sz);
    if (result != NULL) {
        output.assign((char *) result, sz);
        free(result);
    }

    return output;
}

STEE
int CryptUtils::HexStringToChar(const string &src, unsigned char *dest) {
    unsigned char hb;
    unsigned char lb;

    int j = 0;

    if (src.size() % 2 != 0)
        return j;

    for (int i = 0; i < src.size(); i++) {
        hb = src[i];
        if (hb >= 'a' && hb <= 'f')
            hb = hb - 'a' + 10;
        else if (hb >= '0' && hb <= '9')
            hb = hb - '0';
        else
            return j;

        i++;
        lb = src[i];
        if (lb >= 'a' && lb <= 'f')
            lb = lb - 'a' + 10;
        else if (lb >= '0' && lb <= '9')
            lb = lb - '0';
        else
            return j;

        dest[j++] = (hb << 4) | (lb);
    }
    return j;
}

STEE
string CryptUtils::CharToHexString(unsigned char *pSrc, int nL) {
    char buf[nL * 2 + 1];
    string dest;

    memset((char *) buf, 0, sizeof(buf));

    unsigned char hb;
    unsigned char lb;

    for (int i = 0; i < nL; i++) {
        hb = (pSrc[i] & 0xf0) >> 4;

        if (hb >= 0 && hb <= 9)
            hb += 0x30;
        else if (hb >= 10 && hb <= 15)
            hb = hb - 10 + 'a';
        else
            return dest;

        lb = pSrc[i] & 0x0f;
        if (lb >= 0 && lb <= 9)
            lb += 0x30;
        else if (lb >= 10 && lb <= 15)
            lb = lb - 10 + 'a';
        else
            return dest;

        buf[i * 2] = hb;
        buf[i * 2 + 1] = lb;
    }

    dest = buf;

    return dest;
}



STEE
ZipFile::ZipFile(int mode, void *handler, const string &password, ourmemory_t *pData) :
        zipMode_(mode),
        unzFile_(NULL),
        zipFile_(NULL),
        password_(password),
        pZipData_(pData),
        closeState_(false) {

    if (mode == ZMODE_UNZIPPER) {
        unzFile_ = handler;
    } else {
        zipFile_ = handler;
    }
}

STEE
ZipFile::~ZipFile() {
    if (unzFile_ != NULL) {
        unzClose(unzFile_);
        unzFile_ = NULL;
    }

    if (zipFile_ != NULL) {
        zipClose(zipFile_, NULL);
        zipFile_ = NULL;
    }


    if (pZipData_) {
        delete_array(pZipData_->base);
        delete_not_array(pZipData_);
    }
}

STEE
ZipFile *ZipFile::CreateUZipperFromFile(const string &path, const string &password) {
    unzFile zf = NULL;

    if (access(path.c_str(), R_OK) == 0) {
        zf = unzOpen(path.c_str());
    }

    return zf != NULL ? new ZipFile(ZMODE_UNZIPPER, zf, password, NULL) : NULL;
}

STEE
ZipFile *ZipFile::CreateUZipperFromData(const string &rawdata, const string &password) {

    if (rawdata.empty()) {
        return NULL;
    }

    zlib_filefunc_def func_def;
    ourmemory_t *mem = new ourmemory_t;
    memset(mem, 0, sizeof(ourmemory_t));

    mem->base = new char[rawdata.size()];
    mem->size = rawdata.size();
    memcpy(mem->base, rawdata.c_str(), mem->size);

    fill_memory_filefunc(&func_def, mem);
    unzFile zf = unzOpen2("__notused__", &func_def);

    if (zf != NULL) {
        return new ZipFile(ZMODE_UNZIPPER, zf, password, mem);
    } else {
        delete_array(mem->base);
        delete_not_array(mem);
        return NULL;
    }
}

/**
     * 在内存中创建zipper
     */
STEE
ZipFile *ZipFile::CreateZipperFromData(const string &password) {

    zlib_filefunc_def func_def = {0};
    ourmemory_t *mem = new ourmemory_t;
    memset(mem, 0, sizeof(ourmemory_t));
    mem->grow = 1;

    fill_memory_filefunc(&func_def, mem);
    zipFile zf = zipOpen3("__notused__", APPEND_STATUS_CREATE, 0, 0, &func_def);

    if (zf != NULL) {
        return new ZipFile(ZMODE_ZIPPER, zf, password, mem);
    } else {
        delete_array(mem->base);
        delete_not_array(mem);
        return NULL;
    }
}

STEE
bool ZipFile::CheckIsExist(const string &path) {
    unz_file_info info;
    return unzLocateFile(unzFile_, path.c_str(), NULL) == UNZ_OK
           && unzGetCurrentFileInfo(unzFile_, &info, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK;
}

STEE
const string &ZipFile::GetFileData(const string &path) {
    auto value_iterator = nameDataMap_.find(path);

    if (value_iterator == nameDataMap_.end()) {
        int res = unzLocateFile(unzFile_, path.c_str(), NULL);

        if (res != UNZ_END_OF_LIST_OF_FILE) {
            unz_file_info info;
            if (unzGetCurrentFileInfo(unzFile_, &info, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK
                && ((!password_.empty() &&
                     unzOpenCurrentFilePassword(unzFile_, password_.data()) == UNZ_OK) ||
                    (unzOpenCurrentFile(unzFile_) == UNZ_OK))) {

                char *buf = new char[info.uncompressed_size];
                res = unzReadCurrentFile(unzFile_, buf, (unsigned) info.uncompressed_size);

                if (res > 0) {
                    nameDataMap_[path] = string(buf, info.uncompressed_size);
                }

                delete_array(buf);
                unzCloseCurrentFile(unzFile_);
            }
        }

        value_iterator = nameDataMap_.find(path);
        return value_iterator != nameDataMap_.end() ? value_iterator->second : STRING_NULL;
    } else {
        return value_iterator->second;
    }
}

STEE
void ZipFile::AddFileData(const string &name, const string &data) {
    time_t tm_t;
    struct tm *current_date;

    time(&tm_t);
    current_date = localtime(&tm_t);

    zip_fileinfo zi;
    memset(&zi, sizeof(zi), 0);

    int err = zipOpenNewFileInZip(zipFile_, name.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED,
                                  5);
    if (err == ZIP_OK) {
        err = zipWriteInFileInZip(zipFile_, data.c_str(), data.length());
        if (err == ZIP_OK) {
            zipCloseFileInZip(zipFile_);
        }
    }
}

STEE
void ZipFile::Close() {
    if (!closeState_) {
        closeState_ = true;

        if (unzFile_ != NULL) {
            unzClose(unzFile_);
            unzFile_ = NULL;
        }

        if (zipFile_ != NULL) {
            zipClose(zipFile_, NULL);
            zipFile_ = NULL;
        }
    }
}

STEE
string ZipFile::GetZipFileData() {
    ourmemory_t *om = (ourmemory_t *) pZipData_;
    if (om && om->base && closeState_) {
        if (zipMode_ == ZMODE_ZIPPER) {
            return string(om->base, om->limit);
        } else if (zipMode_ == ZMODE_UNZIPPER) {
            return string(om->base, om->limit);
        }
    }

    return STRING_NULL;
}

/**
    * 简化方法，zip压缩
    * @param input
    * @return
    */
STEE
string ZipFile::Compress(const string &input) {
    ZipFile *zf = ZipFile::CreateZipperFromData();
    zf->AddFileData("data", input);
    zf->Close();

    string tmp = zf->GetZipFileData();
    delete_not_array(zf);

    return tmp;
}

/**
 * 简化方法，unzip解压
 * @param input
 * @return
 */
STEE
string ZipFile::UnCompress(const string &input) {
    ZipFile *zf = ZipFile::CreateUZipperFromData(input);
    string tmp = STRING_NULL;

    if (zf != NULL) {
        tmp = zf->GetFileData("data");
    }

    delete_not_array(zf);

    return tmp;
}

STEE
vector<string> ZipFile::TraversalZipFile() {
    vector<string> fileNameList;
    unz_global_info globalInfo;
    if (unzGetGlobalInfo(unzFile_, &globalInfo) != UNZ_OK) {
        return fileNameList;
    }

    for (int i = 0; i < globalInfo.number_entry; ++i) {
        unz_file_info file_info;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(
                unzFile_,
                &file_info,
                fileName,
                MAX_FILENAME,
                NULL, 0, NULL, 0) != UNZ_OK) {
            return fileNameList;
        }

        fileNameList.push_back(fileName);

        if ((i + 1) < globalInfo.number_entry) {
            if (unzGoToNextFile(unzFile_) != UNZ_OK) {
                break;
            }
        }
    }
    return fileNameList;
}


STEE
string CryptUtils::AesEcbEncrypt(const string &key, const string &input) {
    

    AESMini_ctx ctx;
    string kk(key);
    if (kk.length() == AESMINI_128BIT_KEY
        || kk.length() == AESMINI_192BIT_KEY
        || kk.length() == AESMINI_256BIT_KEY) {

        AESMini_Init(&ctx, (const uint8_t *) kk.data(), (uint32_t) kk.length());
    }

    int STEP = 16;
    int total_size = (int) (input.length() + STEP) / STEP * STEP;

    uint8_t *cipher = new uint8_t[total_size];
    memcpy(cipher, (const uint8_t *) input.data(), input.length());
    uint8_t padding = (uint8_t) STEP - (uint8_t) (input.length() % STEP);

    for (int i = (int) input.length(); i < total_size; i++) {
        cipher[i] = padding;
    }

    for (int i = 0; i < total_size; i += STEP) {
        AESMini_ECB_Encrypt(&ctx, cipher + i, cipher + i);
    }

    string res((const char *) cipher, (uint32_t) total_size);
    delete_array(cipher);

    
    
    return res;
}

STEE
string CryptUtils::AesEcbDecrypt(const string &key, const string &input) {
    

    AESMini_ctx ctx;
    string kk(key);
    if (kk.length() == AESMINI_128BIT_KEY
        || kk.length() == AESMINI_192BIT_KEY
        || kk.length() == AESMINI_256BIT_KEY) {

        AESMini_Init(&ctx, (const uint8_t *) kk.data(), (uint32_t) kk.length());
    }

    int STEP = 16;

    if (input.length() % STEP != 0) {
        return STRING_NULL;
    }

    uint8_t *plain = new uint8_t[input.length()];
    memcpy(plain, (const uint8_t *) input.data(), input.length());

    for (int i = 0; i < input.length(); i += STEP) {
        AESMini_ECB_Decrypt(&ctx, plain + i, plain + i);
    }

    // remove padding
    uint8_t padding = (uint8_t) plain[input.length() - 1];

    if (padding > 0x10) {
        delete_array(plain);
        return STRING_NULL;
    }

    string res((char *) plain, input.length() - padding);
    delete_array(plain);

    
    return res;
}

STEE
string CryptUtils::AesCbcEncrypt(const string &key, const string &iv, const string &input) {
    

    struct AES_ctx ctx;

    if (key.length() == AESMINI_128BIT_KEY
        || key.length() == AESMINI_192BIT_KEY
        || key.length() == AESMINI_256BIT_KEY) {

        AES_init_ctx_iv(&ctx, (uint8_t *) key.data(), (uint8_t *) iv.data());
    }

    int STEP = 16;
    int total_size = (int) (input.length() + STEP) / STEP * STEP;

    uint8_t *cipher = new uint8_t[total_size];
    memcpy(cipher, (const uint8_t *) input.data(), input.length());

    AES_CBC_encrypt_buffer(&ctx, cipher, total_size);

    string out((const char *) cipher, (uint32_t) total_size);
    delete_array(cipher);

    
    return out;
}

STEE
string CryptUtils::AesCbcDecrypt(const string &key, const string &iv, const string &input) {


    string out;
    struct AES_ctx ctx;
    if (key.length() == AESMINI_128BIT_KEY
        || key.length() == AESMINI_192BIT_KEY
        || key.length() == AESMINI_256BIT_KEY) {
        AES_init_ctx_iv(&ctx, (uint8_t *)key.data(), (uint8_t *)iv.data());
    }

    int STEP = 16;

    if (input.length() % STEP != 0) {
        
        return STRING_NULL;
    }

    uint8_t *plain = new uint8_t[input.length()];
    memcpy(plain, (const uint8_t *) input.data(), input.length());


    AES_CBC_decrypt_buffer(&ctx, plain, input.length());

    // remove PKCS5Padding
    // AES一般是16字节为一块，所以解密输出的字节数为16倍数，但是实际源数据不是16倍数，就会补全
    // 补全字节数会在AES输出的最后一个字节
    uint8_t padding = (uint8_t) plain[input.length() - 1];
    // 补全数如果大于16，表示解密失败
    if (padding > 0x10) {
        delete_array(plain);
        out = STRING_NULL;
    }
    else{
        out = string((const char *) plain, input.length() - padding);
    }

    delete_array(plain);
    

    return out;
}



/**
 * rc4 加密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::RC4(const string &key, const string &input){
    rc4_state rc4k;
    rc4_init(&rc4k, (u_char *) key.c_str(), key.length());

    u_char *output = new u_char[input.size()];
    memset(output, 0, input.size());

    rc4_crypt(&rc4k, (u_char *) input.c_str(), output, input.size());

    string res((char *) output, input.size());
    delete_array(output);

    return res;
}


/**
 * 白盒aes 加密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::WBAesEncrypt(const string &key, const string &input){
    Wbox wbox_;
    if (key.length() == sizeof(wbox_)) {
        memcpy((void *) &wbox_, key.data(), sizeof(wbox_));
    } 
    else {
        return STRING_NULL;
    }

    const int STEP = 16;
    int total_size = (input.length() + STEP) / STEP * STEP;

    uint8_t *cipher = new uint8_t[total_size];
    memcpy(cipher, (const uint8_t *) input.data(), input.length());
    uint8_t padding = (uint8_t) STEP - (uint8_t) (input.length() % STEP);

    for (int i = input.length(); i < total_size; i++) {
        cipher[i] = padding;
    }

    for (int i = 0; i < total_size; i += STEP) {
        aes_wb_decryptor(&wbox_, cipher + i, cipher + i);
    }

    string res((const char *) cipher, total_size);
    delete_array(cipher);

    return res;
}

/**
 * 白盒aes 解密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::WBAesDecrypt(const string &key, const string &input){
    Wbox wbox_;
    if (key.length() == sizeof(wbox_)) {
        memcpy((void *) &wbox_, key.data(), sizeof(wbox_));
    } 
    else {
        return STRING_NULL;
    }

    const int STEP = 16;

    if (input.length() % STEP != 0) {
        return STRING_NULL;
    }

    uint8_t *plain = new uint8_t[input.length()];
    memcpy(plain, (const uint8_t *) input.data(), input.length());

    for (int i = 0; i < input.length(); i += STEP) {
        aes_wb_decryptor(&wbox_, plain + i, plain + i);
    }

    uint8_t padding = (uint8_t) plain[input.length() - 1];

    if (padding > 0x10) {
        delete_array(plain);
        return STRING_NULL;
    }

    string res((char *) plain, input.length() - padding);
    delete_array(plain);

    return res;
}

/**
 * sm3 加密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::SM3Encrypt(const string &key, const string &input){
    unsigned char result[32];
    char *inputChar = new char[input.length()];

    string resultStr;

    memcpy(inputChar, (const uint8_t *) input.data(), input.length());

    char buf[3];
    buf[2] = 0;

    if (key.empty()) {
        sm3_encrypt((unsigned char *) inputChar, input.length(), result);
    } else {
        sm3_hmac((const unsigned char *) inputChar, input.length(),
                 (const unsigned char *) key.data(), key.size(), result);
    }

    for (int i = 0; i < 32; i++) {
        sprintf(buf, "%02x", result[i]);
        resultStr.append(buf);
    }

    delete_array(inputChar);
    return resultStr;
}



/**
 * sm4 加密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::SM4Encrypt(const string &key, const string &input){
    unsigned char *key__ = new unsigned char[16];
    CryptUtils::HexStringToChar(key, key__);

    int STEP = 16;
    int total_size = (input.length() + STEP) / STEP * STEP;
    uint8_t *cipher = new uint8_t[total_size];
    memcpy(cipher, (const uint8_t *) input.data(), input.length());
    uint8_t padding = (uint8_t) STEP - (uint8_t) (input.length() % STEP);
    for (int i = input.length(); i < total_size; i++) {
        cipher[i] = padding;
    }
    unsigned char *result = new unsigned char[total_size];
    sm4_encrypt(key__, cipher, total_size, result);
    string resultStr = CryptUtils::CharToHexString(result, total_size);
    delete_array(cipher);
    delete_array(result);
    delete_array(key__);

    return resultStr;
}

/**
 * sm4 解密
 * @param key
 * @param input
 * @return
 */
STEE string CryptUtils::SM4Decrypt(const string &key, const string &input){
    unsigned char *key__ = new unsigned char[16];
    CryptUtils::HexStringToChar(key, key__);

    int STEP = 16;
    if (input.length() % STEP != 0) {
        return STRING_NULL;
    }

    int size = input.length() / 2;
    unsigned char *inputChar = new unsigned char[size];
    CryptUtils::HexStringToChar(input, inputChar);
    unsigned char *result = new unsigned char[size];
    sm4_decrypt(key__, inputChar, size, result);
    delete_array(key__);

    // remove padding
    uint8_t padding = (uint8_t) result[size - 1];
    if (padding > 0x10) {
        delete_array(result);
        return STRING_NULL;
    }
    string res((char *) result, size - padding);
    delete_array(result);
    delete_array(inputChar);
    return res;
}




STEE bool CryptUtils::isValidKeyByAES(const string &key){
    string kk(key);
    if (kk.length() == AESMINI_128BIT_KEY
        || kk.length() == AESMINI_192BIT_KEY
        || kk.length() == AESMINI_256BIT_KEY) {
        return true;
    }

    return false;
}

STEE bool CryptUtils::isValidKeyByWBAES(const string &key){
    Wbox wbox_;
    if (key.length() == sizeof(wbox_)) {
        return true;
    } else {
        return false;
    }
}

STEE bool CryptUtils::isValidKeyBySM4(const string &key){
    unsigned char *key_ = new unsigned char[16];
    int dataLength = CryptUtils::HexStringToChar(key, key_);

    bool result = false;
    if (dataLength == SM4_128BIT_KEY) {
        result = true;
    }

    delete_array(key_);

    return result;
}




STEE string CryptUtils::MTKeyEncrypt(const string &key, const string &input){

    const char* plain = input.c_str();
    uint32_t plain_len = (uint32_t )strlen(plain);
    const char* keyc = key.c_str();
    uint32_t key_len = (uint32_t )strlen(keyc);
    unsigned char* src = (unsigned char* )malloc(plain_len + 1);
    unsigned char* des = (unsigned char* )malloc(plain_len + 1);

    if (kp_set_api(malloc, free)){
        memcpy(src, plain, plain_len + 1);
        kp_encode(des, src, plain_len, (unsigned char* )keyc, key_len);
    }

    string out((char *)des, plain_len);
    return out;
}

STEE string CryptUtils::MTKeyDecrypt(const string &key, const string &input){
    const char* plain = input.c_str();
    uint32_t plain_len = (uint32_t )strlen(plain);
    const char* keyc = key.c_str();
    uint32_t key_len = (uint32_t )strlen(keyc);
    unsigned char* src = (unsigned char* )malloc(plain_len + 1);
    unsigned char* des = (unsigned char* )malloc(plain_len + 1);

    if (kp_set_api(malloc, free)){
        memcpy(src, plain, plain_len + 1);
        kp_decode(des, src, plain_len, (unsigned char* )keyc, key_len);
    }

    
    string out((char *)des, plain_len);
    return out;
}

