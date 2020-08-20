//
//  UIDevice+STEE_Hardware.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <sys/utsname.h>
#import <sys/sysctl.h>
#import <sys/mount.h>
#import <mach-o/arch.h>
#import <mach/mach_host.h>
#import <AdSupport/AdSupport.h>

#import "DXKeychainHelper.h"
#import "UIDevice+STEE_Hardware.h"
#import "STEECommonFunctions.h"


@interface UIDevice()

@end

@implementation UIDevice (STEE_Hardware)

#pragma mark - IDs
#ifdef STEE_IDFA
- (NSString *)stee_idfa
{
    NSString *adId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return adId;
}
#endif

- (NSString *)stee_idfv
{
    NSString *idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    return idfv;
}

- (NSString *)stee_uuid
{
    
    NSString *uuid = [DXKeyChainUtils get:@"DX-UUID-A" default:nil];
    
    if(uuid == nil){
        uuid = [[NSUUID UUID] UUIDString];
        [DXKeyChainUtils set:uuid forkey:@"DX-UUID-A"];
    }
   
    return uuid;
}

#pragma mark - HostInfo
- (host_basic_info_data_t)_getHostInfo
{
    host_basic_info_data_t hostInfo;
    mach_msg_type_number_t infoCount;
    
    infoCount = HOST_BASIC_INFO_COUNT;
    host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&hostInfo, &infoCount);
    return hostInfo;
}

#pragma mark - CPU
- (NSDictionary *)stee_hostCPUInfo
{
    @autoreleasepool {
        host_basic_info_data_t info = [self _getHostInfo];
        NSString *cpuTypeStr = @"";
        switch (info.cpu_type) {
            case CPU_TYPE_ARM:
                cpuTypeStr = @"CPU_TYPE_ARM";
                break;
            case CPU_TYPE_ARM64:
                cpuTypeStr = @"CPU_TYPE_ARM64";
                break;
            case CPU_TYPE_X86:
                cpuTypeStr = @"CPU_TYPE_X86";
                break;
            case CPU_TYPE_X86_64:
                cpuTypeStr = @"CPU_TYPE_X86_64";
                break;
            default:
                break;
        }

        return @{@"type_t":[NSString stringWithFormat:@"%d", info.cpu_type],
                 @"type_s":cpuTypeStr,
                 @"subtype":[NSString stringWithFormat:@"%d", info.cpu_subtype],
                 @"max_cpus":[NSString stringWithFormat:@"%d", info.max_cpus],
                 @"avail_cpus":[NSString stringWithFormat:@"%d", info.avail_cpus]
                 };
    }
}

#pragma mark - Memory
- (unsigned long long)stee_totalMemorySize_1
{
    return [NSProcessInfo processInfo].physicalMemory;
}

- (unsigned long long)stee_totalMemorySize_2
{
    host_basic_info_data_t info = [self _getHostInfo];
    return info.memory_size;
}

- (unsigned long long)stee_availableMemory
{
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(),
                                               HOST_VM_INFO,
                                               (host_info_t)&vmStats,
                                               &infoCount);
    
    if (kernReturn != KERN_SUCCESS) {
        return NSNotFound;
    }
    
    return (vm_page_size *vmStats.free_count + vm_page_size * vmStats.inactive_count);
}

- (long long)stee_totalDiskSpace_1
{
    NSDictionary *fattributes = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
    NSNumber *totalSize = [fattributes objectForKey:NSFileSystemSize];
    return totalSize.longLongValue;
}

-(long long)stee_totalDiskSpace_2
{
    struct statfs buf;
    unsigned long long totalSpace = -1;
    if (statfs("/var", &buf) >= 0)
    {
        totalSpace = (unsigned long long)(buf.f_bsize * buf.f_blocks);
    }
    return totalSpace;
}

- (long long)stee_freeDiskSpace_1
{
    NSDictionary *fattributes = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
    NSNumber *freeSize = [fattributes objectForKey:NSFileSystemFreeSize];
    return freeSize.longLongValue;
}

-(long long)stee_freeDiskSpace_2
{
    struct statfs buf;
    unsigned long long totalSpace = -1;
    if (statfs("/var", &buf) >= 0)
    {
        totalSpace = (unsigned long long)(buf.f_bsize * buf.f_bavail);
    }
    return totalSpace;
}

#pragma mark -  utsname
- (NSDictionary *)stee_utsnameInfo
{
    @autoreleasepool {
        struct utsname systemInfo;
        uname(&systemInfo);
        return @{@"os": CString2NSString(systemInfo.sysname),
                 @"network_node":CString2NSString(systemInfo.nodename),
                 @"release":CString2NSString(systemInfo.release),
                 @"version":CString2NSString(systemInfo.version),
                 @"hardware_type":CString2NSString(systemInfo.machine)
                 };
    }
}

#pragma mark sysctl utils

- (NSUInteger) getSysInfo: (uint) typeSpecifier
{
    size_t size = sizeof(int);
    int results;
    int mib[2] = {CTL_HW, typeSpecifier};
    sysctl(mib, 2, &results, &size, NULL, 0);
    return (NSUInteger) results;
}

@end
