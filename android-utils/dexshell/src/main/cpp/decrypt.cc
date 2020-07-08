#include <stdlib.h>
#include <string.h>

size_t simple_xor_decrypt(unsigned char* buf, size_t len){

    unsigned char xorByte = 0x51;

    for (int i = 0; i < len; ++i) {
        if (buf[i] == 0 || buf[i] == xorByte) {
            buf[i] = buf[i];
        } else {
            buf[i] ^= xorByte;
        }
    }

    return len;
}

size_t src_xor_decrypt(unsigned char* buf, size_t encLen, size_t fileLen){

    size_t encSize = encLen;
    if (fileLen <= encLen){
        encSize = fileLen;
    }

    return simple_xor_decrypt(buf, encSize);
}
