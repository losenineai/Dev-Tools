//
// Created by boyliang on 2017/5/29.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <dirent.h>
#include <string>
#include <risk_config.h>
#include <uuid.h>
#include <sys/stat.h>

#include "utils.h"
#include "DXRiskInter.h"
#include "DXPlatform.h"
#include "jni_utils.h"
#include "dx_env.h"
#include "storage_utils.h"
#include "properties.h"
#include "dx_libc.h"
#include "jni_const.h"
//#include "DXRiskConst.h"

using namespace std;
using namespace json11;

extern bool _is_danger_env;

#define ACTION_IMPL(name) STEE static void action_get_##name(map<string, string> &infos, void *args)
#define ADD_ACTION(name) sp.AddCollectionAction(#name, jni_env_bridge, (void *)action_get_##name)

#define CHECK_STR_VALID(value) (!(value).empty() && (value) != "null")

#define ADD_TO_INFO_STR(key, value) if(CHECK_STR_VALID(value)) infos[key] = (value)
#define ADD_TO_INFO(key, json) ADD_TO_INFO_STR(key, JSonUtils::JSon2string(json))
#define ADD_TO_INFO_JSON(value)                                                                     \
    if (CHECK_STR_VALID(value)) {                                                                   \
        Json jsonVal = JSonUtils::string2JSon(value);                                               \
        map<string, Json> jsonMap = jsonVal.object_items();                                         \
        for (auto it = jsonMap.begin(); it != jsonMap.end(); it++) {                                \
            ADD_TO_INFO_STR(it->first.c_str(), it->second.string_value());                          \
        }                                                                                           \
    }                                                                                               \

#define call_string_method(id)                                                                      \
    LOGR("%d-%d", TAG_COLLECT_ACTION, id);                                                          \
    JNIEnvGuard guard;                                                                              \
    JNIEnv *env = jni_init();                                                                       \
    jstring j_value = (jstring)DXCallStaticObjectMethodByName(env, sJNIHelper_class, FUNC_GET_STR,  \
                        FUNC_GET_STR_SIGN, id);                                                     \
    string value = j_value != NULL ? jstring_to_string(env, j_value) : "";                          \
    delete_ref(env, j_value);                                                                       \


// JNI桥接
static void jni_env_bridge(map<string, string> &infos, void *args) {
    JNIEnvGuard guard;
    (void) guard;

    DataCollectionAction action = (DataCollectionAction) args;
    action(infos, NULL);
}

ACTION_IMPL(cpu_info) {
    LOGR("%d-%s", TAG_COLLECT_ACTION, CryptUtils::Base64Encode("cpu_info").c_str());

    auto get_value_by_key = [&](const string &key, const string &line, string &value) {
        if (value.empty() && line.find(key) != string::npos) {
            size_t start_pos = line.find(":") + 1;
            value = line.substr(start_pos);
        }
    };

    auto strtrip = [&](char *buf) {
        size_t s = dx_strlen(buf) - 1;

        while (s > -1) {

            if (buf[s] == '\n') {
                buf[s] = '\0';
                break;
            }

            s--;
        }

        return buf;
    };

    FILE *cpuinfo_file = fopen("/proc/cpuinfo", "r");
    string Hardware;
    string module_name;
    string Features;
    string Processor;
    string vendor_id;
    string flags;

    if (cpuinfo_file != nullptr) {
        char buf[256] = {'\0'};
        while (fgets(buf, sizeof(buf), cpuinfo_file) != nullptr) {

            string line(strtrip(buf));
            get_value_by_key("Hardware", line, Hardware);
            get_value_by_key("module name", line, module_name);
            get_value_by_key("Features", line, Features);
            get_value_by_key("Processor", line, Processor);
            get_value_by_key("vendor_id", line, vendor_id);
            get_value_by_key("flags", line, flags);
        }

        fclose(cpuinfo_file);
    }

    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX1, VALUE_APP_KOCK);
    ADD_TO_INFO_STR("K30", StringUtils::Format("%d", (int) sysconf(_SC_NPROCESSORS_CONF)));
    ADD_TO_INFO_STR("K31", StringUtils::Trim(Features));
    ADD_TO_INFO_STR("K32", StringUtils::Trim(flags));
    ADD_TO_INFO_STR("K33", StringUtils::Trim(Hardware));
    ADD_TO_INFO_STR("K34", FileUtils::ReadLine("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"));
    ADD_TO_INFO_STR("K35", FileUtils::ReadLine("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"));
    ADD_TO_INFO_STR("K36", StringUtils::Trim(module_name));
    ADD_TO_INFO_STR("K37", StringUtils::Trim(Processor));
    ADD_TO_INFO_STR("K38", StringUtils::Trim(vendor_id));
    ADD_TO_INFO_STR("K39", FileUtils::ReadLine("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"));
    ADD_TO_INFO_STR("K40", FileUtils::ReadLine("/sys/class/thermal/thermal_zone0/temp"));
    ADD_TO_INFO_STR("K41", FileUtils::ReadLine("/sys/devices/system/cpu/present"));

}

