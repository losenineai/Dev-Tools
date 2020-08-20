//
//  KeychainHelper.h
//  STEEForiOS
//
//  Created by boyliang on 2017/7/5.
//  Copyright © 2017年 echonn. All rights reserved.
//

#ifndef KeychainHelper_h
#define KeychainHelper_h

#import <Foundation/Foundation.h>
#import <Security/Security.h>

@interface DXKeyChainUtils: NSObject

+ (void)set:(id)data forkey:(NSString *)key;

+ (id)get:(NSString *)key default:(NSString *)val;

@end


#endif /* KeychainHelper_h */
