//
// Created by white on 2018/5/3.
//


#include <string>

#include <jni.h>
#include <jni_utils.h>
#include <vector>
#include <cmath>
#include <string_utils.h>
#include <src/utils/include/comm_utils.h>
#include "anti_emu_qemu.h"
#include "anti_emu_main.h"
#include "anti_emu_basic.h"
#include "anti_emu_file.h"
#include "utils_anti_log.h"
#include "anti_emu_debug.h"
#include "dx_anti_stee.h"
#include "anti_emu_x86.h"

static string _s_collect_emu_str("");

//anti_emu_basic_checkPhoneNumber()
#define CHECK_IMPL(name) STEE static bool check_##name()

/**
 * 模拟器检测项ID
 * ID 1~100 为100%模拟器
 */
// 检测 andy模拟器
const char *ID_FILE_ANDY = "1";
// 检测 蓝叠模拟器
const char *ID_FILE_BLUESTACK = "2";
// 检测 genymotion模拟器
const char *ID_FILE_GENY = "3";
// 检测 夜神模拟器
const char *ID_FILE_NOX = "4";
// 检测 天天模拟器
const char *ID_FILE_TT = "5";
// 检测 windroy模拟器
const char *ID_FILE_WINDROY = "6";
// 检测 逍遥模拟器
const char *ID_FILE_XY = "7";
// 检测 系统信息
const char *ID_BUILD_INFO = "8";
// 检测 ip地址
const char *ID_IP = "9";
// 检测 adb连接
const char *ID_ADB = "10";
// 检测 设备ID
const char *ID_DEVICE_ID = "11";
// 检测 包名
const char *ID_PKG = "12";
// 检测 手机号
const char *ID_PHONE_NUM = "13";
// 检测 voice mail
const char *ID_VOICE_MAIL = "14";
// 检测 运营商
const char *ID_OPER_NAME = "15";
// 检测 mumu模拟器
const char *ID_FILE_MUMU = "16";
// 检测 x86 硬件相关
const char *ID_X86_HW = "17";
// 检测 vbox引擎相关文件
const char *ID_VBOX = "18";
// 检测 鲁大师模拟器/手机模拟大师
const char *ID_FILE_LUDASHI = "19";
// 检测 小蚁安卓模拟器
const char *ID_FILE_ANT = "20";
// 检测 雷电模拟器
const char *ID_FILE_LD = "21";
// 检测 腾讯手游助手
const char *ID_FILE_TXSY = "22";
// 检测 海马玩模拟器
const char *ID_FILE_HMW = "23";
// 检测 51模拟器
const char *ID_FILE_51 = "24";
// 检测 红手指模拟器
const char *ID_FILE_REDFINGER = "25";

// 检测 qemu 驱动
const char *ID_QEMU_DRIVER = "101";
// 检测 qemu 文件
const char *ID_QEMU_FILE = "102";
// 检测 qemu 属性
const char *ID_QEMU_PROPS = "103";
// 检测 电池
const char *ID_BATTERY = "104";
// 检测 imsi
const char *ID_IMSI = "105";
// 检测 taint类
const char *ID_TAINT = "106";
// 检测 debugger
const char *ID_DEBUGGER = "107";
// 检测 trace 相关
const char *ID_TRACE = "108";
// 检测 monkey
const char *ID_MONKEY = "109";
// 检测 x86文件
const char *ID_FILE_X86 = "110";
// 检测 网卡
const char *ID_ETH0 = "111";
// 检测 qemu 缓存
const char *ID_QEMU_CACHE = "112";
// 检测 qemu pc寄存器
const char *ID_QEMU_PC = "113";
// 检测 qemu bkpt指令
const char *ID_QEMU_BKPT = "114";
// 检测 是否支持运行x86
const char *ID_X86_SUPPORT = "115";

/**
 * 模拟器特征文件列表
 */

//Genymotion模拟器特征
vector <string> FILES_GENY = {
        "/dev/socket/genyd",
        "/dev/socket/baseband_genyd"
};

