#include "ufile.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool ufile::save(const char* filePath, const unsigned char* pdata, size_t length)
{
    bool flag = false;
    FILE* fp = NULL;

    if (filePath == NULL || pdata == NULL || length <= 0) {
        return false;
    }

    if (NULL != (fp = fopen(filePath, "wb"))) {
        size_t left = length;
        size_t ret = 0;
        
        flag = true;
        while (left > 0) {
            ret = (size_t )fwrite(pdata, 1, left, fp);
            if (ret > left) {
                flag = false;
                break;
            }
            left -= ret;
        }
        fclose(fp);
    }
    return flag;
}

size_t ufile::read(const char* filePath, unsigned char** ppdata)
{
    size_t length = 0;
    unsigned char* content = NULL;
    FILE* fp = NULL;
    
    if (filePath == NULL || ppdata == NULL) {
        return 0;
    }

    if (NULL != (fp = fopen(filePath, "rb"))) {
        fseek(fp, 0, SEEK_END);
        length = (int )ftell(fp);
        
        if (length > 0) {
            content = (unsigned char* )malloc((length) * sizeof(char));
        }
        
        if (content != NULL) {
            bool flag = true;
            int left = length;
            int ret = 0;
            
            memset((void* )content, 0, (length) * sizeof(char));
            fseek(fp, 0, SEEK_SET);
            while (left > 0) {
                ret = (int )fread(content, 1, length, fp);
                if (ret > left) {
                    flag = false;
                    break;
                }
                left -= ret;
            }
            
            if (!flag) {
                free(content);
                content = NULL;
            }
        }

        fclose(fp);
    }
    
    if (content == NULL) {
        length = 0;
    }

    *ppdata = content;
    return length;
}

bool ufile::is_existed(const char* filepath)
{
    return true;
}