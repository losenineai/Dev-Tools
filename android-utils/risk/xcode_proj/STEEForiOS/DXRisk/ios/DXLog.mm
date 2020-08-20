//
//  log.mm
//  STEEForiOS
//
//  reCreated by xelz on 24/08/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "DXLog.h"
#import <Foundation/Foundation.h>

#ifdef DEBUG
#define LOG_LEVEL_DEBUG
#else
#define LOG_LEVEL_ERROR
#endif

void sys_log_v(const char *type, const char *fmt, va_list args)
{
    @autoreleasepool {
         NSLogv([NSString stringWithFormat:@"%s - %s", type, fmt], args);
    }
}

void LOGR(const char *fmt, ...)
{
#if defined(LOG_LEVEL_ERROR) || defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_FLOW)
    va_list args;
    va_start(args, fmt);
    sys_log_v("release", fmt, args);
    va_end(args);
#endif
}

void LOGE(const char *fmt, ...)
{
#if defined(LOG_LEVEL_ERROR) || defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_FLOW)
    va_list args;
    va_start(args, fmt);
    sys_log_v("error", fmt, args);
    va_end(args);
#endif
}

#ifdef DEBUG
void LOGD(const char *fmt, ...)
{
#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_FLOW)
    va_list args;
    va_start(args, fmt);
    sys_log_v("info", fmt, args);
    va_end(args);
#endif
}


#endif

