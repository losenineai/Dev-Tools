//
// Created by blue on 2018/10/15.
//

#include "dx_sm4.h"
#include "sm4.h"
#include <memory.h>
#include "dx_tools.h"
#include "dx_crypto_config.h"

using namespace std;

const string STRING_NULL = "";

#define delete_array(x) if((x)){delete [](x);(x) = NULL;}

/**
 * sm4 加密
 * @param key
 * @param input
 * @return
 */
STEE
string DX_SM4Encrypt(const string &key, const string &input) {

    unsigned char *key__ = new unsigned char[16];
    DX_HexStringToChar(key, key__);

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
    string resultStr = DX_CharToHexString(result, total_size);
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
STEE
string DX_SM4Decrypt(const string &key, const string &input){
    unsigned char *key__ = new unsigned char[16];
    DX_HexStringToChar(key, key__);

    int STEP = 16;
    if (input.length() % STEP != 0) {
        return STRING_NULL;
    }

    int size = input.length() / 2;
    unsigned char *inputChar = new unsigned char[size];
    DX_HexStringToChar(input, inputChar);
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

STEE
bool DX_IsValidKeyBySM4(const string &key) {

    unsigned char *key_ = new unsigned char[16];
    int dataLength = DX_HexStringToChar(key, key_);

    bool result = false;
    if (dataLength == SM4_128BIT_KEY) {
        result = true;
    }

    delete_array(key_);

    return result;
}