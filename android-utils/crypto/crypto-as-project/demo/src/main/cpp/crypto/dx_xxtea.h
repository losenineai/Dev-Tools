//
// Created by blue on 2018/10/14.
//

#ifndef CRYPT_AS_PROJECT_DX_XXTEA_H
#define CRYPT_AS_PROJECT_DX_XXTEA_H

#include "dx_api_prefix.h"

/**
 * XXTea加密
 */
std::string DX_XXTeaEncrypt(const std::string &key, const std::string &src);

/**
 * XXTea解密
 */
std::string DX_XXTeaDecrypt(const std::string &key, const std::string &src);


#endif //CRYPT_AS_PROJECT_DX_XXTEA_H
