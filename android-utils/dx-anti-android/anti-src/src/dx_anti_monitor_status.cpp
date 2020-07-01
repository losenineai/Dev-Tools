//
// Created by white on 2018/11/28.
//

#include "dx_anti_monitor_status.h"
#include "utils_anti_log.h"

AntiMonitorStatus::AntiMonitorStatus() {
    m_statusMap = map<int64_t, Status>();
}

AntiMonitorStatus *AntiMonitorStatus::GetInstance() {
    static AntiMonitorStatus status;
    return &status;
}


void AntiMonitorStatus::AddAnti(int64_t flag, int create_ret, pthread_t tid) {
    anti_info_log("AddAnti flag:%lld create_ret:%ld tid:%lu", flag, create_ret, tid);
    // 线程启动成功
    if (create_ret == 0) {
        if (tid <= 0) {
            m_statusMap[flag] = FINISHED;
        }
        else{
            int kill_rc = pthread_kill(tid, 0);
            // 线程已完成
            if (kill_rc == ESRCH || kill_rc == EINVAL) {
                m_statusMap[flag] = FINISHED;
            }
                // 线程正在运行
            else{
                m_statusMap[flag] = RUNNING;
            }
        }
    }
    // 线程启动失败
    else{
        m_statusMap[flag] = START_FAILED;
    }
}

bool AntiMonitorStatus::CheckStatusRunning() {
    anti_info_log("CheckStatusRunning");
    if (m_statusMap.size() <= 0) {
        return false;
    }

    for(auto status = m_statusMap.begin(); status != m_statusMap.end(); status++){
        anti_info_log("CheckAntiStatus flag:%lld status:%ld", status->first, status->second);

        if (status->second == IDLE
                || status->second == START_FAILED) {
            return false;
        }
    }

    return true;
}