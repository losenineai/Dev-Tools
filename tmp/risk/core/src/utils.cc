//
// Created by boyliang on 2017/5/22.
//

#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <zlib.h>
#include <dirent.h>
#include <bitset>
#include <sys/stat.h>
#include <algorithm>
#include "zip.h"
#include "hmac_sha1.h"
#include <output_log_utils.h>
#include "md5.h"
#include "base64.h"
#include "xxtea.h"
#include "aes_mini.h"
#include "rc4.h"
#include "utils.h"
#include "DXPlatform.h"
#include "common.h"
#include "risk_config.h"

using namespace std;
#define MAX_FILENAME 1024

string CryptUtils::MD5(const string &src) {

    uint8_t signature[16];

    md5((const unsigned char *) src.data(), src.size(), signature);

    string result((char *) signature, 16);

    return StringUtils::ToHexString(result);
}

string CryptUtils::SHA1(const string &src) {
    uint8_t sha1_result[20];

    sha1(sha1_result, src.data(), src.size() * 8);
    string result_str((char *) sha1_result, 20);

    return StringUtils::ToHexString(result_str);
}

string CryptUtils::Base64Encode(const string &input) {
    if (input.empty()) {
        return "";
    }

    size_t encode_size = BASE64_ENCODE_OUT_SIZE(input.size());
    char *base64_encode_buf = new char[encode_size];
    memset(base64_encode_buf, 0, encode_size);

    string output;
    base64_encode_imp((const uint8_t *) input.data(), input.size(), base64_encode_buf);
    output.assign(base64_encode_buf, encode_size);
    delete_array(base64_encode_buf);

    return output;
}

