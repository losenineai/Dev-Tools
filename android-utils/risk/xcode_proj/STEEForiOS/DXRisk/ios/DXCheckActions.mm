//
//  check_point.c
//  STEEForiOS
//
//  Created by xelz on 2017/7/12.
//  Copyright © 2017年 echonn. All rights reserved.
//

#include "DXRiskInter.h"
#import "UIDevice+STEE_Application.h"
#import "utils.h"

#include <mach-o/dyld.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <objc/runtime.h>
#include <dlfcn.h>
#import "DXfishhook.h"
#import "STEECommonFunctions.h"
#import <DXEnvCheck.h>
#import "DXGCDWebServer.h"
#import "DXGCDWebServerDataResponse.h"
#import <UIKit/UIKit.h>


using namespace std;

//#define DEFINE_CHECK_IMPL(name) inline void check_is_##name(function<void(void*)> &call)
#define DEFINE_CHECK_IMPL(name) inline void action_get_##name(map<string, string> &infos, void *args ,bool singleRun)
#define ADD_CHECK(name , singleRun) sp.AddCheckAction(#name, oc_env_bridge, (void *)action_get_##name , singleRun); \

#define  ADD_TO_INFO(key,value)                                                                                     \
         auto it = infos.find(NSString2CPPString(key));                                                             \
         if(it != infos.end()) {it->second = NSString2CPPString(value);}                                               \
         else {infos.insert(make_pair<string,string>(NSString2CPPString(key),NSString2CPPString(value)));}                \


#define CHECK_RUN                                                                                                   \
    static mutex _lock;                                                                                             \
    lock_guard<mutex> g(_lock);                                                                                     \
    static bool bRun = true;                                                                                        \
    if (!bRun || GetMapValueByKey(infos, NSString2CPPString(key), "") != "" )                                                            \
        return;                                                                                                     \
    if (singleRun)                                                                                                  \
        bRun = false;                                                                                               \


#define KEY_JAILBREAK @"K23"
// 20180804 新增
#define KEY_DEBUG @"K52"
#define KEY_INJECT @"K53"
#define KEY_HOOK @"K54"

#define KEY_INJECT_STR @"K139"
// 20190604
#define KEY_MUTIL_APP @"K55"

//#define ADD_CHECK(name) sp.AddAction(#name, jni_env_bridge, (void *)action_get_##name); \
//sp.AddCheckList(#name);

static bool isHookInit = false;
bool isHookFlat = false;

string m_appId = "";
DXGCDWebServer* _webServer;
// OC_CALL桥接
static void oc_env_bridge(map<string, string> &infos, void *args ,bool singleRun) {
    DataCheckAction action = (DataCheckAction) args;
    //        DataCollectionAction action = (DataCollectionAction) args;
    action(infos, NULL ,singleRun);
}

// 检测环境
string GetMapValueByKey(const map<string, string> &paramsMap, const string &key, const string &defValue = "") {
    
    auto it = paramsMap.find(key);
    if (it != paramsMap.end()) {
        return paramsMap.at(key);
    }
    else{
        return defValue;
    }
}

BOOL getAppActive(){
    __block BOOL isActiveFlat = NO;
      if ([NSThread isMainThread]) {
          if ([UIApplication sharedApplication].applicationState == UIApplicationStateActive){
              isActiveFlat = YES;
          }
      }else{
          dispatch_sync(dispatch_get_main_queue(), ^{
               if ([UIApplication sharedApplication].applicationState == UIApplicationStateActive){
                   isActiveFlat = YES;
//                    NSLog(@"---isActiveFlat1---" );
               }
//              NSLog(@"---isActiveFlat2---" );
          });
      }
    #ifdef DEBUG
//           NSLog(@"---isActiveFlat---%d" , isActiveFlat);
    #endif
    return isActiveFlat;
}

