//
//  YSHttpManager.h
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//

#import <Foundation/Foundation.h>
#import "YSHttpConstant.h"

@class YSHttpResponse,YSHttpRequest;


/**
 网络请求类
 */
@interface YSHttpManager : NSObject

@property (nonatomic, strong) NSMutableDictionary * _Nullable reqeustDictionary;

@property (nonatomic, strong) NSURLSession * _Nullable sessionManager;

@property (nonatomic, strong) NSMutableArray * _Nullable requestInterceptorObjectArray;

@property (nonatomic, strong) NSMutableArray * _Nullable responseInterceptorObjectArray;

+ (nonnull instancetype)shareManager;

/**
 直接进行请求，不进行参数及 url 的包装

 @param request 请求实体类
 @param result 响应结果
 @return 该请求对应的唯一 task id
 */
- (NSString *)sendRequest:(nonnull YSHttpRequest *)request complete:(nonnull YSHttpResponseBlock) result;


/**
 发送网络请求，紧凑型

 @param requestBlock 请求配置 Block
 @param result 请求结果 Block
 @return 该请求对应的唯一 task id
 */
- (NSString *)sendRequestWithConfigBlock:(nonnull YSHRequestConfigBlock )requestBlock complete:(nonnull YSHttpResponseBlock) result;

/**
 根据请求 ID 取消该任务

 @param requestID 任务请求 ID
 */
- (void)cancelRequestWithRequestID:(nonnull NSString *)requestID;


/**
 根据请求 ID 列表 取消任务

 @param requestIDList 任务请求 ID 列表
 */
- (void)cancelRequestWithRequestIDList:(nonnull NSArray<NSString *> *)requestIDList;

@end
