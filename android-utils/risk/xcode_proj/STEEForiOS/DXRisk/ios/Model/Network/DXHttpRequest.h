//
//  DXHttpRequest.h
//  STEEForiOS
//
//  Created by ccc on 11/12/2017.
//  Copyright © 2017 dingxiang-inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {
    DXHttpRequestTypeNONE,
    DXHttpRequestTypePOST,
    DXHttpRequestTypeGET,
} DXHttpRequestType;


@interface DXHttpRequest : NSOperation

@property (nonatomic, strong) NSMutableData *responseData;

@property (nonatomic, strong) NSMutableDictionary *requestHeaders;

@property (nonatomic, strong) NSDictionary *responseHeaders;

@property (nonatomic, strong) NSMutableData *postData;

@property (nonatomic, strong) NSURL *url;

@property (nonatomic, assign) DXHttpRequestType requestType;

@property (nonatomic, assign) double timeOutSeconds;

@property (nonatomic, strong) NSError *error;

- (void)setRequestType:(DXHttpRequestType)type;

- (void)setUrl:(NSURL *)url;

- (void)startSynchronous;

- (void)startAsynchronous;

@end
