//
//  YSHttpRequest.m
//  Pods-YSHttpManager_Example
//
//  Created by Yshao_Mac on 2019/2/11.
//

#import "YSHttpRequest.h"
#import "YSHttpConfigure.h"

@implementation YSHttpRequest
- (instancetype)init {
    self = [super init];
    if (self) {
        _requestMethod = YSHttpRequestTypePost;
        _reqeustTimeoutInterval = 30.0;
        _encryptParams = @{};
        _normalParams = @{};
        _requestHeader = @{};
        _retryCount = 0;
        _apiVersion = @"1.0";

    }
    return self;
}

/**
 生成请求

 @return NSURLRequest
 */
- (NSURLRequest *)generateRequest{
    __block NSString *urlStr = [self.baseURL stringByAppendingString:self.requestURL];

//    /// 是否是测试环境
//    if (YSHttpConfigure.shareInstance.enableTest)
//    {
//        [request setValue:@"crawltest"forHTTPHeaderField:@"PARAMS-PARTNER-CODE"];
//        [request setValue:@"crawltest"forHTTPHeaderField:@"PARAMS-EVENT-ID"];
//    }

    /// GET = 0 | DELETE = 1
    if (self.URLParams.count > 0) {
        [self.URLParams enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            NSString *string;
            /// 包含 ？
            if ([urlStr containsString:@"?"]) {
                string = [NSString stringWithFormat:@"&%@=%@",key,obj];
            }else {
                string = [NSString stringWithFormat:@"?%@=%@",key,obj];
            }
            urlStr = [urlStr stringByAppendingString:string];
        }];
    }

    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:self.reqeustTimeoutInterval];
    request.HTTPMethod = [self requestMethodName];
    /// 非GET DELETE BODY  请求体
    if (self.requestMethod >= 2) {
        id param = [self generateRequestBody];
        if ([param isKindOfClass:NSDictionary.class]) {
            NSData *data = [NSJSONSerialization dataWithJSONObject:param options:NSJSONWritingPrettyPrinted error:nil];
            request.HTTPBody = data;
        }
        if ([param isKindOfClass:NSString.class]) {
            NSData *data = [param dataUsingEncoding:kCFStringEncodingUTF8];
            request.HTTPBody = data;
        }
    }

    // 请求头
    request.allHTTPHeaderFields = self.requestHeader;
    NSMutableDictionary *header = request.allHTTPHeaderFields.mutableCopy;
    if (!header) {
        header = [[NSMutableDictionary alloc] init];
    }
    [header addEntriesFromDictionary:[YSHttpConfigure shareInstance].generalHeaders];
    request.allHTTPHeaderFields = header;

    return request.copy;
}

/**
 公共请求参数

 @return 请求参数字典
 */
- (id)generateRequestBody{
    /// 先获取 特定的 params 非 nil
    if ([self.normalParams isKindOfClass:NSDictionary.class]) {
        NSMutableDictionary *commonDic = ((NSDictionary *)self.normalParams).mutableCopy;
    //    NSMutableDictionary *encryptDict = @{}.mutableCopy;
    //    NSAssert(self.requestPath.length > 0, @"请求 Path 不能为空");
    //    encryptDict[@"uri"] = self.requestPath;
    //    [encryptDict addEntriesFromDictionary:commonDic];
    //    [encryptDict addEntriesFromDictionary:self.encryptParams];
    //
        /// 将通用的 param 加上去
        [commonDic addEntriesFromDictionary:[YSHttpConfigure shareInstance].generalParameters];
//#warning 这里要看后台怎么设置
    //    rslt[@"params2"] = [[encryptDict toJsonString] base64EncodedString];


    //    NSLog(@"%@", encryptDict);
        return commonDic.copy;
    }else {
        return self.normalParams;
    }
}
- (NSString *)requestMethodName {
    if (!_requestMethodName) {
        YSHttpRequestType type = self.requestMethod;
        switch (type) {
            case YSHttpRequestTypePost:
                _requestMethodName = @"POST";
                break;
            case YSHttpRequestTypeGet:
                _requestMethodName = @"GET";
                break;
            case YSHttpRequestTypePut:
                _requestMethodName = @"PUT";
                break;
            case YSHttpRequestTypeDelete:
                _requestMethodName = @"DELETE";
                break;
            case YSHttpRequestTypePatch:
                _requestMethodName = @"PATCH";
                break;
            default:
                break;
        }
    }
    return _requestMethodName;
}
//- (NSString *)httpMethod {
//    YSHttpRequestType type = self.requestMethod;
//    switch (type) {
//        case YSHttpRequestTypePost:
//            return @"POST";
//        case YSHttpRequestTypeGet:
//            return @"GET";
//        case YSHttpRequestTypePut:
//            return @"PUT";
//        case YSHttpRequestTypeDelete:
//            return @"DELETE";
//        case YSHttpRequestTypePatch:
//            return @"PATCH";
//        default:
//            break;
//    }
//    return @"GET";
//}

- (void)setValue:(nullable NSString *)value forHTTPHeaderField:(NSString *)field {
    
}

- (NSString *)requestURL {
    if (!_requestURL) {
        _requestURL = @"";
    }
    return _requestURL;
}

- (NSString *)baseURL{
    if (!_baseURL) {
        _baseURL = YSHttpConfigure.shareInstance.generalServer;
    }
    return _baseURL;
}
- (NSDictionary *)URLParams {
    if (!_URLParams) {
        _URLParams = YSHttpConfigure.shareInstance.generalURL;
    }
    return _URLParams;
}
- (void)dealloc {
    if ([YSHttpConfigure shareInstance].isDebug) {
        NSLog(@"dealloc: %@", ([self class]));
    }
}
@end
