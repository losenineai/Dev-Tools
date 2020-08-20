//
//  UIDevice+STEE_InfoDic.h
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIDevice (STEE_InfoDic)
- (NSDictionary *)stee_macFromArp;
- (NSDictionary *)stee_deviceIDs;
- (NSDictionary *)stee_bluetoothInfo;
- (NSDictionary *)stee_CPUInfo;
- (NSDictionary *)stee_screenInfo;
- (NSDictionary *)stee_memoryInfo;
- (NSDictionary *)stee_batteryInfo;
- (NSDictionary *)stee_cameraInfo;
- (NSDictionary *)stee_kernalInfo;
@end
