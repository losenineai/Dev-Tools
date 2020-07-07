//
// Created by RoseJames on 2017/8/2.
//
#include <string.h>
#include "sm3.h"
#include "hmac_sm3.h"
#include "dx_utils_crypto_stee.h"

#define IPAD    0x36
#define OPAD    0x5C

STEE
void sm3_hmac_init(sm3_hmac_ctx_t *ctx, const unsigned char *key, size_t key_len) {
    int i;

    if (key_len <= SM3_BLOCK_SIZE) {
        memcpy(ctx->key, key, key_len);
        memset(ctx->key + key_len, 0, SM3_BLOCK_SIZE - key_len);
    } else {
        sm3_init(&ctx->sm3_ctx);
        sm3_update(&ctx->sm3_ctx, key, key_len);
        sm3_final(&ctx->sm3_ctx, ctx->key);
        memset(ctx->key + SM3_RESULT_LENGTH, 0,
               SM3_BLOCK_SIZE - SM3_RESULT_LENGTH);
    }
    for (i = 0; i < SM3_BLOCK_SIZE; i++) {
        ctx->key[i] ^= IPAD;
    }

    sm3_init(&ctx->sm3_ctx);
    sm3_update(&ctx->sm3_ctx, ctx->key, SM3_BLOCK_SIZE);
}

STEE
void sm3_hmac_update(sm3_hmac_ctx_t *ctx,
                     const unsigned char *data, size_t data_len) {
    sm3_update(&ctx->sm3_ctx, data, data_len);
}

STEE
void sm3_hmac_final(sm3_hmac_ctx_t *ctx, unsigned char mac[SM3_RESULT_LENGTH]) {
    int i;
    for (i = 0; i < SM3_BLOCK_SIZE; i++) {
        ctx->key[i] ^= (IPAD ^ OPAD);
    }
    sm3_final(&ctx->sm3_ctx, mac);
    sm3_init(&ctx->sm3_ctx);
    sm3_update(&ctx->sm3_ctx, ctx->key, SM3_BLOCK_SIZE);
    sm3_update(&ctx->sm3_ctx, mac, SM3_RESULT_LENGTH);
    sm3_final(&ctx->sm3_ctx, mac);
}

STEE
void sm3_hmac(const unsigned char *data, size_t data_len,
              const unsigned char *key, size_t key_len,
              unsigned char mac[SM3_RESULT_LENGTH]) {
    sm3_hmac_ctx_t ctx;
    sm3_hmac_init(&ctx, key, key_len);
    sm3_hmac_update(&ctx, data, data_len);
    sm3_hmac_final(&ctx, mac);
    memset(&ctx, 0, sizeof(ctx));
}

