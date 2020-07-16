//
// Created by RoseJames on 2017/7/6.
//

#include <sys/stat.h>
#include <DXPlatform.h>
#include <sys/ptrace.h>
#include <algorithm>
#include <dlfcn.h>
#include <sys/inotify.h>
#include <sys/prctl.h>
#include <cinttypes>
#include <DXEnvCheck.h>
#include <DXLog.h>
#include <uuid.h>
#include "risk_config.h"
#include "properties.h"
#include "jni_utils.h"
#include "utils.h"
#include "dx_env.h"
#include "storage_utils.h"
#include "uuid.h"
#include "dx_anti.h"
#include "dx_libc.h"
#include "jni_const.h"

extern bool _is_danger_env;
bool _is_danger_env = false;

static map<string, string> *g_check_maps = NULL;


#define ACTION_IMPL(name) static void action_get_##name(map<string, string> &infos, void *args, bool singleRun)
#define ADD_CHECK(name, singleRun) sp.AddCheckAction(#name, jni_env_bridge, (void *)action_get_##name, singleRun);

#define ADD_TO_INFO_STR(key, value)                                                                                 \
    _is_danger_env = true;                                                                                          \
    (*g_check_maps)[key] = value;                                                                                   \


#define CHECK_RUN                                                                                                   \
    static bool bRun = true;                                                                                        \
    if (GetMapValueByKey(infos, key, "") != "" || !bRun)                                                            \
        return;                                                                                                     \
    static mutex _lock;                                                                                             \
    lock_guard<mutex> g(_lock);                                                                                     \
    if (singleRun)                                                                                                  \
        bRun = false;                                                                                               \

// JNI桥接
static void jni_env_bridge(map<string, string> &infos, void *args, bool singleRun) {
    JNIEnvGuard guard;
    (void) guard;
    if(g_check_maps == NULL){
        g_check_maps = &infos;
    }
    DataCheckAction action = (DataCheckAction) args;
    action(infos, NULL, singleRun);
}

// 检测环境
string GetMapValueByKey(const map<string, string> &paramsMap, const string &key, const string &defValue = "") {

    auto it = paramsMap.find(key);

    if (it != paramsMap.end()) {
        return paramsMap.at(key);
    }
    else{
        return defValue;
    }
}

/**
 * 获取随机uuid
 */
inline string get_random_uuid(){
    // by java api
//    JNIEnvGuard guard;
//    JNIEnv *env = jni_init();
//    jclass jc_uuid = DXFindClass(env, "java/util/UUID");
//    jobject jo_uuid = DXCallStaticObjectMethodByName(env, jc_uuid, "randomUUID", "()Ljava/util/UUID;");
//    jstring js_uuid = (jstring)DXCallObjectMethodByName(env, jo_uuid, "toString", "()Ljava/lang/String;");
//    string uuid = jstring_to_string(env, js_uuid);
//    delete_ref(env, jc_uuid);
//    delete_ref(env, jo_uuid);
//    delete_ref(env, js_uuid);

    // by dx-uuid
    uuid_t uuid;
    char buffer[37] = {0};
    uuid_generate_random(uuid);
    uuid_unparse(uuid, buffer);
    string uuidStr(buffer);
    LOGD("get_random_uuid %s", uuidStr.c_str());

    return uuidStr;
}