string CryptUtils::Base64Decode(const string &input) {
    if (input.empty()) {
        return "";
    }
    long decode_size = BASE64_DECODE_OUT_SIZE(input.size());

    for (int i = input.size() - 1; i > -1; i--) {
        if (input[i] == '=') {
            decode_size--;
        } else {
            break;
        }
    }

    if (decode_size <= 0) {
        return "";
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

string CryptUtils::RC4_Encrypt(const string &key, const string &src) {
    if (key.empty() || src.empty()) {
        return "";
    }
    rc4_state rc4k;
    rc4_init(&rc4k, (u_char *) key.c_str(), key.length());

    u_char *output = new u_char[src.size()];
    memset(output, 0, src.size());

    rc4_crypt(&rc4k, (u_char *) src.c_str(), output, src.size());

    string res((char *) output, src.size());
    delete_array(output);

    return res;

}

string CryptUtils::RC4_Decrypt(const string &key, const string &src) {
    return RC4_Encrypt(key, src);
}

string CryptUtils::AES_Decrypt(const string &key_, const string &src) {
    if (key_.empty() || src.empty()) {
        return "";
    }
    string kk(key_);
    AESMini_ctx ctx_;

    if (kk.length() == AESMINI_128BIT_KEY
        || kk.length() == AESMINI_192BIT_KEY
        || kk.length() == AESMINI_256BIT_KEY) {

        AESMini_Init(&ctx_, (const uint8_t *) kk.data(), (uint32_t)kk.length());
    }

    int STEP = 16;

    if (src.length() % STEP != 0) {
        err_log("aes decrypt input length is %d (%%%d != 0)", src.length(), STEP);
        return "";
    }

    uint8_t *plain = new uint8_t[src.length()];
    memcpy(plain, (const uint8_t *) src.data(), src.length());

    for (int i = 0; i < src.length(); i += STEP) {
        AESMini_ECB_Decrypt(&ctx_, plain + i, plain + i);
    }

    // remove padding
    uint8_t padding = (uint8_t) plain[src.length() - 1];

    if (padding > 0x10) {
        err_log("aes decrypt fatal error!");

        delete_array(plain);
        return "";
    }

    string res((char *) plain, src.length() - padding);
    delete_array(plain);

    return res;
}

string CryptUtils::AES_Encrypt(const string &key_, const string &src) {
    if (key_.empty() || src.empty()) {
        return "";
    }
    string kk(key_);
    AESMini_ctx ctx_;

    if (kk.length() == AESMINI_128BIT_KEY
        || kk.length() == AESMINI_192BIT_KEY
        || kk.length() == AESMINI_256BIT_KEY) {

        AESMini_Init(&ctx_, (const uint8_t *) kk.data(), (uint32_t)kk.length());
    }

    int STEP = 16;
    int total_size = (int)(src.length() + STEP) / STEP * STEP;

    uint8_t *cipher = new uint8_t[total_size];
    memcpy(cipher, (const uint8_t *) src.data(), src.length());
    uint8_t padding = (uint8_t) STEP - (uint8_t) (src.length() % STEP);

    for (int i = (int)src.length(); i < total_size; i++) {
        cipher[i] = padding;
    }

    for (int i = 0; i < total_size; i += STEP) {
        AESMini_ECB_Encrypt(&ctx_, cipher + i, cipher + i);
    }

    string res((const char *) cipher, (uint32_t)total_size);
    delete_array(cipher);

    return res;
}

string CryptUtils::XXTeaEncrypt(const string &key, const string &src) {
    if (key.empty() || src.empty()) {
        return "";
    }
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

string CryptUtils::XXTeaDecrypt(const string &key, const string &input) {

    if (key.empty() || input.empty()) {
        return "";
    }
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

//OpenSSL中出现的字符串Hash函数
unsigned int CryptUtils::StrHash(const char *str)
{
    int i,l;
    unsigned int ret=0;
    unsigned short *s;

    if (str == NULL) return(0);
    l=(strlen(str)+1)/2;
    s=(unsigned short *)str;
    for (i=0; i <l;++i)
        ret^=(s[i]<<(i&0x0f));
    return(ret);
}

STEE
OthersUtils::HWEERR
OthersUtils::HttpWithEncrypt(const string &url, const string &data, string &respone, const string &appId, const bool isSaas) {

    string sign = CryptUtils::MD5(appId + data + appId);
    string xxtea_key = CryptUtils::SHA1(appId + appId + appId);

    string zip_data = ZipFile::Compress(data);
    string encrypt_zip_data = CryptUtils::XXTeaEncrypt(xxtea_key, zip_data);
//    string report_url = url + "?sign=" + sign + "&appKey=" + appkey + "&app=" + get_pkg_name();
    string report_url = url +
                        (char[]) {(char) 0x3f, (char) 0x73, (char) 0x69, (char) 0x67, (char) 0x6e,
                                  (char) 0x3d, 0} + sign +
                        (char[]) {(char) 0x26, (char) 0x61, (char) 0x70, (char) 0x70, (char) 0x4b,
                                  (char) 0x65, (char) 0x79, (char) 0x3d, 0} + appId +
                        (char[]) {(char) 0x26, (char) 0x61, (char) 0x70, (char) 0x70, (char) 0x3d,
                                  0} + get_pkg_name() +
                        (char[]) {(char) 0x26, (char) 0x76, (char) 0x65, (char) 0x72, (char) 0x73,
                                  (char) 0x69, (char) 0x6f, (char) 0x6e, (char) 0x3d, 0} + get_dx_risk_sdk_version();

    string respone_data = http_request(HTTP_POST, report_url, encrypt_zip_data, isSaas);

    if (respone_data.empty()) {
        return HWEERR::NETWORK_ERR;
    }

    zip_data = CryptUtils::XXTeaDecrypt(xxtea_key, respone_data);
    if (zip_data.empty()) {
        return HWEERR::DECRYPT_ERR;
    }

    string tmp_respone = ZipFile::UnCompress(zip_data);
    if (tmp_respone.empty()) {
        return HWEERR::UNCOMPREESS_ERR;
    }

    respone = tmp_respone;

    return HWEERR::NOERROR;
}

STEE
void OthersUtils::AsyncHttpWithEncrypt(const string &url, const string &data, string &respone, const string &appId, const bool isSaas) {

    string sign = CryptUtils::MD5(appId + data + appId);
    string xxtea_key = CryptUtils::SHA1(appId + appId + appId);

    string zip_data = ZipFile::Compress(data);
    string encrypt_zip_data = CryptUtils::XXTeaEncrypt(xxtea_key, zip_data);
    //    string report_url = url + "?sign=" + sign + "&appKey=" + appkey + "&app=" + get_pkg_name() + "&v=" + sdk_Version;
    string report_url = url +
    (char[]) {(char) 0x3f, (char) 0x73, (char) 0x69, (char) 0x67, (char) 0x6e,
        (char) 0x3d, 0} + sign +
    (char[]) {(char) 0x26, (char) 0x61, (char) 0x70, (char) 0x70, (char) 0x4b,
        (char) 0x65, (char) 0x79, (char) 0x3d, 0} + appId +
    (char[]) {(char) 0x26, (char) 0x61, (char) 0x70, (char) 0x70, (char) 0x3d,
        0} + get_pkg_name() +
    (char[]) {(char) 0x26, (char) 0x76, (char) 0x65, (char) 0x72, (char) 0x73,
        (char) 0x69, (char) 0x6f, (char) 0x6e, (char) 0x3d, 0} + get_dx_risk_sdk_version();

    async_http_request(HTTP_POST, report_url, encrypt_zip_data, isSaas);
}

int64_t OthersUtils::CurrentTimes() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t) tv.tv_sec * 1000 + (int64_t) tv.tv_usec / 1000;
}

//Timestamp OthersUtils::CurrentTimesstamp() {
//    struct timeval tv;
//    gettimeofday(&tv, NULL);
//
//    Timestamp timestamp;
//    timestamp.set_nanos(tv.tv_usec * 1000);
//    timestamp.set_seconds(tv.tv_sec);
//
//    return timestamp;
//}
//

STEE
string OthersUtils::CreateSTEERequestRaw(STEEDataType type, const string appId, const ProtobufCBinaryData data) {

    STEERequest request = COM__DINGXIANG__MOBILE__STEEREQUEST__INIT;
    STEERequestHeader header = COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__INIT;

    header.sdk_ver = StringUtils::StringToChar(get_dx_risk_sdk_version());
    header.app_ver = StringUtils::StringToChar(get_app_version());
    header.app_key = StringUtils::StringToChar(appId);
    header.os_ver = StringUtils::StringToChar(get_os_version());
    header.app_code = StringUtils::StringToChar(get_pkg_name());

    // fix if int or enum equals zero, return 1002
    header.app_ver_code = get_app_version_code();
    header.has_app_ver_code = header.app_ver_code != 0;
    header.os_type = get_os_type();
    header.has_os_type = header.os_type != 0;
    header.proto_version = PB_VERSION;
    header.has_proto_version = header.proto_version != 0;
    header.os_arch = get_os_arch();
    header.has_os_arch = header.os_arch != 0;

    request.header = &header;
    request.type = type;
    request.data = data;

    size_t size = com__dingxiang__mobile__steerequest__get_packed_size(&request);
    uint8_t *out = (uint8_t *) calloc(1, size + 1);
    memset(out, 0, size + 1);
    com__dingxiang__mobile__steerequest__pack(&request, out);

    string s(out, out+size);

    free(out);
    free(header.sdk_ver);
    free(header.app_ver);
    free(header.app_key);
    free(header.os_ver);
    free(header.app_code);

    return s;
}

#ifndef __APPLE__

string OthersUtils::ExecuteCMD(const string &cmd, const string &filter) {

    string ss("");

    FILE *pipe = popen(cmd.c_str(), "r");
    if (pipe != NULL) {
        char buf[512];
        while (!feof(pipe)) {
            if (fgets(buf, sizeof(buf), pipe) != NULL) {
                ss += buf;
            }
        }
        pclose(pipe);
    }

    if (0 == ss.length() && !filter.empty()) {
        vector<string> lines(0);
        StringUtils::SplitString(ss, lines, "\n");
        string rss("");

        for (string &l : lines) {
            if (l.find(filter) != l.npos) {
                rss += (l.append("\n"));
            }
        }

        return rss;
    }

    return ss;
}
#endif


//// 有bug
//void OthersUtils::SetRandom(int randomArr[], int randomArrCount, int max) {
//
//    if(randomArrCount <= 3) {
//        for(int i = 0; i < randomArrCount; i++)
//            randomArr[i] = i;
//
//        return;
//    }
//
//    static auto bIntInArr = [](int in, int* arr, int count) {
//
//        for (int i = 0; i < count; i++) {
//
//            if (in == arr[i]) {
//                return true;
//            }
//        }
//
//        return false;
//    };
//
//    int z = 0, index = 0;
//
//    while(index < randomArrCount) {
//
//        int* chseed = new int;
//#ifdef __LP64__
//        uint64 chseed64 = (uint64)chseed;
//        uint32 chseed32 = (uint32)chseed64;
//        uint32 x = chseed32++ + z++;
//#else
//        uint32 chseed32 = (uint32)chseed;
//        uint32 x = chseed32++ + z++;
//#endif
//        delete chseed;
//
//        SRand(x);
//
//        int r = Rand() % (max - 1);
//
//        if(!bIntInArr(r, randomArr, index)) {
//            randomArr[index++] = r;
//        }
//    }
//}
//
//int OthersUtils::Rand() {
//
//    nextSeed = nextSeed * 0x41c64e6d + 12345;
//
//    return 0x8000 % (int)(nextSeed / 0x1FFFF);
//}
//
//uint32 OthersUtils::nextSeed = 1;
//
//void OthersUtils::SRand(uint32 seed) {
//    nextSeed = seed;
//}

bool FileUtils::Exists(const string &name) {
    return !name.empty() && !access(name.data(), R_OK);
}

bool FileUtils::CanOpen(const string &name) {

    FILE *fp = fopen(name.c_str(), "r");
    bool ret = false;

    if (fp) {
        fclose(fp);
        ret = true;
    }

    return ret;
}

string FileUtils::ReadAll(const string &name, unsigned long limitsize) {

    string result = "";

    if(FileUtils::Exists(name)){

        FILE *fp = fopen(name.c_str(), "rb");

        if (!fp) {
            return result;
        }

        static int BUF_SIZE = 1024;

        // 读取直到遇到eof
        while (1) {
            // 某设备设备文件会没有eof，防止无限读取，默认限制32M
            if (result.length() >= limitsize) {
                break;
            }
            char *buf = new char[BUF_SIZE];
            memset(buf, 0, BUF_SIZE);

            // 为了防止fread被hook，使用dx_fread
            size_t retSize = fread(buf, sizeof(char), BUF_SIZE, fp);

            result.append(buf, retSize);
            delete_array(buf);

            if (retSize < BUF_SIZE) {
                break;
            }
        }

        fclose(fp);
    }

    return result;
}

vector<string> FileUtils::ReadLines(const string &name) {

    FILE *fp = fopen(name.c_str(), "rb");
    vector<string> lines;

    if (fp) {
        char buf[1024];
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            lines.push_back(string(buf));
        }
        fclose(fp);
    }

    return lines;
}

string FileUtils::ReadLine(const string &name) {

    FILE *fp = fopen(name.c_str(), "rb");
    string line;

    if (fp) {
        char buf[1024];
        if (fgets(buf, sizeof(buf), fp) != NULL) {
            line = string(buf);
        }
        fclose(fp);
    }

    // filter invisible char, contains 0x20 space char
    if (line.length() == 1) {
        if (line.c_str()[0] >= 0x0 && line.c_str()[0] <= 0x20) {
            line = "";
        }
    }

    return line;
}

void FileUtils::WriteAll(const string &name, const string &data, bool append) {

    FILE* file = NULL;

    if (append) {
        file = fopen(name.c_str(), "ab");
    } else {
        file = fopen(name.c_str(), "wb");
    }

    if (file != NULL) {
        fwrite(data.data(), data.length(), 1, file);
        fflush(file);
        fclose(file);
    }
}

void FileUtils::DeleteIfExist(const string &name) {
    if (access(name.data(), R_OK | W_OK) == 0) {
        unlink(name.data());
    }
}

bool FileUtils::MakeDir(const string &name, int mode) {
    return mkdir(name.c_str(), mode) == 0;
}

vector<string> FileUtils::List(const string &dir, const vector<string> &filters) {
    vector<string> files;
    DIR *D = opendir(dir.c_str());
    if (D != NULL) {
        dirent *ent = NULL;
        while ((ent = readdir(D)) != NULL) {
            if (ent->d_type & DT_DIR) {
                continue;
            }

            bool skip = false;
            for (auto i = filters.begin(); i != filters.end(); i++) {
                if (strstr(ent->d_name, i->c_str()) != NULL) {
                    skip = true;
                    break;
                }
            }

            if (skip) {
                continue;
            }

            files.push_back(string(ent->d_name));
        }
        closedir(D);
    }

    return files;
}

void StringUtils::SplitString(const string &s, vector<string> &v, const string &c) {
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;

    while (string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }

    if (pos1 != s.length()) {
        v.push_back(s.substr(pos1));
    }
}

string StringUtils::Format(const char *fmt, ...) {
    char *buf = NULL;

    va_list vl;
    va_start(vl, fmt);
    vasprintf(&buf, fmt, vl);
    va_end(vl);

    string result(buf);
    delete_not_array(buf);

    return result;
}

string StringUtils::ToHexString(const string &input) {
    char *buf = new char[input.length() * 2 + 1];
    memset(buf, 0, input.length() * 2 + 1);

    for (int i = 0; i < input.size(); i++) {
        sprintf(buf + i * 2, "%02x", (uint8_t) input.at(i));
    }

    string res(buf, input.size() * 2);
    delete_array(buf);
    return res;
}

char* StringUtils::Int64ToChar(int64_t value) {

    char *pStr = new char[20];

#ifdef __aarch64__
    sprintf(pStr, "%li", value);
#else
    sprintf(pStr, "%lli", value);
#endif

    return pStr;
}

string StringUtils::Int64ToString(int64_t value) {

    char str[20] = {0};

#ifdef __aarch64__
    sprintf(str, "%li", value);
#else
    sprintf(str, "%lli", value);
#endif

    string s(str);
    return s;
}

//取一个字符串数组里出现频率最高的字符串，且过滤掉""来进行筛选
string StringUtils::FindString(string *strArray, int sum) {

    map<string, int> strMap;

    for (int i = 0; i < sum; i++) {
        if (strArray[i] == "") {
            continue;
        }

        if (strMap.find(strArray[i]) == strMap.end())
            strMap.insert(make_pair<string, int>((string &&) strArray[i], 1));
        else
            strMap[strArray[i]]++;
    }

    if (strMap.empty()) {
        return "";
    }

    map<string, int>::const_iterator cit = strMap.begin();

    string targetStr = cit->first;
    int mostFrequency = cit->second;

    for (++cit; cit != strMap.end(); ++cit) {

        if (cit->second > mostFrequency) {
            targetStr = cit->first;
            mostFrequency = cit->second;
        }
    }

    return targetStr;
}

string StringUtils::Trim(string &s) {

    if (s.empty())
        return s;

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);

    return s;
}

