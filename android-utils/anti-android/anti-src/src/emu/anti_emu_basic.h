//
// Created by amlloc on 18-7-11.
//

#ifndef DX_ANTI_EMULATOR_ANTI_EMU_BASIC_H
#define DX_ANTI_EMULATOR_ANTI_EMU_BASIC_H

#include "anti_config.h"
using namespace std;

bool anti_emu_basic_checkEmuBasicAll();
bool anti_emu_basic_checkPhoneNumber();
bool anti_emu_basic_checkDeviceId();
bool anti_emu_basic_checkImsi();
bool anti_emu_basic_checkVoiceMail();
bool anti_emu_basic_checkOperatorNameAndroid();
bool anti_emu_basic_checkIp();
bool anti_emu_basic_checkEth0();
bool anti_emu_basic_checkBuildInfo();
bool anti_emu_basic_checkPackageName();
bool anti_emu_basic_checkBattery();
bool anti_emu_basic_checkTaint();

#endif //DX_ANTI_EMULATOR_ANTI_EMU_BASIC_H
