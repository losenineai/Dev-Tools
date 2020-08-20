//
//  STEEApplicationProxy.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "STEEApplicationProxy.h"

@implementation STEEApplicationProxy

- (NSDictionary *)dictionary
{
    @autoreleasepool {
        return @{@"pkg":self.bundleId.length > 0 ? self.bundleId : @"",
                 @"appName":self.appName.length > 0 ? self.appName : @"",
                 @"version":self.version> 0 ? self.version : @"",
                 @"buidNumber":self.buidNumber> 0 ? self.buidNumber : @"",
                 @"itemName":self.itemName> 0 ? self.itemName : @"",
                 @"teamID":self.teamID> 0 ? self.teamID : @"",
                 @"itemID":[NSString stringWithFormat:@"%@", self.itemID]};
    }
}

@end
