//
// Created by blue on 2017/10/22.
//

#include <thread>
#include "DXRiskInter.h"
#include "DXPlatform.h"
#include "common.h"
#include <pthread.h>
#include "risk_config.h"
#include "DXRiskConst.h"
#include "DXEnvCheck.h"
#include <algorithm>
#include <protobuf-c/dx_protobuf-c.h>
#include <cinttypes>
#include <stee_riskmgr.pb-c.h>
#include <DXAsyncReport.h>

#ifdef __APPLE__
#include "prefix.h"
#else
#include "dx_libc.h"
#endif

using namespace std;

DXRiskInter::DXRiskInter() : m_appId(string("")),
                   m_bSetup(false),
                   m_token(string("")),
                   m_collection_head(NULL),
                   m_in_https_progress(false),
                   m_bSaas(true),
                   m_asyncReport()
{
    DXRiskConst::init();
}

DXRiskInter::~DXRiskInter() {
    DestoryCollectionAction();
}


STEE
void DXRiskInter::Init(const string &appId, map<string, string> &paramsMap){

    LOGR("%d", TAG_DXRISK + 100);

    m_appId = appId;
    m_paramsMap = paramsMap;


    char buf[16] = {0};
    dx_sprintf(buf, "%d", m_delayTime);
    string defaultTime(buf);

    m_delayTime = atoi(GetParamByKey(MAP_KEY_DELAY_MS_TIME , defaultTime).c_str());
    if (m_delayTime <= 0) {
        m_delayTime = 1;
    }
    if (m_delayTime > 30000) {
        m_delayTime = 30000;
    }

    if(!m_bSetup) {
        dx_risk_init(*this);
    }


    m_bSetup = true;
}

STEE
void DXRiskInter::DoFetchCollectionInfo(map<string, string> &infos) {

    LOGR("%d", TAG_DXRISK + 101);


    int64_t before_time = OthersUtils::CurrentTimes();


    DataCollectionActionNode *node = m_collection_head;

    while (node != NULL) {

//        LOGE("action '%s' begin", node->name.c_str());
//        int64_t start = OthersUtils::CurrentTimes();

        node->action(infos, node->args);

//        int64_t end = OthersUtils::CurrentTimes();
//        LOGE("const: %lli  action '%s'  ", (end - start), node->name.c_str());

        node = node->next;
    }

    // 最后加入采集耗时字段
    int64_t cost = OthersUtils::CurrentTimes() - before_time;
    char cost_buf[10] = {0};
    dx_sprintf(cost_buf, "%" PRId64 , cost);
    string costStr(cost_buf);
    infos["K300"] = costStr;

    for (auto it = m_paramsMap.begin(); it != m_paramsMap.end(); it++) {
        // Fake risk for poc
        if (it->first == "PRIVATE_FAKE_EMULATOR") {
            infos["K130"] = "5";
        } else if (it->first == "PRIVATE_FAKE_INJECT") {
            infos["K131"] = "6";
        } else if(it->first == "PRIVATE_FAKE_MEMDUMP") {
            infos["K132"] = "7";
        } else if(it->first == "PRIVATE_FAKE_DEBUG") {
            infos["K133"] = "8";
        } else if(it->first == "PRIVATE_FAKE_MULTIRUN") {
            infos["K134"] = "9";
        } else if(it->first == "PRIVATE_FAKE_ROOT") {
            infos["K136"] = "11";
        }
        // add user map
        else if(it->first.find("KEY_", 0, 4) == string::npos
                && it->first.find("PRIVATE_", 0, 8) == string::npos){
            infos["user_" + it->first] = it->second;
        }
    }

    LOGR("=====value");

#ifdef DEBUG
    // for debug, print all key
    for (auto i = infos.begin(); i != infos.end(); i++) {
        LOGD("collect key:%s value:%s", i->first.c_str(), i->second.c_str());
    }
#endif
}


