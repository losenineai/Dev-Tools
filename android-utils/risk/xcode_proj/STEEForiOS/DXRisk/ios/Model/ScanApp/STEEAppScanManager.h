//
//  STEEAppScanManager.h
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <Foundation/Foundation.h>

@class STEEApplicationProxy;

@interface STEEAppScanManager : NSObject
+ (instancetype)sharedAppScanManager;
- (NSArray<NSString *> *)getAllAppsPackageName;
- (NSArray<STEEApplicationProxy *> *)getAllAppsInfo;
@end