////保留原来的交换函数
//void (* _exchangeImplementations)(Method _Nonnull m1, Method _Nonnull m2);
//IMP _Nonnull (* _setImplementation)(Method _Nonnull m, IMP _Nonnull imp);
//IMP _Nonnull (* _getImplementation)(Method _Nonnull m);
//
////新的函数
//void exchangeImplementations(Method _Nonnull m1, Method _Nonnull m2){
//    //    NSLog(@"1检测到了hook");
//    //强制退出
//    if (isHookInit) {
//        isHookFlat = true;
//         _exchangeImplementations(m1 , m2);
//    }else{
//        _exchangeImplementations(m1 , m2);
//    }
//}
//
////新的函数
//void setImplementation(Method _Nonnull m, IMP _Nonnull imp){
//    //    NSLog(@"2检测到了hook");
//    //强制退出
//    if (isHookInit) {
//          isHookFlat = true;
//         _setImplementation(m , imp);
//    }else{
//        _setImplementation(m , imp);
//    }
//}
//
//
////新的函数
//void getImplementation(Method _Nonnull m){
//    //    NSLog(@"3检测到了hook");
//    //强制退出
//    if (isHookInit) {
//          isHookFlat = true;
//        _getImplementation(m);
//    }else{
//        _getImplementation(m);
//    }
//}

//@interface HookDelegate:NSObject
//
//@end
//
//@implementation HookDelegate
//
//+ (void) load{
//    struct rebinding bd;
//    bd.name = "method_exchangeImplementations";
//    bd.replacement =(void *) exchangeImplementations;
//    bd.replaced= (void **)&_exchangeImplementations;
//
//    struct rebinding bd1;
//    bd1.name = "method_setImplementation";
//    bd1.replacement=(void *)setImplementation;
//    bd1.replaced=(void **)&_setImplementation;
//
//    //目前保护越狱设备在该方法不闪退
//    if([[UIDevice currentDevice] stee_isJailbreak_1] || [[UIDevice currentDevice] stee_isJailbreak_2] || [[UIDevice currentDevice] stee_isJailbreak_3] || [[UIDevice currentDevice] stee_isJailbreak_4])
//    {
//        struct rebinding rebindings[]  = {bd,bd1};
//        rebind_symbols(rebindings, sizeof(rebindings) / sizeof(rebinding));
//    }else{
//        struct rebinding bd2;
//        bd2.name = "method_getImplementation";
//        bd2.replacement=(void *)getImplementation;
//        bd2.replaced=(void **)&_getImplementation;
//        struct rebinding rebindings[] = {bd,bd1,bd2};
//        rebind_symbols(rebindings, sizeof(rebindings) / sizeof(rebinding));
//    }
//
//}
//
//@end


//DEFINE_CHECK_IMPL(hook_new) {
//    NSString *key = KEY_HOOK;
//    CHECK_RUN
//    if (isHookInit && isHookFlat) {
//         ADD_TO_INFO(KEY_HOOK, @"true");
//    }
//    //isHookInit 的作用在于标示 是否Hook流程初始化 ，false 的时候 还是走原有的流程
//    isHookInit = true;
//}


DEFINE_CHECK_IMPL(inject) {
    @autoreleasepool {
        NSString *key = KEY_INJECT;
//        if (!getAppActive()) {
//            return;
//        }
        CHECK_RUN
        auto check_env = [&]() {
            char *env = getenv("DYLD_INSERT_LIBRARIES");
            return (env!=NULL && strstr(env, "MobileSubstrate.dylib") != NULL);
        };

        auto check_image = [&]() {
            uint32_t count = _dyld_image_count();
            for(uint32_t i=0; i<count; i++) {
                const char *name = _dyld_get_image_name(i);
                if(strstr(name, "/Library/MobileSubstrate/DynamicLibraries") != NULL) {
                    return true;
                }
            }
            return false;
        };

        auto check_substrate_cycrip = [&]() {
            uint32_t count = _dyld_image_count();
            for(uint32_t i=0; i<count; i++) {
                if([[NSString stringWithUTF8String:_dyld_get_image_name(i)].lastPathComponent isEqualToString:@"libsubstrate.dylib"]
                   || [[NSString stringWithUTF8String:_dyld_get_image_name(i)].lastPathComponent isEqualToString:@"libcycript.dylib"])
                {
                    return true;
                }
            }
            return false;
        };

        BOOL isCheckCycriptFileFind = NO;
        NSString *filePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/Frameworks/libcycript.db"];
        if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]){
            //        err_log("文件存在");
            isCheckCycriptFileFind = YES;
        }else{
            //        err_log("文件不存在");
            isCheckCycriptFileFind = NO;
        }

        if(check_env() || check_image() || check_substrate_cycrip() || isCheckCycriptFileFind) {
            {ADD_TO_INFO(KEY_INJECT, @"true");}
            {ADD_TO_INFO(KEY_INJECT_STR, @"MobileSubstrate.dylib");}
        }
    }

}

