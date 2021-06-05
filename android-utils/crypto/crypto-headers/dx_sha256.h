//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_SHA256_H
#define CRYPTO_AS_PROJECT_DX_SHA256_H

#include "dx_api_prefix.h"

/**
 * SHA256运算
 * @param src
 * @return
 */
std::string DX_SHA256(const std::string &src);

std::string DX_SHA256WithKey(const std::string &src, const std::string &key);

#endif //CRYPTO_AS_PROJECT_DX_SHA256_H
