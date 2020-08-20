//
//  DXRiskManager.m
//  STEEForiOS
//
//  Created by echonn on 04/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "DXRisk.h"
#import "STEECommonFunctions.h"
#import <DXEnvCheck.h>
#import <Foundation/Foundation.h>
#import <sys/signal.h>
#include <libkern/OSAtomic.h>
#include <execinfo.h>
#import <DXRiskInter.h>

/* 用户相关字段 */
NSString* const DXRiskManagerKeyUserId = @"U1";
NSString* const DXRiskManagerKeyEmail = @"U2";
NSString* const DXRiskManagerKeyPhone = @"U3";
NSString* const DXRiskManagerKeyUserExtend1 = @"U100";
NSString* const DXRiskManagerKeyUserExtend2 = @"U101";

/* 私有化url */
NSString* const DXRiskManagerKeyURL = @"KEY_URL";
/* 私有化服务器是否双写 */
NSString* const DXRiskManagerKeyBackup = @"KEY_BACKUP";
NSString* const DXRiskManagerKeyBackupEnable = @"VALUE_ENABLE_BACKUP";


NSString* const DXRiskManagerKeyBackupAppId = @"KEY_BACKUP_APPID";

NSString* const DXRiskManagerKeyDelayMsTime = @"KEY_DELAY_MS_TIME";

/* 数据降级发送统计请求 */
NSString* const DXRiskManagerKeyDegradeNotify = @"KEY_DEGRADENOTIFY";
NSString* const DXRiskManagerKeyDegradeNotifyEnable = @"DEGRADENOTIFY";

/* nr: 普通token  light: 轻量级token */
NSString* const DXRiskManagerConfigToken = @"KEY_TOKEN";
NSString* const DXRiskManagerTokenNormal = @"NORMAL";
NSString* const DXRiskManagerTokenLight = @"LIGHT";

/* 接入saas服务器所在国家 */
NSString* const DXRiskManagerKeyCountry = @"KEY_COUNTRY";
NSString* const DXRiskManagerCountryChina = @"CHINA";
NSString* const DXRiskManagerCountryIndonesia = @"INDONESIA";

@interface DXRisk()
{
    DXRiskInter *_compt;
    BOOL mIsSetup;
    NSString *appId;
    NSLock *_lock;
}

@end

@implementation DXRisk

static map<string, string> t_params;

+ (instancetype)sharedInstance
{
    static DXRisk *_instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (_instance == nil) {
            _instance = [[DXRisk alloc] init];
            [_instance _getMgrInstance];
        }
    });
    
    return _instance;
}

- (void)_getMgrInstance
{
    _compt = new DXRiskInter();
    _lock = [[NSLock alloc] init];
}

- (id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}


+ (NSString *)getToken:(NSString*) appId extendsParams:(NSDictionary *)extendsParams
{
    @autoreleasepool {
        [[DXRisk sharedInstance]->_lock lock];
//        [DXRiskManager sharedInstance]->t_extendsParams = extendsParams;
        map<string, string> params;
        for (int i = 0; i < extendsParams.count; i ++) {
            NSString *key = extendsParams.allKeys[i];
            NSString *value = [extendsParams objectForKey:key];
            params.insert(pair<string,string>(NSString2CPPString(key), NSString2CPPString(value)));
        }

        if (![DXRisk sharedInstance]->mIsSetup || ![[DXRisk sharedInstance]->appId isEqualToString:appId]){
            [DXRisk sharedInstance]->_compt->Init(NSString2CPPString(appId) , params);
            [DXRisk sharedInstance]->mIsSetup = YES;
            [DXRisk sharedInstance]->appId = appId;
        }
        
        string constID = [DXRisk sharedInstance]->_compt->GetToken(true);
        [[DXRisk sharedInstance]->_lock unlock];
        return CPPString2NSString(constID);
    }
}

+ (BOOL)setup  {
       @autoreleasepool {
        DXEnvCheck::GetInstance()->Setup();
        return YES;
    }
}


@end


void UncaughtExceptionHandler(NSException *exception) {
//    NSString* st =  [[NSUserDefaults standardUserDefaults] stringForKey:@"DX-ExceptionContent"];
//        NSLog(@"---st---%@" , st);
//    NSArray *callStack = [exception callStackSymbols];
//    NSString *reason = [exception reason];
//    NSString *name = [exception name];
//    NSString *content = [NSString stringWithFormat:@"========异常错误报告========name:%@reason:%@callStackSymbols:%@",name,reason,[callStack componentsJoinedByString:@""]];
//    NSLog(@"---st1---%@" , content);
//    //将崩溃信息持久化在本地，下次程序启动时、或者后台，将崩溃信息作为日志发送给开发者。
//    [[NSUserDefaults standardUserDefaults] setObject:content forKey:@"DX-ExceptionContent"];
    if (exception) {
        [[NSUserDefaults standardUserDefaults] setObject:@"ExceptionHandler" forKey:@"DX-ExceptionFlat"];
    }
    
}

volatile int32_t UncaughtExceptionCount = 0;
const NSInteger UncaughtExceptionHandlerSkipAddressCount = 4;
const NSInteger UncaughtExceptionHandlerReportAddressCount = 5;
const int32_t UncaughtExceptionMaximum = 10;
void SignalHandler(int signal){
    int32_t exceptionCount = OSAtomicIncrement32(&UncaughtExceptionCount);
    if (exceptionCount > UncaughtExceptionMaximum)
    {
//        NSLog(@"exceptionCount------%d",exceptionCount);
        return;
    }
    
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    
    int i;
    NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
    for (
         i = UncaughtExceptionHandlerSkipAddressCount;
         i < UncaughtExceptionHandlerSkipAddressCount +
         UncaughtExceptionHandlerReportAddressCount;
         i++)
    {
        [backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
    }
    free(strs);
    
    if ([backtrace count] > 0) {
        [[NSUserDefaults standardUserDefaults] setValue:@"SignalHandler" forKey:@"DX-ExceptionFlat"];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
//    NSLog(@"SignalHandler------%@",backtrace);
//    NSLog(@"SignalHandler------%@", [NSString stringWithFormat:
//                  NSLocalizedString(@"Signal %d was raised.", nil),
//                  signal]);
}

void  inline initSignalCrash(){
     signal(SIGHUP, SignalHandler);
     signal(SIGINT, SignalHandler);
     signal(SIGQUIT, SignalHandler);
     signal(SIGABRT, SignalHandler);
     signal(SIGILL, SignalHandler);
     signal(SIGSEGV, SignalHandler);
     signal(SIGFPE, SignalHandler);
     signal(SIGBUS, SignalHandler);
     signal(SIGPIPE, SignalHandler);
}


__attribute__((constructor(101)))
__attribute__((visibility("hidden")))
static void DX_CrashInit(void) {
    NSSetUncaughtExceptionHandler(&UncaughtExceptionHandler);
    initSignalCrash();
}


