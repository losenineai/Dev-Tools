#ifndef STEE_HMACSHA1_H_
#define STEE_HMACSHA1_H_

#include "sha1.h"
#include "dx_crypto_config.h"

#ifdef __cplusplus
extern "C" {
#endif

void
hmac_sha1(void *dest, const void *key, uint16_t keyLength, const void *msg, uint32_t msgLength);

#ifdef __cplusplus
}
#endif
#endif /*HMACSHA1_H_*/
