//
// Created by blue on 2018/10/11.
//

#include "dx_tools.h"
#include <string>
#include <memory.h>

using namespace std;

STEE
int DX_HexStringToChar(const string &src, unsigned char *dest) {

    unsigned char hb;
    unsigned char lb;

    int j = 0;

    if (src.size() % 2 != 0)
        return j;

    for (int i = 0; i < src.size(); i++) {
        hb = src[i];
        if (hb >= 'a' && hb <= 'f')
            hb = hb - 'a' + 10;
        else if (hb >= '0' && hb <= '9')
            hb = hb - '0';
        else
            return j;

        i++;
        lb = src[i];
        if (lb >= 'a' && lb <= 'f')
            lb = lb - 'a' + 10;
        else if (lb >= '0' && lb <= '9')
            lb = lb - '0';
        else
            return j;

        dest[j++] = (hb << 4) | (lb);
    }
    return j;
}

STEE
string DX_CharToHexString(unsigned char *pSrc, int nL) {

    char buf[nL * 2 + 1];
    string dest;

    memset((char *) buf, 0, sizeof(buf));

    unsigned char hb;
    unsigned char lb;

    for (int i = 0; i < nL; i++) {
        hb = (pSrc[i] & 0xf0) >> 4;

        if (hb >= 0 && hb <= 9)
            hb += 0x30;
        else if (hb >= 10 && hb <= 15)
            hb = hb - 10 + 'a';
        else
            return dest;

        lb = pSrc[i] & 0x0f;
        if (lb >= 0 && lb <= 9)
            lb += 0x30;
        else if (lb >= 10 && lb <= 15)
            lb = lb - 10 + 'a';
        else
            return dest;

        buf[i * 2] = hb;
        buf[i * 2 + 1] = lb;
    }

    dest = buf;

    return dest;
}