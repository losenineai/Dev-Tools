//
// Created by blue on 2018/10/12.
//

#include "dx_md5.h"

#include "hmac_md5.h"
#include "md5.h"

#include "dx_tools.h"
#include "dx_crypto_config.h"

using namespace std;

STEE
string DX_MD5(const string &src) {

    uint8_t signature[16];

    only_md5((const unsigned char *) src.data(), src.size(), signature);

    string result((char *) signature, 16);

    return DX_CharToHexString((unsigned char *)result.c_str(), result.size());

}

STEE
string DX_MD5WithKey(const string &src, const string &key) {

    uint8_t signature_hmac[16];

    hmac_md5((const unsigned char *) key.data(), key.size(), (const unsigned char *) src.data(),
             src.size(), signature_hmac);

    string result_hmac((char *) signature_hmac, 16);

    return DX_CharToHexString((unsigned char *)result_hmac.data(), result_hmac.size());
}