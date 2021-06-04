//
// Created by boyliang on 2017/5/19.
//

#ifndef ANTI_LOG_H
#define ANTI_LOG_H

#include <stdio.h>
#include <errno.h>

#include "anti_config.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ANTI_ERRMSG strerror(errno)


/**
 * 错误日志输出
 */
void anti_err_log(const char *fmt, ...);

/**
 * 错误日志加强版
 */
#define err_ext_log(fmt, ...) err_log("where: %s, error: " fmt, __func__, #__VA_ARGS__)
    
#ifdef ANTI_DEBUG
/**
 * 普通日志输出
 */
void anti_info_log(const char *fmt, ...);
    
/**
 * 流程日志，非敏感日志
 */
void anti_flow_log(const char *fmt, ...);
#else
#define anti_info_log(...)
#define anti_flow_log(...)
#endif


#ifdef __cplusplus
}
#endif

#endif //ANTI_LOG_H
