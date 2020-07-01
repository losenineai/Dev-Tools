//
// Created by blue on 2018/2/4.
//

#ifndef ANTI_EMULATOR_ANTI_EMULATOR_H
#define ANTI_EMULATOR_ANTI_EMULATOR_H

#include "anti_config.h"

bool anti_emu_qemu_checkArm_pc();

bool anti_emu_qemu_checkArm_cache();

bool anti_emu_qemu_checkArm_bkpt();

bool anti_emu_qemu_checkVbox();


//from Java
bool anti_emu_qemu_checkQemuProps();

bool anti_emu_qemu_checkQemuDriver();

bool anti_emu_qemu_checkQemuFiles();

#endif //ANTI_EMULATOR_ANTI_EMULATOR_H
