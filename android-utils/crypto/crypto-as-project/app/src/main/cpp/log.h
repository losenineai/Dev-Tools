//
// Created by boyliang on 2017/5/19.
//

#ifndef DXANTI_TEST_LOG_H
#define DXANTI_TEST_LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 错误日志输出
 */
void err_log(const char *fmt, ...);

/**
 * 错误日志加强版
 */
#define err_ext_log(fmt, ...) err_log("where: %s, error: " fmt, __func__, #__VA_ARGS__)


/**
 * 普通日志输出
 */
void info_log(const char *fmt, ...);

#ifdef DEBUG

/**
 * 流程日志，非敏感日志
 */
void flow_log(const char *fmt, ...);
#else
static inline void flow_log(const char *fmt, ...) { }
#endif

#define logB(x) info_log(x " begin")
#define logF(x) info_log(x " finish")

#ifdef __cplusplus
}
#endif

#endif //DXANTI_TEST_LOG_H
