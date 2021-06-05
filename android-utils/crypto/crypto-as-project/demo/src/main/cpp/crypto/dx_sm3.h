//
// Created by blue on 2018/10/15.
//

#ifndef CRYPTO_AS_PROJECT_DX_SM3_H
#define CRYPTO_AS_PROJECT_DX_SM3_H

#include "dx_api_prefix.h"

/**
 * sm3 加密
 * @param key
 * @param input
 * @return
 */
std::string DX_SM3Encrypt(const std::string &key, const std::string &input);


#endif //CRYPTO_AS_PROJECT_DX_SM3_H
