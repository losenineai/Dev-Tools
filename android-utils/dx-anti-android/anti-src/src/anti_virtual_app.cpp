//
// Created by white on 2018/10/15.
//

#include <string.h>
#include "anti_virtual_app.h"

#include "anti_crash_log_prefix.h"
#include "utils_anti_log.h"
#include "file_utils.h"
#include "string_utils.h"
#include "anti_utils.h"
#include "anti_inject.h"
#include "dx_anti_stee.h"
#include "dx_libc.h"
#include "jni_comm_utils.h"
#include "comm_utils.h"
#include "jni_utils.h"
#include "platform.h"
#include "anti_result_handler.h"


#define SO_APP_LIB "/data/app-lib/"
#define SO_APP_LIB_LEN strlen(SO_APP_LIB)

#define SO_DATA_APP "/data/app/"
#define SO_DATA_APP_LEN strlen(SO_DATA_APP)

#define SO_DATA_DATA "/data/data/"
#define SO_DATA_DATA_LEN strlen(SO_DATA_DATA)



inline void readMaps(vector<string> &mapsVec){
    pid_t pid = getpid();
    char path[32] = {0};

    if (dx_sprintf(path, "/proc/%d/maps", pid) == -1){
        return ;
    }

    string data;
    anti_file_readAll(path, data);
    anti_str_splitString(data, mapsVec, "\n");
}


inline void readProcessName(string &result){
    pid_t pid = getpid();

    char path[32] = {0};
    if (dx_sprintf(path, "/proc/%d/cmdline", pid) == -1) {
        return;
    }

    anti_file_readAll(path, result);
}




inline void findSoPathByFilter(vector<string> &filters, vector<string> &src,
                               vector<string> &result){

    if (src.size() == 0){
        return;
    }

    char permission[10] = {0};
    char soPath[1024] = {0};

    for (auto iterInSrc = src.begin(); iterInSrc != src.end(); iterInSrc++){

        string strLine = *iterInSrc;
        const char* line = strLine.c_str();
        size_t len = dx_strlen(line);
        if (len <= 0){
            continue;
        }

        // TODO:why permission must be "r-xp"?
        if (dx_sscanf(line, "%*p-%*p %s %*s %*s %*d %1023[^\n]", permission, soPath) != 2){
            continue;
        }


        for (auto iterInFilters = filters.begin();iterInFilters != filters.end();iterInFilters++){
            string filter = *iterInFilters;
            if (dx_strcmp(permission, "r-xp") == 0 && anti_str_end_with(soPath, filter.c_str())){
                result.insert(result.end(), soPath);
                //availed repeat
                break;
            }
        }
    }
}

inline int countVAPathBySo(vector<string> &filters){

    //analysis the path of the .so by itselfe
    vector<string> mapsVec;
    vector<string> soPathVec;

    readMaps(mapsVec);
    if(mapsVec.size() == 0){
        anti_info_log("read maps failed");
        return 0;
    }

    string processNameStr;
    readProcessName(processNameStr);
    const char *processName = processNameStr.c_str();
    anti_info_log("cur processName : %s", processName);
    const size_t processNameLen = strlen(processName);
    if (processNameLen == 0) {
        return 0;
    }

    findSoPathByFilter(filters, mapsVec, soPathVec);


    int i = 0;
    for(auto itt = soPathVec.begin(); itt != soPathVec.end(); itt++){
        const char *lib = itt->c_str();//lib can only be used once
        anti_info_log("countVAPathBySo path : %s", lib);
        if (dx_strstr(lib, processName) != NULL){
            if (anti_str_start_with(lib, SO_APP_LIB)){
                //no 0, is true
                if (dx_strncmp(lib + SO_APP_LIB_LEN, processName, processNameLen)){
                    i++;
                }
            } else if(anti_str_start_with(lib, SO_DATA_APP)){
                if (dx_strncmp(lib + SO_DATA_APP_LEN, processName, processNameLen)) {
                    i++;
                }
            } else if (anti_str_start_with(lib, SO_DATA_DATA)){
                anti_info_log(SO_DATA_DATA);
                if (dx_strncmp(lib + SO_DATA_DATA_LEN, processName, processNameLen)){
                    i++;
                }
            }
        }
    }
    anti_info_log("countVAPathBySo count %d", i);

    mapsVec.clear();
    vector<string>(mapsVec).swap(mapsVec);
    soPathVec.clear();
    vector<string>(soPathVec).swap(soPathVec);
    return i;
}

