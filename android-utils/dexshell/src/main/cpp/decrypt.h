#ifndef TESTDEMO_DECOMPRESS_H
#define TESTDEMO_DECOMPRESS_H

#include <stddef.h>
size_t simple_xor_decrypt(unsigned char* buf, size_t len);
size_t src_xor_decrypt(unsigned char* buf, size_t encLen, size_t fileLen);
#endif //TESTDEMO_DECOMPRESS_H
