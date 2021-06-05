//
// Created by blue on 2018/10/15.
//

#include "dx_sha256.h"
#include "sha2_mini.h"
#include "hmac_sha256.h"
#include "dx_tools.h"
#include "dx_crypto_config.h"

using namespace std;

STEE
string DX_SHA256(const string &src) {
    uint8_t buf[SHA256MINI_HASHLEN];
    SHA256Mini(src.data(), src.size(), buf);
    string input((char *) buf, SHA256MINI_HASHLEN);

    return DX_CharToHexString((unsigned char *)input.data(), input.size());
}

STEE
string DX_SHA256WithKey(const string &src, const string &key) {
    uint8_t buf[SHA256MINI_HASHLEN];

    hmac_sha256(buf, (const uint8_t *) src.data(), src.size(), (const uint8_t *) key.data(),
                key.size());

    string input((char *) buf, SHA256MINI_HASHLEN);
    string result = DX_CharToHexString((unsigned char *)input.data(), input.size());

    return result;
}