STEE
void DXRiskInter::AddCollectionAction(const string &name, DataCollectionAction action, void *args) {

    DataCollectionActionNode *node = new DataCollectionActionNode();
    node->name = name;
    node->action = action;
    node->next = m_collection_head;
    node->args = args;
    m_collection_head = node;
}


STEE
void DXRiskInter::DestoryCollectionAction() {

    DataCollectionActionNode *collection_node = m_collection_head;

    while (collection_node) {
        DataCollectionActionNode *old = collection_node;
        collection_node = collection_node->next;
        delete_not_array(old);
    }

    m_collection_head = NULL;
}


#define ERRPRINT_AND_RET(type)                                                                      \
    LOGE("DXRiskError: name %s, code %d", #type, (int)type);                                     \
    m_in_https_progress = false;                                                                    \
    m_networkCv.notify_one();                                                                       \
    return;                                                                                         \

STEE
string DXRiskInter::Degrade(const string &appId, const string& str) {

    LOGR("%d", TAG_DXRISK + 104);

    string xxtea_key = CryptUtils::SHA1(appId + appId + appId);

    string zip_data = ZipFile::Compress(str);
    string encrypt_zip_data = CryptUtils::XXTeaEncrypt(xxtea_key, zip_data);

    return encrypt_zip_data;
}

STEE
string DXRiskInter::GetDeviceInfo(map<string, string> &infos, bool isFull) {
    std::lock_guard<std::mutex> lg(m_dataLock);

    LOGR("%d", TAG_DXRISK + 105);

    map<string, string> check_result_map = DXEnvCheck::GetInstance()->GetResultMap();

    size_t n_indos_size = infos.size() + check_result_map.size();

    pb_stee_base_com__dingxiang__mobile__init();
    pb_stee_riskmgr_com__dingxiang__mobile__init();

    STEERiskMgrReportDataRequest request_data = COM__DINGXIANG__MOBILE__STEERISK_MGR_REPORT_DATA_REQUEST__INIT;
    STEERiskMgrReportDataRequest_STEEInfoItem **stee_infos = (STEERiskMgrReportDataRequest_STEEInfoItem **) calloc(n_indos_size,
            sizeof(STEERiskMgrReportDataRequest_STEEInfoItem *));

    int n_index = 0;
    for (auto it = infos.begin(); it != infos.end(); it++) {

        // 临时屏蔽K1，加快采集速度
//        if (!isFull && (string) (it->first) == "K1")
//            continue;

        stee_infos[n_index] = (STEERiskMgrReportDataRequest_STEEInfoItem *) calloc(1,
                sizeof(STEERiskMgrReportDataRequest_STEEInfoItem));

        com__dingxiang__mobile__steerisk_mgr_report_data_request__steeinfo_item__init(stee_infos[n_index]);

        stee_infos[n_index]->name = StringUtils::StringToChar(it->first);
        stee_infos[n_index]->value = StringUtils::StringToChar(it->second.empty() ? "null" : it->second);
        n_index++;
    }

    for (auto it = check_result_map.begin(); it != check_result_map.end(); it++) {

        stee_infos[n_index] = (STEERiskMgrReportDataRequest_STEEInfoItem *) calloc( 1, sizeof(STEERiskMgrReportDataRequest_STEEInfoItem));

        com__dingxiang__mobile__steerisk_mgr_report_data_request__steeinfo_item__init(stee_infos[n_index]);

        stee_infos[n_index]->name = StringUtils::StringToChar(it->first);
        stee_infos[n_index]->value = StringUtils::StringToChar(it->second.empty() ? "null" : it->second);
        n_index++;
    }

//    stee_infos[n_index] = (STEERiskMgrReportDataRequest_STEEInfoItem *) malloc(
//            sizeof(STEERiskMgrReportDataRequest_STEEInfoItem));
//
//    com__dingxiang__mobile__steerisk_mgr_report_data_request__steeinfo_item__init(
//            stee_infos[n_index]);
//
//    stee_infos[n_index]->name = StringUtils::StringToChar("K300");
//    stee_infos[n_index]->value = StringUtils::Int64ToChar(cost);-

    request_data.n_infos = n_indos_size;
    request_data.infos = stee_infos;

    size_t size = com__dingxiang__mobile__steerisk_mgr_report_data_request__get_packed_size(
            &request_data);
    uint8_t *pack_data = (uint8_t *) calloc(1,size);
    size_t size1 = com__dingxiang__mobile__steerisk_mgr_report_data_request__pack(&request_data,
                                                                                  pack_data);
    ProtobufCBinaryData binaryData;
    binaryData.len = size1;
    binaryData.data = pack_data;

    string request_str = OthersUtils::CreateSTEERequestRaw(COM__DINGXIANG__MOBILE__STEEDATA_TYPE__DATATYPE_DO_REPORT_DATA, m_appId, binaryData);;

    for (int i = 0; i < n_indos_size; i++) {
        free(stee_infos[i]->name);
        free(stee_infos[i]->value);
        free(stee_infos[i]);
    }

    free(stee_infos);
    free(pack_data);

    return request_str;
}

STEE
void DXRiskInter::ReportData(const string& appId, const string& final_url, const string& request_str, const string& backup_url) {

    LOGR("%d", TAG_DXRISK + 107);

    string respone_str;

    if (!backup_url.empty()) {
        string backupAppId = GetParamByKey(MAP_KEY_BACKUP_APPID);
        string bkAppId = backupAppId.empty() ? appId : backupAppId;
        OthersUtils::AsyncHttpWithEncrypt(backup_url, request_str, respone_str, bkAppId, this->IsSaas());
    }

    OthersUtils::HWEERR err = OthersUtils::HttpWithEncrypt(final_url, request_str, respone_str, appId, this->IsSaas());
    lock_guard<mutex> lg(m_tokenLock);

    switch (err) {
        case OthersUtils::HWEERR::NETWORK_ERR:
            ERRPRINT_AND_RET(DXRISK_REQUEST_NETWORK_ERR);

        case OthersUtils::HWEERR::DECRYPT_ERR:
            ERRPRINT_AND_RET(DXRISK_REQUEST_DECRYPT_ERR);

        case OthersUtils::HWEERR::UNCOMPREESS_ERR:
            ERRPRINT_AND_RET(DXRISK_REQUEST_UNCOMPRESS_ERR);

        default:
             break;;
    }

    if (respone_str.empty()) {
        ERRPRINT_AND_RET(DXRISK_REQUEST_RESPONSE_EMPTY_ERR);
    }

    STEEResponse *pbRespone = com__dingxiang__mobile__steeresponse__unpack(NULL , respone_str.size(), (uint8_t*) respone_str.c_str());
    
    if (pbRespone == nullptr) {
        LOGE("DXRiskError: STEEResponse Parse Empty.");
        ERRPRINT_AND_RET(DXRISK_REQUEST_DATA_PARSE_ERR);
    }

    // 服务器限流
    if (pbRespone->err_code == COM__DINGXIANG__MOBILE__STEEERROR_CODE__ERR_RESOURE_LIMITE){
        m_token = "";
        set_property(m_appId, string(TOKEN), m_token);
        set_property(m_appId, string(KEY_IS_LIMIT), "true");
        ERRPRINT_AND_RET(DXRISK_REQUEST_LIMIT);
    }

    // 请求数据问题
    if (pbRespone->err_code != COM__DINGXIANG__MOBILE__STEEERROR_CODE__ERR_NOERROR
        || pbRespone->type != COM__DINGXIANG__MOBILE__STEEDATA_TYPE__DATATYPE_DO_REPORT_DATA
        || pbRespone->data.len <= 0) {

        ERRPRINT_AND_RET(DXRISK_REQUEST_DIRTY_DATA_ERR);
    }

    STEERiskMgrReportDataResponse *pbRespone_data =
            com__dingxiang__mobile__steerisk_mgr_report_data_response__unpack(NULL ,pbRespone->data.len , pbRespone->data.data);

    if (pbRespone_data == nullptr) {
        ERRPRINT_AND_RET(DXRISK_REQUEST_DATA_PARSE_ERR);
    }

    m_token = string(pbRespone_data->constid);

    com__dingxiang__mobile__steeresponse__free_unpacked(pbRespone, NULL);
    com__dingxiang__mobile__steerisk_mgr_report_data_response__free_unpacked(pbRespone_data, NULL);

    set_property(m_appId, string(LAST_FETCH_TIME), StringUtils::Int64ToString(OthersUtils::CurrentTimes()));
    set_property(m_appId, string(KEY_IS_LIMIT), "");
    set_property(m_appId, string(TOKEN), m_token);
    set_property(m_appId, string(LAST_URL), final_url);

    m_in_https_progress = false;

    m_networkCv.notify_one();

    // 异步上报模块
    string enable = GetParamByKey(MAP_KEY_ENABLE_ASYNC);
    string url = GetTokenUrl();
    if (enable == "true" && !url.empty()) {
        if (url[url.size() - 1] == '1') {
            url[url.size() - 1] = '2';
        }
        m_asyncReport.Init(m_appId, url);
        m_asyncReport.AddModule("A1", get_app_list);
        m_asyncReport.SendDataAsync();
    }
    
};


STEE
string DXRiskInter::GetToken(bool bGetFullToken) {
    LOGD("GetToken appid:%s m_bSetup%d", m_appId.c_str(), m_bSetup);

#ifdef DEBUG
    int64_t before_time = OthersUtils::CurrentTimes();
#endif

    if (!m_bSetup) {
        return "";
    }

    LOGR("%d", TAG_DXRISK + 120);

    string config_url   = GetParamByKey(MAP_KEY_URL);
    LOGD("config_url:%s", config_url.c_str());
    string token_url    = GetTokenUrl();

    // 是否使用缓存中的token
    string config_clear = GetParamByKey("PRIVATE_CLEAR_TOKEN");
    bool clean_token = !config_clear.empty();

    m_bSaas = config_url.empty();

    auto need_fetch = [&]() {
        if (clean_token)
            return true;

//        if (last_fetch_time != 0 && OthersUtils::CurrentTimes() - last_fetch_time <= 3000) {
//             return get_property(m_appId, string(TOKEN)).empty();
//        }
        
        if(DXEnvCheck::GetInstance()->GetResultMap().size() > 0)
            return true;

        LOGR("%d", TAG_DXRISK + 121);

        if (get_property(m_appId, (LAST_URL)) != token_url)
            return true;

        string last_fetch_time_string = get_property(m_appId, string(LAST_FETCH_TIME));
        int64_t last_fetch_time = atoll(last_fetch_time_string.c_str());
        if (last_fetch_time_string.empty())
            return true;

        if ((last_fetch_time == 0) || (OthersUtils::CurrentTimes() - last_fetch_time >= FETCH_INTERVAL))
            return true;


        LOGD("GetToken need_fetch appid:%s token:%s", m_appId.c_str(), get_property(m_appId, string(TOKEN)).c_str());

        return get_property(m_appId, string(TOKEN)).empty();
    };


    if (need_fetch()) {

        map<string, string> info;
        DoFetchCollectionInfo(info);
        string request = GetDeviceInfo(info);

        unique_lock <std::mutex> lck(m_networkLock);
        {
            if (!m_in_https_progress) {
                m_in_https_progress = true;

                std::thread t(&DXRiskInter::ReportData, this, m_appId, token_url, request, GetBackupUrl());

                t.detach();
            }
        }

        LOGD("m_delayTime : %d", m_delayTime);

        m_networkCv.wait_for(lck, chrono::milliseconds(m_delayTime));

        LOGR("%d", TAG_DXRISK + 123);

        lock_guard<mutex> g(m_tokenLock);

        LOGD("get_property(KEY_IS_LIMIT) : %s", get_property(m_appId, KEY_IS_LIMIT).c_str());
        LOGD("get_property(TOKEN) : %s", get_property(m_appId, TOKEN).c_str());

        // 临时屏蔽k1
        bGetFullToken = true;

        // 服务器限流，直接返回空
        if (get_property(m_appId, KEY_IS_LIMIT) == "true") {
            m_token = "";
        }
        // token 降级
        else if(m_token.empty() || (m_token.length() > 40 && !bGetFullToken)) {
            LOGD("GetDegradeToken m_token len:%ld", m_token.length());
            m_token = GetDegradeToken(info, request, bGetFullToken);
        }
    } else {
        lock_guard<mutex> lg(m_tokenLock);
        m_token = get_property(m_appId, string(TOKEN));
        LOGD("GetToken appid:%s token:%s", m_appId.c_str(), m_token.c_str());
    }

#ifdef DEBUG
    LOGD("GetToken len:%ld, cost:%lld", m_token.size(), OthersUtils::CurrentTimes() - before_time);
#endif

    lock_guard<mutex> lg(m_tokenLock);

    return m_token;
}

STEE
string DXRiskInter::GetDegradeToken(map<string, string> &info, const string& request, bool isFull) {

    string degrade_request;

    if(isFull) {
        degrade_request = request;
    } else {
        degrade_request = GetDeviceInfo(info, false);
    }

    string sign = CryptUtils::MD5(m_appId + degrade_request + m_appId);

    return SafeBase64Encode(CryptUtils::Base64Encode(Degrade(m_appId, degrade_request)) + "!" + m_appId + "!" + sign + "!" + get_dx_risk_sdk_version());
}

STEE
string DXRiskInter::SafeBase64Encode(string in) {

    LOGR("%d", TAG_DXRISK + 109);

    if(in.empty())
        return "";

    replace(in.begin(), in.end(), '+', '-');
    replace(in.begin(), in.end(), '/', '_');
    in.erase(remove(in.begin(),in.end(),'='), in.end());

    return in;
}

STEE
string DXRiskInter::GetCountry() {

    string config_country = GetParamByKey(MAP_KEY_COUNTRY, DXRiskConst::SERVER_CHINA);

    map<string,string>::iterator it;
    it = DXRiskConst::serverMap.find(config_country);
    config_country = it == DXRiskConst::serverMap.end() ? DXRiskConst::SERVER_CHINA : config_country;

    return config_country;
}

STEE
string DXRiskInter::GetTokenUrl() {

    string config_url       = GetParamByKey(MAP_KEY_URL);
    string config_country   = GetCountry();
    bool isFullUrl          = IsInputUrlFull();

    m_bSaas = config_url.find(DXRiskConst::serverMap[config_country]) != string::npos;

    return config_url.empty() ? DXRiskConst::serverMap[config_country] + DXRiskConst::serviceMap[DXRiskConst::SERVICE_TOKEN]
                              : config_url + (isFullUrl ? "" : DXRiskConst::serviceMap[DXRiskConst::SERVICE_TOKEN]);
}

STEE
string DXRiskInter::GetBackupUrl() {

    string config_url      = GetParamByKey(MAP_KEY_URL);
    string config_isBackUp = GetParamByKey(MAP_KEY_BACKUP);
    string config_country  = GetCountry();

    return config_url.empty() ? "" : (config_isBackUp == MAP_VALUE_ENABLE_BACKUP ?
                                      DXRiskConst::serverMap[config_country] + DXRiskConst::serviceMap[DXRiskConst::SERVICE_BACKUP] : "");
}

STEE
bool DXRiskInter::IsInputUrlFull() {
    string config_url = GetParamByKey(MAP_KEY_URL);
    return !config_url.empty() && (count(config_url.begin(), config_url.end(),'/') > 3);
}

STEE
string DXRiskInter::GetParamByKey(const string &key, const string &defValue) {
    auto it = m_paramsMap.find(key);

    if (it != m_paramsMap.end()) {
        return m_paramsMap.at(key);
    }
    else{
        return defValue;
    }
}
