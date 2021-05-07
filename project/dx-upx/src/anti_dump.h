#ifndef PHDRSERASE
#define PHDRSERASE
#include <iostream>
#include "elf_utils.h"

typedef struct _ANTIDUMP_MODULE {
    const char *note;// aka upx's command line argument
    unsigned char *shellcode; //shellcode pointer
    size_t length; // shellcode's length
}ANTIDUMP_MODULE;

class anti_dump {
public:
    static bool update_anti_dump(uint8_t* shellcode,
        int length,
        long rel_curfun2init,
        long rel_curfunaddr,
        arch_bit mode);
    static void update_magic(uint8_t* magic, long relvalue, arch_bit mode);

    static struct _ANTIDUMP_MODULE anti_modules[];
};



#endif