//小蚁安卓模拟器特征
vector <string> FILES_ANT = {
        "cn.antplayer.launcher",
        "cn.antplayer.appstore"
};

//x86模拟器特征
vector <string> FILES_X86 = {
        "ueventd.android_x86.rc",
        "ueventd.android_x86_64.rc",
        "x86.prop",
        "ueventd.ttVM_x86.rc",
        "init.ttVM_x86.rc",
        "fstab.ttVM_x86",
        "fstab.vbox86",
        "init.vbox86.rc",
        "ueventd.vbox86.rc"
};

//Andy模拟器特征
vector <string> FILES_ANDY = {
        "fstab.andy",
        "ueventd.andy.rc"
};


vector <string> FILES_NOX = {
        "/sys/module/vboxsf",
        "/system/bin/nox-vbox-sf",
        "/system/bin/nox-prop",
        "/sdcard/Android/data/com.bignox.app.store.hd",
        "/sdcard/BigNoxHD"
};

//夜神模拟器特征
vector <string> FEATRUE_NOX = {
        "vboxsf",
        "nox"
};


//WINDROY模拟器特征（与海马玩重复）
vector <string> FILES_WINDROY = {
        "/system/lib/libdroid4x.so",
        "/system/bin/windroyed"
};

//逍遥模拟器特征
vector <string> FILES_XY = {
        "/sdcard/Android/data/com.microvirt.guide",
        "/data/data/com.microvirt.launcher",
        "/data/data/com.microvirt.tools",
        "/data/data/com.microvirt.installer"
};

//天天模拟器特征
vector <string> FILES_TT = {
        "/system/bin/ttVM-prop"
};


//bluestack模拟器特征
vector <string> FILES_BLUESTACK = {
        "/data/app/com.bluestacks.appmart-1.apk",
        "/data/app/com.bluestacks.BstCommandProcessor-1.apk",
        "/data/app/com.bluestacks.help-1.apk",
        "/data/app/com.bluestacks.home-1.apk",
        "/data/app/com.bluestacks.s2p-1.apk",
        "/data/app/com.bluestacks.searchapp-1.apk",
        "/data/bluestacks.prop",
        "data/data/com.androVM.vmconfig",
        "data/data/com.bluestacks.accelerometerui",
        "data/data/com.bluestacks.appfinder",
        "data/data/com.bluestacks.appmart",
        "data/data/com.bluestacks.appsettings",
        "data/data/com.bluestacks.BstCommandProcessor",
        "data/data/com.bluestacks.bstfolder",
        "data/data/com.bluestacks.help",
        "data/data/com.bluestacks.home",
        "data/data/com.bluestacks.s2p",
        "data/data/com.bluestacks.searchapp",
        "data/data/com.bluestacks.settings",
        "data/data/com.bluestacks.setup",
        "data/data/com.bluestacks.spotlight",
        "/mnt/prebundledapps/bluestacks.prop.orig"
};

//mumu模拟器特征
vector <string> FILES_MUMU = {
        "/system/app/com.mumu.launcher",
        "/system/app/com.mumu.store"
};

//雷电模拟器特征
vector <string> FILES_LD = {
        "/system/lib/libldutils.so"
};

//腾讯手游助手特征
vector <string> FILES_TXSY = {
        "/system/lib/libTX_GLESv1_enc.so",
        "/system/lib/libTX_GLESv2_enc.so",
        "/system/lib/libTX_OpenglSystemCommon.so",
        "/system/lib/libTX_renderControl_enc.so"
};

//海马玩模拟器特征
vector <string> FILES_HMW = {
        "me.haima.androidassist",
        "me.haima.helpcenter"
};


//51模拟器特征
vector <string> FILES_51 = {
        "/data/data/com.fiftyone/shared_prefs/com.fiftyone_preferences.xml",
        "/sdcard/.51service/whitelist",
        "/system/lib/arm/libpatts_engine_jni_api_ub.210030103.so",
        "/system/lib/arm/libfb_breakpad_client.so"
};

//红手指模拟器特征
vector <string> FILES_REDFINGER = {
        "/sys/class/redfinger_gps",
        "/sys/class/redfinger_audio"
};


