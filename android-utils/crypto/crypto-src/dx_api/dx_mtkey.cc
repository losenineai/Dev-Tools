//
// Created by blue on 2018/10/15.
//

#include "dx_mtkey.h"
#include "mt_kp.h"
#include <memory.h>
#include "dx_crypto_config.h"

using namespace std;

STEE
string DX_MTKeyEncrypt(const string &key, const string &input){

    const char* plain = input.c_str();
    uint32_t plain_len = (uint32_t ) input.length();
    const char* keyc = key.c_str();
    uint32_t key_len = (uint32_t ) key.length();
    unsigned char* src = (unsigned char* )malloc(plain_len + 1);
    unsigned char* des = (unsigned char* )malloc(plain_len + 1);

    if (kp_set_api(malloc, free)){
        memcpy(src, plain, plain_len + 1);
        kp_encode(des, src, plain_len, (unsigned char* )keyc, key_len);
    }

    string out((char *)des, plain_len);
    return out;
}

STEE
string DX_MTKeyDecrypt(const string &key, const string &input){
    const char* plain = input.c_str();
    uint32_t plain_len = (uint32_t )input.length();
    const char* keyc = key.c_str();
    uint32_t key_len = (uint32_t )key.length();
    unsigned char* src = (unsigned char* )malloc(plain_len + 1);
    unsigned char* des = (unsigned char* )malloc(plain_len + 1);

    if (kp_set_api(malloc, free)){
        memcpy(src, plain, plain_len + 1);
        kp_decode(des, src, plain_len, (unsigned char* )keyc, key_len);
    }

    string out((char *)des, plain_len);
    return out;
}
