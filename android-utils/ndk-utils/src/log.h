//
// Created by white on 2017/5/19.
//

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#define TAG "white-dumpdex"

/**
 * 错误日志输出
 */
void LOGE(const char *fmt, ...);


/**
 * 普通日志输出
 */
void LOGI(const char *fmt, ...);

    

#ifdef __cplusplus
}
#endif

#endif //LOG_H
