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
            return true;
        }
    }

    return false;
}