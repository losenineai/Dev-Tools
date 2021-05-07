#include "anti_dump.h"
#include "elf.h"
#include "log.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct _ANTIDUMP_HASHSIGN {
    unsigned char CURFUN2INIT[4];
    unsigned char CURFUNADDRESS[4];
    unsigned char ORIGPROT[4];
} ANTIDUMP_HASHSIGN={
    {0x10, 0xAB, 0xAB, 0xAB},
    {0x11, 0xAB, 0xAB, 0xAB},
    {0x12, 0xAB, 0xAB, 0xAB},
};

bool anti_dump::update_anti_dump(uint8_t* shellcode,
    int length,
    long rel_curfun2init,
    long rel_curfunaddr,
    arch_bit mode)
{
    off_t offx = 0;
    
    offx = elf_utils::search_bytes((const char*)shellcode, length, (const char*)ANTIDUMP_HASHSIGN.CURFUN2INIT, 4);
    //LOGD("offx : %d", offx);
    if (offx <0 ) {
        LOGE("can not find cur2checkintegeriry...");
        return false;
    }
    //LOGD("relCurfun2IntegrityCheck:%lx", relCurfun2IntegrityCheck);
    update_magic(shellcode+offx, rel_curfun2init, mode);
    // *(unsigned int *)(shellcode + offx) = (unsigned int)rel_curfun2init;

    offx = elf_utils::search_bytes((const char*)shellcode, length, (const char*)ANTIDUMP_HASHSIGN.CURFUNADDRESS, 4);
    if (offx < 0) {
        LOGE("can not find CURFUNADDRESS...");
        return false;
    }
    // update_magic(shellcode+offx, rel_curfunaddr, mode);
    *(unsigned int *)(shellcode + offx) = rel_curfunaddr;

    offx = elf_utils::search_bytes((const char*)shellcode, length, (const char*)ANTIDUMP_HASHSIGN.ORIGPROT, 4);
    //LOGD("offx : %d", offx);
    if(offx < 0 ){
        LOGE("can not find ORIGPROT...");
        return false;
    }
    *(unsigned int *)(shellcode + offx) = (unsigned int)(PF_X | PF_R);


    return true;
}


void anti_dump::update_magic(uint8_t* magic, long relvalue, arch_bit mode){
    if(mode == ARCH_BIT_32){
        *(uint32_t*)(magic) = (uint32_t)relvalue;
    }else if(mode == ARCH_BIT_64){
        *(uint64_t *)(magic) = relvalue;
    }
}

