//
//  NSDictionary+STEE_Convert.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "NSDictionary+STEE_Convert.h"
#include "DXLog.h"
#include "STEECommonFunctions.h"

@implementation NSDictionary (STEE_Convert)

- (NSString *)stee_convertToJsonString
{
    NSString *jsonString = nil;
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:self
                                                       options:NSJSONWritingPrettyPrinted
                                                         error:&error];
    if (! jsonData) {
        LOGE("Got an error: %s", NSString2CString([error localizedDescription]));
    } else {
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    return jsonString;
}

@end