CHECK_IMPL(phone_num) {
    anti_info_log("=====check phone num=====");
    return anti_emu_basic_checkPhoneNumber();
}

CHECK_IMPL(device_id) {
    anti_info_log("=====check device id=====");
    return anti_emu_basic_checkDeviceId();
}

CHECK_IMPL(imsi) {
    anti_info_log("=====check imsi=====");
    return anti_emu_basic_checkImsi();
}

CHECK_IMPL(voice_mail) {
    anti_info_log("=====check voidce mail=====");
    return anti_emu_basic_checkVoiceMail();
}

CHECK_IMPL(operator_name) {
    anti_info_log("=====check operator name=====");
    return anti_emu_basic_checkOperatorNameAndroid();
}

CHECK_IMPL(ip) {
    anti_info_log("=====check ip=====");
    return anti_emu_basic_checkIp();
}

CHECK_IMPL(eth0) {
    anti_info_log("=====check eth0=====");
    return anti_emu_basic_checkEth0();
}

CHECK_IMPL(build_info) {
    anti_info_log("=====check build info=====");
    return anti_emu_basic_checkBuildInfo();
}

CHECK_IMPL(package_name) {
    anti_info_log("=====check packageName=====");
    return anti_emu_basic_checkPackageName();
}

CHECK_IMPL(battery) {
    anti_info_log("=====check battery=====");
    return anti_emu_basic_checkBattery();
}

CHECK_IMPL(taint) {
    anti_info_log("=====check taint=====");
    return anti_emu_basic_checkTaint();
}

CHECK_IMPL(file_xy) {
    anti_info_log("=====check file xy=====");
    return anti_emu_file_checkFiles(FILES_XY);
}

CHECK_IMPL(file_x86) {
    anti_info_log("=====check file x86=====");
    return anti_emu_file_checkFiles(FILES_X86);
}

CHECK_IMPL(file_windroy) {
    anti_info_log("=====check file windroy=====");
    return anti_emu_file_checkFiles(FILES_WINDROY);
}

CHECK_IMPL(file_tt) {
    anti_info_log("=====check file tt=====");
    return anti_emu_file_checkFiles(FILES_TT);
}


CHECK_IMPL(file_nox) {
    anti_info_log("=====check file nox=====");
    return anti_emu_file_checkFeatures("/sys/module", FEATRUE_NOX);
}

CHECK_IMPL(file_geny) {
    anti_info_log("=====check file geny=====");
    return anti_emu_file_checkFiles(FILES_GENY);
}

CHECK_IMPL(file_bluestack) {
    anti_info_log("=====check file bluestack=====");
    return anti_emu_file_checkFiles(FILES_BLUESTACK);
}

CHECK_IMPL(file_andy) {
    anti_info_log("=====check file andy=====");
    return anti_emu_file_checkFiles(FILES_ANDY);
}

CHECK_IMPL(qemu_props) {
    anti_info_log("=====check qemu props=====");
    return anti_emu_qemu_checkQemuProps();

}

CHECK_IMPL(qemu_driver) {
    anti_info_log("=====check qemu driver=====");
    return anti_emu_qemu_checkQemuDriver();
}

CHECK_IMPL(qemu_files) {
    anti_info_log("=====check qemu files=====");
    return anti_emu_qemu_checkQemuFiles();
}

CHECK_IMPL(file_ant){
    anti_info_log("======check file ant=====");
    return anti_emu_file_checkPackages(FILES_ANT);
}

/**
 * FIXME assembly code will anr or crash in MI PAD2
 * @return
 */
CHECK_IMPL(qemu_pc) {
//    anti_info_log("=====check qemu pc=====");
//    return anti_emu_qemu_checkArm_pc();
    return false;
}

/**
 * FIXME bkpt will crash in some machine
 * @return
 */
CHECK_IMPL(qemu_bkpt) {
//    anti_info_log("=====check qemu bkpt=====");
//    return anti_emu_qemu_checkArm_bkpt();
    return false;
}