char* StringUtils::StringToChar(const string& s) {

    unsigned long charLen = s.size() + 1;

    char* ret = (char* )calloc(1, charLen);
    memset((void *)ret, 0, charLen);
    memcpy(ret, s.data(), charLen - 1);

    return ret;
}

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

//ZipFile *ZipFile::CreateUZipperFromFile(const string &path, const string &password) {
//    unzFile zf = NULL;
//
//    if (access(path.c_str(), R_OK) == 0) {
//        zf = unzOpen(path.c_str());
//    }
//
//    return zf != NULL ? new ZipFile(ZMODE_UNZIPPER, zf, password, NULL) : NULL;
//}

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

//bool ZipFile::CheckIsExist(const string &path) {
//    unz_file_info info;
//    return unzLocateFile(unzFile_, path.c_str(), NULL) == UNZ_OK
//           && unzGetCurrentFileInfo(unzFile_, &info, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK;
//}

const void ZipFile::GetFileData(const string &path, string &data) {

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
        data = value_iterator != nameDataMap_.end() ? value_iterator->second : "";
    } else {
        data = value_iterator->second;
    }
}

void ZipFile::AddFileData(const string &name, const string &data) {

    time_t tm_t;
    time(&tm_t);

    zip_fileinfo zi;
    memset(&zi, sizeof(zi), 0);

    int err = zipOpenNewFileInZip(zipFile_, name.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 5);
    if (err == ZIP_OK) {
        err = zipWriteInFileInZip(zipFile_, data.c_str(), data.length());
        if (err == ZIP_OK) {
            zipCloseFileInZip(zipFile_);
        }
    }
}

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

