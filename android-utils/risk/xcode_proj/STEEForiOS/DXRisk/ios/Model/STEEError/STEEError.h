//
//  STEEError.h
//  STEEForiOS
//
//  Created by xelz on 2017/7/10.
//  Copyright © 2017年 echonn. All rights reserved.
//

#import <Foundation/NSError.h>

@class NSString;

NSString * const STEEErrorDomain = @"com.dingxiang.stee";

NS_ENUM(NSInteger)
{
    STEESignAlreadyExistsError = -1000,
    STEENullPointerError = -1001
};
