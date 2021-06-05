//
// Created by blue on 2018/10/15.
//

#include "dx_aes.h"
#include "aes_mini.h"
#include "aes.h"
#include <memory.h>

#include "dx_crypto_config.h"

using namespace std;

//常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}

const string STRING_NULL = "";

STEE
string DX_AesEcbEncrypt(const string &key, const string &input) {


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
string DX_AesEcbDecrypt(const string &key, const string &input) {

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
string DX_AesCbcEncrypt(const string &key, const string &iv, const string &input) {


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
string DX_AesCbcDecrypt(const string &key, const string &iv, const string &input) {

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

bool DX_IsValidKeyByAES(const string &key)  {

    string kk(key);

    return (kk.length() == AESMINI_128BIT_KEY
            || kk.length() == AESMINI_192BIT_KEY
            || kk.length() == AESMINI_256BIT_KEY);
}