#define IMP(class_name, method_name) class_getMethodImplementation(objc_getClass(class_name),sel_registerName(method_name))

DEFINE_CHECK_IMPL(hook) {
    @autoreleasepool {
        NSString *key = KEY_HOOK;
//        if (!getAppActive()) {
//            return;
//        }
        CHECK_RUN
        uintptr_t start = (uintptr_t)_dyld_get_image_header(0);
        
        IMP ocmethods[] = {
#ifdef STEE_IDFA
            IMP("ASIdentifierManager", "advertisingIdentifier"),
#endif
            IMP("NSBundle", "bundleIdentifier"),
            IMP("NSBundle", "mainBundle"),
            IMP("NSFileManager", "contentsOfDirectoryAtPath:error:"),
            IMP("NSFileManager", "fileExistsAtPath:"),
            IMP("NSProcessInfo", "processInfo"),
            IMP("NSString", "initWithUTF8String:"),
        };
        
        size_t ocmethod_count = sizeof(ocmethods) / sizeof(IMP);
        for(size_t i=0; i<ocmethod_count; i++) {
            uintptr_t ptr = (uintptr_t)ocmethods[i];
            if(ptr !=0 && ptr < start) {
                ADD_TO_INFO(KEY_HOOK, @"true");
            }
        }
    }
}




DEFINE_CHECK_IMPL(debug) {
    @autoreleasepool {
        NSString *key = KEY_DEBUG;
        static bool isDebugFlat = false;
        
        NSLog(@"- - - - - ");
        
        if (!isDebugFlat){
//            if (!getAppActive()) {
//                return;
//            }
            CHECK_RUN
            int                 junk;
            int                 mib[4];
            struct kinfo_proc   info;
            size_t              size;
            info.kp_proc.p_flag = 0;
            
            mib[0] = CTL_KERN;
            mib[1] = KERN_PROC;
            mib[2] = KERN_PROC_PID;
            mib[3] = getpid();
            
            size = sizeof(info);
            junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
            
            if(junk != 0 || (info.kp_proc.p_flag & P_TRACED) != 0) {
                isDebugFlat = true;
                ADD_TO_INFO(KEY_DEBUG, @"true");
            }
        }
    }
   
//    //以下是不让调试代码
//    typedef int (*ptrace_ptr_t)(int _request, pid_t _pid, caddr_t _addr, int _data);
//    void *handle = dlopen(0, RTLD_GLOBAL | RTLD_NOW);
//    ptrace_ptr_t ptrace_ptr = (ptrace_ptr_t)dlsym(handle, "ptrace");
//    ptrace_ptr(31, 0, 0, 0);
//    syscall(26,31,0,0,0);
//    
//#ifdef __arm__
//    asm volatile(
//                 "mov r0,#31\n"
//                 "mov r1,#0\n"
//                 "mov r2,#0\n"
//                 "mov r12,#26\n"
//                 "svc #80\n"
//                 );
//#endif
//#ifdef __arm64__
//    asm volatile(
//                 "mov x0,#26\n"
//                 "mov x1,#31\n"
//                 "mov x2,#0\n"
//                 "mov x3,#0\n"
//                 "mov x16,#0\n"
//                 "svc #128\n"
//                 );
//#endif
}


DEFINE_CHECK_IMPL(jailbreak) {
    @autoreleasepool {
        NSString *key = KEY_JAILBREAK;
        static bool isDebugFlat = false;
        if (!isDebugFlat){
//            if (!getAppActive()) {
//                return;
//            }
            CHECK_RUN
             if([[UIDevice currentDevice] stee_isJailbreak_1] || [[UIDevice currentDevice] stee_isJailbreak_2] || [[UIDevice currentDevice]         stee_isJailbreak_3] || [[UIDevice currentDevice] stee_isJailbreak_4]) {
                 isDebugFlat = true;
                 ADD_TO_INFO(KEY_JAILBREAK, @"true");
             }
        }
//#ifdef DEBUG
//        NSLog(@"--Check--");
//#endif
    }
}

