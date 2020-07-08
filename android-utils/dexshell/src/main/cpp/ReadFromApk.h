//
// Created by 冯永健 on 2019/2/26.
//

#ifndef DXSTUB_READFROMAPK_H
#define DXSTUB_READFROMAPK_H
#include <errno.h>
#include <string.h>
#include "zip/unzip.h"
#include "common.h"
size_t readFromApk(const char *apkFilepath, const char *filename, uint8_t **ppdata);
#endif //DXSTUB_READFROMAPK_H
