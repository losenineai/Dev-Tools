//
//  CheckEnv.m
//  STEEForiOS
//
//  Created by weaiken on 2019/10/8.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//
#import "UIDevice+STEE_Application.h"
#import "CheckEnv.h"
#import <mach-o/dyld.h>
#import <string.h>
#import <sys/types.h>
#import <unistd.h>
#import <sys/sysctl.h>
#import <objc/runtime.h>
#import <dlfcn.h>
#import "STEECommonFunctions.h"
#import <DXEnvCheck.h>
#import "DXGCDWebServer.h"
#import "DXGCDWebServerDataResponse.h"
#import <UIKit/UIKit.h>


@implementation CheckEnv


+ (BOOL) checkInject{
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
       return YES;
    }

    /**  针对monkey 环境检测  **/
    NSString* exeFileName = [[[NSBundle mainBundle] executableURL] lastPathComponent];
    NSString* appName = [NSString stringWithFormat:@"%@%@" , exeFileName , @".app"];
    if ([[NSFileManager defaultManager] fileExistsAtPath:appName]){
        return YES;
    }
    
    return NO;
}


+ (BOOL) checkDebug{
    @autoreleasepool {
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
                  return YES;
              }
        return NO;
    }
}
+ (BOOL) checkJailbreak{
      @autoreleasepool {
                 if([[UIDevice currentDevice] stee_isJailbreak_1] || [[UIDevice currentDevice] stee_isJailbreak_2] || [[UIDevice currentDevice]         stee_isJailbreak_3] || [[UIDevice currentDevice] stee_isJailbreak_4]) {
                     return YES;
                 }
          return NO;
            }
}


@end



