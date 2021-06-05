//
// Created by blue on 2018/10/11.
//

#include "dx_base64.h"
#include "base64.h"
#include <string>
#include <memory.h>

#include "dx_crypto_config.h"

//常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}

using namespace std;

STEE
string DX_Base64Encode(const string &input) {

    size_t encode_size = BASE64_ENCODE_OUT_SIZE(input.size());
    char *base64_encode_buf = new char[encode_size];
    memset(base64_encode_buf, 0, encode_size);

    string output;
    base64_encode_imp((const uint8_t *) input.data(), input.size(), base64_encode_buf);
    output.assign(base64_encode_buf, encode_size);
    delete_array(base64_encode_buf);

    return output;
}

STEE
string DX_Base64Decode(const string &input) {

    size_t decode_size = BASE64_DECODE_OUT_SIZE(input.size());

    for (int i = input.size() - 1; i > -1; i--) {
        if (input[i] == '=') {
            decode_size--;
        } else {
            break;
        }
    }

    unsigned char *base64_decode_buf = new unsigned char[decode_size];
    memset(base64_decode_buf, 0, decode_size);

    string output;
    int res_code = base64_decode_imp(input.data(), input.size(), base64_decode_buf);
    if (res_code == BASE64_OK) {
        output.assign((char *) base64_decode_buf, decode_size);
    }

    delete_array(base64_decode_buf);
    return output;
}