// 通过c层获取MAC地址
ACTION_IMPL(mac_from_native) {
    LOGR("%d-%s", TAG_COLLECT_ACTION, CryptUtils::Base64Encode("mac_from_native").c_str());

    { // mac_native_list
        struct ifreq ifr;
        struct ifconf ifc;
        char buf[1024];

        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (sock == -1) {
            LOGE("socket fails: %s", errmsg);
            return;
        }

        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = buf;
        if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {
            LOGE("ioctl fails: %s", errmsg);
            return;
        }

        struct ifreq *it = ifc.ifc_req;
        const struct ifreq *const end = it + (ifc.ifc_len / sizeof(struct ifreq));

        vector<Json> jsonItems;

        for (; it != end; ++it) {

            if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0
                && !(ifr.ifr_flags & IFF_LOOPBACK)
                && ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {

                char mac_address[256] = {0};
                const char *hwaddr = ifr.ifr_hwaddr.sa_data;
                snprintf(mac_address, sizeof(mac_address), "%s-%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                         ifr.ifr_name, hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);

                jsonItems.emplace_back(mac_address);
            }
        }

        if (!jsonItems.empty()) {
            Json json = Json::array(jsonItems);
            ADD_TO_INFO_STR("K76", JSonUtils::JSon2string(json));
        }

        if (sock > 0) {
            close(sock);
        }
    }

    { // native mac

        char line[1024] = {0};
        dxFILE *pf = dx_popen("ip link", "r");
        string native_mac;

        if (pf) {

            while (dx_fgets(line, 1024, pf)) {
                if(strstr(line, "wlan0") != NULL)
                    break;
            }

            if(dx_fgets(line, 1024, pf)) {

                char mac[0x20] = { 0 };
                if(dx_sscanf(line, "%*[ ]link/ether %17[a-z0-9:]", mac) == 1) {
                    native_mac = string(mac);
                }
            }
            //todo popen跟pclose间隔时间太短，在夜神模拟器中可能存在崩溃。
            usleep(1000);

            dx_pclose(pf);
        }
        LOGD("K1800:%s", native_mac.c_str());
        ADD_TO_INFO_STR("K1800", native_mac);
    }

    { // native mac eth

        static auto get_mac = [&](const char* mac_addr){
            struct stat s;
            return (!stat(mac_addr, &s)) ? OthersUtils::ExecuteCMD("mac_cmd") : "";
        };
        ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX2, VALUE_APP_KOCK);
        ADD_TO_INFO_STR("K1801", get_mac("/sys/class/net/eth0/address"));
        ADD_TO_INFO_STR("K1802", get_mac("/sys/class/net/eth1/address"));
        ADD_TO_INFO_STR("K1803", get_mac("/sys/class/net/eth2/address"));
        ADD_TO_INFO_STR("K1804", get_mac("/sys/class/net/wlan0/address"));
    }
}

ACTION_IMPL(fonts_hash) {
    LOGR("%d-%s", TAG_COLLECT_ACTION, CryptUtils::Base64Encode("fonts_hash").c_str());

    vector<Json> jsonItem;

    string fonts_dir = "/system/fonts";
    string fonts_str;
    if (FileUtils::Exists(fonts_dir)) {

        DIR *dir = opendir(fonts_dir.c_str());
        struct dirent *ent = NULL;

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type & DT_DIR) {
                continue;
            }

            fonts_str += ent->d_name;
            fonts_str += "_";
        }

        if (dir != NULL) {
            closedir(dir);
        }
    }
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX3, VALUE_APP_KOCK);
    ADD_TO_INFO_STR("K60", StringUtils::Format("%u", CryptUtils::StrHash(fonts_str.c_str())));
}


// 最后一次开机时间
ACTION_IMPL(times_info) {

    LOGR("%d-%s", TAG_COLLECT_ACTION, CryptUtils::Base64Encode("times_info").c_str());
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX4, VALUE_APP_KOCK);

    { // cur time
        int64_t curTime = OthersUtils::CurrentTimes();
        ADD_TO_INFO_STR("K200", StringUtils::Int64ToString(curTime));
    }

    { // time in java
        call_string_method(GET_TIME_INFO);
        ADD_TO_INFO_JSON(value);
    }

    { // app_lastboot_time
        string uptimePath = FileUtils::ReadLine("/proc/uptime");

        if (!uptimePath.empty()) {

            float bootTime = 0;
            float freeTime = 0;
            dx_sscanf(uptimePath.c_str(), "%f %f", &bootTime, &freeTime);

            long bootTime2 = (long)(bootTime);
            struct timeval tv;

            gettimeofday(&tv, NULL);

            long bootTime3 = tv.tv_sec - bootTime2;
            char strBootTime3[256] = {0};
            dx_sprintf(strBootTime3, "%ld", bootTime3);

            ADD_TO_INFO_STR("K202", string(strBootTime3));
            ADD_TO_INFO_STR("K205", uptimePath);
        }
    }
}

// 获取网络代理信息
ACTION_IMPL(proxy_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX5, VALUE_APP_KOCK);

    call_string_method(GET_PROXY_INFO);
    ADD_TO_INFO_JSON(value);
}

// 应用信息
ACTION_IMPL(app_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX6, VALUE_APP_KOCK);
    LOGD("app_info");
    call_string_method(GET_APP_INFO);
    LOGD("=====app info value:%s", value.c_str());
    ADD_TO_INFO_JSON(value);
}

// 应用信息
ACTION_IMPL(app_list) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX7, VALUE_APP_KOCK);
    LOGD("app list");
    // 无危险环境不手机appInfo，因为数据量和app个数有直接关系
    if (!_is_danger_env) {
        return;
    }

    call_string_method(GET_APP_LIST);
    LOGD("app list, value:%s", value.c_str());
    ADD_TO_INFO_STR("K1", value);
}

// 环境信息
ACTION_IMPL(build_strs) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX8, VALUE_APP_KOCK);

    call_string_method(GET_BUILD_STRS);
    ADD_TO_INFO_JSON(value);
}

// 设备唯一码
ACTION_IMPL(device_ids) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX9, VALUE_APP_KOCK);
    call_string_method(GET_DEVICE_IDS);
    ADD_TO_INFO_JSON(value);
}

