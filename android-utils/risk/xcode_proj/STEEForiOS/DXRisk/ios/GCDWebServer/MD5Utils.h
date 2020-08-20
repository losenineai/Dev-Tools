//
//  MD5Utils.h
//  STEEForiOS
//
//  Created by weaiken on 2019/11/11.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MD5Utils : NSObject

+ (NSString*) getMD5:(NSString*) input;
@end

NS_ASSUME_NONNULL_END
