//
// Created by RoseJames on 2017/8/4.
//
#include "hmac_md5.h"
#include "md5.h"
// #include "dx_utils_crypto_stee.h"
#include "dx_anti_stee.h"
#include <string.h>

#define KEY_IOPAD_SIZE 64


STEE
void hmac_md5(const unsigned char *key, int key_len,
              const unsigned char *text, int text_len, unsigned char *hmac) {
    MD5_CTX context;
    unsigned char k_ipad[KEY_IOPAD_SIZE];
    unsigned char k_opad[KEY_IOPAD_SIZE];
    int i;

    memset(k_ipad, 0, sizeof(k_ipad));
    memset(k_opad, 0, sizeof(k_opad));
    memcpy(k_ipad, key, key_len);
    memcpy(k_opad, key, key_len);

    for (i = 0; i < KEY_IOPAD_SIZE; i++) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    MD5Init(&context);
    MD5Update(&context, k_ipad, KEY_IOPAD_SIZE);
    MD5Update(&context, text, text_len);
    MD5Final(hmac, &context);

    MD5Init(&context);
    MD5Update(&context, k_opad, KEY_IOPAD_SIZE);
    MD5Update(&context, hmac, MD5_DIGEST_SIZE);
    MD5Final(hmac, &context);
}