// 设备唯一码
ACTION_IMPL(device_ids_native) {

    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX10, VALUE_APP_KOCK);
    { // K1500 native_imei
        const char* cmd = "service call iphonesubinfo 1";
        char line[1024] = { 0 };
        dxFILE* pf = dx_popen(cmd, "r");

        if(pf) {

            int line_num = 0;
            int index = 0;
            char imei[15] = { 0 };

            while(dx_fgets(line, 1024, pf)) {
                LOGD("=====imei:%s", line);
                if(dx_strlen(line) >= 50) {

                    if (line_num == 0) {
                        line_num = dx_sscanf(&line[50], "%*[^a-zA-Z0-9]%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].",
                                             &imei[0], &imei[1], &imei[2], &imei[3]) == 4 ? 1 : 0;
                    } else if (line_num == 1) {
                        line_num = dx_sscanf(&line[50], "%*[^a-zA-Z0-9]%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].",
                                             &imei[4], &imei[5], &imei[6], &imei[7], &imei[8], &imei[9], &imei[10], &imei[11]) == 8 ? 2 : 1;
                    } else if (line_num == 2) {
                        dx_sscanf(&line[50], "%*[^a-zA-Z0-9]%1[a-zA-Z0-9].%1[a-zA-Z0-9].%1[a-zA-Z0-9].",
                                  &imei[12], &imei[13], &imei[14]);
                    }
                }

                if(++index >= 10)
                    break;
            }

            dx_pclose(pf);

            if(line_num == 2)
                ADD_TO_INFO_STR("K1700", string(imei));
        }
    }

}

ACTION_IMPL(gpu_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX11, VALUE_APP_KOCK);

    call_string_method(GET_GPU_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(mem_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX12, VALUE_APP_KOCK);

    call_string_method(GET_MEM_INFO);
    ADD_TO_INFO_JSON(value);

    {
        char line[1024] = { 0 };
        dxFILE* pf = dx_popen("df", "r");
        string disk = "";

        if(pf) {

            string json = "";
            while(dx_fgets(line, 1024, pf)) {

                char fs[200] = { 0 };
                char size[32] = { 0 };
                char used[32] = { 0 };
                char free[32] = { 0 };
                char jsonLine[400] = { 0 };

                if(dx_sscanf(line, "%254[0-9a-z]%*[ ]%64[^ ]%*[ ]%64[^ ]%*[ ]%64[^ ]", fs, size, used, free) == 4){
                    dx_sprintf(jsonLine, "{\"fs\":\"%s\", \"size\":\"%s\", \"used\":\"%s\", \"free\":\"%s\"}", fs, size, used, free);
                    json += string(jsonLine) + ",";
                }
            }

            dx_pclose(pf);

            disk = json.length() == 0 ? "" : "[" + json.substr(0, json.length() - 1) + "]";
            ADD_TO_INFO_STR("K69", disk);
        }
    }
}

ACTION_IMPL(mac_list) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX13, VALUE_APP_KOCK);

    call_string_method(GET_MAC_LIST);
    ADD_TO_INFO_STR("K77", value);
}

ACTION_IMPL(bt_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX14, VALUE_APP_KOCK);

    call_string_method(GET_BLUETOOTH_INFO);
    ADD_TO_INFO_JSON(value);

    { // native_bt_addr

        string bdaddr_path = property_get_ex("ro.bt.bdaddr_path");

        char c_bdaddr_path[512] = { 0 };
        dx_sprintf(c_bdaddr_path, "cat %s", bdaddr_path.c_str());

        char line[1024] = {0};
        dxFILE *pf = dx_popen(c_bdaddr_path, "r");
        string bt = "";

        if (pf) {

            string result = "";
            while (dx_fgets(line, 1024, pf)) {

                size_t i = dx_strlen(line);
                if(line[i-1]=='\n')
                    line[i-1]=0;

                result += string(line) + ",";
            }

            dx_pclose(pf);

            bt = result.substr(0, result.length() - 1);
        }

        ADD_TO_INFO_STR("K84", bt);
    }
}

// 获取电池信息
ACTION_IMPL(battery_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX15, VALUE_APP_KOCK);

    call_string_method(GET_BATTERY_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(user_agent) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX16, VALUE_APP_KOCK);

    call_string_method(GET_USER_AGENT);
    ADD_TO_INFO_STR("K100", value);
}

// 获取屏幕信息
ACTION_IMPL(screen_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX17, VALUE_APP_KOCK);

    call_string_method(GET_SCREEN_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(sensor_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX18, VALUE_APP_KOCK);

    call_string_method(GET_SENSOR_INFO);
    ADD_TO_INFO_STR("K120", value);
}

ACTION_IMPL(gps_location) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX19, VALUE_APP_KOCK);

    call_string_method(GET_GPS_INFO);
    ADD_TO_INFO_JSON(value);
}

//获得系统USB状态
ACTION_IMPL(read_usb_state) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX20, VALUE_APP_KOCK);

    call_string_method(GET_USB_STATE);
    ADD_TO_INFO_STR("K252", value);
}

// getTelephoneInfo
ACTION_IMPL(telephone_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX21, VALUE_APP_KOCK);

    call_string_method(GET_TELEPHONE_INFO);
    ADD_TO_INFO_JSON(value);
}

