//
// Created by white on 2018/5/16.
//

#ifndef DX_ANTI_ANDROID_STRING_UTILS_H
#define DX_ANTI_ANDROID_STRING_UTILS_H


#include <string>
#include <vector>

#include "anti_config.h"

using namespace std;


// 常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}
#define delete_not_array(x) if((x)){delete (x);(x) = NULL;}
#define ANTI_ERRMSG strerror(errno)
// 同步锁
#define synchronized(lock) lock_guard<recursive_mutex> __lg__(lock)


string anti_str_replaceAll(const string &s, const string &search, const string &replace);

void anti_str_splitString(const string &s, vector<string> &v, const string &c);

string anti_str_format(const char *fmt, ...);

string anti_str_MD5(const string &src);

string anti_str_toHexString(const string &input);

int anti_str_start_with(const char *str, const char *suffix);

int anti_str_end_with(const char *str, const char *suffix);

void anti_str_containWithFilter(vector<string> &filters, vector<string> &src, vector<string> &result);

string anti_str_charToHexString(unsigned char *pSrc, int nL);


void anti_str_ltrim(std::string &s);

// trim from end (in place)
void anti_str_rtrim(std::string &s);

// trim from both ends (in place)
void anti_str_trim(std::string &s);

// trim from start (copying)
std::string anti_str_ltrim_copy(std::string s);

// trim from end (copying)
std::string anti_str_rtrim_copy(std::string s);

// trim from both ends (copying)
std::string anti_str_trim_copy(std::string s);

// trim both start and end by c
void anti_str_trim_c(char *src);


#endif //DX_ANTI_ANDROID_STRING_UTILS_H
