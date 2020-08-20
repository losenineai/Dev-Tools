//
//  STEEApplicationProxy.h
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface STEEApplicationProxy : NSObject
@property(nonatomic,strong) NSString *bundleId;
@property(nonatomic,strong) NSString *appName;
@property(nonatomic,assign) BOOL     is_sys;//是否为系统应用
@property(nonatomic,strong) NSString *version;
@property(nonatomic,strong) NSString *buidNumber;
@property(nonatomic,strong) NSString *itemName;
@property(nonatomic,strong) NSString *teamID;
@property(nonatomic,strong) NSNumber *itemID;
- (NSDictionary *)dictionary;
@end