//获得WifiInfo
ACTION_IMPL(wifi_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX22, VALUE_APP_KOCK);

    call_string_method(GET_WIFI_INFO);
    ADD_TO_INFO_JSON(value);

    { // arp
        char line[1024] = {0};
        dxFILE *pf = dx_popen("cd /proc/net/ && cat arp", "r");
        string arp = "";

        if (pf) {

            string json = "";
            while (dx_fgets(line, 1024, pf)) {

                char ipAddr[30] = {0};
                char hwType[10] = {0};
                char flags[10] = {0};
                char hwAddr[30] = {0};
                char mask[30] = {0};
                char device[30] = {0};
                char jsonLine[400] = {0};

                if (dx_sscanf(line,
                           "%15[0-9.]%*[ ]%8[^ ]%*[ ]%8[^ ]%*[ ]%17[a-z0-9:]%*[ ]%16[0-9.*]%*[ ]%15[a-z0-9]",
                           ipAddr, hwType, flags, hwAddr, mask, device) == 6) {
                    dx_sprintf(jsonLine,
                            "{\"ipAddr\":\"%s\", \"hwType\":\"%s\", \"flags\":\"%s\", \"hwAddr\":\"%s\", \"mask\":\"%s\", \"device\":\"%s\"}",
                            ipAddr, hwType, flags, hwAddr, mask, device);
                    json += string(jsonLine) + ",";
                }
            }

            dx_pclose(pf);

            arp = json.length() == 0 ? "" : "[" + json.substr(0, json.length() - 1) + "]";
            ADD_TO_INFO_STR("K620", arp);
        }
    }

    { // socket
        char line[1024] = {0};
        dxFILE *pf = dx_popen("ls /dev/socket", "r");
        string socket = "";

        if (pf) {

            string result = "";
            while (dx_fgets(line, 1024, pf)) {

                size_t i = dx_strlen(line);
                if(line[i-1]=='\n')
                    line[i-1]=0;

                result += string(line) + ",";
            }

            dx_pclose(pf);

            socket = result.substr(0, result.length() - 1);
        }

        ADD_TO_INFO_STR("K621", socket);
    }

}

//获得Base Station Info
ACTION_IMPL(bs_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX23, VALUE_APP_KOCK);

    call_string_method(GET_BASESTATION_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(dx_risk_sdk_version) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX24, VALUE_APP_KOCK);
    ADD_TO_INFO_STR("K8", get_dx_risk_sdk_version());
}

ACTION_IMPL(hard_id_list) {

    /**
     * 唯一字段文件路径, sdcard信息
     */
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX25, VALUE_APP_KOCK);
    ADD_TO_INFO_STR("K268", FileUtils::ReadLine(FILE_SDCARD_CID));
    ADD_TO_INFO_STR("K269", FileUtils::ReadLine(FILE_SDCARD_CSD));
    ADD_TO_INFO_STR("K270", FileUtils::ReadLine(FILE_SDCARD_DATE));
    ADD_TO_INFO_STR("K271", FileUtils::ReadLine(FILE_SDCARD_NAME));
    ADD_TO_INFO_STR("K272", FileUtils::ReadLine(FILE_SDCARD_RAW_ID));
    ADD_TO_INFO_STR("K273", FileUtils::ReadLine(FILE_SDCARD_TYPE));
    ADD_TO_INFO_STR("K274", FileUtils::ReadLine(FILE_MMC_CID));
    ADD_TO_INFO_STR("K275", FileUtils::ReadLine(FILE_MMC_CSD));
    ADD_TO_INFO_STR("K276", FileUtils::ReadLine(FILE_MMC_DATE));
    ADD_TO_INFO_STR("K277", FileUtils::ReadLine(FILE_MMC_NAME));
    ADD_TO_INFO_STR("K278", FileUtils::ReadLine(FILE_MMC_RAW_ID));
    ADD_TO_INFO_STR("K279", FileUtils::ReadLine(FILE_MMC_TYPE));

    /**
     * 唯一字段文件路径, 系统芯片信息
     */
    ADD_TO_INFO_STR("K280", FileUtils::ReadLine(FILE_SOC_BUILD_ID));
    ADD_TO_INFO_STR("K281", FileUtils::ReadLine(FILE_SOC_FAMILY));
    ADD_TO_INFO_STR("K282", FileUtils::ReadLine(FILE_SOC_MACHINE));
    ADD_TO_INFO_STR("K283", FileUtils::ReadLine(FILE_SOC_NUMBER));
    ADD_TO_INFO_STR("K284", FileUtils::ReadLine(FILE_SOC_RAW_ID));
    ADD_TO_INFO_STR("K285", FileUtils::ReadLine(FILE_BUS_BUILD_ID));
    ADD_TO_INFO_STR("K286", FileUtils::ReadLine(FILE_BUS_MACHINE));
    ADD_TO_INFO_STR("K287", FileUtils::ReadLine(FILE_BUS_NUMBER));
    ADD_TO_INFO_STR("K288", FileUtils::ReadLine(FILE_BUS_RAW_ID));

    /**
     * 唯一字段文件路径, 系统信息
     */
    ADD_TO_INFO_STR("K290", FileUtils::ReadLine(ID_BOOT_ID));
    ADD_TO_INFO_STR("K291", FileUtils::ReadLine(ID_UUID));
    ADD_TO_INFO_STR("K292", FileUtils::ReadLine(ID_OSRELEASE));
}

ACTION_IMPL(uuid) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX26, VALUE_APP_KOCK);

    static auto get_random_uuid = [&](){
        uuid_t uuid;
        char buffer[37] = {0};
        uuid_generate_random(uuid);
        uuid_unparse(uuid, buffer);
        string uuidStr(buffer);
        LOGD("get_random_uuid %s", uuidStr.c_str());
        return uuidStr;
    };

    string uuid_stor = StorageUtils::Get(KEY_UUID, StorageUtils::GLOBAL);
    LOGD("ACTION_IMPL(uuid) stor:%s", uuid_stor.c_str());
    if (uuid_stor.empty()) {
        string uuid = get_random_uuid();
        if (!uuid.empty()) {
            uuid_stor = uuid;
        }

        StorageUtils::Put(KEY_UUID, uuid_stor, StorageUtils::GLOBAL);
    }

    ADD_TO_INFO_STR("K58", uuid_stor);
}

