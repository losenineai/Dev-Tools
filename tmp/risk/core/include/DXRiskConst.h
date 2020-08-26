//
// Created by blue on 2018/1/26.
//

#ifndef ANDROID_DXRISKCONST_H
#define ANDROID_DXRISKCONST_H

#include <string>

using namespace std;

#define MAP_KEY_URL "KEY_URL"
#define MAP_KEY_COUNTRY "KEY_COUNTRY"
#define MAP_KEY_BACKUP "KEY_BACKUP"
#define MAP_KEY_BACKUP_APPID "KEY_BACKUP_APPID"
#define MAP_VALUE_ENABLE_BACKUP "VALUE_ENABLE_BACKUP"
//超时ms时间
#define MAP_KEY_DELAY_MS_TIME "KEY_DELAY_MS_TIME"

class DXRiskConst {

public:

    static string SERVER_CHINA;
    static string SERVER_INDONESIA;

    static string SERVICE_TOKEN;
    static string SERVICE_BACKUP;
    static string SERVICE_REPORT;

    static map<string, string> serverMap;
    static map<string, string> serviceMap;

    static void init() {

        SERVER_CHINA = "CHINA";
        SERVER_INDONESIA = "INDONESIA";
        
        SERVICE_TOKEN  = "TOKEN";
        SERVICE_BACKUP = "BACKUP";
        SERVICE_REPORT = "REPORT";
        
        serverMap[SERVER_CHINA] = "https://constid.dingxiang-inc.com";   // 中国
        serverMap[SERVER_INDONESIA] = "https://sgid.dingxiang-inc.com";  // 印尼

        serviceMap[SERVICE_TOKEN ]  = "/udid/m1";              // 获取token
        serviceMap[SERVICE_BACKUP] = "/udid/m2";              // 双写
        serviceMap[SERVICE_REPORT] = "/udid/getMsgReport";   // 数据降级上传

    }

};

map<string, string> DXRiskConst::serverMap;
map<string, string> DXRiskConst::serviceMap;

string DXRiskConst::SERVER_CHINA;
string DXRiskConst::SERVER_INDONESIA;

string DXRiskConst::SERVICE_TOKEN;
string DXRiskConst::SERVICE_BACKUP;
string DXRiskConst::SERVICE_REPORT;


#endif //ANDROID_DXRISKCONST_H
