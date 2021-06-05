//
// Created by blue on 2018/10/11.
//

#ifndef CRYPTO_AS_PROJECT_DX_UTILS_H
#define CRYPTO_AS_PROJECT_DX_UTILS_H

#include <string>
#include "dx_crypto_config.h"

int DX_HexStringToChar(const std::string &str, unsigned char *out);

std::string DX_CharToHexString(unsigned char *pSrc, int nL);

#endif //CRYPTO_AS_PROJECT_DX_UTILS_H