/**
 * FIXME qemu_cache will crash in some machine
 * @return
 */
CHECK_IMPL(qemu_cache) {
//    anti_info_log("=====check qemu cache=====");
//    return anti_emu_qemu_checkArm_cache();
    return false;
}

// FIXME close. some real device will match.
CHECK_IMPL(adb) {
//    anti_info_log("=====check adb=====");
//    return anti_emu_debug_check_adb();
    return false;
}

CHECK_IMPL(monkey) {
    anti_info_log("=====check monkey=====");
    return anti_emu_debug_check_UserAMonkey();
}

CHECK_IMPL(debugger) {
    anti_info_log("=====check debugger=====");
    return anti_emu_debug_check_DebuggerConnected();
}

CHECK_IMPL(trace) {
    anti_info_log("=====check trace=====");
    return anti_emu_debug_check_TracerPid();
}

CHECK_IMPL(vbox) {
    anti_info_log("=====check vbox=====");
    vector <string> feaVect;
    feaVect.push_back("vbox");
    return anti_emu_file_checkFeatures("/dev", feaVect)
           || anti_emu_file_checkFeatures("/system/lib/hw", feaVect)
           || anti_emu_file_checkFeatures("/system/bin", feaVect);
}

CHECK_IMPL(ludashi) {
    anti_info_log("=====check ludashi=====");
    vector <string> feaVect;
    feaVect.push_back("ludashi");
    return anti_emu_file_checkFeatures("/", feaVect)
           || anti_emu_file_checkFeatures("/system/bin", feaVect);
}

CHECK_IMPL(mumu) {
    anti_info_log("=====check mumu=====");
    return anti_emu_file_checkFiles(FILES_MUMU);
}

CHECK_IMPL(x86_hw) {
    anti_info_log("=====check x86_hw=====");
    return anti_emu_x86_by_hw();
}

CHECK_IMPL(x86_support) {
    anti_info_log("=====check x86_support=====");
    return anti_emu_x86_support();
}

CHECK_IMPL(file_leidian){
    anti_info_log("=====check leidian======");
    return anti_emu_file_checkFiles(FILES_LD);
}

CHECK_IMPL(file_txsy){
    anti_info_log("=====check tengxunshouyou=====");
    return anti_emu_file_checkFiles(FILES_TXSY);
}

CHECK_IMPL(file_haimawan){
    anti_info_log("=====check haimawan=====");
    return anti_emu_file_checkPackages(FILES_HMW);
}

CHECK_IMPL(file_51){
    anti_info_log("=====check 51=====");
    return anti_emu_file_checkFiles(FILES_51);
}

CHECK_IMPL(file_redfinger){
    anti_info_log("=====red finger=====");
    return anti_emu_file_checkFiles(FILES_REDFINGER);
}

