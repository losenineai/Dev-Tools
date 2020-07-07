//
// Created by white on 2018/5/17.
//

#ifndef DX_ANTI_ANDROID_COMM_UTILS_H
#define DX_ANTI_ANDROID_COMM_UTILS_H


#include <string>
#include <functional>

#include "anti_config.h"


using namespace std;

string anti_util_executeCMD(const string &cmd, const string &filter = "");


uint64_t anti_util_createRandom(int base);

void anti_util_sleepMS(uint64_t ms);

void anti_util_sleepSEC(uint32_t sec);

template<class FUNC>
void anti_util_startThread(function<FUNC> fun, bool wait = false);



#endif //DX_ANTI_ANDROID_COMM_UTILS_H
