#ifndef ufile_h
#define ufile_h

#include <stdbool.h>
#include <stdio.h>

class ufile
{
public:
    static bool save(const char* filePath, const unsigned char* pdata, size_t length);
    static size_t read(const char* filePath, unsigned char** ppdata);
    static bool is_existed(const char* filepath);
};

#endif // ufile_h