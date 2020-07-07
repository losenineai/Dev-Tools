#ifndef HEADER_SM4_H
#define HEADER_SM4_H

#ifdef DX_UTILS_CRYPTO_PREFIX
DX_UTILS_CRYPTO_PREFIX
#else
#include "dx_utils_crypto_prefix.h"
#endif

#define SM4_128BIT_KEY 16

#ifdef __cplusplus
extern "C" {
#endif

void sm4_encrypt(unsigned char key[16], unsigned char *input, int length, unsigned char *output);

void sm4_decrypt(unsigned char key[16], unsigned char *input, int length, unsigned char *output);


#ifdef __cplusplus
}
#endif

#endif  //HEADER_SM4_H
