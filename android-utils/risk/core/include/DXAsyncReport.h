//
// Created by white on 2019/10/22.
//

#ifndef AS_RISK_DXASYNCREPORT_H
#define AS_RISK_DXASYNCREPORT_H

#include <string>
#include <map>

using namespace std;

// 异步上报默认间隔时间，单位s, 12 * 60 * 60 12小时
#define REPORT_DEFAULT_INTERVAL_TIME_S (43200)

typedef string (*AsyncReportAction)();

class DXAsyncReport{
private:
    map<string, string> m_data_map;
    map<string, AsyncReportAction> m_module_map;
    mutex m_networkLock;
    bool m_is_running;
    string m_appId;
    string m_url;

    void DoCollectData();

    void DoReportData();

    void ReqReportThread();

public:
    DXAsyncReport();

    ~DXAsyncReport();

    void Init(const string &appId, const string &url);

    void AddModule(const string &key, const AsyncReportAction &action);

    void SendDataAsync();

    bool IsNeedToReport();

    bool IsNeedToConfig();
};

#endif //AS_RISK_DXASYNCREPORT_H
