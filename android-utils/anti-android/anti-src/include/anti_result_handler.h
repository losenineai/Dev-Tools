//
// Created by white on 2018/8/4.
//

#ifndef ANTI_AS_PROJECT_ANTI_CALLBACK_H
#define ANTI_AS_PROJECT_ANTI_CALLBACK_H

#include <mutex>
#include "anti_config.h"
#include "dx_anti.h"
#include <string>

using namespace std;

enum AntiType {
    AT_NONE,
    AT_DEBUG,
    AT_EMULATOR,
    AT_FLAW_JANUS,
    AT_INJECT,
    AT_LOG,
    AT_MULTIRUN,
    AT_ROOT,
    AT_MEMDUMP,
//    AT_VIRTUAL_APP, 合并到multrun检测中。
    AT_HOOK
};

struct AntiResult {
    AntiType type;
    int crashCode;
    string emulatorStr;

};

typedef void (*AntiResultCallback)(AntiResult *result);


class AntiResultHandler {
private:


    AntiResultHandler();

public:
    AntiResultCallback m_callback = NULL;
    pid_t m_mainPid;
    int m_pipe_sub_to_main[2] = {0};
    bool m_inited;
    mutex m_handleLock;
    mutex m_readLock;


    static AntiResultHandler *getInstance();

    void Init();

    void HandleResult(AntiResult *result);

    void SetResultCallback(AntiResultCallback callback);

};

#endif //ANTI_AS_PROJECT_ANTI_CALLBACK_H
