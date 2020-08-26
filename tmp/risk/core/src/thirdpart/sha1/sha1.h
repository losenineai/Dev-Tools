#ifndef _STEE_SHA1_H_
#define _STEE_SHA1_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "prefix.h"

#define SHA1_HASH_BITS  160
#define SHA1_HASH_BYTES (SHA1_HASH_BITS/8)
#define SHA1_BLOCK_BITS 512
#define SHA1_BLOCK_BYTES (SHA1_BLOCK_BITS/8)

typedef struct {
    uint32_t h[5];
    uint64_t length;
} sha1_ctx_t;

void sha1_init(sha1_ctx_t *state);

void sha1_nextBlock(sha1_ctx_t *state, const void *block);

void sha1_lastBlock(sha1_ctx_t *state, const void *block, uint16_t length_b);

void sha1_ctx2hash(void *dest, sha1_ctx_t *state);

void sha1(void *dest, const void *msg, uint32_t length_b);

#ifdef __cplusplus
}
#endif

#endif /*SHA1_H_*/
