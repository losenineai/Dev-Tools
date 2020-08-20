//
//  PrivateEvnTester.m
//  DXRiskTestDemo
//
//  Created by weaiken on 2019/9/4.
//  Copyright © 2019 weaiken. All rights reserved.
//

#import "PrivateEvnTester.h"

#define APPID     @"36a77a038d9358b886a16ed21523405c"
#define APP_URL     @"https://security.gome.com.cn/udid/m1"

@interface PrivateEvnTester()<BaseTester>

@end
@implementation PrivateEvnTester

- (void)start {
    dispatch_queue_t stee_queue = dispatch_queue_create("com.dingxiang.stee", DISPATCH_QUEUE_CONCURRENT);
    dispatch_async(stee_queue, ^{
        NSMutableDictionary *dic = [[NSMutableDictionary alloc]init];
        dic[DXRiskManagerKeyUserId] = @"123456";
        dic[DXRiskManagerKeyURL] = APP_URL;
        dic[@"PRIVATE_CLEAR_TOKEN"] = @"clear";
        
        [DXRisk setup];
        NSString *constID1 = [DXRisk getToken:APPID extendsParams:dic];
        printf("__FILE__值:%s\n" , __FILE__);
        NSLog(@"----请求结果----\n%@", constID1);
    });
}
@end
