//
// Created by blue on 2018/10/15.
//

#include "dx_rc4.h"
#include "rc4.h"
#include <memory.h>
#include "dx_crypto_config.h"

using namespace std;

/**
 * rc4 加密
 * @param key
 * @param input
 * @return
 */
STEE
string DX_RC4(const string &key, const string &input) {

    rc4_state rc4k;
    rc4_init(&rc4k, (u_char *) key.c_str(), key.length());

    u_char *output = new u_char[input.size()];
    memset(output, 0, input.size());

    rc4_crypt(&rc4k, (u_char *) input.c_str(), output, input.size());

    string res((char *) output, input.size());

    if(output) {
        delete [](output);
        output = NULL;
    }

    return res;
}
