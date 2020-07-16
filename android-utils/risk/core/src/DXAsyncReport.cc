//
// Created by white on 2019/10/22.
//


#include <thread>
#include <algorithm>
#include <protobuf-c/dx_protobuf-c.h>
#include <cinttypes>
#include <pb/stee_riskmgr.pb-c.h>
#include <pb/stee_base.pb-c.h>
#include <DXPlatform.h>
#include "utils.h"
#include "risk_config.h"
#include "DXAsyncReport.h"

DXAsyncReport::DXAsyncReport():m_data_map(),
                                m_module_map(),
                                m_networkLock(),
                                m_is_running(false),
                                m_appId(),
                                m_url()

{

}

DXAsyncReport::~DXAsyncReport(){
    
}

void DXAsyncReport::Init(const string &appId, const string &url) {
    m_appId = appId;
    m_url = url;
}


void DXAsyncReport::DoCollectData(){

    int64_t start = OthersUtils::CurrentTimes();

    for (auto it = m_module_map.begin(); it != m_module_map.end(); it++) {
        string data = it->second();
        m_data_map[it->first] = data;
    }
    char cost_c[10] = {0};
    sprintf(cost_c, "%" PRId64 , (OthersUtils::CurrentTimes() - start));
    m_data_map["cost"] = cost_c;

    m_data_map["token"] = get_property(m_appId, TOKEN);

    char time_c[16] = {0};
    sprintf(time_c, "%" PRId64 , time);
    m_data_map["createTime"] = time_c;

#ifdef DEBUG
    // for debug, print all key
    for (auto i = m_data_map.begin(); i != m_data_map.end(); i++) {
        LOGD("DXAsyncReport::DoCollectData key:%s value:%s", i->first.c_str(), i->second.c_str());
    }
#endif
}


void DXAsyncReport::DoReportData(){
    LOGD("DoReportData");

    string request_str = OthersUtils::CreateSTEERequestRaw(COM__DINGXIANG__MOBILE__STEEDATA_TYPE__DATATYPE_ASYNC_REPORT, m_appId, m_data_map);

    LOGD("DoReportData m_appId:%s, url:%s, req_str_len:%d", m_appId.c_str(), m_url.c_str(), request_str.size());

    string respone_str;
    OthersUtils::HWEERR err = OthersUtils::HttpWithEncrypt(m_url, request_str, respone_str, m_appId, true);

    LOGD("DoReportData res code:%d", err);
    if (err != OthersUtils::HWEERR::NOERROR) {
        return;
    }

    STEEResponse *pbRespone = com__dingxiang__mobile__steeresponse__unpack(NULL , respone_str.size(), (uint8_t*) respone_str.c_str());
    LOGD("DoReportData pbRespone->err_code:%d", pbRespone->err_code);

    if (pbRespone && pbRespone->err_code == COM__DINGXIANG__MOBILE__STEEERROR_CODE__ERR_NOERROR) {
        int64_t time = OthersUtils::CurrentTimesBySec();
        char time_c[16] = {0};
        sprintf(time_c, "%" PRId64 , time);

        set_property(m_appId, KEY_ASYNC_REPORT_TIME, time_c);
        LOGD("DoReportData set_property time:%s", time_c);
    }

    com__dingxiang__mobile__steeresponse__free_unpacked(pbRespone, NULL);

}


void DXAsyncReport::ReqReportThread(){
    LOGD("ReqReportThread");

    // 检查本地配置文件时间
    if (!IsNeedToConfig()) {
        m_is_running = false;
        return;
    }

    unique_lock <std::mutex> lck(m_networkLock);

    map<string, string> null_data;
    string request_str = OthersUtils::CreateSTEERequestRaw(COM__DINGXIANG__MOBILE__STEEDATA_TYPE__DATATYPE_ASYNC_REPORT_CONFIG, m_appId, null_data);

    LOGD("ReqReportThread m_appId:%s, url:%s, req_str_len:%d", m_appId.c_str(), m_url.c_str(), request_str.size());

    string respone_str;
    OthersUtils::HWEERR err = OthersUtils::HttpWithEncrypt(m_url, request_str, respone_str, m_appId, true);

    LOGD("ReqReportThread res code:%d", err);
    if (err != OthersUtils::HWEERR::NOERROR) {
        m_is_running = false;
        return;
    }

    STEEResponse *pbRespone = com__dingxiang__mobile__steeresponse__unpack(NULL , respone_str.size(), (uint8_t*) respone_str.c_str());
    LOGD("ReqReportThread pbRespone->err_code:%d", pbRespone->err_code);

    if (pbRespone && pbRespone->err_code == COM__DINGXIANG__MOBILE__STEEERROR_CODE__ERR_NOERROR && pbRespone->data.len > 0) {

        Com__Dingxiang__Mobile__STEERiskMgrGetReportConfigResponse *configRes =
                com__dingxiang__mobile__steerisk_mgr_get_report_config_response__unpack(NULL ,pbRespone->data.len, pbRespone->data.data);

        for(int i = 0; i < configRes->n_filters; i++){
            char *item = *(configRes->filters + i);
            LOGD("ReqReportThread item:%s", item);
            if (item) {
                set_property(m_appId, KEY_ASYNC_REPORT_CONFIG_JSON, item);

                if (IsNeedToReport()) {
                    // 采集数据
                    DoCollectData();
                    // 上报数据
                    DXAsyncReport::DoReportData();
                }
            }
        }
    }

    com__dingxiang__mobile__steeresponse__free_unpacked(pbRespone, NULL);
    m_is_running = false;


}



