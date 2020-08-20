//
//  MD5Utils.m
//  STEEForiOS
//
//  Created by weaiken on 2019/11/11.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import "MD5Utils.h"
#import "utils.h"
@implementation MD5Utils

+ (NSString*) getMD5:(NSString*) input{
    string cppstring = CryptUtils::MD5((input == nil ? [@"" cStringUsingEncoding:NSUTF8StringEncoding] : [input cStringUsingEncoding:NSUTF8StringEncoding]));
    return  [NSString stringWithCString:cppstring.data() encoding:NSUTF8StringEncoding];
}

@end