STEE
string anti_emulator_get_str() {
    if (_s_collect_emu_str.empty()) {

        static const char *SPLIT = "|";

        if (check_mumu()) {  //mumu模拟器
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_MUMU;
        }
        if (check_ludashi()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_LUDASHI;
        }
        if (check_x86_hw()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_X86_HW;
        }
        if (check_x86_support()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_X86_SUPPORT;
        }
        if (check_qemu_driver()) {
            _s_collect_emu_str += ID_QEMU_DRIVER;
        }
        if (check_qemu_files()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_QEMU_FILE;
        }
        if (check_qemu_props()) { //error
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_QEMU_PROPS;
        }
        if (check_file_andy()) {  //error
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_ANDY;
        }
        if (check_file_bluestack()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_BLUESTACK;
        }
        if (check_file_geny()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_GENY;
        }
        if (check_file_nox()) {//夜神模拟器
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_NOX;
        }
        if (check_file_tt()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_TT;
        }
        if (check_file_windroy()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_WINDROY;
        }
        if (check_file_x86()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_X86;
        }
        if (check_file_xy()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_XY;
        }
        if (check_battery()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_BATTERY;
        }
        if (check_build_info()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_BUILD_INFO;
        }
        if (check_device_id()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_DEVICE_ID;
        }
        if (check_imsi()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_IMSI;
        }
        if (check_ip()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_IP;
        }
        if (check_eth0()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_ETH0;
        }
        if (check_operator_name()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_OPER_NAME;
        }
        if (check_package_name()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_PKG;
        }
        if (check_phone_num()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_PHONE_NUM;
        }
        if (check_voice_mail()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_VOICE_MAIL;
        }
        if (check_taint()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_TAINT;
        }
        if (check_debugger()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_DEBUGGER;
        }
        if (check_trace()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_TRACE;
        }
        if (check_monkey()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_MONKEY;
        }
        if (check_vbox()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_VBOX;
        }
        if (check_adb()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_ADB;
        }
        // FIXME 部分机型会crash，待完善
        if (check_qemu_bkpt()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_QEMU_BKPT;
        }
        if (check_qemu_pc()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_QEMU_PC;
        }
        if (check_qemu_cache()) {
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_QEMU_CACHE;
        }

        if(check_file_ant()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_ANT;
        }
        if(check_file_leidian()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_LD;
        }
        if(check_file_txsy()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_TXSY;
        }
        if(check_file_haimawan()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_HMW;
        }
        if(check_file_51()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_51;
        }
        if(check_file_redfinger()){
            _s_collect_emu_str += SPLIT;
            _s_collect_emu_str += ID_FILE_REDFINGER;
            anti_info_log("detected red finger");
        }
    }


    anti_info_log("anti_emulator_get_str :%s", _s_collect_emu_str.c_str());
    return _s_collect_emu_str;
};


STEE
bool anti_detect_emulator() {
    // 0~1.0 之间，1.0表示为100%模拟器
    float score = anti_emu_calculateByPercent(anti_emulator_get_str());
    bool ret = score == 1.0F;
    anti_info_log("anti_detect_emulator ret:%d", ret);
    return ret;
}

//STEE
//map<string, int> anti_emu_getItemValueMap(){
//
//    map<string, int> sItemMap;
//    sItemMap.insert({string("101"), 5});
//    sItemMap.insert({string("102"), 2});
//    sItemMap.insert({string("103"), 5});
//    sItemMap.insert({string("104"), 5});
//    sItemMap.insert({string("105"), 5});
//    sItemMap.insert({string("106"), 5});
//    sItemMap.insert({string("107"), 5});
//    sItemMap.insert({string("108"), 5});
//    sItemMap.insert({string("109"), 5});
//    sItemMap.insert({string("110"), 5});
//    sItemMap.insert({string("111"), 5});
//    sItemMap.insert({string("112"), 2});
//    sItemMap.insert({string("113"), 5});
//    sItemMap.insert({string("114"), 5});
//    sItemMap.insert({string("115"), 5});
//    sItemMap.insert({string("116"), 5});
//    sItemMap.insert({string("117"), 5});
//    sItemMap.insert({string("118"), 5});
//    sItemMap.insert({string("119"), 5});
//    sItemMap.insert({string("120"), 5});
//
//    return sItemMap;
//};

STEE
float anti_emu_calculateByPercent(const string &collecStr) {
    if (collecStr.length() == 0) {
        return 0;
    }

    float percent = 0;
    float total = 0;

    vector <string> splitKey;
    anti_str_splitString(collecStr, splitKey, "|");

    for (string key : splitKey) {
        if (key.empty()) {
            continue;
        }
//        map<string, int> strMap = anti_emu_getItemValueMap();;

        int keyInt = 0;
        keyInt = atoi(key.c_str());
        // 0~100
        if (keyInt >= 0 && keyInt <= 100) {
            total = 100;
            break;
        }
        // above 100
//        else if (strMap.find(key) != strMap.end()){
//            int value = strMap.at(key);
//            total += value;
//        }
    }

    if (total != 0) {
        float s_collect_emu_str = total / (100 * 1);
        //Two bits after a decimal point
        percent = round(s_collect_emu_str * 100) / 100.0;
    }

    anti_info_log("anti_emu_calculateByPercent percent:%.1f", percent);
    return percent;
}