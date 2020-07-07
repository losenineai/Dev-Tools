#ifndef platforms_arch_arm64_regs_h
#define platforms_arch_arm64_regs_h

#include "zkit.h"

#include "CommonKit/log/log_kit.h"

#include "instructions.h"

typedef enum _ARM64Reg {
    ARM64_REG_X0 = 0,
    ARM64_REG_X1,
    ARM64_REG_X2,
    ARM64_REG_X3,
    ARM64_REG_X4,
    ARM64_REG_X5,
    ARM64_REG_X6,
    ARM64_REG_X7,
    ARM64_REG_X8,
    ARM64_REG_X9,
    ARM64_REG_X10,
    ARM64_REG_X11,
    ARM64_REG_X12,
    ARM64_REG_X13,
    ARM64_REG_X14,
    ARM64_REG_X15,
    ARM64_REG_X16,
    ARM64_REG_X17,
    ARM64_REG_X18,
    ARM64_REG_X19,
    ARM64_REG_X20,
    ARM64_REG_X21,
    ARM64_REG_X22,
    ARM64_REG_X23,
    ARM64_REG_X24,
    ARM64_REG_X25,
    ARM64_REG_X26,
    ARM64_REG_X27,
    ARM64_REG_X28,
    ARM64_REG_X29,
    ARM64_REG_X30,
    ARM64_REG_X31,
    ARM64_REG_FP = ARM64_REG_X29,
    ARM64_REG_LR = ARM64_REG_X30,
    ARM64_REG_SP = ARM64_REG_X31
} ARM64Reg;

typedef struct _ARM64RegInfo {
    int index;
    int meta;
    int width;
    bool is_integer;
} ARM64RegInfo;

void arm64_register_describe(ARM64Reg reg, ARM64RegInfo *ri);

#endif