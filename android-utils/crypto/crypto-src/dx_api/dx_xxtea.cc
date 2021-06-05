//
// Created by blue on 2018/10/14.
//

#include "dx_xxtea.h"
#include "xxtea.h"
#include <string>

#include "dx_crypto_config.h"

using namespace std;

STEE
string DX_XXTeaEncrypt(const string &key, const string &src) {
    size_t sz;
    void *result = NULL;

    string output;
    result = xxtea_encrypt_in(src.data(), src.size(), key.data(), &sz);
    if (result != NULL) {
        output.assign((char *) result, sz);
        free(result);
    }

    return output;
}

STEE
string DX_XXTeaDecrypt(const string &key, const string &input) {
    size_t sz;
    void *result = NULL;

    string output;
    result = xxtea_decrypt_in(input.data(), input.size(), key.data(), &sz);
    if (result != NULL) {
        output.assign((char *) result, sz);
        free(result);
    }

    return output;
}