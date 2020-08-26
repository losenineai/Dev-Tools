//
// Created by blue on 2017/10/22.
//

#ifndef ANDROID_DXRisk_H
#define ANDROID_DXRisk_H

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "utils.h"
#include "stee_riskmgr.pb-c.h"
#include "pb_prefix.h"

using namespace std;

using std::map;
using std::vector;
using std::mutex;
using std::condition_variable;

typedef void (*DataCollectionAction)(map<string, string> &infos, void *args);

typedef struct DataCollectionActionNode {
    string name;
    DataCollectionAction action;
    DataCollectionActionNode *next;
    void *args;
} DataCollectionActionNode;


/**
 * DX风控组件
 */
class DXRisk {

public:
    map<string, string> m_paramsMap;
    string m_appId;
    mutex m_tokenLock;

private:

    bool m_bSetup;
    string m_url;
    string m_request;
    string m_token;
    
    int m_delayTime = 500;
    condition_variable m_networkCv;
    mutex m_networkLock;
    mutex m_dataLock;

    DataCollectionActionNode *m_collection_head;

    bool m_in_https_progress;
    bool m_bSaas;


private:

    string  Degrade(const string &appId, const string& str);
    string  GetDeviceInfo(map<string, string> &infos, bool isFull = true);
    static void    *ReportData(void *args);
    void    DoFetchCollectionInfo(map<string, string> &infos);

    string  GetDegradeToken(map<string, string> &infos, const string &request, bool isFull);

    string  GetCountry();
    string  GetTokenUrl();
    string  GetBackupUrl();
    bool    IsInputUrlFull();
    string  SafeBase64Encode(string in);

    void    DestoryCollectionAction();

public:

    DXRisk();

    virtual ~DXRisk();

    void    Init(const string &appId, map<string, string> &paramsMap);
    string  GetToken(bool bGetFullToken);
    
    void    AddCollectionAction( const string &name, DataCollectionAction action, void *args);

    string  GetParamByKey(const string &key, const string &defValue = "");

    bool    IsSaas() { return m_bSaas; }

};

#endif //ANDROID_DXRisk_H