ACTION_IMPL(native_properties) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX27, VALUE_APP_KOCK);
    ADD_TO_INFO_STR("K137", property_get_ex("ro.debuggable"));
    ADD_TO_INFO_STR("K138", property_get_ex("gsm.version.baseband"));
    ADD_TO_INFO_STR("K253", property_get_ex("net.dns1"));

    ADD_TO_INFO_STR("K1000", property_get_ex("dhcp.wlan0.dns1"));
    ADD_TO_INFO_STR("K1001", property_get_ex("dhcp.wlan0.dns2"));
    ADD_TO_INFO_STR("K1002", property_get_ex("dhcp.wlan0.dns3"));
    ADD_TO_INFO_STR("K1003", property_get_ex("dhcp.wlan0.dns4"));
    ADD_TO_INFO_STR("K1004", property_get_ex("dhcp.wlan0.domain"));
    ADD_TO_INFO_STR("K1005", property_get_ex("dhcp.wlan0.gateway"));
    ADD_TO_INFO_STR("K1006", property_get_ex("dhcp.wlan0.ipaddress"));
    ADD_TO_INFO_STR("K1007", property_get_ex("dhcp.wlan0.server"));

    ADD_TO_INFO_STR("K1020", property_get_ex("dhcp.eth1.dns1"));
    ADD_TO_INFO_STR("K1021", property_get_ex("dhcp.eth1.dns2"));
    ADD_TO_INFO_STR("K1022", property_get_ex("dhcp.eth1.dns3"));
    ADD_TO_INFO_STR("K1023", property_get_ex("dhcp.eth1.dns4"));
    ADD_TO_INFO_STR("K1024", property_get_ex("dhcp.eth1.domain"));
    ADD_TO_INFO_STR("K1025", property_get_ex("dhcp.eth1.gateway"));
    ADD_TO_INFO_STR("K1026", property_get_ex("dhcp.eth1.ipaddress"));
    ADD_TO_INFO_STR("K1027", property_get_ex("dhcp.eth1.server"));

    ADD_TO_INFO_STR("K1040", property_get_ex("wlan.interface"));
    ADD_TO_INFO_STR("K1041", property_get_ex("wlan.driver.status"));
    ADD_TO_INFO_STR("K1042", property_get_ex("wlan.driver.macaddr"));
    ADD_TO_INFO_STR("K1043", property_get_ex("wifi.interface"));
    ADD_TO_INFO_STR("K1044", property_get_ex("dhcp.wlan"));
    ADD_TO_INFO_STR("K1045", property_get_ex("net.hostname"));
    ADD_TO_INFO_STR("K1046", property_get_ex("net.bt.name"));
    ADD_TO_INFO_STR("K1047", property_get_ex("net.dns2"));
    ADD_TO_INFO_STR("K1048", property_get_ex("net.eth0.gw"));
    ADD_TO_INFO_STR("K1048", property_get_ex("net.gprs.local-ip"));

    ADD_TO_INFO_STR("K1060", property_get_ex("ro.build.version.release"));
    ADD_TO_INFO_STR("K1061", property_get_ex("ro.build.version.security_patch"));
    ADD_TO_INFO_STR("K1062", property_get_ex("ro.build.fingerprint"));
    ADD_TO_INFO_STR("K1063", property_get_ex("ro.build.host"));
    ADD_TO_INFO_STR("K1064", property_get_ex("ro.build.date.utc"));
    ADD_TO_INFO_STR("K1065", property_get_ex("ro.build.display.id"));
    ADD_TO_INFO_STR("K1066", property_get_ex("ro.build.id"));
    ADD_TO_INFO_STR("K1067", property_get_ex("ro.build.tags"));
    ADD_TO_INFO_STR("K1068", property_get_ex("ro.build.user"));
    ADD_TO_INFO_STR("K1069", property_get_ex("ro.build.description"));
    ADD_TO_INFO_STR("K1070", property_get_ex("ro.build.changelist"));
    ADD_TO_INFO_STR("K1071", property_get_ex("ro.build.hidden_ver"));
    ADD_TO_INFO_STR("K1072", property_get_ex("ro.build.TAGs"));
    ADD_TO_INFO_STR("K1073", property_get_ex("ro.build.type"));
    ADD_TO_INFO_STR("K1074", property_get_ex("ro.build.version.sdk"));
    ADD_TO_INFO_STR("K1075", property_get_ex("ro.build.hidden_ver"));
    ADD_TO_INFO_STR("K1076", property_get_ex("ro.build.PDA"));
    ADD_TO_INFO_STR("K1077", property_get_ex("ro.build.date"));
    ADD_TO_INFO_STR("K1078", property_get_ex("ro.build.characteristics"));
    ADD_TO_INFO_STR("K1079", property_get_ex("ro.build.hide.settings"));
    ADD_TO_INFO_STR("K1080", property_get_ex("ro.build.expect.baseband"));
    ADD_TO_INFO_STR("K1081", property_get_ex("ro.build.expect.bootloader"));
    ADD_TO_INFO_STR("K1082", property_get_ex("ro.build.version.base_os"));
    ADD_TO_INFO_STR("K1083", property_get_ex("ro.build.version.codename"));
    ADD_TO_INFO_STR("K1084", property_get_ex("ro.build.version.incremental"));
    ADD_TO_INFO_STR("K1085", property_get_ex("ro.build.version.preview_sdk"));
    ADD_TO_INFO_STR("K1086", property_get_ex("ro.build.version.sdk"));
    ADD_TO_INFO_STR("K1087", property_get_ex("ro.build.flavor"));


    ADD_TO_INFO_STR("K1090", property_get_ex("ro.boot.bootloader"));
    ADD_TO_INFO_STR("K1091", property_get_ex("ro.boot.serialno"));
    ADD_TO_INFO_STR("K1092", property_get_ex("ro.boot.sec_atd.tty"));
    ADD_TO_INFO_STR("K1093", property_get_ex("ro.boot.btmacaddr"));
    ADD_TO_INFO_STR("K1094", property_get_ex("ro.boot.wifimacaddr"));
    ADD_TO_INFO_STR("K1095", property_get_ex("ro.boot.securebootkeyhash"));
    ADD_TO_INFO_STR("K1096", property_get_ex("ro.boot.hardware"));

    ADD_TO_INFO_STR("K1110", property_get_ex("ro.product.device"));
    ADD_TO_INFO_STR("K1111", property_get_ex("ro.product.model"));
    ADD_TO_INFO_STR("K1112", property_get_ex("ro.product.brand"));
    ADD_TO_INFO_STR("K1113", property_get_ex("ro.product.name"));
    ADD_TO_INFO_STR("K1114", property_get_ex("ro.product.cpu.abi"));
    ADD_TO_INFO_STR("K1115", property_get_ex("ro.product.cpu.abi2"));
    ADD_TO_INFO_STR("K1116", property_get_ex("ro.product.cpu.abilist"));
    ADD_TO_INFO_STR("K1117", property_get_ex("ro.product.manufacturer"));
    ADD_TO_INFO_STR("K1118", property_get_ex("ro.product.board"));

    ADD_TO_INFO_STR("K1130", property_get_ex("ro.hardware"));
    ADD_TO_INFO_STR("K1131", property_get_ex("ro.serialno"));
    ADD_TO_INFO_STR("K1132", property_get_ex("ro.secure"));
    ADD_TO_INFO_STR("K1133", property_get_ex("ro.recovery_id"));
    ADD_TO_INFO_STR("K1134", property_get_ex("ro.chipname"));
    ADD_TO_INFO_STR("K1135", property_get_ex("ro.carrier"));
    ADD_TO_INFO_STR("K1136", property_get_ex("ro.board.platform"));
    ADD_TO_INFO_STR("K1137", property_get_ex("ro.board.boardid"));
    ADD_TO_INFO_STR("K1138", property_get_ex("ro.opengles.version"));
    ADD_TO_INFO_STR("K1139", property_get_ex("ro.qc.sdk.gestures.camera"));
    ADD_TO_INFO_STR("K1140", property_get_ex("ro.qc.sdk.sensors.gestures"));
    ADD_TO_INFO_STR("K1141", property_get_ex("ro.gps.agps_provider"));
    ADD_TO_INFO_STR("K1142", property_get_ex("ro.com.google.clientidbase"));
    ADD_TO_INFO_STR("K1143", property_get_ex("ro.adb.secure"));
    ADD_TO_INFO_STR("K1144", property_get_ex("ro.multisim.simslotcount"));
    ADD_TO_INFO_STR("K1145", property_get_ex("ro.nfc.port"));
    ADD_TO_INFO_STR("K1146", property_get_ex("ro.kernel.qemu"));
    ADD_TO_INFO_STR("K1147", property_get_ex("ro.vendor.extension_library"));
    ADD_TO_INFO_STR("K1148", property_get_ex("ro.csc.country_code"));
    ADD_TO_INFO_STR("K1149", property_get_ex("ro.csc.sales_code"));
    ADD_TO_INFO_STR("K1150", property_get_ex("ro.fota.device"));
    ADD_TO_INFO_STR("K1151", property_get_ex("ro.runtime.firstboot"));
    ADD_TO_INFO_STR("K1152", property_get_ex("ro.mtk.hardware"));
    ADD_TO_INFO_STR("K1153", property_get_ex("ro.config.browser_useragent1"));
    ADD_TO_INFO_STR("K1154", property_get_ex("ro.config.browser_useragent2"));
    ADD_TO_INFO_STR("K1155", property_get_ex("ro.ril.miui.imei0"));
    ADD_TO_INFO_STR("K1156", property_get_ex("ro.ril.miui.imei1"));
    ADD_TO_INFO_STR("K1157", property_get_ex("ro.ril.oem.imei"));
    ADD_TO_INFO_STR("K1158", property_get_ex("ro.ril.oem.imei1"));
    ADD_TO_INFO_STR("K1159", property_get_ex("ro.ril.oem.imei2"));
    ADD_TO_INFO_STR("K1160", property_get_ex("ro.ril.oem.meid"));
    ADD_TO_INFO_STR("K1161", property_get_ex("ro.sys.oem.sno"));
    ADD_TO_INFO_STR("K1162", property_get_ex("ro.aliyun.clouduuid"));
    ADD_TO_INFO_STR("K1163", property_get_ex("ro.yunos.version"));
    ADD_TO_INFO_STR("K1164", property_get_ex("ro.yunos.device.cpu"));
    ADD_TO_INFO_STR("K1165", property_get_ex("ro.yunos.device.romsize"));
    ADD_TO_INFO_STR("K1166", property_get_ex("ro.mediatek.project.path"));
    ADD_TO_INFO_STR("K1167", property_get_ex("ro.mediatek.version.release"));
    ADD_TO_INFO_STR("K1168", property_get_ex("ro.meizu.hardware.emmc"));
    ADD_TO_INFO_STR("K1169", property_get_ex("ro.meizu.hardware.psn"));
    ADD_TO_INFO_STR("K1170", property_get_ex("ro.meizu.hardware.version"));
    ADD_TO_INFO_STR("K1171", property_get_ex("ro.meizu.product.model"));
    ADD_TO_INFO_STR("K1172", property_get_ex("ro.genymotion.version"));
    ADD_TO_INFO_STR("K1173", property_get_ex("ro.kernel.androidboot.hardware"));
    ADD_TO_INFO_STR("K1174", property_get_ex("ro.telephony.sim.count"));

    ADD_TO_INFO_STR("K1190", property_get_ex("gsm.serial"));
    ADD_TO_INFO_STR("K1191", property_get_ex("gsm.sim.hw_atr"));
    ADD_TO_INFO_STR("K1192", property_get_ex("gsm.sim.hw_atr1"));
    ADD_TO_INFO_STR("K1193", property_get_ex("gsm.sim.state")); //  [ABSENT,READY] 只要检测这个属性的值便可以得到双卡的状态
    ADD_TO_INFO_STR("K1194", property_get_ex("gsm.sim.state2"));
    ADD_TO_INFO_STR("K1195", property_get_ex("gsm.version.baseband2"));
    ADD_TO_INFO_STR("K1196", property_get_ex("gsm.version.ril-impl"));
    ADD_TO_INFO_STR("K1197", property_get_ex("gsm.current.phone-type"));
    ADD_TO_INFO_STR("K1198", property_get_ex("gsm.current.phone-type2"));
    ADD_TO_INFO_STR("K1199", property_get_ex("gsm.network.type"));
    ADD_TO_INFO_STR("K1200", property_get_ex("gsm.network.type2"));
    ADD_TO_INFO_STR("K1201", property_get_ex("gsm.operator.isroaming"));
    ADD_TO_INFO_STR("K1202", property_get_ex("gsm.operator.isroaming2"));
    ADD_TO_INFO_STR("K1203", property_get_ex("gsm.operator.numeric"));
    ADD_TO_INFO_STR("K1204", property_get_ex("gsm.operator.numeric2"));
    ADD_TO_INFO_STR("K1205", property_get_ex("gsm.device.sn"));

    ADD_TO_INFO_STR("K1210", property_get_ex("ril.hw_ver"));
    ADD_TO_INFO_STR("K1211", property_get_ex("ril.modem.board"));
    ADD_TO_INFO_STR("K1212", property_get_ex("ril.modem.board2"));
    ADD_TO_INFO_STR("K1213", property_get_ex("ril.official_cscver"));
    ADD_TO_INFO_STR("K1214", property_get_ex("ril.product_code"));
    ADD_TO_INFO_STR("K1215", property_get_ex("ril.sw_ver"));
    ADD_TO_INFO_STR("K1216", property_get_ex("ril.sw_ver2"));
    ADD_TO_INFO_STR("K1217", property_get_ex("ril.ICC_TYPE"));
    ADD_TO_INFO_STR("K1218", property_get_ex("ril.ICC_TYPE2"));
    ADD_TO_INFO_STR("K1219", property_get_ex("ril.serialnumber"));
    ADD_TO_INFO_STR("K1220", property_get_ex("ril.subscription.types"));
    ADD_TO_INFO_STR("K1221", property_get_ex("ril.timezoneID"));
    ADD_TO_INFO_STR("K1222", property_get_ex("ril.iccid.sim1"));
    ADD_TO_INFO_STR("K1223", property_get_ex("ril.iccid.sim2"));
    ADD_TO_INFO_STR("K1224", property_get_ex("ril.hw_modem0.rssi"));
    ADD_TO_INFO_STR("K1225", property_get_ex("ril.hw_modem1.rssi"));
    ADD_TO_INFO_STR("K1226", property_get_ex("ril.hw_modem2.rssi"));

    ADD_TO_INFO_STR("K1240", property_get_ex("phone.imei"));
    ADD_TO_INFO_STR("K1241", property_get_ex("phone.imsi"));
    ADD_TO_INFO_STR("K1242", property_get_ex("phone.linenum"));
    ADD_TO_INFO_STR("K1243", property_get_ex("phone.simserial"));

    ADD_TO_INFO_STR("K1260", property_get_ex("persist.radio.plmnname_1"));
    ADD_TO_INFO_STR("K1261", property_get_ex("persist.radio.plmnname_2"));
    ADD_TO_INFO_STR("K1262", property_get_ex("persist.radio.previousiccid0"));
    ADD_TO_INFO_STR("K1263", property_get_ex("persist.radio.previousiccid1"));
    ADD_TO_INFO_STR("K1264", property_get_ex("persist.radio.balong0_iccid"));
    ADD_TO_INFO_STR("K1265", property_get_ex("persist.radio.imei"));
    ADD_TO_INFO_STR("K1266", property_get_ex("persist.radio.imei1"));
    ADD_TO_INFO_STR("K1267", property_get_ex("persist.radio.imei2"));
    ADD_TO_INFO_STR("K1268", property_get_ex("persist.radio.meid"));
    ADD_TO_INFO_STR("K1269", property_get_ex("persist.radio.data.iccid"));
    ADD_TO_INFO_STR("K1270", property_get_ex("persist.radio.device.imei"));
    ADD_TO_INFO_STR("K1271", property_get_ex("persist.radio.cfu.iccid.0"));
    ADD_TO_INFO_STR("K1272", property_get_ex("persist.sys.timezone"));
    ADD_TO_INFO_STR("K1273", property_get_ex("persist.sys.country"));
    ADD_TO_INFO_STR("K1274", property_get_ex("persist.sys.oppo.device.imei"));
    ADD_TO_INFO_STR("K1275", property_get_ex("persist.sys.updater.imei"));
    ADD_TO_INFO_STR("K1276", property_get_ex("persist.service.bdroid.bdaddr"));
    ADD_TO_INFO_STR("K1277", property_get_ex("persist.sys.language"));

    ADD_TO_INFO_STR("K1290", property_get_ex("androVM.vkeyboard_mode"));
    ADD_TO_INFO_STR("K1291", property_get_ex("androVM.vbox_graph_mode"));
    ADD_TO_INFO_STR("K1292", property_get_ex("androVM.vbox_dpi"));
    ADD_TO_INFO_STR("K1293", property_get_ex("qemu.sf.lcd_density"));
    ADD_TO_INFO_STR("K1294", property_get_ex("qemu.sf.fake_camera"));

    ADD_TO_INFO_STR("K1310", property_get_ex("init.svc.adbd"));
    ADD_TO_INFO_STR("K1311", property_get_ex("rild.libargs"));
    ADD_TO_INFO_STR("K1312", property_get_ex("keyguard.no_require_sim"));
    ADD_TO_INFO_STR("K1313", property_get_ex("sys.usb.state"));
    ADD_TO_INFO_STR("K1314", property_get_ex("sys.usb.config"));

}

