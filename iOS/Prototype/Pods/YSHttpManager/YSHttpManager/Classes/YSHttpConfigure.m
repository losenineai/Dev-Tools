//
//  YSHttpConfigure.m
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//

#import "YSHttpConfigure.h"

@implementation YSHttpConfigure
+ (instancetype)shareInstance{
    static dispatch_once_t onceToken;
    static YSHttpConfigure *manager = nil;
    dispatch_once(&onceToken, ^{
        manager = [[[self class] alloc] init];
    });
    return manager;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _isDebug = false;
        _isTest = false;
        _isAvailable = true;
    }
    return self;
}

#pragma mark - interface
/**
 添加公共请求参数
 */
+ (void)addGeneralParameter:(NSString *)sKey value:(id)sValue {
    YSHttpConfigure *manager = [YSHttpConfigure shareInstance];
    NSMutableDictionary *mDict = [[NSMutableDictionary alloc] init];
    mDict[sKey] = sValue;
    [mDict addEntriesFromDictionary:manager.generalParameters];
    manager.generalParameters = mDict.copy;
}

/**
 移除请求参数
 */
+ (void)removeGeneralParameter:(NSString *)sKey {
    YSHttpConfigure *manager = [YSHttpConfigure shareInstance];
    NSMutableDictionary *mDict = manager.generalParameters.mutableCopy;
    [mDict removeObjectForKey:sKey];
    manager.generalParameters = mDict.copy;
}
@end
