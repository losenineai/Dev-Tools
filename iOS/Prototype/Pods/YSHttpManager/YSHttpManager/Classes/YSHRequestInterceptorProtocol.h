//
//  YSHRequestInterceptorProtocol.h
//  Pods
//
//  Created by Yshao_Mac on 2019/2/12.
//

#import <Foundation/Foundation.h>
@class YSHttpRequest;
/**
 请求前的拦截协议
 */
@protocol YSHRequestInterceptorProtocol <NSObject>

- (BOOL)needRequestWithRequest:(YSHttpRequest *)request;
- (NSData *)cacheDataFromRequest:(YSHttpRequest *)request;
@end
