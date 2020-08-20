//
//  DXICMP.h
//  iOS MAC addr
//
//  Created by TUTU on 2017/2/28.
//  Copyright © 2017年 TUTU. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DXICMP : NSObject

+ (void)sendICMPEchoRequestTo: (nonnull NSString *)ipAddress;

@end
