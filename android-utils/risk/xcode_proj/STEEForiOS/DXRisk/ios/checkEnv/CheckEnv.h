//
//  CheckEnv.h
//  STEEForiOS
//
//  Created by weaiken on 2019/10/8.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface CheckEnv : NSObject

+ (BOOL) checkInject;
+ (BOOL) checkDebug;
+ (BOOL) checkJailbreak;

@end

NS_ASSUME_NONNULL_END
