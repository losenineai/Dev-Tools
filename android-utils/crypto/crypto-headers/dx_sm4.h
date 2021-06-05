//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_SM4_H
#define CRYPTO_AS_PROJECT_DX_SM4_H

#include "dx_api_prefix.h"

/**
 * sm4 加密
 * @param key
 * @param input
 * @return
 */
std::string DX_SM4Encrypt(const std::string &key, const std::string &input);

/**
 * sm4 解密
 * @param key
 * @param input
 * @return
 */
std::string DX_SM4Decrypt(const std::string &key, const std::string &input);

bool DX_IsValidKeyBySM4(const std::string &key);

#endif //CRYPTO_AS_PROJECT_DX_SM4_H