STEE
int countVAPathByPkg(vector<string> filters){

    vector<string> mapsVec;
    vector<string> result;

    readMaps(mapsVec);
    if(mapsVec.size() == 0){
        anti_info_log("read maps failed");
        return 0;
    }

    //analysis the path of the .so by itselfe
    string packageName = anti_platform_getPkgName();
    anti_str_containWithFilter(filters, mapsVec, result);

    //count must be > 0;
    size_t count = result.size();
    anti_info_log("countVAPathByPkg %d", count);

    mapsVec.clear();
    vector<string>(mapsVec).swap(mapsVec);
    result.clear();
    vector<string>(result).swap(result);

    return (int)count;
}


/*
 * according to the packageName of the fake space from java.
 */
STEE
bool checkVAByJava(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject jcontext = anti_jni_getGlobalContext(env);
    // java.io
    jobject jfile = anti_jni_callMethodByName(env, jcontext, "getFilesDir", "()Ljava/io/File;");
    jstring jpath = (jstring)anti_jni_callMethodByName(env, jfile, "getPath", "()Ljava/lang/String;");
    string path = anti_jni_jstring_to_string(env, jpath);

    vector<string> fake_space_packege_names;
    fake_space_packege_names.push_back("io.virtual"); //virtual
    fake_space_packege_names.push_back("com.lbe.parallel");//平行空间
    fake_space_packege_names.push_back("com.excelliance");//双开助手
    fake_space_packege_names.push_back("com.qihoo.magic");//分身大师


    bool result = false;

    for (int i = 0; i < fake_space_packege_names.size(); ++i) {
        if(path.find(fake_space_packege_names.at(i)) != string::npos){
            result = true;
            anti_info_log("checkVAByJava : %s", fake_space_packege_names.at(i).c_str());
            break;
        }
    }

    fake_space_packege_names.clear();

    anti_info_log("======checkVAByJava result:%d", result);

    return result;
}

/*
 * according to the maps msg: can't work for com.lbe.parallel
 */
STEE
bool checkVABySo(){
    vector<string>  filters;
    filters.push_back(".so");

    bool result = countVAPathBySo(filters) > 0;

    anti_info_log("checkVABySo result:%d", result);
    return result;
}

STEE
bool checkVAByPkgName(){
    vector<string> fake_space_packege_names;

    fake_space_packege_names.push_back("io.virtual"); //virtual
    fake_space_packege_names.push_back("com.qihoo.magic");//分身大师
    fake_space_packege_names.push_back("com.lbe.parallel");//平行空间
    fake_space_packege_names.push_back("com.excellimnce.dualaid");//双开助手

    bool result = countVAPathByPkg(fake_space_packege_names) > 0;

    anti_info_log("checkVAByPkgName result:%d", result);

    fake_space_packege_names.clear();
    return result;
}



STEE
void *DXAnti_detect_virtual_app(void * arg){

    anti_info_log("DXAnti_detect_virtual_app start");

    int err_code = 0;

    if (checkVAByJava()) {
        err_code = ANTI_VIRTUAL_APP_ERR_BY_JAVA;
    } else if(checkVABySo()) {
        err_code = ANTI_VIRTUAL_APP_ERR_BY_SO;
    } else if(checkVAByPkgName()) {
        err_code = ANTI_VIRTUAL_APP_ERR_BY_PKG;
    }

    if (err_code) {
        AntiResult result = {AntiType::AT_MULTIRUN, err_code};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    return NULL;

}

STEE
int DXAnti_detect_virtual_app_async() {

    pthread_t newthread;
    int ret = -1;

    dx_anti_pthread_create(ret, &newthread, NULL, DXAnti_detect_virtual_app, NULL);

    return ret;
}



STEE
void DXAnti_detect_virtual_app_entry() {
    DXAnti_detect_virtual_app_async();
}