ACTION_IMPL(hard_id) {
    string key = "K267";

    CHECK_RUN

#ifdef DEBUG
    int64_t last_time = OthersUtils::CurrentTimes();
#endif

    JNIEnvGuard guard;
    JNIEnv *env = jni_init();

    string hardID("");

    /* 这段代码中的函数序号，在java中没有找到对应的值 - blue 2019.11.01
    // mac addr
    jstring j_mac_addr = (jstring)DXCallStaticObjectMethodByName(env, sJNIHelper_class, FUNC_GET_STR, FUNC_GET_STR_SIGN, 8);
    string mac_addr = jstring_to_string(env, j_mac_addr);
    delete_ref(env, j_mac_addr);
    if (!mac_addr.empty()) {
        hardID.append(mac_addr).append("&");
    }

    // deviceID1
    jstring j_did1= (jstring)DXCallStaticObjectMethodByName(env, sJNIHelper_class, FUNC_GET_STR, FUNC_GET_STR_SIGN, 38);
    string did1 = jstring_to_string(env, j_did1);
    delete_ref(env, j_did1);
    if (!did1.empty()) {
        hardID.append(did1).append("&");
    }

    // deviceID2
    jstring j_did2= (jstring)DXCallStaticObjectMethodByName(env, sJNIHelper_class, FUNC_GET_STR, FUNC_GET_STR_SIGN, 39);
    string did2 = jstring_to_string(env, j_did2);
    delete_ref(env, j_did2);
    if (!did2.empty()) {
        hardID.append(did2).append("&");
    }

    // androidID
    jstring j_aid= (jstring)DXCallStaticObjectMethodByName(env, sJNIHelper_class, FUNC_GET_STR, FUNC_GET_STR_SIGN, 40);
    string aid = jstring_to_string(env, j_aid);
    delete_ref(env, j_aid);
    if (!aid.empty()) {
        hardID.append(aid).append("&");
    }
     */

    // UUID
    string uuid_stor = StorageUtils::Get(KEY_UUID, StorageUtils::GLOBAL);
    LOGD("ACTION_IMPL(hard_id) uuid_stor:%s", uuid_stor.c_str());
    if (uuid_stor.empty()) {
        string uuid = get_random_uuid();
        if (!uuid.empty()) {
            uuid_stor = uuid;
        }

        StorageUtils::Put(KEY_UUID, uuid_stor, StorageUtils::GLOBAL);
    }

    if(!uuid_stor.empty()){
        hardID.append(uuid_stor).append("&");
    }


    // hard files
    static const char *HARD_FILES[21] = {0};
    HARD_FILES[0] = FILE_SDCARD_CID;
    HARD_FILES[1] = FILE_SDCARD_CSD;
    HARD_FILES[2] = FILE_SDCARD_DATE;
    HARD_FILES[3] = FILE_SDCARD_NAME;
    HARD_FILES[4] = FILE_SDCARD_RAW_ID;
    HARD_FILES[5] = FILE_SDCARD_TYPE;
    HARD_FILES[6] = FILE_MMC_CID;
    HARD_FILES[7] = FILE_MMC_CSD;
    HARD_FILES[8] = FILE_MMC_DATE;
    HARD_FILES[9] = FILE_MMC_NAME;
    HARD_FILES[10] = FILE_MMC_RAW_ID;
    HARD_FILES[11] = FILE_MMC_TYPE;
    HARD_FILES[12] = FILE_SOC_BUILD_ID;
    HARD_FILES[13] = FILE_SOC_FAMILY;
    HARD_FILES[14] = FILE_SOC_MACHINE;
    HARD_FILES[15] = FILE_SOC_NUMBER;
    HARD_FILES[16] = FILE_SOC_RAW_ID;
    HARD_FILES[17] = FILE_BUS_BUILD_ID;
    HARD_FILES[18] = FILE_BUS_MACHINE;
    HARD_FILES[19] = FILE_BUS_NUMBER;
    HARD_FILES[20] = FILE_BUS_RAW_ID;


    int count = sizeof(HARD_FILES) / sizeof(char *);
    for (int i = 0; i < count; ++i) {
        string value = FileUtils::ReadLine(HARD_FILES[i]);
        if (!value.empty()) {
            hardID.append(value).append("&");
        }
    }

//    info_log("ACTION_IMPL(hard_id) hardID:%s, len:%d", hardID.c_str(), hardID.length());

    // check hardid
    string md5_stor = StorageUtils::Get(KEY_HARD_ID, StorageUtils::GLOBAL);
    string md5_hardid = CryptUtils::MD5(hardID);

    if (!md5_stor.empty() && dx_strcmp(md5_stor.c_str(), md5_hardid.c_str()) != 0) {
//        info_log("ACTION_IMPL(hard_id) md5_hardid != md5_stor");
        ADD_TO_INFO_STR(key, "true");
    }
    StorageUtils::Put(KEY_HARD_ID, md5_hardid, StorageUtils::GLOBAL);

#ifdef DEBUG
    int64_t cost_time = OthersUtils::CurrentTimes() - last_time;
    LOGD("ACTION_IMPL(hard_id) cost:%lld", cost_time);
#endif
}

/**
 * anti 结果回调
 */
