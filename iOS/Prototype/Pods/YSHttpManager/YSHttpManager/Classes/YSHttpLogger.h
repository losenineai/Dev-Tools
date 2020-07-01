//
//  YSHttpLog.h
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/12.
//

#import <Foundation/Foundation.h>

@class YSHttpRequest;

@interface YSHttpLogger : NSObject
/**
输出签名
*/
+ (void)logSignInfoWithString:(NSString *)sign;


/**
 请求参数
 */
+ (void)logDebugInfoWithRequest:(YSHttpRequest *)request;


/**
 响应数据输出
 */
+ (void)logDebugInfoWithTask:(NSURLSessionTask *)sessionTask data:(NSData *)data error:(NSError *)error;

@end

