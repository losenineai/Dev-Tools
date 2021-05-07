#ifndef UPX_SIGNATURE_VERIFY
#define UPX_SIGNATURE_VERIFY

#include "elf_utils.h"
#include <iostream>

typedef struct _SIGNVERIFY_MODULE {
    const char *note;// aka upx's command line argument
    unsigned char *shellcode; //shellcode pointer
    size_t length; // shellcode's length
    //unsigned int debug_hashcode;
    //unsigned int hashcode;
    //const char *packname;
    //uint32_t offset;
}SIGNVERIFY_MODULE;


class signature_verify {
public:
    static bool update_signature_verify(uint8_t* shellcode,
        int length,
        long rel_onload2syscall,
        long rel_onload2checksign,
        long original_hook_func,
        int relHashcode,
        int relDebug_hashcode,
        arch_bit mode,
        arch_type archtype);

    static struct _SIGNVERIFY_MODULE signverify_modules[];

};

#endif