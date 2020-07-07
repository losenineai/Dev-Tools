#ifndef SHA2_MINI_H
#define SHA2_MINI_H
/*
 * SHA256 implementation from minicrypt library
 *
 * https://github.com/IanHarvey/minicrypt
 *
 * In the public domain. Note there is NO WARRANTY.
 */

#ifdef DX_UTILS_CRYPTO_PREFIX
DX_UTILS_CRYPTO_PREFIX
#else
#include "dx_utils_crypto_prefix.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "minicrypt.h"

typedef struct
{
  size_t count;
  uint8_t msgbuf[64];
  uint32_t H[8];
}
  SHA256Mini_ctx;

#define SHA256MINI_HASHLEN 32

extern void SHA256Mini_Init(SHA256Mini_ctx *ctx);
extern void SHA256Mini_Update(SHA256Mini_ctx *ctx, const void *pvmsg, size_t msglen);
extern void SHA256Mini_Final(SHA256Mini_ctx *ctx, uint8_t *out);

extern void SHA256Mini(const void *msg, size_t msglen, uint8_t *out32);

#ifdef __cplusplus
}
#endif

#endif /* SHA2_MINI_H */
