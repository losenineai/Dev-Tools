//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_WBAES_H
#define CRYPTO_AS_PROJECT_DX_WBAES_H

#include "dx_api_prefix.h"

/**
 * 白盒aes 加密
 * @param key
 * @param input
 * @return
 */
std::string DX_WBAesEncrypt(const std::string &key, const std::string &input);

/**
 * 白盒aes 解密
 * @param key
 * @param input
 * @return
 */
std::string DX_WBAesDecrypt(const std::string &key, const std::string &input);

bool DX_IsValidKeyByWBAES(const std::string &key);

#endif //CRYPTO_AS_PROJECT_DX_WBAES_H
