//
//  NormalSaasTester.m
//  DXRiskTestDemo
//
//  Created by weaiken on 2019/9/3.
//  Copyright © 2019 weaiken. All rights reserved.
//

#import "NormalSaasTester.h"
#import "BaseTester.h"

#define DEFAULT_URL @"https://constid.dingxiang-inc.com"
#define DEFAULT_APPID @"5cfb563ec02ea30cf53f026ef76ce397"
#define DEFAULT_APPSEC @"46d4ba9009d2477b12573d586f226005"

@interface NormalSaasTester()<BaseTester>


@end

@implementation NormalSaasTester

- (void)start {
    dispatch_queue_t stee_queue = dispatch_queue_create("com.dingxiang.stee", DISPATCH_QUEUE_CONCURRENT);
    dispatch_async(stee_queue, ^{
        NSMutableDictionary *dic = [[NSMutableDictionary alloc]init];
        dic[DXRiskManagerKeyUserId] = @"123456";
        dic[@"PRIVATE_CLEAR_TOKEN"] = @"clear";
        
        BOOL isSuccess = [DXRisk setup];
        NSString *constID = [DXRisk getToken:DEFAULT_APPID extendsParams:dic];
        printf("__FILE__值:%s\n" , __FILE__);
        NSLog(@"----请求结果----\n%@" , constID);
    });
}

@end
