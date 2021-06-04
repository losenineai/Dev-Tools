//
// Created by amlloc on 18-7-17.
//

#include <string>
#include <cstdlib>
#include <algorithm>

#include "anti_emu_debug.h"
#include "string_utils.h"
#include "jni_utils.h"
#include "jni_comm_utils.h"
#include "file_utils.h"
#include "dx_anti_stee.h"


using namespace std;

STEE
bool anti_emu_debug_check_adb(){
    bool adbInEmulator = false;
    vector<string> tcpVector;
    anti_file_readLines("/proc/net/tcp", tcpVector);

    vector<uint32_t> ips(0);
    vector<uint32_t> ports(0);
    uint32_t id;
    uint32_t ip;
    uint32_t port;

    //no tcp massage
    if (tcpVector.size() <  2){
        anti_info_log(" lack of tcp massage!");
        return false;
    }

    anti_info_log("start to create DxTcp");
    //first line is abandoned
    for(int i = 1; i < tcpVector.size(); i++ ){
        string strTcp = tcpVector.at(i);
        anti_info_log("tcp log %d : %s", i, strTcp.c_str());

        if (3 == sscanf(strTcp.c_str(), "%X:%X:%X", &id, &ip, &port)) {
            ips.push_back(ip);
            ports.push_back(port);
            anti_info_log("local tcp : %d-%X-%X", id, ip, port);
        }
    }
    int ipsLength = (int )ips.size();
    (void )ipsLength;
    anti_info_log("ipsLength = %d", ipsLength);
    uint32_t adbPort = 0xffffFFFF;
    for (int i = 0; i < ipsLength; ++i)
    {
        if (ips[i] == 0) {
            adbPort = ports[i];
            anti_info_log("adbPort 0x%X: ", adbPort);
            break;
        }
    }

    if (adbPort != 0xffffFFFF){
        for (int i = 0; i < ipsLength; ++i) {
            if (ips[0] != 0 && ports[i] == adbPort) {
                adbInEmulator = true;
            }
        }
    }

    return adbInEmulator;
}

STEE
bool anti_emu_debug_check_UserAMonkey(){
    anti_info_log("=====debug_check_UserAMonkey=====");
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jclass jActivityManager = anti_jni_findClassByName(env, "android/app/ActivityManager");
    jboolean jresult = anti_jni_callStaticMethodReturnBool(env, jActivityManager, "isUserAMonkey", "()Z");
    anti_info_log("check UserAMonkey : %d", jresult);
    anti_jni_delete_ref(env, jActivityManager);
    return jresult == JNI_TRUE;
}

STEE
bool anti_emu_debug_check_DebuggerConnected(){
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jclass jDebugclxx = anti_jni_findClassByName(env, "android/os/Debug");
    jboolean jresult = anti_jni_callStaticMethodReturnBool(env, jDebugclxx, "isDebuggerConnected", "()Z");
    anti_info_log("check debuggerConnected : %d", jresult);
    anti_jni_delete_ref(env, jDebugclxx);
    return jresult ==JNI_TRUE;
}

STEE
bool anti_emu_debug_check_TracerPid(){
    const string tracerPid = "tracerpid";
    vector<string> statuLines;
    anti_file_readLines("/proc/self/status", statuLines);
    if (statuLines.size() == 0){
        anti_info_log("read /proc/self/status failed");
    }

    bool result = false;

    for(string statuLine : statuLines){

        if (statuLine.length() > tracerPid.length()){

            string transInLine = statuLine.substr(0, tracerPid.length());
            std::transform(transInLine.begin(), transInLine.end(), transInLine.begin(), ::tolower);

            if (transInLine == tracerPid){

                string str = statuLine.substr(tracerPid.length() + 1);
                anti_str_trim(str);
                int trancerPidNum = atoi(str.c_str());
                anti_info_log("TrancerPid num is : %d", trancerPidNum);
                if (trancerPidNum > 0){
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}