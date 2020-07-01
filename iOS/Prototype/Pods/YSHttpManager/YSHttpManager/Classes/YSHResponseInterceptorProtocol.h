//
//  YSHResponseInterceptorProtocol.h
//  Pods
//
//  Created by Yshao_Mac on 2019/2/12.
//

#import <Foundation/Foundation.h>

@class YSHttpResponse;
/**
 网络响应返回前的拦截协议
 */
@protocol YSHResponseInterceptorProtocol <NSObject>
- (void)validatorResponse:(YSHttpResponse *)rsp;
@end
