//
// Created by blue on 2018/10/15.
//

#include "dx_sha1.h"
#include "hmac_sha1.h"

#include "dx_tools.h"
#include "dx_crypto_config.h"

using namespace std;

STEE
string DX_SHA1(const string &src) {
    uint8_t sha1_result[20];

    sha1(sha1_result, src.data(), src.size() * 8);
    string result_str((char *) sha1_result, 20);

    return DX_CharToHexString((unsigned char *)result_str.data(), result_str.size());
}

STEE
string DX_SHA1WithKey(const string &src, const string &key) {
    uint8_t hmac_sha1_result[20];
    hmac_sha1(hmac_sha1_result, key.data(), key.size() * 8, src.data(), src.size() * 8);
    string hmac_result_str((char *) hmac_sha1_result, 20);

    return DX_CharToHexString((unsigned char *)hmac_result_str.data(), hmac_result_str.size());
}
