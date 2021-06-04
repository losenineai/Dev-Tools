//
// Created by blue on 2018/11/19.
//

#include <unistd.h>
#include "dx_anti.h"

#include "jni_comm_utils.h"
#include "jni_utils.h"
#include "comm_utils.h"
#include "anti_root.h"
#include "anti_log.h"
#include "anti_utils.h"
#include "anti_inject.h"
#include "anti_emulator.h"
#include "anti_virtual_app.h"
#include "anti_config.h"
#include "anti_thread.h"
#include "anti_multirun.h"
#include "anti_memdump.h"
#include "anti_debug.h"
#include "anti_flaw_janus.h"
#include "anti_debug_inner.h"
#include "dx_anti_stee.h"
#include "dx_anti_monitor_status.h"

STEE
void DXAnti_monitor_start(int64_t flag) {

    static bool l_started = false;

    if (l_started) {
        return;
    }
    l_started = true;

    pid_t pid = getpid();

    if (flag & DXANTI_FLAG_LOG) {
        int ret = DXAnti_detect_log_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_LOG, ret, 0);
    }

    if (flag & DXANTI_FLAG_ROOT) {
        int ret = DXAnti_detect_root_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_ROOT, ret, 0);
    }

    if (flag & DXANTI_FLAG_INJECT) {
        int ret = DXAnti_detect_inject_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_INJECT, ret, 0);
    }

    if (flag & DXANTI_FLAG_EMULATOR) {
        int ret = DXAnti_detect_emulator_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_EMULATOR, ret, 0);
    }

    if (flag & DXANTI_FLAG_MULTIRUN) {
        int ret = DXAnti_detect_multirun_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_MULTIRUN, ret, 0);
    }

    if (flag & DXANTI_FLAG_FLAW_JANUS) {
        int ret = DXAnti_detect_flaw_janus_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_FLAW_JANUS, ret, 0);
    }

    if (flag & DXANTI_FLAG_DEBUG) {
        // FIXME: 暂时屏蔽daemon模式，因为不稳定
        //            int ret = DXAnti_detect_debug_async();
        //            AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_DEBUG, ret, 0);
        pthread_t tid;
        int ret = DXAnti_detect_debug_inner_async(tid);
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_DEBUG_EXT_NO_DAEMON, ret, tid);
    }

    if (flag & DXANTI_FLAG_THREAD) {
        int ret = DXAnti_detect_thread_async(pid);
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_THREAD, ret, 0);
    }

    if (flag & DXANTI_FLAG_MEMDUMP) {
        // FIXME 在Android10上，会有误判的情况，目前先屏蔽功能，待完善
        if(anti_jni_get_sdk_int() != 29){
            pthread_t tid;
            int ret_dump = DXAnti_detect_memdump_async(tid, pid);
            AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_MEMDUMP, ret_dump, tid);  
        }
    }

    if (flag & DXANTI_FLAG_VIRTUAL_APP) {
        int ret = DXAnti_detect_virtual_app_async();
        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_VIRTUAL_APP, ret, 0);
    }


    // 无守护进程的ptrace 反调试
//    if (flag & DXANTI_FLAG_DEBUG_EXT_NO_DAEMON) {
//        pthread_t tid;
//        int ret = DXAnti_detect_debug_inner_async(tid);
//        AntiMonitorStatus::GetInstance()->AddAnti(DXANTI_FLAG_DEBUG_EXT_NO_DAEMON, ret, tid);
//    }

}

STEE
bool DXAnti_monitor_check() {
    return AntiMonitorStatus::GetInstance()->CheckStatusRunning();
}
