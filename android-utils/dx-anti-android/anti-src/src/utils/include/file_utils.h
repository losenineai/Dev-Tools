//
// Created by white on 2018/5/16.
//

#ifndef DX_ANTI_ANDROID_FILE_UTIL_H
#define DX_ANTI_ANDROID_FILE_UTIL_H

#include <vector>
#include <string>
#include <mutex>

#include "anti_config.h"

using namespace std;

// 常用宏定义
#define delete_array(x) if((x)){delete [](x);(x) = NULL;}
#define delete_not_array(x) if((x)){delete (x);(x) = NULL;}
// 同步锁
#define synchronized(lock) lock_guard<recursive_mutex> __lg__(lock)

/**
 * 判断文件是否存在
 * @param name
 * @return
 */
bool anti_file_exist(const string &name);

/**
 * 判断文件是否可执行
 * @param name
 * @return
 */
bool anti_file_executable(const string &name);

/**
 * 判断文件可打开
 * @param name
 * @return
 */
bool anti_file_canOpen(const string &name);

/**
 * 读取文件所有内容
 * @param path
 * @param result
 */
void anti_file_readAll(const string &path, string &result, int64_t limitSize = 1024 * 1024 * 32);

/**
 * 按行读取文件内容
 * @param path
 * @param resultVec
 */
void anti_file_readLines(const string &path, vector<string> &resultVec, int32_t limitLine = 1024 * 32);

/**
 * 重写文件内容
 * @param name
 * @param data
 */
void anti_file_writeAll(const string &name, const string &data, bool append = false);

/**
 * 删除文件
 * @param name
 */
void anti_file_deleteIfExist(const string &name);

/**
 * 创建目录
 * @param name
 * @param mode
 * @return
 */
bool anti_file_makeDir(const string &name, int mode = 0755);

/**
 * 删除目录
 * @param name
 * @return
 */
bool anti_file_deleteDir(const string &name);

/**
 * 获取目录子文件列表
 * @param dir
 * @param filters
 * @return
 */
vector<string> anti_file_list(const string &dir, const vector<string> &filters = vector<string>());



#endif //DX_ANTI_ANDROID_FILE_UTIL_H
