//
//  NSString+STEE_Convert.m
//  STEEForiOS
//
//  Created by xelz on 2017/7/10.
//  Copyright © 2017年 echonn. All rights reserved.
//

#import "NSString+STEE_Convert.h"

@implementation NSString (STEE_Convert)

- (NSString *) urlEncodedString
{
    return [self stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]];
}

@end