string ZipFile::GetZipFileData() {
    ourmemory_t *om = (ourmemory_t *) pZipData_;
    if (om && om->base && closeState_) {
        if (zipMode_ == ZMODE_ZIPPER) {
            return string(om->base, om->limit);
        } else if (zipMode_ == ZMODE_UNZIPPER) {
            return string(om->base, om->limit);
        }
    }

    return "";
}

/**
    * 简化方法，zip压缩
    * @param input
    * @return
    */
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
string ZipFile::UnCompress(const string &input) {

    ZipFile *zf = ZipFile::CreateUZipperFromData(input);
    string tmp = "";

    if (zf != NULL) {
        zf->GetFileData("data", tmp);
    }

    delete_not_array(zf);

    return tmp;
}


//vector<string> ZipFile::TraversalZipFile() {
//    vector<string> fileNameList;
//    unz_global_info globalInfo;
//    if (unzGetGlobalInfo(unzFile_, &globalInfo) != UNZ_OK) {
//        err_log("get global info failed");
//        return fileNameList;
//    }
//
//    for (int i = 0; i < globalInfo.number_entry; ++i) {
//        unz_file_info file_info;
//        char fileName[MAX_FILENAME];
//        if (unzGetCurrentFileInfo(
//                unzFile_,
//                &file_info,
//                fileName,
//                MAX_FILENAME,
//                NULL, 0, NULL, 0) != UNZ_OK) {
//            printf("could not read file info\n");
//            return fileNameList;
//        }
//
//        fileNameList.push_back(fileName);
//
//        if ((i + 1) < globalInfo.number_entry) {
//            if (unzGoToNextFile(unzFile_) != UNZ_OK) {
//                printf("cound not read next file\n");
//                break;
//            }
//        }
//    }
//    return fileNameList;
//}
