//
//  YSHttpResponse.h
//  shujumoheSDK
//
//  Created by Yshao_Mac on 2019/2/11.
//  Copyright © 2019 数据魔盒. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "YSHttpConstant.h"

/**
 网络响应类
 */
@interface YSHttpResponse : NSObject
@property (nullable, nonatomic, copy, readonly) NSData *rawData;
/**
 请求状态
 */
@property (nonatomic, assign, readonly) YSHttpResponseStatus status;

@property (nullable, nonatomic, copy, readonly) id content;
@property (nonatomic, assign, readonly) NSInteger statueCode;
@property (nonatomic, assign, readonly) NSInteger requestId;
@property (nonnull, nonatomic, copy, readonly) NSURLRequest *request;

- (nonnull instancetype)initWithRequestId:(nonnull NSNumber *)requestId
                                  request:(nonnull NSURLRequest *)request
                             responseData:(nullable NSData *)responseData;

- (nonnull instancetype)initWithRequestId:(nonnull NSNumber *)requestId
                                  request:(nonnull NSURLRequest *)request
                             responseData:(nullable NSData *)responseData
                                    error:(nullable NSError *)error;

@end
