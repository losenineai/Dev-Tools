#ifndef ANTIDEBUGGING
#define ANTIDEBUGGING
#include <iostream>
#include "elf_utils.h"


typedef struct _ANTI_MODULE {
    const char *note;// aka upx's command line argument
    unsigned char *shellcode; //shellcode pointer
    size_t length; // shellcode's length
}ANTI_MODULE;


class anti_debugging {
public:
    static bool update_anti_debugging(uint8_t* shellcode, 
        size_t length, 
        long rel_curfun2int, 
        long rel_curfun2antifun,
        arch_bit mode);
    static void update_magic(uint8_t* magic, long relvalue, arch_bit mode);
    static struct _ANTI_MODULE anti_modules[];
    
};



#endif
