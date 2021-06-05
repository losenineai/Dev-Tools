//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_MTKEY_H
#define CRYPTO_AS_PROJECT_DX_MTKEY_H

#include "dx_api_prefix.h"

/**
 * MT马涛 加密
 * @param key
 * @param input
 * @return
 */
std::string DX_MTKeyEncrypt(const std::string &key, const std::string &input);

/**
 * MT马涛 解密
 * @param key
 * @param input
 * @return
 */
std::string DX_MTKeyDecrypt(const std::string &key, const std::string &input);


#endif //CRYPTO_AS_PROJECT_DX_MTKEY_H
