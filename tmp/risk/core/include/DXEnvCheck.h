//
// Created by white on 2019/3/19.
//

#ifndef AS_RISK_DXENVGUARD_H
#define AS_RISK_DXENVGUARD_H

#include <string>
#include <map>

using namespace std;

typedef void (*DataCheckAction)(map<string, string> &infos, void *args, bool singleRun);

typedef struct DataCheckActionNode {
    string name;
    DataCheckAction action;
    DataCheckActionNode *next;
    void *args;
    bool singleRun;
} DataCheckActionNode;


/**
 * 环境风险检测
 */
class DXEnvCheck{

private:

    bool m_bSetup;
    pthread_t m_check_pid;
    DataCheckActionNode *m_check_head;
    map<string, string> m_check_result_map;

public:

    string m_appid;
    static DXEnvCheck* GetInstance();

    void Setup();

    void AddCheckAction(const string &name, DataCheckAction action, void *args, bool singleRun);

    map<string, string> &GetResultMap();

    void OnSetupSuccess();

    void DoFetchCheckInfo();

    void DestoryCheckAction();
    
    ~DXEnvCheck();
    
    DXEnvCheck();
};


#endif //AS_RISK_DXENVGUARD_H
