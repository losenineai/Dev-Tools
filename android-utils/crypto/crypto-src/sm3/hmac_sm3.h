//
// Created by RoseJames on 2017/8/2.
//

#ifndef PROJECT_HMAC_SM3_H
#define PROJECT_HMAC_SM3_H

#include "sm3.h"

#include "dx_crypto_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sm3 sm3_ctx;
    unsigned char key[SM3_BLOCK_SIZE];
} sm3_hmac_ctx_t;

STEE
void sm3_hmac(const unsigned char *data, size_t data_len,
              const unsigned char *key, size_t key_len,
              unsigned char mac[SM3_RESULT_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif //PROJECT_HMAC_SM3_H
