#ifndef PATCH_COMMON_H
#define PATCH_COMMON_H
#include <sys/types.h>

#define NUM_OF_PATCH    4
enum PATCH_INDEX {
    PI_SEG_FILESZ, PI_SEG_MEMSZ, PI_SEC_SIZE, PI_RET
};
enum PATCH_DT_INIT_INDEX {
    PDII_TYPE, PDII_ADDR
};
struct patch_info32 {
    off_t offset;
    unsigned int data;
};

struct patch_info64 {
    off_t offset;
    unsigned long data;
};
#endif