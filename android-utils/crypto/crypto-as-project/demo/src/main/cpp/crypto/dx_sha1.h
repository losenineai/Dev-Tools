//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_SHA1_H
#define CRYPTO_AS_PROJECT_DX_SHA1_H

#include "dx_api_prefix.h"

/**
 * SHA1运算
 */
std::string DX_SHA1(const std::string &src);

/**
 * 带Key的SHA1运算
 */
std::string DX_SHA1WithKey(const std::string &src, const std::string &key);

#endif //CRYPTO_AS_PROJECT_DX_SHA1_H