void anti_result_callback(int64_t flag, void *args){
    LOGD("anti_result_callback flag:%" PRId64 " args:%s", flag, args);

    switch (flag) {
        case DXANTI_FLAG_EMULATOR:
            if (args) {
                const char *emu = (const char *)args;
                const char *p = strchr(emu, ':');
                if (p) {
                    LOGD("anti_result_callback emu info %s", p + 1);
                    if (emu[0] == '1') {
                        ADD_TO_INFO_STR("K130", p + 1);
                    }
                }
            }

            break;
        case DXANTI_FLAG_INJECT:
            ADD_TO_INFO_STR("K131", "6");
            break;
        case DXANTI_FLAG_MEMDUMP:
            ADD_TO_INFO_STR("K132", "7");
            break;
        case DXANTI_FLAG_DEBUG:
            ADD_TO_INFO_STR("K133", "8");
            break;
        case DXANTI_FLAG_MULTIRUN:
            ADD_TO_INFO_STR("K134", "9");
            break;
        case DXANTI_FLAG_FLAW_JANUS:
            ADD_TO_INFO_STR("K135", "10");
            break;
        case DXANTI_FLAG_ROOT:
            ADD_TO_INFO_STR("K136", "11");
            break;
        default:
            break;
    }
}

ACTION_IMPL(anti_check) {

    // 解决 CHECK_RUN 编译问题
    string key = "K136";

    CHECK_RUN

    // 新版anti
    DXAnti_monitor_init(AndroidRuntime::getJavaVM(), AndroidRuntime::getContext());
    DXAnti_set_monitor_callback(anti_result_callback);
    DXAnti_monitor_start(DXANTI_FLAG_DEBUG
                         | DXANTI_FLAG_EMULATOR
                         | DXANTI_FLAG_FLAW_JANUS
                         | DXANTI_FLAG_INJECT
                         | DXANTI_FLAG_MEMDUMP
                         | DXANTI_FLAG_MULTIRUN
                         | DXANTI_FLAG_ROOT
    );

    LOGD("anti_check");

}


ACTION_IMPL(inject_frame) {

    LOGD("inject frame start");

    string key = "K139";

    CHECK_RUN

    auto trim_left = [](char *buf) {
        int len = (int) dx_strlen(buf);
        int lastSpaceIdx = -1;

        for (int i = 0; i < len; ++i) {
            if (isspace(buf[i])) {
                lastSpaceIdx = i;
            } else {
                break;
            }
        }

        memmove(buf, &buf[lastSpaceIdx + 1], len - lastSpaceIdx + 1);
        return buf;
    };

    auto trim_right = [](char *buf) {
        for (int i = (int) dx_strlen(buf) - 1; i >= 0; --i) {
            if (isspace(buf[i])) {
                buf[i] = '\0';
            } else {
                break;
            }
        }
        return buf;
    };


    string frame_info;
    pid_t pid = getpid();
    char maps_path[56] = {0};
    char maps_line[1024] = {0};
    char module_path[1024] = {0};

    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
    FILE *fp = fopen(maps_path, "rb");

    if (fp == NULL) {
        return;
    }

    while (fgets(maps_line, sizeof(maps_line), fp) != NULL) {

        memset(module_path, 0, sizeof(module_path));
        if(sscanf(maps_line, "%*p-%*p %*c%*c%*c%*c %*s %*s %*d%1023s", module_path) != 1)
            continue;

        trim_left(module_path);
        trim_right(module_path);

        if (strcasestr(module_path, "substrate")) {
            frame_info.append("substrate");
            if (frame_info.find("substrate") == string::npos) {
                frame_info.append("|substrate");
            }
        }
        else if (strcasestr(module_path, "libAKCompat")) {
            if (frame_info.find("AndHook") == string::npos) {
                frame_info.append("|AndHook");
            }
        }
        else if (strcasestr(module_path, "andfix")) {
            if (frame_info.find("AndFix") == string::npos) {
                frame_info.append("|AndFix");
            }
        }
        else if (strcasestr(module_path, "yahfa")) {
            if (frame_info.find("yahfa") == string::npos) {
                frame_info.append("|yahfa");
            }
        }
        else if (strcasestr(module_path, "legend")) {
            if (frame_info.find("legend") == string::npos) {
                frame_info.append("|legendv");
            }
        }
        else if (strcasestr(module_path, "frida_agent")) {
            if (frame_info.find("frida") == string::npos) {
                frame_info.append("|frida");
            }
        }
        else if (strcasestr(module_path, "xposed_art")
                 || strcasestr(module_path, ".xposed.")
                 || strcasestr(module_path, "xposedbridge")) {
            if (frame_info.find("xposed") == string::npos) {
                frame_info.append("|xposed");
            }
        }
    }

    if (!frame_info.empty()){
        ADD_TO_INFO_STR(key, frame_info);
    }

    fclose(fp);

}

STEE
int dx_risk_check_android_impl(DXEnvCheck &sp) {

    ADD_CHECK(hard_id, true);       // 新增，唯一id
    ADD_CHECK(anti_check, true);   // 新增，anti全部检测
    ADD_CHECK(inject_frame, true);   // 攻击框架检测

    return 0;
}

