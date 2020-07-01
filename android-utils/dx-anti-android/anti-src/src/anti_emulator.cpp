//
// Created by blue on 2018/7/4.
//

#include <unistd.h>
#include "anti_utils.h"
#include <comm_utils.h>
#include "anti_crash_log_prefix.h"
#include "anti_emu_main.h"
#include "anti_emulator.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"
#include "anti_result_handler.h"
#include "anti_config.h"


STEE
void *DXAnti_detect_emulator(void *arg) {
    anti_info_log("DXAnti_detect_emulator");

#ifdef ANTI_RETURN_ON

        string isEmuStr = anti_detect_emulator() ? "1:" : "0:";
        string emuInfo = anti_emulator_get_str();
        // 返回所有检测结果
        AntiResult result = {AntiType::AT_EMULATOR,
                             ANTI_EMULATOR_ERR_BASE + (int)anti_util_createRandom(ANTI_CRASH_RANDOM),
                             isEmuStr.append(emuInfo)};
        LOG_WARNING("%s", isEmuStr.c_str());
        AntiResultHandler::getInstance()->HandleResult(&result);
#else
    // 只有100%为模拟器才返回结果
    if(anti_detect_emulator()){

        string emuInfo = "1:" + anti_emulator_get_str();
        LOG_WARNING("%s", emuInfo.c_str());

        // 统一anti结果处理
        AntiResult result = {AntiType::AT_EMULATOR, ANTI_EMULATOR_ERR_BASE, anti_emulator_get_str()};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }
#endif


    return NULL;
}

STEE
int DXAnti_detect_emulator_async() {

    pthread_t newthread;
    int ret = -1;

    dx_anti_pthread_create(ret, &newthread, NULL, DXAnti_detect_emulator, NULL);

    return ret;
}

STEE
void DXAnti_detect_emulator_entry() {
    DXAnti_detect_emulator_async();
}