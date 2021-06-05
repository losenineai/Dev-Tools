//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_AES_H
#define CRYPTO_AS_PROJECT_DX_AES_H

#include "dx_api_prefix.h"

/**
 * aes ecb 加密
 * @param key
 * @param src
 * @return
 */
std::string DX_AesEcbEncrypt(const std::string &key, const std::string &input);


/**
 * aes ecb 解密
 * @param key
 * @param src
 * @return
 */
std::string DX_AesEcbDecrypt(const std::string &key, const std::string &input);

/**
 * aes cbc 加密
 * @param key
 * @param src
 * @return
 */
std::string DX_AesCbcEncrypt(const std::string &key, const std::string &iv, const std::string &input);


/**
 * aes cbc 解密
 * @param key
 * @param src
 * @return
 */
std::string DX_AesCbcDecrypt(const std::string &key, const std::string &iv, const std::string &input);

bool DX_IsValidKeyByAES(const std::string &key);

#endif //CRYPTO_AS_PROJECT_DX_AES_H
