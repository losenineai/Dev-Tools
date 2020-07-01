//
//  YSHttpResponse.m
//  shujumoheSDK
//
//  Created by Yshao_Mac on 2019/2/11.
//  Copyright © 2019 数据魔盒. All rights reserved.
//

#import "YSHttpResponse.h"

@interface YSHttpResponse()

@property (nonatomic, copy, readwrite) NSData *rawData;
@property (nonatomic, assign, readwrite) YSHttpResponseStatus status;
@property (nonatomic, copy, readwrite) id content;
@property (nonatomic, assign, readwrite) NSInteger statueCode;
@property (nonatomic, assign, readwrite) NSInteger requestId;
@property (nonatomic, copy, readwrite) NSURLRequest *request;

@end

@implementation YSHttpResponse
- (instancetype)initWithRequestId:(NSNumber *)requestId
                          request:(NSURLRequest *)request
                     responseData:(NSData *)responseData
                            error:(NSError *)error {
    self = [super init];
    if (self)
    {
        self.requestId = [requestId unsignedIntegerValue];
        self.request = request;
        self.rawData = responseData;
        [self inspectionResponse:error];
    }
    return self;
}

- (instancetype)initWithRequestId:(NSNumber *)requestId
                          request:(NSURLRequest *)request
                     responseData:(NSData *)responseData {
    self = [super init];
    if (self)
    {
        self.requestId = requestId.unsignedIntegerValue;
        self.request = request;
        self.rawData = responseData;
        [self inspectionResponse:nil];
    }
    return self;
}

#pragma mark - Private methods
- (void)inspectionResponse:(NSError *)error {
    if (error) {
        self.status = YSHttpResponseStatusError;
        self.content = @{@"result_message":@"网络连接失败"};
        self.statueCode = error.code;
    } else {
        if (self.rawData.length > 0) {
            NSDictionary *dic = [self jsonWithData:self.rawData];
            self.status = YSHttpResponseStatusSuccess;
            self.content = dic;

//            BOOL result = [dic[@"result_code"] integerValue] == 0;
//            if (result) {
//                self.status = YSHttpResponseStatusSuccess;
//                self.content = [self processCotnentValue:dic];
//                NSString *code = dic[@"result_code"];
//                if (code && [code isKindOfClass:[NSString class]]) {
//                    self.statueCode = ((NSString*)code).integerValue;
//                }
//            } else {
//                self.status = YSHttpResponseStatusError;
//                self.content = [self processCotnentValue:dic];
//                NSString *code = dic[@"result_code"];
//                if (code && [code isKindOfClass:[NSString class]]) {
//                    self.statueCode = ((NSString*)code).integerValue;
//
//                }
//            }
        } else {
            self.status = YSHttpResponseStatusError;
            self.statueCode = 13;
            self.content =  @{@"result_message":@"返回数据为空"};
        }
    }
}

/**
 临时 返回数据处理
 */
- (id)processCotnentValue:(id)content {
    if ([content isKindOfClass:[NSDictionary class]]) {
        NSMutableDictionary *contentDict = ((NSDictionary *)content).mutableCopy;
//        [contentDict removeObjectForKey:@"result"];
//        [contentDict removeObjectForKey:@"result_msg"];

        if ([contentDict[@"data"] isKindOfClass:[NSNull class]]) {
            [contentDict removeObjectForKey:@"data"];
        }

        return contentDict.copy;
    }
    return content;
}


- (id)jsonWithData:(NSData *)data {
    return [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:NULL];
}

@end
