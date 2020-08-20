//
//  UIDevice+STEE_Hardware.h
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIDevice (STEE_Hardware)
#ifdef STEE_IDFA
- (NSString *)stee_idfa;
#endif
- (NSString *)stee_idfv;
- (NSString *)stee_uuid;

- (NSDictionary *)stee_hostCPUInfo;

#pragma mark - Memory
- (unsigned long long)stee_totalMemorySize_1;
- (unsigned long long)stee_totalMemorySize_2;
- (unsigned long long)stee_availableMemory;
- (long long)stee_totalDiskSpace_1;
- (long long)stee_totalDiskSpace_2;
- (long long)stee_freeDiskSpace_1;
- (long long)stee_freeDiskSpace_2;

- (NSDictionary *)stee_utsnameInfo;

@end