unsigned char anti_dump_shellcode32[] = {
  0x2d, 0xe9, 0xf8, 0x4f, 0x7f, 0x46, 0xa7, 0xf1, 0x08, 0x07, 0xdf, 0xf8, 0x14, 0x60, 0x05, 0x4d, 
  0x05, 0x4c, 0xa7, 0xeb, 0x05, 0x05, 0xa7, 0xeb, 0x06, 0x07, 0x07, 0xf1, 0x01, 0x07, 0x05, 0xe0, 
  0x10, 0xab, 0xab, 0xab, 0x11, 0xab, 0xab, 0xab, 0x12, 0xab, 0xab, 0xab, 0xb4, 0xf5, 0x80, 0x3f, 
  0x04, 0xd2, 0x2b, 0x78, 0x7f, 0x2b, 0x05, 0xd0, 0x16, 0xb9, 0x00, 0xbf, 0xbd, 0xe8, 0xf8, 0x8f, 
  0xb8, 0x47, 0xfa, 0xe7, 0x6b, 0x78, 0x45, 0x2b, 0xf6, 0xd1, 0xab, 0x78, 0x4c, 0x2b, 0xf3, 0xd1, 
  0xeb, 0x78, 0x46, 0x2b, 0xf0, 0xd1, 0xb5, 0xf8, 0x2c, 0xb0, 0x29, 0x46, 0xd5, 0xf8, 0x1c, 0x80, 
  0x03, 0x23, 0x7d, 0x20, 0x4f, 0xea, 0x4b, 0x1a, 0x0a, 0xf1, 0x34, 0x09, 0x4a, 0x46, 0x00, 0xf0, 
  0x42, 0xe8, 0x02, 0x21, 0xa9, 0x70, 0x05, 0x21, 0x69, 0x71, 0x08, 0x21, 0x29, 0x72, 0x0b, 0x21, 
  0xe9, 0x72, 0x0e, 0x21, 0xa9, 0x73, 0x08, 0xeb, 0x0a, 0x01, 0x00, 0x22, 0x01, 0x20, 0x03, 0x23, 
  0x68, 0x70, 0xeb, 0x70, 0x04, 0x20, 0x06, 0x23, 0x28, 0x71, 0xab, 0x71, 0x07, 0x20, 0x09, 0x23, 
  0xe8, 0x71, 0x6b, 0x72, 0x0a, 0x20, 0x0c, 0x23, 0xa8, 0x72, 0x2b, 0x73, 0x0d, 0x20, 0x0f, 0x23, 
  0x2a, 0x70, 0xeb, 0x73, 0x29, 0x44, 0x6a, 0x62, 0x05, 0xeb, 0x08, 0x03, 0x2a, 0x82, 0x68, 0x73, 
  0xbb, 0xf1, 0x00, 0x0f, 0x07, 0xd0, 0x5a, 0x60, 0x20, 0x33, 0x43, 0xf8, 0x0c, 0x2c, 0x43, 0xf8, 
  0x10, 0x2c, 0x8b, 0x42, 0xf7, 0xd1, 0x29, 0x46, 0x4a, 0x46, 0x23, 0x46, 0x7d, 0x20, 0x00, 0xf0, 
  0x0a, 0xe8, 0x00, 0x2e, 0xa9, 0xd0, 0xab, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0d, 0xc0, 0xa0, 0xe1, 0xf0, 0x00, 0x2d, 0xe9, 0x00, 0x70, 0xa0, 0xe1, 
  0x01, 0x00, 0xa0, 0xe1, 0x02, 0x10, 0xa0, 0xe1, 0x03, 0x20, 0xa0, 0xe1, 0x78, 0x00, 0x9c, 0xe8, 
  0x00, 0x00, 0x00, 0xef, 0xf0, 0x00, 0xbd, 0xe8, 0x1e, 0xff, 0x2f, 0xe1, 0x00, 0x00, 0x00, 0x00
};//should have more than 4 0x00 at the end of shellcode.


