#ifndef __BASE64_H__
#define __BASE64_H__

#include "dx_crypto_config.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {BASE64_OK = 0, BASE64_INVALID};

#define BASE64_ENCODE_OUT_SIZE(s)	(((s) + 2) / 3 * 4)
#define BASE64_DECODE_OUT_SIZE(s)	(((s)) / 4 * 3)

int base64_encode_imp(const unsigned char *in, unsigned int inlen, char *out);

int base64_decode_imp(const char *in, unsigned int inlen, unsigned char *out);

#ifdef __cplusplus
}
#endif

#endif /* __BASE64_H__ */

