//
//  YSHttpManager.m
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//

#import "YSHttpManager.h"
#import "YSHttpConfigure.h"
#import "YSHttpRequest.h"
#import "YSHttpResponse.h"
#import "YSHttpLogger.h"

@implementation YSHttpManager
+ (nonnull instancetype)shareManager {
    static dispatch_once_t onceToken;
    static YSHttpManager *manager = nil;
    dispatch_once(&onceToken, ^{
        manager = [[self alloc] init];
    });
    return manager;
}
- (instancetype)init{
    self = [super init];
    if (self)
    {
        _requestInterceptorObjectArray = [NSMutableArray arrayWithCapacity:3];
        _responseInterceptorObjectArray = [NSMutableArray arrayWithCapacity:3];
        _reqeustDictionary = [[NSMutableDictionary alloc] init];
    }
    return self;
}
- (NSURLSession *)sessionManager{
    if (_sessionManager == nil){
        NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
        configuration.HTTPMaximumConnectionsPerHost = 4;
        /// 防止charls之类的工具抓包  只对release 环境生效
//        configuration.connectionProxyDictionary = @{};

#if DEBUG

#else
        /// 防止charls之类的工具抓包  只对release 环境生效
        configuration.connectionProxyDictionary = @{};
#endif
        _sessionManager = [NSURLSession sessionWithConfiguration:configuration];
    }
    return _sessionManager;
}
- (NSString *)sendRequest:(YSHttpRequest *)request complete:(YSHttpResponseBlock)result{
    //拦截器处理
    if (![self needRequestInterceptor:request]) {
        if ([YSHttpConfigure shareInstance].isDebug) {
            NSLog(@"该请求已经取消");
            [YSHttpLogger logDebugInfoWithRequest:request];
        }
        return nil;
    }

    return [self requestWithRequest:request.generateRequest  complete:result];
}
- (NSString *_Nullable)sendRequestWithConfigBlock:(nonnull YSHRequestConfigBlock)requestBlock complete:(nonnull YSHttpResponseBlock) result{
    YSHttpRequest *request = [YSHttpRequest new];
    requestBlock(request);
    // 拦截器处理
    if (![self needRequestInterceptor:request])
    {
        if ([YSHttpConfigure shareInstance].isDebug)
        {
            NSLog(@"该请求已经取消");
            [YSHttpLogger logDebugInfoWithRequest:request];
        }
        return nil;
    }
    return [self requestWithRequest:request.generateRequest complete:result];
}


/**
 取消一个网络请求

 @param requestID 请求id
 */
- (void)cancelRequestWithRequestID:(nonnull NSString *)requestID{
    NSURLSessionDataTask *requestOperation = self.reqeustDictionary[requestID];
    [requestOperation cancel];
    [self.reqeustDictionary removeObjectForKey:requestID];
}


/**
 取消很多网络请求

 @param requestIDList @[请求id,请求id]
 */
- (void)cancelRequestWithRequestIDList:(nonnull NSArray<NSString *> *)requestIDList{
    for (NSString *requestId in requestIDList){
        [self cancelRequestWithRequestID:requestId];
    }
}
#pragma - private

/**
 发起请求

 @param request NSURLRequest
 @param complete 回调
 @return requestId
 */
- (NSString *)requestWithRequest:(NSURLRequest *)request complete:(YSHttpResponseBlock)complete{
    // 网络检查
    if (![YSHttpConfigure shareInstance].isAvailable)
    {
        NSError *err = [NSError errorWithDomain:@"" code:-1 userInfo:nil];
        YSHttpResponse *rsp = [[YSHttpResponse alloc] initWithRequestId:@(0) request:request responseData:nil error:err];
        for (id obj in self.responseInterceptorObjectArray)
        {
            if ([obj respondsToSelector:@selector(validatorResponse:)])
            {
                [obj validatorResponse:rsp];
                break;
            }
        }
        complete ? complete(rsp) : nil;
        return nil;
    }
    __block NSURLSessionDataTask *task = nil;
    task = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
       
        [self.reqeustDictionary removeObjectForKey:@([task taskIdentifier])];
        dispatch_async(dispatch_get_main_queue(), ^{
            [self requestFinishedWithBlock:complete task:task data:data error:error];
        });

    }];

    NSString *requestId = [[NSString alloc] initWithFormat:@"%lu", (unsigned long)[task taskIdentifier]];
    self.reqeustDictionary[requestId] = task;
    [task resume];
    return requestId;
}

- (void)requestFinishedWithBlock:(YSHttpResponseBlock)blk task:(NSURLSessionTask *)task data:(NSData *)data error:(NSError *)error
{
    if ([YSHttpConfigure shareInstance].isDebug){
        //打印返回参数
       [YSHttpLogger logDebugInfoWithTask:task data:data error:error];
    }

    if (error){
        YSHttpResponse *rsp = [[YSHttpResponse alloc] initWithRequestId:@([task taskIdentifier]) request:task.originalRequest responseData:data error:error];
        for (id obj in self.responseInterceptorObjectArray)
        {
            if ([obj respondsToSelector:@selector(validatorResponse:)])
            {
                [obj validatorResponse:rsp];
                break;
            }
        }
        blk ? blk(rsp) : nil;
    } else{
        YSHttpResponse *rsp = [[YSHttpResponse alloc] initWithRequestId:@(task.taskIdentifier) request:task.originalRequest responseData:data];
        for (id obj in self.responseInterceptorObjectArray)
        {
            if ([obj respondsToSelector:@selector(validatorResponse:)])
            {
                [obj validatorResponse:rsp];
                break;
            }
        }
        blk ? blk(rsp) : nil;
    }
}

- (BOOL)needRequestInterceptor:(YSHttpRequest *)request
{
    BOOL need = YES;
    for (id obj in self.requestInterceptorObjectArray){
        if ([obj respondsToSelector:@selector(needRequestWithRequest:)]){
            need = [obj needRequestWithRequest:request];
            if (need)
            {
                break;
            }
        }
    }
    return need;
}
@end
