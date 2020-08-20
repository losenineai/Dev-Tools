//
//  KeychainHelper.m
//  STEEForiOS
//
//  Created by boyliang on 2017/7/5.
//  Copyright © 2017年 echonn. All rights reserved.
//

#include "DXLog.h"
#import "DXKeychainHelper.h"
#import "STEECommonFunctions.h"

static NSString * const kRHKeyChainKey = @"com.dingxiang.mobile.keychain.ids";

@implementation DXKeyChainUtils

+ (NSMutableDictionary *)getKeychainQuery:(NSString *)service {
    return [NSMutableDictionary dictionaryWithObjectsAndKeys:
            (id)kSecClassGenericPassword,(id)kSecClass,
            service, (id)kSecAttrService,
            service, (id)kSecAttrAccount,
            (id)kSecAttrAccessibleAlways,(id)kSecAttrAccessible,
            nil];
}

+ (NSMutableDictionary *)loadKeychainDict {
    NSMutableDictionary *dict = nil;
    
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:kRHKeyChainKey];
    [keychainQuery setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];
    [keychainQuery setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
    CFDataRef keyData = NULL;
    
    if (SecItemCopyMatching((CFDictionaryRef)keychainQuery, (CFTypeRef *)&keyData) != noErr){
        dict = [[NSMutableDictionary alloc] init];
        
        keychainQuery = [self getKeychainQuery:kRHKeyChainKey];
        [keychainQuery setObject:[NSKeyedArchiver archivedDataWithRootObject:dict] forKey:(id)kSecValueData];
        SecItemAdd((CFDictionaryRef)keychainQuery, NULL);
    }else{
        dict = [NSKeyedUnarchiver unarchiveObjectWithData:(__bridge NSData *)keyData];
    }
    
    if(keyData != nil){
        CFRelease(keyData);
    }
    
    return dict;
}

+ (void)saveKeychainDict:(id)dict {
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:kRHKeyChainKey];
    SecItemDelete((CFDictionaryRef)keychainQuery);
    
    [keychainQuery setObject:[NSKeyedArchiver archivedDataWithRootObject:dict] forKey:(id)kSecValueData];
    SecItemAdd((CFDictionaryRef)keychainQuery, NULL);
}

+ (void)set:(id)data forkey:(NSString *)key{
    NSMutableDictionary *dict = [self loadKeychainDict];
    [dict setObject:data forKey:key];
    [self saveKeychainDict:dict];
}

+ (NSString *)get:(NSString *)key default:(NSString *)def{
    NSMutableDictionary *dict = [self loadKeychainDict];
    NSString *res = [dict valueForKey:key];
    return res != nil ? res : def;
}

//+ (void)delete:(NSString *)service {
//    NSMutableDictionary *keychainQuery = [self getKeychainQuery:service];
//    SecItemDelete((CFDictionaryRef)keychainQuery);
//}
@end
