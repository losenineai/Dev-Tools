//
// Created by blue on 2018/10/12.
//

#ifndef CRYPTO_AS_PROJECT_DX_MD5_H
#define CRYPTO_AS_PROJECT_DX_MD5_H

#include "dx_api_prefix.h"

std::string DX_MD5(const std::string &src);

std::string DX_MD5WithKey(const std::string &src, const std::string &key);


#endif //CRYPTO_AS_PROJECT_DX_MD5_H
