//
//  mt_kp.h
//  temp
//
//  Created by mt on 2017/8/15.
//  Copyright © 2017年 mt. All rights reserved.
//

#include "mt_kp.h"

#define DX_ROR(v, n, base) (((v) >> n) | ((v) << ((base) - n)))
#define DX_ROL(v, n, base) (((v) << n) | ((v) >> ((base) - n)))

#define DX_ROR8(v, n) DX_ROR(v, ((n) & 0x0FF), 8)
#define DX_ROL8(v, n) DX_ROL(v, ((n) & 0x0FF), 8)

static PFUNC_MALLOC l_malloc = NULL;
static PFUNC_FREE   l_free = NULL;

uint32_t kex2k(uint32_t k_ex, uint32_t k_len)
{
    uint32_t v = k_ex;
    while (v >= k_len)
    {
        v = v - k_len;
    }
    return v;
}

bool kp_set_api(PFUNC_MALLOC pfunc_malloc, PFUNC_FREE pfunc_free)
{
    if (pfunc_malloc != NULL && pfunc_free != NULL)
    {
        l_malloc = pfunc_malloc;
        l_free = pfunc_free;
        return true;
    }
    return false;
}

void kp_unset_api()
{
    l_malloc = NULL;
    l_free = NULL;
}

void kp_encode_base(uint8_t* d, uint8_t* s, uint32_t len, uint8_t* k, uint32_t k_len)
{
    uint8_t tmp = 0;
    uint32_t i = 0;
    uint32_t ieven = 0;
    uint32_t iodd = 0;
    uint8_t sum = 0;
    
    for (i = 0; i < len; i++)
    {
        ieven = (i << 2);
        iodd = ieven + 1;
        tmp = s[i];
        tmp = (DX_ROR8(tmp, 2) + k[kex2k(ieven, k_len)]) ^ (~k[kex2k(iodd, k_len)]);
        tmp = (DX_ROR8(tmp, 3) + k[kex2k(iodd, k_len)]) ^ (~k[kex2k(ieven, k_len)]);
        d[len - 1 - i] = tmp;
    }
    
    if (len > 2)
    {
        d[0] = d[0] + ((d[len - 1] ^ (~d[len - 2])) ^ 0);
        d[1] = d[1] + ((d[0] ^ (~d[len - 1])) ^ 1);
        for (i = 2; i <= len  - 2; i++) {
            d[i] = d[i] + ((DX_ROL8(d[i - 1], 1) ^ (~d[i - 2])) ^ i);
        }
        
        d[len - 1] = d[len - 1] ^ d[len - 2];
        
        for (i = 1; i < len; i++) {
            sum += d[i];
        }
        d[0] = (d[0] ^ d[1]) + sum;
    }
}

void kp_decode_base(uint8_t* d, uint8_t* s, uint8_t* t,uint32_t len, uint8_t* k, uint32_t k_len)
{
    uint8_t tmp = 0;
    uint32_t i = 0;
    uint32_t ieven = 0;
    uint32_t iodd = 0;
    uint8_t sum = 0;
    uint8_t* pt = t;
    uint8_t* ps = s;

    for (int i = 0; i < len; ++i, pt++, ps++)
    {
        *pt = *ps;
    }
    
    if (len > 2)
    {
        for (i = 1; i < len; i++) {
            sum += t[i];
        }
        t[0] = (t[0] - sum) ^ t[1];
        
        t[len - 1] = t[len - 1] ^ t[len - 2];
        for (i = len - 2; i >= 2; i--) {
            t[i] = t[i] - ((DX_ROL8(t[i - 1], 1) ^ (~t[i - 2])) ^ i);
        }
        t[1] = t[1] - ((t[0] ^ (~t[len - 1])) ^ 1);
        t[0] = t[0] - ((t[len - 1] ^ (~t[len - 2])) ^ 0);
    }
    
    for (i = 0; i < len; i++)
    {
        ieven = (i << 2);
        iodd = ieven + 1;
        tmp = t[len - 1 - i];
        tmp = DX_ROL8((uint8_t )((tmp ^ (~k[kex2k(ieven, k_len)])) - k[kex2k(iodd, k_len)]), 3);
        tmp = DX_ROL8((uint8_t )((tmp ^ (~k[kex2k(iodd, k_len)])) - k[kex2k(ieven, k_len)]), 2);
        d[i] = tmp;
    }
}

void kp_encode(uint8_t* des, uint8_t* src, uint32_t data_len, uint8_t* k, uint32_t k_len)
{
    kp_encode_base(des, src, data_len, k, k_len);
    
}

bool kp_decode(uint8_t* des, uint8_t* src, uint32_t data_len, uint8_t* k, uint32_t k_len)
{
    if (l_malloc != NULL && l_free != NULL)
    {
        uint8_t* t = (uint8_t* )l_malloc(data_len);
        if (t == NULL)
        {
            return false;
        }
        kp_decode_base(des, src, t, data_len, k, k_len);
        l_free(t);
    }
    else
    {
        kp_decode_base(des, src, src, data_len, k, k_len);
    }
    return true;
}
