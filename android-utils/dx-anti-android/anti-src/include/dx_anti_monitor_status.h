//
// Created by white on 2018/11/28.
//

#ifndef ANTI_AS_PROJECT_DX_ANTI_MONITOR_STATUS_H
#define ANTI_AS_PROJECT_DX_ANTI_MONITOR_STATUS_H


#include <cstdint>
#include <pthread.h>
#include <map>
#include "anti_config.h"
#include "anti_utils.h"

using namespace std;


class AntiMonitorStatus{

    enum Status{
        IDLE, START_FAILED, RUNNING, FINISHED
    };

private:
    map<int64_t, Status> m_statusMap;

    AntiMonitorStatus();

public:

    static AntiMonitorStatus *GetInstance();

    void AddAnti(int64_t flag, int create_ret, pthread_t tid);

    bool CheckStatusRunning();

};

#endif //ANTI_AS_PROJECT_DX_ANTI_MONITOR_STATUS_H
