//
//  STEEAppScanManager.m
//  STEEForiOS
//
//  Created by echonn on 02/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "STEEAppScanManager.h"
#include <objc/runtime.h>
#import "STEEApplicationProxy.h"

#ifdef IOS_PRIVATE_API
@interface NSObject(LSApplicationWorkspace)
+ (id)defaultWorkspace;
- (id)allInstalledApplications;
- (id)installedApplications;
@end
#endif

@implementation STEEAppScanManager

+ (instancetype)sharedAppScanManager
{
    static STEEAppScanManager *_sharedAppScanManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (_sharedAppScanManager == nil) {
            _sharedAppScanManager = [[STEEAppScanManager alloc] init];
        }
    });
    
    return _sharedAppScanManager;
}

- (NSArray *)scanAllApps
{
    NSArray *appList = nil;
#ifdef IOS_PRIVATE_API
    Class LSApplicationWorkspace_class = objc_getClass("LSApplicationWorkspace");
    if (LSApplicationWorkspace_class != nil && [LSApplicationWorkspace_class respondsToSelector:@selector(defaultWorkspace)]) {
        id LSApplicationDefaultWorkspace = [LSApplicationWorkspace_class defaultWorkspace];
        if ([LSApplicationDefaultWorkspace respondsToSelector:@selector(allInstalledApplications)]) {
            appList = [LSApplicationDefaultWorkspace allInstalledApplications];
        }
        else if ([LSApplicationDefaultWorkspace respondsToSelector:@selector(installedApplications)]) {
            appList = [LSApplicationDefaultWorkspace installedApplications];
        }
    }
#endif
    return appList;
}

- (NSArray<STEEApplicationProxy *> *)getAllAppsInfo
{
    NSMutableArray<STEEApplicationProxy *> *appInfos = @[].mutableCopy;
#ifdef IOS_PRIVATE_API
    NSArray *appList = [self scanAllApps];
    if (appList != nil) {
        for (id proxy in appList) {
            STEEApplicationProxy *proxyObject = [[STEEApplicationProxy alloc] init];
            proxyObject.bundleId = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(applicationIdentifier)];
            proxyObject.version = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(shortVersionString)];
            proxyObject.buidNumber = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(bundleVersion)];
            proxyObject.itemName = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(itemName)];
            proxyObject.teamID = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(teamID)];
            proxyObject.itemID = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(itemID)];
            proxyObject.appName = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(localizedName)];
            NSString *appType = [self _getProxyValueFromObjectClass:proxy performSelector:@selector(applicationType)];
            
            proxyObject.is_sys = [appType isEqualToString:@"System"];
            
            [appInfos addObject:proxyObject];
        }
    }
#endif
    return appInfos;
}

- (NSArray<NSString *> *)getAllAppsPackageName
{
    NSMutableArray *packageList = @[].mutableCopy;
#ifdef IOS_PRIVATE_API
    NSArray *appList = [self scanAllApps];
    if (appList != nil) {
        for (id proxy in appList) {
            NSString *bundleID =  [self _getProxyValueFromObjectClass:proxy performSelector:@selector(applicationIdentifier)];
            
            if (bundleID.length > 0) {
                [packageList addObject:bundleID];
            }
        }
    }
#endif
    return packageList;
}

- (id)_getProxyValueFromObjectClass:(id)objectClass performSelector:(SEL)performSelector
{
    if ([objectClass respondsToSelector:performSelector]) {
        return [objectClass performSelector:performSelector];
    }
    return nil;
}

@end
