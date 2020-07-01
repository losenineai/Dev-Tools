//
// Created by white on 2018/5/15.
//

#include <cstring>
#include <unistd.h>
#include <include/anti_utils.h>
#include <src/utils/include/comm_utils.h>
#include <include/anti_crash_log_prefix.h>
#include "utils_anti_log.h"
#include "platform.h"
#include "anti_flaw_janus.h"
#include "dx_anti_stee.h"
#include "anti_result_handler.h"

STEE
void *DXAnti_detect_flaw_janus(void *arg) {

    char firstBytes[2]={ 0 };
    string pkgPath = anti_platform_getResPath();

    dxFILE* file = dx_fopen(pkgPath.c_str(), "rb");

    if (file != NULL) {
        dx_fread(firstBytes, 2, 1, file);
        dx_fclose(file);
    }

    if (memcmp("PK", firstBytes, 2)) {

        // 统一anti结果处理
        AntiResult result = {AntiType::AT_FLAW_JANUS, ANTI_FLAW_JANUS_ERR_BASE};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    return NULL;
}

STEE
int DXAnti_detect_flaw_janus_async() {

    pthread_t newthread;
    int ret = -1;

    dx_anti_pthread_create(ret, &newthread, NULL, DXAnti_detect_flaw_janus, NULL);

    return ret;
}

STEE
void DXAnti_detect_flaw_janus_entry() {
    DXAnti_detect_flaw_janus_async();
}