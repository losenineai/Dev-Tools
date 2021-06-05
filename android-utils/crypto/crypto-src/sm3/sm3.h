#ifndef HEADER_SM3_H
#define HEADER_SM3_H

#include "dx_crypto_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#define SM3_RESULT_LENGTH 32
#define SM3_BLOCK_SIZE 64


typedef struct {
    unsigned char result[SM3_BLOCK_SIZE];
    uint32_t digest[8];
    int blocks;
    int number;
} sm3;

void sm3_encrypt(unsigned char *data, size_t dataLen, unsigned char result[SM3_RESULT_LENGTH]);

void sm3_final(sm3 *ctx, unsigned char *result);

void sm3_init(sm3 *ctx);

void sm3_update(sm3 *ctx, const unsigned char *data, size_t dataLen);

#ifdef __cplusplus
}
#endif

#endif  //HEADER_SM3_H