unsigned char anti_dump_shellcode64[] = {
  0xfd, 0x7b, 0xba, 0xa9, 0xfd, 0x03, 0x00, 0x91, 0xf3, 0x53, 0x01, 0xa9, 0xf5, 0x5b, 0x02, 0xa9, 
  0xf7, 0x63, 0x03, 0xa9, 0xf9, 0x23, 0x00, 0xf9, 0x16, 0x00, 0x00, 0x10, 0xd6, 0x62, 0x00, 0xd1, 
  0xd5, 0x00, 0x00, 0x58, 0xf4, 0x00, 0x00, 0x58, 0x13, 0x01, 0x00, 0x58, 0xd4, 0x02, 0x14, 0xcb, 
  0xd6, 0x02, 0x15, 0xcb, 0x07, 0x00, 0x00, 0x14, 0x10, 0xab, 0xab, 0xab, 0x00, 0x00, 0x00, 0x00, 
  0x11, 0xab, 0xab, 0xab, 0x00, 0x00, 0x00, 0x00, 0x12, 0xab, 0xab, 0xab, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0xff, 0x9f, 0x52, 0x7f, 0x02, 0x00, 0x6b, 0xc8, 0x00, 0x00, 0x54, 0x80, 0x02, 0x40, 0x39, 
  0x1f, 0xfc, 0x01, 0x71, 0x60, 0x01, 0x00, 0x54, 0x15, 0x01, 0x00, 0x35, 0x1f, 0x20, 0x03, 0xd5, 
  0xf3, 0x53, 0x41, 0xa9, 0xf5, 0x5b, 0x42, 0xa9, 0xf7, 0x63, 0x43, 0xa9, 0xf9, 0x23, 0x40, 0xf9, 
  0xfd, 0x7b, 0xc6, 0xa8, 0xc0, 0x03, 0x5f, 0xd6, 0xc0, 0x02, 0x3f, 0xd6, 0xf8, 0xff, 0xff, 0x17, 
  0x80, 0x06, 0x40, 0x39, 0x1f, 0x14, 0x01, 0x71, 0x81, 0xfe, 0xff, 0x54, 0x80, 0x0a, 0x40, 0x39, 
  0x1f, 0x30, 0x01, 0x71, 0x21, 0xfe, 0xff, 0x54, 0x80, 0x0e, 0x40, 0x39, 0x1f, 0x18, 0x01, 0x71, 
  0xc1, 0xfd, 0xff, 0x54, 0x98, 0x72, 0x40, 0x79, 0xe1, 0x03, 0x14, 0xaa, 0x40, 0x1c, 0x80, 0x52, 
  0x63, 0x00, 0x80, 0x52, 0x19, 0x67, 0x1a, 0x53, 0x97, 0x12, 0x40, 0xf9, 0x39, 0x0f, 0x18, 0x4b, 
  0x39, 0x03, 0x01, 0x11, 0xe2, 0x03, 0x19, 0x2a, 0x9b, 0x00, 0x00, 0x94, 0xe1, 0x03, 0x14, 0xcb, 
  0x21, 0x08, 0x00, 0x72, 0xe0, 0x0f, 0x00, 0x54, 0x9f, 0x02, 0x00, 0x39, 0x3f, 0x04, 0x00, 0x71, 
  0x80, 0x11, 0x00, 0x54, 0x20, 0x00, 0x80, 0x52, 0x3f, 0x08, 0x00, 0x71, 0x80, 0x06, 0x00, 0x39, 
  0xc0, 0x11, 0x00, 0x54, 0x40, 0x00, 0x80, 0x52, 0x3f, 0x0c, 0x00, 0x71, 0x80, 0x0a, 0x00, 0x39, 
  0xe0, 0x10, 0x00, 0x54, 0x60, 0x00, 0x80, 0x52, 0x3f, 0x10, 0x00, 0x71, 0x80, 0x0e, 0x00, 0x39, 
  0x40, 0x0f, 0x00, 0x54, 0x80, 0x00, 0x80, 0x52, 0x3f, 0x14, 0x00, 0x71, 0x80, 0x12, 0x00, 0x39, 
  0x60, 0x0e, 0x00, 0x54, 0xa0, 0x00, 0x80, 0x52, 0x3f, 0x1c, 0x00, 0x71, 0x80, 0x16, 0x00, 0x39, 
  0xa1, 0x0e, 0x00, 0x54, 0xc0, 0x00, 0x80, 0x52, 0x22, 0x01, 0x80, 0x52, 0x80, 0x1a, 0x00, 0x39, 
  0xe0, 0x03, 0x01, 0x2a, 0x05, 0x02, 0x80, 0x52, 0xe3, 0x03, 0x01, 0x2a, 0x04, 0x01, 0x80, 0x52, 
  0xa1, 0x00, 0x01, 0x4b, 0x25, 0x00, 0x80, 0x52, 0x06, 0x04, 0x00, 0x11, 0xa0, 0x5b, 0x00, 0xb9, 
  0xa6, 0x5f, 0x00, 0xb9, 0xa6, 0x63, 0x01, 0x91, 0x42, 0x04, 0x00, 0x0f, 0x83, 0x02, 0x03, 0x8b, 
  0xc0, 0x00, 0x40, 0xfd, 0xbf, 0x08, 0x00, 0x71, 0x83, 0x04, 0x00, 0x0f, 0xc5, 0x04, 0x00, 0x0f, 
  0x04, 0x05, 0x00, 0x0f, 0x02, 0x84, 0xa2, 0x0e, 0x03, 0x84, 0xa3, 0x0e, 0x01, 0x1c, 0xa0, 0x0e, 
  0x05, 0x84, 0xa5, 0x0e, 0x04, 0x84, 0xa4, 0x0e, 0x41, 0x04, 0x18, 0x6e, 0x62, 0x1c, 0xa3, 0x0e, 
  0x23, 0x28, 0x61, 0x0e, 0xa2, 0x04, 0x18, 0x6e, 0x61, 0x1c, 0xa3, 0x0e, 0x42, 0x28, 0x61, 0x0e, 
  0x41, 0x04, 0x18, 0x6e, 0x21, 0x28, 0x21, 0x0e, 0x61, 0x00, 0x00, 0xfd, 0x41, 0x02, 0x00, 0x54, 
  0x41, 0x05, 0x00, 0x0f, 0x63, 0x20, 0x00, 0x91, 0x83, 0x05, 0x00, 0x0f, 0x82, 0x1c, 0xa4, 0x0e, 
  0xc4, 0x05, 0x00, 0x0f, 0x01, 0x84, 0xa1, 0x0e, 0x03, 0x84, 0xa3, 0x0e, 0x00, 0x84, 0xa4, 0x0e, 
  0x22, 0x04, 0x18, 0x6e, 0x61, 0x1c, 0xa3, 0x0e, 0x42, 0x28, 0x61, 0x0e, 0x01, 0x04, 0x18, 0x6e, 
  0x40, 0x1c, 0xa2, 0x0e, 0x21, 0x28, 0x61, 0x0e, 0x20, 0x04, 0x18, 0x6e, 0x00, 0x28, 0x21, 0x0e, 
  0x60, 0x00, 0x00, 0xfd, 0x3f, 0x00, 0x04, 0x6b, 0x00, 0x00, 0x04, 0x0b, 0x42, 0x00, 0x04, 0x4b, 
  0x40, 0x03, 0x00, 0x54, 0x80, 0xca, 0x20, 0x38, 0x5f, 0x04, 0x00, 0x71, 0x01, 0x04, 0x00, 0x11, 
  0xc0, 0x02, 0x00, 0x54, 0x81, 0xca, 0x21, 0x38, 0x5f, 0x08, 0x00, 0x71, 0x01, 0x08, 0x00, 0x11, 
  0x40, 0x02, 0x00, 0x54, 0x81, 0xca, 0x21, 0x38, 0x5f, 0x0c, 0x00, 0x71, 0x01, 0x0c, 0x00, 0x11, 
  0xc0, 0x01, 0x00, 0x54, 0x81, 0xca, 0x21, 0x38, 0x5f, 0x10, 0x00, 0x71, 0x01, 0x10, 0x00, 0x11, 
  0x40, 0x01, 0x00, 0x54, 0x81, 0xca, 0x21, 0x38, 0x5f, 0x14, 0x00, 0x71, 0x01, 0x14, 0x00, 0x11, 
  0xc0, 0x00, 0x00, 0x54, 0x81, 0xca, 0x21, 0x38, 0x5f, 0x18, 0x00, 0x71, 0x00, 0x18, 0x00, 0x11, 
  0x40, 0x00, 0x00, 0x54, 0x80, 0xca, 0x20, 0x38, 0x9f, 0x32, 0x00, 0xb9, 0x9f, 0x22, 0x00, 0x79, 
  0xb8, 0x01, 0x00, 0x34, 0x18, 0x07, 0x00, 0x51, 0xe1, 0xe2, 0x00, 0x91, 0x02, 0x07, 0x80, 0x52, 
  0x80, 0x02, 0x17, 0x8b, 0x01, 0x07, 0xa2, 0x9b, 0x81, 0x02, 0x01, 0x8b, 0x1f, 0x04, 0x00, 0xf9, 
  0x00, 0xe0, 0x00, 0x91, 0x1f, 0x00, 0x01, 0xeb, 0x1f, 0x00, 0x1f, 0xf8, 0x1f, 0x80, 0x1e, 0xf8, 
  0x61, 0xff, 0xff, 0x54, 0xe1, 0x03, 0x14, 0xaa, 0xe2, 0x03, 0x19, 0x2a, 0xe3, 0x03, 0x13, 0x2a, 
  0x40, 0x1c, 0x80, 0x52, 0x1c, 0x00, 0x00, 0x94, 0xb5, 0xec, 0xff, 0x34, 0x6b, 0xff, 0xff, 0x17, 
  0x04, 0x02, 0x80, 0x52, 0x45, 0x00, 0x80, 0x52, 0xe1, 0x03, 0x04, 0x2a, 0x03, 0x00, 0x80, 0xd2, 
  0xe2, 0x03, 0x04, 0x2a, 0x00, 0x00, 0x80, 0x52, 0x9c, 0xff, 0xff, 0x17, 0x62, 0x01, 0x80, 0x52, 
  0xe0, 0x03, 0x01, 0x2a, 0x94, 0xff, 0xff, 0x17, 0x82, 0x01, 0x80, 0x52, 0x80, 0x00, 0x80, 0x52, 
  0x91, 0xff, 0xff, 0x17, 0x42, 0x01, 0x80, 0x52, 0xc0, 0x00, 0x80, 0x52, 0x8e, 0xff, 0xff, 0x17, 
  0xe2, 0x01, 0x80, 0x52, 0xe0, 0x03, 0x01, 0x2a, 0x8b, 0xff, 0xff, 0x17, 0xa2, 0x01, 0x80, 0x52, 
  0xe0, 0x03, 0x01, 0x2a, 0x88, 0xff, 0xff, 0x17, 0xc2, 0x01, 0x80, 0x52, 0xe0, 0x03, 0x01, 0x2a, 
  0x85, 0xff, 0xff, 0x17, 0xe8, 0x03, 0x00, 0xaa, 0xe0, 0x03, 0x01, 0xaa, 0xe1, 0x03, 0x02, 0xaa, 
  0xe2, 0x03, 0x03, 0xaa, 0xe3, 0x03, 0x04, 0xaa, 0xe4, 0x03, 0x05, 0xaa, 0xe5, 0x03, 0x06, 0xaa, 
  0x01, 0x00, 0x00, 0xd4, 0xc0, 0x03, 0x5f, 0xd6, 0x00, 0x00, 0x00, 0x00
};//should have more than 4 0x00 at the end of shellcode.


