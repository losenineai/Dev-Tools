//
// Created by boyliang on 2017/5/26.
//check_license_validity

#ifndef STEE_CLIENT_SDK_PLATFORM_H
#define STEE_CLIENT_SDK_PLATFORM_H

#include <stdio.h>
#include <limits.h>
#include <string>

#include "stee_base.pb-c.h"
#include "DXRiskInter.h"
#include "DXEnvCheck.h"
#include "core_prefix.h"
#include <map>
#include "pb_prefix.h"

using std::string;

/**
 * HTTP_POST
 */
#define HTTP_POST 0

/**
 * HTTP_GET
 */
#define HTTP_GET 1

/**
 * 发起HTTP请求
 * @param type post or get
 * @param url
 * @param data
 * @return
 */
string sync_http_request(const int type, const string &url, const string &data, const bool isSaas, const string &contentType = "");

/**
 * 发起异步HTTP请求
 * @param type post or get
 * @param url
 * @param data
 * @return
 */
void async_http_request(const int type, const string &url, const string &data, const bool isSaas, const string &contentType = "");

/**
 * 风控组件风险检测项配置初始化
 * @param sp
 * @return
 */
int dx_risk_check_init(DXEnvCheck &sp);

/**
 * 风控组件收集项的平台相关实现
 * @param sp
 * @return
 */
int dx_risk_init(DXRiskInter &sp);

/**
 * 获取app版本
 * @return
 */
string get_app_version();

/**
 * 获取app版本号
 * @return
 */
int get_app_version_code();

/**
 * 获取系统版本
 * @return
 */
string get_os_version();

/**
 * 获取应用包名
 * @return
 */
string get_pkg_name();

/**
 * 获取操作系统类型
 * @return
 */
Com__Dingxiang__Mobile__STEERequestHeader__OSType get_os_type();

/**
 * 获取操作CPU架构
 * @return
 */
Com__Dingxiang__Mobile__STEERequestHeader__OSArch get_os_arch();

/**
 * 获取STEE-SDK版本号
 * @return
 */
string get_dx_risk_sdk_version();

/**
 * 获取持久化数据, key value的形式
 */
#ifdef __APPLE__
string get_property(const string &appId, const string &key);
#else
string get_property(const string &appId, const string &key, const int scope = 1);
#endif

/**
 * 设置持久化数据，key value的形式
 * @param key
 * @param value
 */
#ifdef __APPLE__
void set_property(const string &appId, const string &key, const string &value);
#else
void set_property(const string &appId, const string &key, const string &value, const int scope = 1);
#endif

/**
 * 获取当前可读可写的工作目录
 * @return
 */
string get_work_dir();

/**
 * 获取内置sdcard卡路径
 * @return
 */
string get_sdcard_dir();

/**
 * 获取外置sdcard卡路径
 * @return
 */
string get_exsdcard_dir();

/**
 * 获取sdcard状态
 * @return
 */
int get_sdcard_status();

/**
 * 获取AppList
 * @return
 */
string get_app_list();


#endif //STEE_CLIENT_SDK_PLATFORM_H
