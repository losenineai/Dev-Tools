//
// Created by amlloc on 18-7-16.
//

#include <string>
#include <jni.h>
#include <vector>
using namespace std;
#ifndef DX_ANTI_EMULATOR_SNTI_EMU_FILE_H
#define DX_ANTI_EMULATOR_SNTI_EMU_FILE_H

#include "anti_config.h"

bool anti_emu_file_checkFiles(vector<string> &targets);
bool anti_emu_file_checkFeatures(const string &dir, vector<string> &targets);


#endif //DX_ANTI_EMULATOR_SNTI_EMU_FILE_H
