#include "sm3.h"
#include <string.h>
#include "dx_utils_crypto_stee.h"


/* 原始代码：#define ROTATE_LEFT(X, n)  (((X)<<(n)) | ((X)>>(32-(n))))
 * 不同android版本中运行左移结果不一致，原因是对于左移的处理不同。
 * 部分版本结果是循环左移，部分版本是左移
 * 例，1 << 33，部分版本结果为1，部分版本结果为0
 * 此处先对代码进行处理，后续最好是在编译选项上直接处理而不是修改代码
 * from blue 2017.10.16 */

#define ROTATE_LEFT(X, n)  (((X)<<(n & 0x1F)) | ((X)>>(32-(n & 0x1F))))

#define P0(x) ((x) ^  ROTATE_LEFT((x),9)  ^ ROTATE_LEFT((x),17))
#define P1(x) ((x) ^  ROTATE_LEFT((x),15) ^ ROTATE_LEFT((x),23))

#define FF0(x, y, z) ( (x) ^ (y) ^ (z))
#define FF1(x, y, z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x, y, z) ( (x) ^ (y) ^ (z))
#define GG1(x, y, z) (((x) & (y)) | ( (~(x)) & (z)) )


#ifdef CPU_BIGENDIAN
#define be16_to_cpu(v) (v)
#else
#define cpu_to_be32(v) (((v)>>24) | (((v)>>8)&0xff00) | (((v)<<8)&0xff0000) | ((v)<<24))
#endif

STEE
void sm3_init(sm3 *ctx) {
    ctx->digest[0] = 0x7380166F;
    ctx->digest[1] = 0x4914B2B9;
    ctx->digest[2] = 0x172442D7;
    ctx->digest[3] = 0xDA8A0600;
    ctx->digest[4] = 0xA96F30BC;
    ctx->digest[5] = 0x163138AA;
    ctx->digest[6] = 0xE38DEE4D;
    ctx->digest[7] = 0xB0FB0E4E;

    ctx->number = 0;
    ctx->blocks = 0;
}

STEE
void sm3_compress(uint32_t digest[8], const unsigned char block[64]) {
    int j;
    uint32_t W[68], W1[64];
    const uint32_t *pblock = (const uint32_t *) block;

    uint32_t A = digest[0];
    uint32_t B = digest[1];
    uint32_t C = digest[2];
    uint32_t D = digest[3];
    uint32_t E = digest[4];
    uint32_t F = digest[5];
    uint32_t G = digest[6];
    uint32_t H = digest[7];
    uint32_t SS1, SS2, TT1, TT2, T[64];

    for (j = 0; j < 16; j++) {
        W[j] = cpu_to_be32(pblock[j]);
    }
    for (j = 16; j < 68; j++) {
        W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTATE_LEFT(W[j - 3], 15)) ^ ROTATE_LEFT(W[j - 13], 7) ^
               W[j - 6];;
    }
    for (j = 0; j < 64; j++) {
        W1[j] = W[j] ^ W[j + 4];
    }

    for (j = 0; j < 16; j++) {

        T[j] = 0x79CC4519;
        SS1 = ROTATE_LEFT((ROTATE_LEFT(A, 12) + E + ROTATE_LEFT(T[j], j)), 7);
        SS2 = SS1 ^ ROTATE_LEFT(A, 12);
        TT1 = FF0(A, B, C) + D + SS2 + W1[j];
        TT2 = GG0(E, F, G) + H + SS1 + W[j];
        D = C;
        C = ROTATE_LEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATE_LEFT(F, 19);
        F = E;
        E = P0(TT2);
    }

    for (j = 16; j < 64; j++) {

        T[j] = 0x7A879D8A;
        SS1 = ROTATE_LEFT((ROTATE_LEFT(A, 12) + E + ROTATE_LEFT(T[j], j)), 7);
        SS2 = SS1 ^ ROTATE_LEFT(A, 12);
        TT1 = FF1(A, B, C) + D + SS2 + W1[j];
        TT2 = GG1(E, F, G) + H + SS1 + W[j];
        D = C;
        C = ROTATE_LEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTATE_LEFT(F, 19);
        F = E;
        E = P0(TT2);
    }

    digest[0] ^= A;
    digest[1] ^= B;
    digest[2] ^= C;
    digest[3] ^= D;
    digest[4] ^= E;
    digest[5] ^= F;
    digest[6] ^= G;
    digest[7] ^= H;
}

STEE
void sm3_update(sm3 *ctx, const unsigned char *data, size_t dataLen) {
    if (ctx->number) {
        unsigned int left = SM3_BLOCK_SIZE - ctx->number;
        if (dataLen < left) {
            memcpy(ctx->result + ctx->number, data, dataLen);
            ctx->number += dataLen;
            return;
        } else {
            memcpy(ctx->result + ctx->number, data, left);
            sm3_compress(ctx->digest, ctx->result);
            ctx->blocks++;
            data += left;
            dataLen -= left;
        }
    }
    while (dataLen >= SM3_BLOCK_SIZE) {
        sm3_compress(ctx->digest, data);
        ctx->blocks++;
        data += SM3_BLOCK_SIZE;
        dataLen -= SM3_BLOCK_SIZE;
    }
    ctx->number = dataLen;
    if (dataLen) {
        memcpy(ctx->result, data, dataLen);
    }
}

STEE
void sm3_final(sm3 *ctx, unsigned char *result) {
    int i;
    uint32_t *pdigest = (uint32_t *) result;
    uint32_t *count = (uint32_t *) (ctx->result + SM3_BLOCK_SIZE - 8);

    ctx->result[ctx->number] = 0x80;

    if (ctx->number + 9 <= SM3_BLOCK_SIZE) {
        memset(ctx->result + ctx->number + 1, 0, SM3_BLOCK_SIZE - ctx->number - 9);
    } else {
        memset(ctx->result + ctx->number + 1, 0, SM3_BLOCK_SIZE - ctx->number - 1);
        sm3_compress(ctx->digest, ctx->result);
        memset(ctx->result, 0, SM3_BLOCK_SIZE - 8);
    }

    count[0] = cpu_to_be32((ctx->blocks) >> 23);
    count[1] = cpu_to_be32((ctx->blocks << 9) + (ctx->number << 3));

    sm3_compress(ctx->digest, ctx->result);
    for (i = 0; i < sizeof(ctx->digest) / sizeof(ctx->digest[0]); i++) {
        pdigest[i] = cpu_to_be32(ctx->digest[i]);
    }
}

STEE
void sm3_encrypt(unsigned char *data, size_t dataLen, unsigned char result[SM3_RESULT_LENGTH]) {

    sm3 ctx;
    sm3_init(&ctx);
    sm3_update(&ctx, data, dataLen);
    sm3_final(&ctx, result);

    memset(&ctx, 0, sizeof(sm3));
}






