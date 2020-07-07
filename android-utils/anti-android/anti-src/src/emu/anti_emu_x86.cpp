//
// Created by white on 2019/1/21.
//

#include "anti_emu_x86.h"
#include "dx_anti_stee.h"
#include "utils_anti_log.h"
#include "dx_anti_prefix.h"
#include "file_utils.h"
#include "properties.h"
#include "dx-libc/dx_libc.h"

STEE
bool anti_emu_x86_support(){
    bool result = false;

    result = anti_file_exist("/system/lib/arm");


    if (!result) {
        char buf[256];
        string props[] = {"ro.product.cpu.abi", "ro.product.cpu.abilist"};

        for (int i = 0; i < 2; i++) {
            anti_property_get(props[i].c_str(), buf, "none");
            if (dx_strstr(buf, "x86")) {
                anti_info_log("\t%s", buf);
                result = true;
            }
        }
    }


    if (!result) {
        vector<string> lines;
        anti_file_readLines("/system/build.prop", lines);

        for (string &l : lines) {

            if (l[0] != '#' && l.find('=') != l.npos && l.find("x86") != l.npos ) {
                result = true;
            }
        }
    }

    anti_info_log("anti_emu_x86_support: %d", result);

    return result;
}


STEE
bool anti_emu_x86_by_hw(){
    bool result = false;
    if (anti_file_exist("/system/lib/hw")) {
        anti_info_log("anti_emu_x86_by_hw exist.");
        for (auto &n : anti_file_list("/system/lib/hw")) {
            anti_info_log("anti_emu_x86_by_hw file:%s", n.c_str());
            if (n.find("x86") != n.npos) {
                result = true;
            }
        }
    }

    anti_info_log("anti_emu_x86_by_hw: %d", result);
    return result;
}