// 获取屏幕信息
ACTION_IMPL(setting_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX28, VALUE_APP_KOCK);

    call_string_method(GET_SETTING_INFO);
    ADD_TO_INFO_JSON(value);
}

// 获取音频信息
ACTION_IMPL(audio_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX29, VALUE_APP_KOCK);

    call_string_method(GET_AUDIO_INFO);
    ADD_TO_INFO_JSON(value);
}

// 获取厂商ids
ACTION_IMPL(supplier_ids) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX30, VALUE_APP_KOCK);

    call_string_method(GET_SUPPLIER_IDS);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(camera_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX31, VALUE_APP_KOCK);

    call_string_method(GET_CAMERA_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(sdcard_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX32, VALUE_APP_KOCK);

    call_string_method(GET_SDCARD_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(xposed_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX33, VALUE_APP_KOCK);

    call_string_method(GET_XPOSED_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(nds_info) {
    ADD_TO_INFO_STR(KEY_APP_KOCK_INDEX34, VALUE_APP_KOCK);

    call_string_method(GET_DNS_INFO);
    ADD_TO_INFO_JSON(value);
}

ACTION_IMPL(checting_app_info){
    dxFILE* file = dx_popen("pm list package -3", "r");
    string app_list = "";
    if(file != NULL){
        char buf[256];
        while(dx_fgets(buf, sizeof(buf), file) != NULL){
            char* pkgname = strstr(buf, ":");
            pkgname[strlen(pkgname)-1] = '\0';

            app_list.append(string(pkgname));
        }

        dx_pclose(file);
    }else{
        LOGD("popen failed");
    }
    ADD_TO_INFO_STR("K15000", app_list);

}

ACTION_IMPL(checking_gameguardian){
    dxFILE* file = dx_popen("logcat -300 | grep android-daemon:", "r");
    if(file != NULL){
        char buf[256];
        if(dx_fgets(buf, sizeof(buf), file)){
            ADD_TO_INFO_STR("K1501", string(buf));
        }
        dx_pclose(file);
    }
}


bool isArrive(string buf){
    if(buf.find("64 bytes from") != string::npos && buf.find("icmp_seq=2") != string::npos){
        return true;
    }
    return false;
}

string parseIpFromString(string buf){
    string start_str = "From";
    string end_str = "icmp_seq=1";
    unsigned int start_pos = buf.find(start_str);
    unsigned int end_pos = buf.find(end_str);
    if(start_pos != string::npos && end_pos != string::npos){
        string ip = buf.substr(start_pos + start_str.size(), (end_pos - start_pos - start_str.size()));
        ip.replace(ip.find(":"), 1, " ");
        ip.erase(0, ip.find_first_not_of(" "));
        ip.erase(ip.find_last_not_of(" ") + 1);
        return ip;
    }

    return "";
}

string getIpFromPing(string ip, int ttl){
    const char* ping_format = "ping -c 4 -t %d %s";
    char command[256];
    sprintf(command, ping_format, ttl, ip.c_str());

    dxFILE* pfile = dx_popen(command, "r");
    if(pfile == NULL){
        return "";
    }
    char buf[256];

    while(dx_fgets(buf, sizeof(buf), pfile) != NULL){
        if(isArrive(buf)){
            return ip;
        }
        string ip = parseIpFromString(buf);
        if(!ip.empty()){
            return ip;
        }
    }

    return "";
}

ACTION_IMPL(trace_route_info){
    string ip = "8.8.8.8";
    int maxTTL = 20;
    string trace_route = "";
    for(int i=1; i<maxTTL; i++){
        string routeIp = getIpFromPing(ip, i);
        trace_route.append(routeIp + "|");
        if(routeIp == ip){
            break;
        }
    }
    ADD_TO_INFO_STR("K15002", trace_route);
}

STEE
int dx_risk_init_android_impl(DXRiskInter &sp) {

    // 执行顺序，自下而上

    ADD_ACTION(battery_info);
    ADD_ACTION(gps_location);
    ADD_ACTION(device_ids);     // 必须是第一个，因为它将初始化备份检查信息。
    ADD_ACTION(device_ids_native);
    ADD_ACTION(telephone_info);
    ADD_ACTION(wifi_info);
    ADD_ACTION(bs_info);
    ADD_ACTION(setting_info);
    ADD_ACTION(audio_info);
    ADD_ACTION(native_properties);
    ADD_ACTION(hard_id_list);
    ADD_ACTION(app_info);
    ADD_ACTION(build_strs);
    ADD_ACTION(cpu_info);
    ADD_ACTION(fonts_hash);
    ADD_ACTION(gpu_info);
    ADD_ACTION(mem_info);
    ADD_ACTION(proxy_info);
    ADD_ACTION(mac_from_native);
    ADD_ACTION(mac_list);
    ADD_ACTION(bt_info);
    ADD_ACTION(user_agent);
    ADD_ACTION(sensor_info);
    ADD_ACTION(screen_info);
    ADD_ACTION(dx_risk_sdk_version);
    ADD_ACTION(uuid);
    ADD_ACTION(times_info);
    ADD_ACTION(supplier_ids);
    ADD_ACTION(camera_info);
    ADD_ACTION(sdcard_info);
    ADD_ACTION(xposed_info);
    ADD_ACTION(nds_info);

//    ADD_ACTION(read_usb_state); // 放到最后采集，因为usb广播初始化需要时间

//    ADD_ACTION(app_list);
//    ADD_ACTION(emulator_info);

    auto it = sp.m_paramsMap.find(MAP_KEY_USER_APP_LISTS_CHECK);
    if (it != sp.m_paramsMap.end()) {
        ADD_ACTION(checting_app_info);
        ADD_ACTION(checking_gameguardian);
    }

    it = sp.m_paramsMap.find(MAP_KEY_USER_IP_ROUTE_CHECK);
    if (it != sp.m_paramsMap.end()) {
        ADD_ACTION(trace_route_info);
    }

    return 0;
}