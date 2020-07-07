#include "hmac_sha256.h"
#include "sha2_mini.h"

#define B 64
#define L (SHA256_DIGEST_SIZE)
#define K (SHA256_DIGEST_SIZE * 2)

#define I_PAD 0x36
#define O_PAD 0x5C

void hmac_sha256(uint8_t out[HMAC_SHA256_DIGEST_SIZE],
                 const uint8_t *data, size_t data_len,
                 const uint8_t *key, size_t key_len) {

    SHA256Mini_ctx ss;
    uint8_t kh[SHA256_DIGEST_SIZE];

    if (key_len > B) {
        SHA256Mini_Init(&ss);
        SHA256Mini_Update(&ss, key, key_len);
        SHA256Mini_Final(&ss, kh);
        key_len = SHA256_DIGEST_SIZE;
        key = kh;
    }

    uint8_t kx[B];
    for (size_t i = 0; i < key_len; i++) kx[i] = I_PAD ^ key[i];
    for (size_t i = key_len; i < B; i++) kx[i] = I_PAD ^ 0;

    SHA256Mini_Init(&ss);
    SHA256Mini_Update(&ss, kx, B);
    SHA256Mini_Update(&ss, data, data_len);
    SHA256Mini_Final(&ss, out);

    for (size_t i = 0; i < key_len; i++) kx[i] = O_PAD ^ key[i];
    for (size_t i = key_len; i < B; i++) kx[i] = O_PAD ^ 0;

    SHA256Mini_Init(&ss);
    SHA256Mini_Update(&ss, kx, B);
    SHA256Mini_Update(&ss, out, SHA256_DIGEST_SIZE);
    SHA256Mini_Final(&ss, out);
}