unsigned char anti_dump_shellcodeX86[] = {
  0x55, 0x57, 0x56, 0x53, 0xe8, 0x9c, 0x01, 0x00, 0x00, 0x81, 0xc3, 0x3f, 0x1b, 0x00, 0x00, 0x8d, 
  0x64, 0x24, 0xc4, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x5a, 0x83, 0xea, 0x18, 0xbd, 0x10, 0xab, 0xab, 
  0xab, 0xbf, 0x11, 0xab, 0xab, 0xab, 0xbe, 0x12, 0xab, 0xab, 0xab, 0x89, 0xd0, 0x29, 0xf8, 0x89, 
  0xc7, 0x29, 0xea, 0x81, 0xfe, 0xff, 0xff, 0x00, 0x00, 0x77, 0x0a, 0x80, 0x3f, 0x7f, 0x74, 0x18, 
  0x85, 0xed, 0x75, 0x0c, 0x90, 0x8d, 0x64, 0x24, 0x3c, 0x5b, 0x5e, 0x5f, 0x5d, 0xc3, 0x66, 0x90, 
  0xff, 0xd2, 0xeb, 0xf0, 0x8d, 0x74, 0x26, 0x00, 0x80, 0x7f, 0x01, 0x45, 0x8d, 0x74, 0x26, 0x00, 
  0x75, 0xde, 0x80, 0x7f, 0x02, 0x4c, 0x75, 0xd8, 0x80, 0x7f, 0x03, 0x46, 0x75, 0xd2, 0x0f, 0xb7, 
  0x47, 0x2c, 0x8b, 0x4f, 0x1c, 0xc7, 0x44, 0x24, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x66, 0x89, 0x44, 
  0x24, 0x1c, 0xc1, 0xe0, 0x05, 0x89, 0x4c, 0x24, 0x28, 0x8d, 0x48, 0x34, 0x89, 0x7c, 0x24, 0x04, 
  0xc7, 0x04, 0x24, 0x7d, 0x00, 0x00, 0x00, 0x89, 0x4c, 0x24, 0x08, 0x89, 0x54, 0x24, 0x2c, 0x89, 
  0x44, 0x24, 0x20, 0x89, 0x4c, 0x24, 0x24, 0xe8, 0xbc, 0x00, 0x00, 0x00, 0x8b, 0x4c, 0x24, 0x28, 
  0xc6, 0x07, 0x00, 0xc6, 0x47, 0x01, 0x01, 0xc6, 0x47, 0x02, 0x02, 0x8d, 0x04, 0x0f, 0xc6, 0x47, 
  0x03, 0x03, 0x03, 0x4c, 0x24, 0x20, 0xc6, 0x47, 0x04, 0x04, 0xc6, 0x47, 0x05, 0x05, 0xc6, 0x47, 
  0x06, 0x06, 0x01, 0xf9, 0xc6, 0x47, 0x07, 0x07, 0x66, 0x83, 0x7c, 0x24, 0x1c, 0x00, 0xc6, 0x47, 
  0x08, 0x08, 0xc6, 0x47, 0x09, 0x09, 0xc6, 0x47, 0x0a, 0x0a, 0xc6, 0x47, 0x0b, 0x0b, 0xc6, 0x47, 
  0x0c, 0x0c, 0xc6, 0x47, 0x0d, 0x0d, 0xc6, 0x47, 0x0e, 0x0e, 0xc6, 0x47, 0x0f, 0x0f, 0xc7, 0x47, 
  0x24, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x54, 0x24, 0x2c, 0x66, 0xc7, 0x47, 0x10, 0x00, 0x00, 0x74, 
  0x2b, 0xeb, 0x0d, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 
  0xc7, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc0, 0x20, 0xc7, 0x40, 0xf4, 0x00, 0x00, 0x00, 
  0x00, 0xc7, 0x40, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x39, 0xc8, 0x75, 0xe4, 0x8b, 0x44, 0x24, 0x24, 
  0x89, 0x74, 0x24, 0x0c, 0x89, 0x7c, 0x24, 0x04, 0xc7, 0x04, 0x24, 0x7d, 0x00, 0x00, 0x00, 0x89, 
  0x44, 0x24, 0x08, 0x89, 0x54, 0x24, 0x1c, 0xe8, 0x0c, 0x00, 0x00, 0x00, 0x8b, 0x54, 0x24, 0x1c, 
  0xe9, 0xdb, 0xfe, 0xff, 0xff, 0x8d, 0x76, 0x00, 0x53, 0x56, 0x57, 0x55, 0x50, 0x8b, 0x44, 0x24, 
  0x18, 0x8b, 0x5c, 0x24, 0x1c, 0x8b, 0x4c, 0x24, 0x20, 0x8b, 0x54, 0x24, 0x24, 0x8b, 0x74, 0x24, 
  0x28, 0x8b, 0x7c, 0x24, 0x2c, 0x8b, 0x6c, 0x24, 0x30, 0xcd, 0x80, 0x83, 0xc4, 0x04, 0x3d, 0x01, 
  0xf0, 0xff, 0xff, 0x72, 0x06, 0xf7, 0xd8, 0x50, 0x83, 0xc4, 0x04, 0x5d, 0x5f, 0x5e, 0x5b, 0xc3, 
  0x90,  0x8d,  0x74,  0x26,  0x00,  0x8b,  0x1c,  0x24,  0xc3,  0x00, 0x00, 0x00, 0x00
}; //should have more than 4 0x00 at the end of shellcode.


_ANTIDUMP_MODULE anti_dump::anti_modules[] = {
    {
        .note = "anti_dump32",
        .shellcode = anti_dump_shellcode32,
        .length = sizeof(anti_dump_shellcode32)
    },
    {
        .note = "anti_dump64",
        .shellcode = anti_dump_shellcode64,
        .length = sizeof(anti_dump_shellcode64)
    },
    {
        .note = "anti_dumpX86",
        .shellcode = anti_dump_shellcodeX86,
        .length = sizeof(anti_dump_shellcodeX86)
    }
};