DEFINE_CHECK_IMPL(proxy) {
    NSDictionary *proxySettings = (__bridge NSDictionary *) CFNetworkCopySystemProxySettings();
    NSNumber *proxyHttpEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesHTTPEnable];
    NSNumber *proxyAutoConfigEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesProxyAutoConfigEnable];
    if( [proxyHttpEnable boolValue] || [proxyAutoConfigEnable boolValue] ) {
        ADD_TO_INFO(@"proxy", @"true");
    }
}

DEFINE_CHECK_IMPL(websocket_multu_app){
    @autoreleasepool {
        NSString *key = KEY_MUTIL_APP;
        m_appId = DXEnvCheck::GetInstance()->m_appid.data();
//        err_log("m_appId----%s" , m_appId.c_str());
        if (m_appId == "") {
            return;
        }
//        if (!getAppActive()) {
//            return;
//        }
        CHECK_RUN
            if ([NSThread isMainThread]) {
                if (_webServer == nil)
                    _webServer = [[DXGCDWebServer alloc]init];
                [_webServer addDefaultHandlerForMethod:@"GET" requestClass:[DXGCDWebServerRequest class] processBlock:^DXGCDWebServerResponse *(DXGCDWebServerRequest* request) {
                    NSDictionary *dictM = @{@"a": [NSNumber numberWithInt:getpid()]};
                    NSString *resp =[[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:dictM options:kNilOptions error:nil] encoding:NSUTF8StringEncoding];
                    return [DXGCDWebServerDataResponse responseWithText:resp];
                }];
                string sign = CryptUtils::MD5(m_appId);
                //            err_log("sign------%s" , sign.c_str()); //0bc31a31559db24b6e3df7d198c64c60
                int _port = 0;
                for (int i = 0 ; i < sign.length(); i ++) {
                    _port += (int) sign[i];
                }
                NSError *err;
                [_webServer startWithPort:(8000 + _port%1000) bonjourName:nil error:&err];
                if (err.code != 0){
//                    err_log("KEY_MUTIL_APP----true");
                    ADD_TO_INFO(KEY_MUTIL_APP, @"true");
                }
//                err_log("KEY_MUTIL_APP----1");
            }else{
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (_webServer == nil)
                        _webServer = [[DXGCDWebServer alloc]init];
                    [_webServer addDefaultHandlerForMethod:@"GET" requestClass:[DXGCDWebServerRequest class] processBlock:^DXGCDWebServerResponse *(DXGCDWebServerRequest* request) {
                        NSDictionary *dictM = @{@"a": [NSNumber numberWithInt:getpid()]};
                        NSString *resp =[[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:dictM options:kNilOptions error:nil] encoding:NSUTF8StringEncoding];
                        return [DXGCDWebServerDataResponse responseWithText:resp];
                    }];
                    string sign = CryptUtils::MD5(m_appId);
                    //            err_log("sign------%s" , sign.c_str()); //0bc31a31559db24b6e3df7d198c64c60
                    int _port = 0;
                    for (int i = 0 ; i < sign.length(); i ++) {
                        _port += (int) sign[i];
                    }
                    NSError *err;
                    [_webServer startWithPort:(8000 + _port%1000) bonjourName:nil error:&err];
                    if (err.code != 0){
//                        err_log("KEY_MUTIL_APP----true");
                        ADD_TO_INFO(KEY_MUTIL_APP, @"true");
                    }
                    
//                    err_log("KEY_MUTIL_APP----2");
                });
            }
        //            getRequest(8000 + _port%1000);
    }
}




int dx_risk_check_ios_impl(DXEnvCheck &sp){
    
        ADD_CHECK(inject ,false);
        ADD_CHECK(hook , false);
    //  ADD_CHECK(hook_new , false);
        ADD_CHECK(debug , false);
        ADD_CHECK(jailbreak , true);
        ADD_CHECK(websocket_multu_app, true);
    //  ADD_CHECK(proxy);
    return 0;
}



