//
//  UIDevice+STEE_Application.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "UIDevice+STEE_Application.h"
#include <dlfcn.h>
#include <sys/stat.h>
#include <mach-o/dyld.h>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

const char* dx_risk_jailbreak_tool_pathes[] = {
    // checkra1n
    "/private/var/checkra1n.dmg",
    "/var/binpack/Applications/loader.app",
    "/binpack/loader.dmg",
    "/binpack/bin/launchctl",
    "/usr/libexec/cydia/cydo",
    "/var/binpack/loaderd_hook",
    "/private/var/dropbear_rsa_host_key",
    "/private/var/binpack/Applications/loader.app",
    "/usr/bin/arch",
    "/usr/bin/dpkg",
    "/usr/bin/dpkg-divert",
    "/usr/libexec/firmware.sh",
    "/usr/libexec/rd.dmg",
    "/usr/libexec/sysstatuscheck",
    "/var/lib/dpkg/info",
    "/var/lib/dpkg/status",
    "/var/run/utmp",
    // other
    "/Applications/Cydia.app",
    "/Applications/FakeCarrier.app",
    "/Applications/Icy.app",
    "/Applications/IntelliScreen.app",
    "/Applications/MxTube.app",
    "/Applications/RockApp.app",
    "/Applications/SBSettings.app",
    "/Applications/WinterBoard.app",
    "/Applications/blackra1n.app",
    "/Library/MobileSubstrate/DynamicLibraries/LiveClock.plist",
    "/Library/MobileSubstrate/DynamicLibraries/Veency.plist",
    "/Library/MobileSubstrate/MobileSubstrate.dylib",
    "/System/Library/LaunchDaemons/com.ikey.bbot.plist",
    "/System/Library/LaunchDaemons/com.saurik.Cydia.Startup.plist",
    "/bin/bash",
    "/bin/sh",
    "/etc/apt",
    "/etc/ssh/sshd_config",
    "/private/var/lib/apt",
    "/private/var/lib/cydia",
    "/private/var/mobile/Library/SBSettings/Themes",
    "/private/var/stash",
    "/private/var/tmp/cydia.log",
    "/usr/bin/sshd",
    "/usr/libexec/sftp-server",
    "/usr/libexec/ssh-keysign",
    "/usr/sbin/sshd",
    "/var/cache/apt",
    "/var/lib/apt",
    "/var/lib/cydia",
    "/usr/sbin/frida-server",
    "/usr/bin/cycript",
    "/usr/local/bin/cycript",
    "/usr/lib/libcycript.dylib",
};

@implementation UIDevice (STEE_Application)

- (BOOL)stee_isJailbreak_1
{
    struct stat stat_info;
    for (int i = 0; i < ARRAY_SIZE(dx_risk_jailbreak_tool_pathes); i ++) {
        if (0 == stat(dx_risk_jailbreak_tool_pathes[i], &stat_info)) {
            return YES;
        }
    }
    return NO;
}



- (BOOL)stee_isJailbreak_2
{
    uint32_t count = _dyld_image_count();
    //防越狱插件后会把原有的越狱文件名称统一修改成在/usr/lib/目录下的libSystem.B.dylib
    NSString *jtpath=@"/usr/lib/libSystem.B.dylib";
    
    uint32_t countyueyu=0;
    
    for(uint32_t i = 0; i < count; i++)
    {
        //Name of image (includes full path)
        const char *dyld = _dyld_get_image_name(i);
        
        //Get name of file
        int slength = (int)strlen(dyld);
        
        int j;
        for(j = slength - 1; j>= 0; --j)
            if(dyld[j] == '/') break;
        
        NSString *name = [[NSString alloc]initWithUTF8String:_dyld_get_image_name(i)];
        if([name compare:jtpath] == NSOrderedSame)
        {
            countyueyu++;
        }
        if([name containsString:@"/Library/MobileSubstrate"])
        {
            return YES;
        }
    }
    if( countyueyu > 2 )
        return YES;
    return NO;
}


//10.0.1 5s 越狱无此特征、8.1.3 4s越狱有
- (BOOL)stee_isJailbreak_3
{
    struct stat s;
    lstat("/Library/Wallpaper", &s);
    if (s.st_mode & S_IFLNK) {
        return YES;
    }
    else {
        return NO;
    }
}

- (BOOL)stee_isJailbreak_4
{

    char *env = getenv("DYLD_INSERT_LIBRARIES");
    
    
    BOOL checkImage;
    uint32_t count = _dyld_image_count();
    for(uint32_t i=0; i<count; i++) {
        const char *name = _dyld_get_image_name(i);
        if(strstr(name, "/Library/MobileSubstrate/DynamicLibraries") != NULL) {
            checkImage = YES;
        }
    }
    checkImage = NO;
    
    if((env!=NULL && strstr(env, "MobileSubstrate.dylib") != NULL) || checkImage) {
        return YES;
    }
    
    return NO;
}


- (BOOL)stee_isJailbreak_5
{
    [@"test" writeToURL:[NSURL URLWithString:@"/private/dxTest_Jailbreak.txt"] atomically:YES encoding:NSUTF8StringEncoding error:NULL];
    BOOL is = [[NSFileManager defaultManager] removeItemAtPath:@"/private/dxTest_Jailbreak.txt" error:NULL];
    return is;
}

@end
