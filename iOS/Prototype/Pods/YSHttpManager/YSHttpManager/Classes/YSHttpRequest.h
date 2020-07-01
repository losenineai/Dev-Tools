//
//  YSHttpRequest.h
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//

#import <Foundation/Foundation.h>
#import "YSHttpConstant.h"

/**
 网络请求参数数据类
 */
@interface YSHttpRequest : NSObject


/**
 请求 Base URL，优先级高于 YSHttpConfigure.generalServer;
 */
@property (nonatomic, copy) NSString *baseURL;

/**
 请求路径 eg: /login2
 */
@property (nonatomic, copy) NSString *requestURL;

/**
 请求路径2 eg: ?code=value&key=value,优先级高于 [YSHttpConfigure generalURL]
 */
@property (nonatomic, strong)NSDictionary *URLParams;
/**
 请求头，默认为空 @{}
 */
@property (nonatomic, strong) NSDictionary *requestHeader;

/**
 请求参数，加密参数 默认为空 @{}
 */
@property (nonatomic, strong) NSDictionary *encryptParams;

/**
 请求参数，不用加密 默认为 @{}
 */
@property (nonatomic, strong) id normalParams;

/**
 请求方式 默认为 YSHttpRequestTypePost
 */
@property (nonatomic, assign) YSHttpRequestType requestMethod;

/**
 请求方式string
 */
@property (nonatomic,copy) NSString *requestMethodName;

/**
 请求超时时间 默认 30s
 */
@property (nonatomic, assign) NSTimeInterval reqeustTimeoutInterval;

/**
 api 版本号，默认 1.0
 */
@property (nonatomic, copy) NSString *apiVersion;

/**
 重试次数，默认为 0
 */
@property (nonatomic, assign) UInt8 retryCount NS_UNAVAILABLE;

/**
 生成请求

 @return NSURLRequest
 */
- (NSURLRequest *)generateRequest;
@end

