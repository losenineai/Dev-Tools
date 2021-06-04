//
// Created by white on 2018/8/4.
//

#include <unistd.h>
#include <thread>
#include <mutex>
#include <sys/syscall.h>
#include "anti_result_handler.h"
#include "anti_utils.h"
#include "utils_anti_log.h"
#include "anti_crash_log_prefix.h"
#include "comm_utils.h"
#include "dx_anti_stee.h"
#include "anti_config.h"

class lock_guard;

AntiResultHandler::AntiResultHandler() : m_mainPid(0),
                               m_inited(false){
}

STEE
AntiResultHandler* AntiResultHandler::getInstance() {
    static AntiResultHandler s_instance;
    return &s_instance;
}

STEE
void *dx_receive_sub_process_thread(void* arg) {

    AntiResultHandler* pHandler = (AntiResultHandler *) arg;

    AntiResult *remoteRet = NULL;
    int readInput = pHandler->m_pipe_sub_to_main[0];
    bool flag = true;
    while (flag) {

        bool locked = pHandler->m_readLock.try_lock();

        errno = 0;
        // block
        int ret = read(readInput, remoteRet, sizeof(AntiResult));
        anti_info_log("receive_sub_process_thread errno:%d return:%d type:%d ", errno, ret, remoteRet->type);

        // 处理子进程发送数据
        if (ret != -1 && remoteRet != NULL) {
            AntiResult *result = new AntiResult;
            result->crashCode = remoteRet->crashCode;
            result->emulatorStr = remoteRet->emulatorStr;
            result->type = remoteRet->type;
            if (pHandler->m_callback != NULL) {
                pHandler->m_callback(result);
            }
        }

        if(locked){
            pHandler->m_readLock.unlock();
        }
    }

    close(readInput);

    return NULL;
}

STEE
void AntiResultHandler::Init() {
    if (m_inited) {
        return;
    }

    m_inited = true;
    m_mainPid = getpid();
    pipe(m_pipe_sub_to_main);

    pthread_t newthread;
    int ret = 0;
    dx_pthread_create(ret, &newthread, NULL, dx_receive_sub_process_thread, this);
}

STEE
void AntiResultHandler::HandleResult(AntiResult *result){
    lock_guard<mutex> lockGuard(m_handleLock);

    pid_t curPid = getpid();
    AntiResult *ret = new AntiResult();
    ret->crashCode = result->crashCode + (int)anti_util_createRandom(ANTI_CRASH_RANDOM);
    if (!result->emulatorStr.empty()) {
        ret->emulatorStr.assign(result->emulatorStr);
    }
    ret->type = result->type;


#ifdef ANTI_RETURN_ON   // 返回结果，外部自行处理
    LOG_WARNING("%d", ret->crashCode);
    // cur process is main process
    if (curPid == m_mainPid) {
        if (m_callback != NULL) {
            m_callback(ret);
        }
    }
    // sub process, notify main process
    else{
        int wirteOutput = m_pipe_sub_to_main[1];
        write(wirteOutput, ret, sizeof(AntiResult));
    }

#else   // anti 内部处理
    LOG_CRASH("%d", ret->crashCode);

        // 父进程崩溃pipe中断，子进程不使用go_hell自杀
//        if (result->crashCode > ANTI_DEBUG_ERR_BASE && result->crashCode < ANTI_EMULATOR_ERR_BASE) {
//            anti_info_log("crashCode == anti debug");
//            syscall(__NR_kill, curPid, SIGKILL);
//            syscall(__NR_kill, m_mainPid, SIGKILL);
//        }
//        else{
//            dx_kill(curPid, SIGKILL);
//            dx_kill(m_mainPid, SIGKILL);
//        }
    dx_kill(curPid, SIGKILL);
    dx_kill(m_mainPid, SIGKILL);
#endif

}

STEE
void AntiResultHandler::SetResultCallback(AntiResultCallback callback) {
    m_callback = callback;
}
