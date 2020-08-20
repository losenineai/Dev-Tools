//
//  UIDevice+STEE_InfoDic.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <Security/Security.h>
#import "UIDevice+STEE_InfoDic.h"
#import "UIDevice+STEE_Hardware.m"
#import "DXKeychainHelper.h"

#import "DXICMP.h"
#import "DXARP.h"
#import "DXMDNS.h"
#import "DXAddress.h"

@implementation UIDevice (STEE_InfoDic)
- (NSDictionary *)stee_macFromArp
{
#if TARGET_IPHONE_SIMULATOR 
    return @{};
#else
    NSString *ip = [DXAddress currentIPAddressOf:@"en0"];
    if (ip.length <= 0) {
        return @{};
    }
    
    [DXICMP sendICMPEchoRequestTo:ip];
    NSString *mac1 = [DXARP walkMACAddressOf:ip];
    
    [DXICMP sendICMPEchoRequestTo:ip];
    NSString *mac2 = [DXARP MACAddressOf:ip];
    
//    NSString *mac3 = [DXMDNS getMacAddressFromMDNS];
    
    return @{@"api1":mac1.length > 0 ? mac1 : @"",
             @"api2":mac2.length > 0 ? mac2 : @""/*,
             @"api3":mac3.length > 0 ? mac3 : @""*/
             };
#endif
}

- (NSDictionary *)stee_deviceIDs
{
    
    return @{@"uuid":[self stee_uuid],
             @"idfv":[self stee_idfv]
             
#ifdef STEE_IDFA
             ,@"idfa":[self stee_idfa]
#endif
             };
}

- (NSDictionary *)stee_bluetoothInfo
{
    return @{};
}

- (NSDictionary *)stee_CPUInfo
{
    return [self stee_hostCPUInfo];
}

- (NSDictionary *)stee_screenInfo
{
     @autoreleasepool {
         CGSize screenSize = [UIScreen mainScreen].bounds.size;
         CGFloat scale = [UIScreen mainScreen].scale;
         return @{@"width":[NSString stringWithFormat:@"%.0f", screenSize.width],
                  @"height":[NSString stringWithFormat:@"%.0f", screenSize.height],
                  @"scale":[NSString stringWithFormat:@"%.0f", scale],
                  @"resolution":[NSString stringWithFormat:@"%.0fX%.0f", screenSize.width * scale, screenSize.height * scale]
                  };
     }

}

- (NSDictionary *)stee_memoryInfo
{
     @autoreleasepool {
    return @{@"ram_1":[NSString stringWithFormat:@"%lld", [self stee_totalMemorySize_1]],
             @"ram_2":[NSString stringWithFormat:@"%lld", [self stee_totalMemorySize_2]],
//             @"available_memory":[NSString stringWithFormat:@"%lld", [self stee_availableMemory]],
             @"rom_1":[NSString stringWithFormat:@"%lld", [self stee_totalDiskSpace_1]],
             @"rom_2":[NSString stringWithFormat:@"%lld", [self stee_totalDiskSpace_2]]//,
//             @"free_disk_1":[NSString stringWithFormat:@"%lld", [self stee_freeDiskSpace_1]],
//             @"free_disk_2":[NSString stringWithFormat:@"%lld", [self stee_freeDiskSpace_2]]
             };
     }
}

- (NSDictionary *)stee_batteryInfo
{
     @autoreleasepool {
         [UIDevice currentDevice].batteryMonitoringEnabled = YES;

         NSString *batteryState = @"";
         switch ([UIDevice currentDevice].batteryState) {
             case UIDeviceBatteryStateUnknown:
                 batteryState = @"UIDeviceBatteryStateUnknown";
                 break;
             case UIDeviceBatteryStateCharging:
                 batteryState = @"UIDeviceBatteryStateCharging";
                 break;
             case UIDeviceBatteryStateUnplugged:
                 batteryState = @"UIDeviceBatteryStateUnplugged";
                 break;
             case UIDeviceBatteryStateFull:
                 batteryState = @"UIDeviceBatteryStateFull";
                 break;
             default:
                 break;
         }

         return @{@"plugged":batteryState,
                  @"level":[NSString stringWithFormat:@"%.0f", [UIDevice currentDevice].batteryLevel * 100]
                  };
     }

}

- (NSDictionary *)stee_cameraInfo
{
    return @{};
}

- (NSDictionary *)stee_kernalInfo
{
    return [self stee_utsnameInfo];
}

@end
