//
// Created by amlloc on 18-1-28.
//

#ifndef SYSCALLDEMO2_DX_UTILS_H
#define SYSCALLDEMO2_DX_UTILS_H

#include <string>
#include <stdio.h>
#include "dx_libc_stdio.h"
#include "dx_libc_config.h"

using namespace std;

dxFILE* dx_fopen(const char *file, const char* mode);

size_t dx_fread(void *buf, size_t size, size_t count, dxFILE *fp);

size_t dx_fwrite(const void *buf, size_t size, size_t count, dxFILE *fp);

int dx_fseeko(dxFILE *fp, off_t offset, int whence);

int dx_fseek(dxFILE *fp, off_t offset, int whence);

int dx_fclose(dxFILE *fp);

char* dx_fgets(char *buf, int n, dxFILE *fp);

long dx_ftell(dxFILE *fp);

/*****************************************************/


#endif //SYSCALLDEMO2_DX_UTILS_H
