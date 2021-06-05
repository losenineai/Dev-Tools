//
// Created by blue on 2018/10/15.
//

#include "dx_sm3.h"
#include "hmac_sm3.h"
#include <memory.h>
#include "dx_crypto_config.h"

using namespace std;

#define delete_array(x) if((x)){delete [](x);(x) = NULL;}

/**
 * sm3 加密
 * @param key
 * @param input
 * @return
 */
STEE
string DX_SM3Encrypt(const string &key, const string &input){
    unsigned char result[32];
    char *inputChar = new char[input.length()];

    string resultStr;

    memcpy(inputChar, (const uint8_t *) input.data(), input.length());

    char buf[3];
    buf[2] = 0;

    if (key.empty()) {
        sm3_encrypt((unsigned char *) inputChar, input.length(), result);
    } else {
        sm3_hmac((const unsigned char *) inputChar, input.length(),
                 (const unsigned char *) key.data(), key.size(), result);
    }

    for (int i = 0; i < 32; i++) {
        sprintf(buf, "%02x", result[i]);
        resultStr.append(buf);
    }

    delete_array(inputChar);
    return resultStr;
}
