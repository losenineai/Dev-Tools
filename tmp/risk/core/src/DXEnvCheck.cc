//
// Created by white on 2019/3/19.
//

#include <mutex>
#include <output_log_utils.h>
#include "DXEnvCheck.h"
#include "utils.h"
#include "DXPlatform.h"


static DXEnvCheck* m_instance = NULL;
static mutex _mtx;//保证安全的互斥锁

DXEnvCheck::DXEnvCheck() : m_bSetup(false),
                           m_check_pid(),
                           m_check_head(NULL),
                           m_check_result_map()
{

}


DXEnvCheck::~DXEnvCheck() {
    DestoryCheckAction();
}

DXEnvCheck* DXEnvCheck::GetInstance() {
    info_log("DXEnvCheck::GetInstance");
    if(NULL == m_instance)
    {
        lock_guard<mutex> lock(_mtx);
        if(NULL == m_instance)
        {
            m_instance = new DXEnvCheck;
        }
    }
    return m_instance;
//    static DXEnvCheck instance;
//    return &instance;
}


NO_STEE
void* do_check_env_thread(void *arg) {

    DXEnvCheck *envCheck = (DXEnvCheck *) arg;
    if (envCheck){
        envCheck->OnSetupSuccess();
    }
#ifndef __APPLE__
    LOG_OUTPUT("%d", DXENVCHECK + 101);
#endif
    static mutex check_lock;
    lock_guard<mutex> g(check_lock);
//    bool run_flag = true;
    if (envCheck){
        while (1) {
            if(envCheck){
                envCheck->DoFetchCheckInfo();
            }else{
                break;
            }
#ifdef __APPLE__
            sleep(5);
#else
            sleep(5);
#endif
        }
    }
    return NULL;
}

NO_STEE
void DXEnvCheck::Setup(){
    if (m_bSetup) {
        return;
    }

    info_log("DXEnvCheck::Setup");
    LOG_OUTPUT("%d", DXENVCHECK + 100);


    // 检查危险环境线程
    static auto startCheckThread =[&](){
        dx_risk_check_init(*this);
        int result = pthread_create(&m_check_pid, NULL, do_check_env_thread, this);
        if(result != 0) {
            err_log("pthread_create error(-1)");
        }
        else{
            pthread_detach(m_check_pid);
        }
    };


    // 开启线程循环检测
    if (!m_bSetup) {
        startCheckThread();
    } else {
        int kill_rc = pthread_kill(m_check_pid, 0);
        if (kill_rc == ESRCH) {
            startCheckThread();
        }else if(kill_rc == EINVAL){
            err_log("pthread_create error(-2)");
        }else{
            err_log("pthread had setuped already (-3)");
        }
    }
}

STEE
void DXEnvCheck::AddCheckAction(const string &name, DataCheckAction action, void *args, bool singleRun) {

    DataCheckActionNode *node = new DataCheckActionNode();
    node->name = name;
    node->action = action;
    node->next = m_check_head;
    node->args = args;
    node->singleRun = singleRun;
    m_check_head = node;
}

STEE
void DXEnvCheck::DoFetchCheckInfo() {

    DataCheckActionNode *node = m_check_head;
    while (node != NULL ) {
        node->action(m_check_result_map, node->args, node->singleRun);
        node = node->next;
    }

#ifdef DEBUG
    // for debug, print all key
    for (auto i = m_check_result_map.begin(); i != m_check_result_map.end(); i++) {
        info_log("check key:%s value:%s", i->first.c_str(), i->second.c_str());
    }
#endif

}

STEE
map<string, string>& DXEnvCheck::GetResultMap() {
    return m_check_result_map;
}

NO_STEE
void DXEnvCheck::OnSetupSuccess(){
    m_bSetup = true;
}

STEE
void DXEnvCheck::DestoryCheckAction() {

    DataCheckActionNode *check_node = m_check_head;

    while (check_node) {
        DataCheckActionNode *old = check_node;
        check_node = check_node->next;
        delete_not_array(old);
    }

    m_check_head = NULL;
}
