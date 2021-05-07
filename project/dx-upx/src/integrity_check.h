#ifndef INTEGERITYCHECK
#define INTEGERITYCHECK
#include <iostream>
#include "elf_utils.h"

typedef struct _INTEGRITY_MODULE {
    const char *note;// aka upx's command line argument
    unsigned char *shellcode; //shellcode pointer
    size_t length; // shellcode's length    
}INTEGRITY_MODULE;


class integrity_check {
public:
    static bool update_integrity_check(uint8_t* shellcode, 
        int length, 
        long relCurfun2Syscall, 
        long relCurfun2IntegrityCheck, 
        long relCurfun2init,
        std::string filename,
        arch_bit mode);

    static struct _INTEGRITY_MODULE integrity_modules[];
    static bool update_hashcode(std::string filename);

private:
    static void update_magic(uint8_t* magic, long relvalue, arch_bit mode);
    static unsigned int cal_hashcode(unsigned char* buf, size_t length);
   
};



#endif
