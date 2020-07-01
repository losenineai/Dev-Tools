//
// Created by white on 2018/6/8.
//

#ifndef ANDROID_ANTI_PKG_CERT_H
#define ANDROID_ANTI_PKG_CERT_H

#include <string>
#include "anti_config.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 包名检验
 * @param pkgVec
 * @return
 */
bool DXAnti_verify_pkg(const string &pkg);

/**
 * 签名检验
 * @param sigVec
 * @return
 */
bool DXAnti_verify_signature(const string &sign);



#ifdef __cplusplus
}
#endif

#endif //ANDROID_ANTI_PKG_CERT_H







