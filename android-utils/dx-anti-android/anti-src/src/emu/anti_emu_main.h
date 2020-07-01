//
// Created by white on 2018/5/3.
//

#ifndef ANTI_EMULATOR_COLLECT_EMULATOR_H
#define ANTI_EMULATOR_COLLECT_EMULATOR_H

#include <string>
#include "anti_config.h"

using namespace std;

/**
 * 检测模拟器，返回详细信息
 * @return 检测到信息 以 ID|ID|ID 形式返回
 */
string anti_emulator_get_str();

/**
 * 模拟器检测
 * @return
 */
bool anti_detect_emulator();

/**
 *
 * 模拟器计算机，计算出可能是模拟器的百分比
 *
 */
float anti_emu_calculateByPercent(const string &collecStr);

#endif //ANTI_EMULATOR_COLLECT_EMULATOR_H
