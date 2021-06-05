//
// Created by RoseJames on 2017/8/4.
//

#ifndef ANDROID_HMAC_MD5_H
#define ANDROID_HMAC_MD5_H

#ifdef __cplusplus
extern "C" {
#endif

#define MD5_DIGEST_SIZE  16

#include "dx_crypto_config.h"

void hmac_md5(const unsigned char *key, int key_len,
              const unsigned char *text, int text_len, unsigned char *hmac);


#ifdef __cplusplus
}
#endif

#endif //ANDROID_HMAC_MD5_H
