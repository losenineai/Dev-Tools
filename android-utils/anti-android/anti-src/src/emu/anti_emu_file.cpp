//
// Created by amlloc on 18-7-16.
//

#include "anti_emu_file.h"
#include <string>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <comm_utils.h>
#include "file_utils.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"

using namespace std;

STEE
bool anti_emu_file_checkFeatures(const string &dir, vector<string> &features){
    bool result = false;

    if (anti_file_exist(dir)) {
        for (auto &p : anti_file_list(dir)) {
            for(auto &f : features){
                if (p.find(f) != string::npos) {
                    result = true;
                }
            }
        }
    }
    return result;

}

STEE
bool anti_emu_file_checkFiles(vector<string> &targets){
    for (string &pipe : targets){
        if(anti_file_exist(pipe)){
//            anti_info_log("=====find file %s", pipe.c_str());
            return true;
        }
    }

    return false;
}

STEE
bool anti_emu_file_checkPackages(vector<string> &targets){
    dxFILE* file = dx_popen("pm list package", "r");
    char buf[256] = {0};
    while(dx_fgets(buf, sizeof(buf), file)){
        const char* pkg_name = strstr(buf, ":");
        if(pkg_name == NULL){
            continue;
        }
        pkg_name += 1;
        int len = strlen(pkg_name) - 1;
        for(string target_name : targets){
            if(strncmp(target_name.c_str(), pkg_name, len) == 0){
                return true;
            }
        }
    }
    return false;
}