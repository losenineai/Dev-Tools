//
// Created by blue on 2018/10/15.
//

#include "dx_wbaes.h"
#include "wbox.h"
#include <memory.h>

#include "dx_crypto_config.h"

using namespace std;

#define delete_array(x) if((x)){delete [](x);(x) = NULL;}
#define delete_not_array(x) if((x)){delete (x);(x) = NULL;}

const string STRING_NULL = "";

/**
 * 白盒aes 加密
 * @param key
 * @param input
 * @return
 */
STEE
string DX_WBAesEncrypt(const string &key, const string &input){
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
STEE
string DX_WBAesDecrypt(const string &key, const string &input) {

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

STEE
bool DX_IsValidKeyByWBAES(const string &key) {
    Wbox wbox_;
    if (key.length() == sizeof(wbox_)) {
        return true;
    } else {
        return false;
    }
}