void DXAsyncReport::AddModule(const string &key, const AsyncReportAction &action){
    LOGD("AddModule key:%s", key.c_str());
    m_module_map[key] = action;
}




void DXAsyncReport::SendDataAsync(){
    if (!m_is_running) {
        m_is_running = true;

        thread t(&DXAsyncReport::ReqReportThread, this);
        t.detach();
        LOGD("SendDataAsync ReqConfigThread");
    }

}


bool DXAsyncReport::IsNeedToConfig(){

    int interval = 0;

    string json_config = get_property(m_appId, KEY_ASYNC_REPORT_CONFIG_JSON);
    LOGD("IsNeedToConfig json_config:%s", json_config.c_str());
    Json jsonRoot = JSonUtils::string2JSon(json_config);
    if (jsonRoot.is_object()) {
        map<string, Json> jsonMap = jsonRoot.object_items();
        if (jsonMap.find("interval") != jsonMap.end()) {
            interval = jsonMap["interval"].int_value();
        }
    }

    interval = interval > 0 ? interval : REPORT_DEFAULT_INTERVAL_TIME_S;


    string timeStr = get_property(m_appId, KEY_ASYNC_REPORT_TIME);
    LOGD("IsNeedToConfig timeStr:%s", timeStr.c_str());
    int64_t lastTime = 0;
    if (!timeStr.empty()) {
        lastTime = atoi(timeStr.c_str());
    }
    int64_t diff = OthersUtils::CurrentTimesBySec() - lastTime;
    LOGD("IsNeedToConfig lastTime:%lld", lastTime);
    LOGD("IsNeedToConfig diff:%lld", diff);
    LOGD("IsNeedToConfig interval:%d", interval);

    bool result = diff > interval;
    LOGD("IsNeedToConfig result:%d", result);
    return result;
}


bool DXAsyncReport::IsNeedToReport(){

    int interval = 0;

    string json_config = get_property(m_appId, KEY_ASYNC_REPORT_CONFIG_JSON);
    LOGD("IsNeedToReport json_config:%s", json_config.c_str());
    Json jsonRoot = JSonUtils::string2JSon(json_config);
    if (jsonRoot.is_object()) {
        map<string, Json> jsonMap = jsonRoot.object_items();

        int enable = 0;
        if (jsonMap.find("enable") != jsonMap.end()) {
            enable = jsonMap["enable"].int_value();
            if (enable == 0) {
                return false;
            }
        }
        if (jsonMap.find("interval") != jsonMap.end()) {
            interval = jsonMap["interval"].int_value();
        }
    }

    interval = interval > 0 ? interval : REPORT_DEFAULT_INTERVAL_TIME_S;

    string timeStr = get_property(m_appId, KEY_ASYNC_REPORT_TIME);
    LOGD("IsNeedToReport timeStr:%s", timeStr.c_str());
    int64_t lastTime = 0;
    if (!timeStr.empty()) {
        lastTime = atol(timeStr.c_str());
    }
    int64_t diff = OthersUtils::CurrentTimesBySec() - lastTime;
    LOGD("IsNeedToReport lastTime:%lld", lastTime);
    LOGD("IsNeedToReport diff:%lld", diff);
    LOGD("IsNeedToReport interval:%d", interval);

    bool result = diff > interval;
    LOGD("IsNeedToReport result:%d", result);
    return result;
}

