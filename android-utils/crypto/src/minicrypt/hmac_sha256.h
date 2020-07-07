//
// Created by RoseJames on 2017/8/2.
//

#ifndef PROJECT_HMAC_SHA256_H
#define PROJECT_HMAC_SHA256_H


#include <stddef.h>
#include <stdint.h>

#define HMAC_SHA256_DIGEST_SIZE 32
#define SHA256_DIGEST_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

void
hmac_sha256(uint8_t out[HMAC_SHA256_DIGEST_SIZE],
            const uint8_t *data, size_t data_len,
            const uint8_t *key, size_t key_len);

#ifdef __cplusplus
}
#endif

#endif //PROJECT_HMAC_SHA256_H
