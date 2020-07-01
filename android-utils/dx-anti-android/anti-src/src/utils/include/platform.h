//
// Created by white on 2018/5/15.
//

#ifndef DX_ANTI_ANDROID_PLATFORM_H
#define DX_ANTI_ANDROID_PLATFORM_H


#include <string>

#include "anti_config.h"

using namespace std;

/**
 * 获取应用包名
 * @return
 */
string anti_platform_getPkgName();

/**
 * 获取当前可读可写的工作目录
 * @return
 */
string anti_platform_getWorkDir();

/**
 * 获取资源路径
 * @return
 */
string anti_platform_getResPath();

/**
 * 是否正在调试连接
 * @return
 */
bool anti_platform_isDebuggerConnected();


#endif //DX_ANTI_ANDROID_PLATFORM_H
