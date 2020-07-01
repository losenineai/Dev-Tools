//
//  YSHttpConfigure.h
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//
/**
 参数配置类
 */

#import <Foundation/Foundation.h>

@interface YSHttpConfigure : NSObject

/// 公共参数
@property (nonatomic, strong, nullable) NSDictionary<NSString *, id> *generalParameters;


/// 公共请求头
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *generalHeaders;
/// 公共请求路径
@property (nonatomic, strong, nullable) NSDictionary<NSString *, NSString *> *generalURL;

/// 服务器地址 默认：
@property (nonatomic, copy, readwrite, nonnull) NSString *generalServer;

/// 是否为调试模式（默认 false, 当为 true 时，会输出 网络请求日志）
@property (nonatomic, readwrite) BOOL isDebug;
/// 是否为测试模式
@property (nonatomic, readwrite) BOOL isTest;
/// 网络是否可达
@property (nonatomic, readwrite) BOOL isAvailable;

+ (_Nonnull instancetype)shareInstance;

/**
 添加公共请求参数
 */
+ (void)addGeneralParameter:(NSString * _Nonnull)sKey value:(id _Nonnull )sValue;

/**
 移除请求参数
 */
+ (void)removeGeneralParameter:(NSString * _Nonnull)sKey;
@end

