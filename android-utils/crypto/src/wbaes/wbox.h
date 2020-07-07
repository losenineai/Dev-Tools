#ifndef _STEE_WBAES_H_
#define _STEE_WBAES_H_

#ifdef DX_UTILS_CRYPTO_PREFIX
DX_UTILS_CRYPTO_PREFIX
#else
#include "dx_utils_crypto_prefix.h"
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Wbox {
  uint8_t a[592*256];
};
void aes_wb_decryptor(struct Wbox *thiz, const uint8_t in[16],
                      uint8_t out[16]);

#ifdef __cplusplus
}
#endif

#endif
