//
//  YSHttpConstant.h
//  YSHttpManager
//
//  Created by Yshao_Mac on 2019/2/13.
//

#ifndef YSHttpConstant_h
#define YSHttpConstant_h

#import "YSHResponseInterceptorProtocol.h"
#import "YSHRequestInterceptorProtocol.h"

@class YSHttpResponse,YSHttpRequest;

typedef NS_ENUM (NSUInteger, YSHttpRequestType){
    YSHttpRequestTypeGet = 0,
    YSHttpRequestTypeDelete,
    YSHttpRequestTypePut,
    YSHttpRequestTypePost,
    YSHttpRequestTypePatch
};

typedef NS_ENUM (NSUInteger, YSHttpResponseStatus){
    YSHttpResponseStatusSuccess = 0,
    YSHttpResponseStatusError
};

/// 响应配置 Block
typedef void (^YSHttpResponseBlock)(YSHttpResponse * _Nullable response);

/// 请求配置 Block
typedef void (^YSHRequestConfigBlock)(YSHttpRequest * _Nullable request);

#endif /* YSHttpConstant_h */
