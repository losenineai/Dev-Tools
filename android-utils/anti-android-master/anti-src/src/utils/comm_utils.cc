//
// Created by white on 2018/5/17.
//

#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <src/utils/include/utils_anti_log.h>
#include "include/comm_utils.h"
#include "include/string_utils.h"

using namespace std;

string anti_util_executeCMD(const string &cmd, const string &filter){
    string ss("");

    FILE *pipe = popen(cmd.c_str(), "r");
    if (pipe != NULL) {
        char buf[512];
        while (!feof(pipe)) {
            if (fgets(buf, sizeof(buf), pipe) != NULL) {
                ss += buf;
            }
        }
        pclose(pipe);
    }

    if (0 == ss.length() && !filter.empty()) {
        vector<string> lines(0);
        anti_str_splitString(ss, lines, "\n");
        string rss("");

        for (string &l : lines) {
            if (l.find(filter) != l.npos) {
                rss += (l.append("\n"));
            }
        }

        return rss;
    }

    return ss;
}


 uint64_t anti_util_createRandom(int base){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t curTime = (int64_t) tv.tv_sec * 1000 + (int64_t) tv.tv_usec / 1000;

    srand(curTime);
    return (uint64_t)rand() % base;
}

void anti_util_sleepMS(uint64_t ms){
    usleep(ms * 1000);
}

void anti_util_sleepSEC(uint32_t sec){
    anti_util_sleepMS(sec * 1000);
}

template<class FUNC>
void anti_util_startThread(function<FUNC> fun, bool wait){
    thread tt(fun);

    if(tt.joinable()){
        if(wait){
            tt.joinable();
        }else{
            tt.detach();
        }
    }
}