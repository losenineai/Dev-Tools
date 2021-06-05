#include <stdint.h>
#include <string.h>
#include "sha1.h"
#include "hmac_sha1.h"

#define IPAD 0x36
#define OPAD 0x5C

void hmac_sha1(void *dest, const void *key, uint16_t keyLength, const void *msg,
               uint32_t msgLength) {
    sha1_ctx_t s;
    uint8_t i;
    uint8_t buffer[SHA1_BLOCK_BYTES];

    memset(buffer, 0, SHA1_BLOCK_BYTES);

    if (keyLength > SHA1_BLOCK_BITS) {
        sha1((void *) buffer, key, keyLength);
    } else {
        memcpy(buffer, key, (keyLength + 7) / 8);
    }

    for (i = 0; i < SHA1_BLOCK_BYTES; ++i) {
        buffer[i] ^= IPAD;
    }
    sha1_init(&s);
    sha1_nextBlock(&s, buffer);
    while (msgLength >= SHA1_BLOCK_BITS) {
        sha1_nextBlock(&s, msg);
        msg = (uint8_t *) msg + SHA1_BLOCK_BYTES;
        msgLength -= SHA1_BLOCK_BITS;
    }
    sha1_lastBlock(&s, msg, msgLength);
    for (i = 0; i < SHA1_BLOCK_BYTES; ++i) {
        buffer[i] ^= IPAD ^ OPAD;
    }
    sha1_ctx2hash(dest, &s); /* save inner hash temporary to dest */
    sha1_init(&s);
    sha1_nextBlock(&s, buffer);
    sha1_lastBlock(&s, dest, SHA1_HASH_BITS);
    sha1_ctx2hash(dest, &s);
}

