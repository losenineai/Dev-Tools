//
// Created by white on 2018/5/16.
//

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include "utils_anti_log.h"
#include "dx_libc_utils.h"
#include "include/file_utils.h"

using namespace std;


bool anti_file_exist(const string &name){
    return !name.empty() && !access(name.data(), R_OK);
}

void anti_file_readLines(const string &path, vector<string> &resultVec, int32_t limitLine){

    resultVec.clear();
    FILE *fp = fopen(path.c_str(), "rb");

    if (fp) {
        char buf[1024] = {0};
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            // 某设备设备文件会没有eof，防止无限读取，默认限制1024行
            if (resultVec.size() >= limitLine) {
                break;
            }
//            resultVec.push_back(string(buf));
            resultVec.insert(resultVec.end(), string(buf));
        }
        fclose(fp);
    }
}

void anti_file_writeAll(const string &name, const string &data, bool append){
    FILE* file = NULL;

    if (append) {
        file = fopen(name.c_str(), "ab");
    } else {
        file = fopen(name.c_str(), "wb");
    }

    if (file != NULL) {
        fwrite(data.data(), data.length(), 1, file);
        fflush(file);
        fclose(file);
    }
}

void anti_file_deleteIfExist(const string &name){
    if (access(name.data(), R_OK | W_OK) == 0) {
        unlink(name.data());
    }
}

bool anti_file_makeDir(const string &name, int mode){
    return mkdir(name.c_str(), mode) == 0;
}

bool anti_file_deleteDir(const string &name){
    const char *dirname = name.c_str();
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    dir = opendir(dirname);
    if (dir == NULL) {
        return false;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {

            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                anti_file_deleteDir(path);
            } else {
                remove(path);
            }
        }

    }

    closedir(dir);
    rmdir(dirname);

    return true;
}

vector<string> anti_file_list(const string &dir, const vector<string> &filters){
    vector<string> files;
    DIR *D = opendir(dir.c_str());
    if (D != NULL) {
        dirent *ent = NULL;
        while ((ent = readdir(D)) != NULL) {
//            if (ent->d_type & DT_DIR) {
//                continue;
//            }

            bool skip = false;
            for (auto i = filters.begin(); i != filters.end(); i++) {
                if (strstr(ent->d_name, i->c_str()) != NULL) {
                    skip = true;
                    break;
                }
            }

            if (skip) {
                continue;
            }

//            files.push_back(string(ent->d_name));
            files.insert(files.end(), string(ent->d_name));
        }
        closedir(D);
    }

    return files;
}




void anti_file_readAll(const string &path, string &result, int64_t limitSize){
    result.clear();

    if(anti_file_exist(path)){

        dxFILE *fp = dx_fopen(path.c_str(), "rb");

        if (!fp) {
            return;
        }

        static int BUF_SIZE = 1024;
        // 读取直到遇到eof
        char *buf = new char[BUF_SIZE];

        while (1) {
            // 某设备设备文件会没有eof，防止无限读取，默认限制32M
            if (result.length() >= limitSize) {
                break;
            }

            memset(buf, 0, BUF_SIZE);

            // 为了防止fread被hook，使用dx_fread
            size_t retSize = dx_fread(buf, sizeof(char), BUF_SIZE, fp);

            if (retSize == 0){
                break;
            }

            result.append(buf, retSize);

            if (retSize < BUF_SIZE) {
                break;
            }
        }

        delete_array(buf);
        dx_fclose(fp);

    }
}
