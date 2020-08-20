//
//  MultiThreadTester.m
//  DXRiskTestDemo
//
//  Created by weaiken on 2019/9/4.
//  Copyright © 2019 weaiken. All rights reserved.
//

#import "MultiThreadTester.h"

#define APPID     @"5cfb563ec02ea30cf53f026ef76ce397"

@interface MultiThreadTester()<BaseTester>
@end


@implementation MultiThreadTester
- (void)start {
    dispatch_queue_t stee_queue = dispatch_queue_create("com.dingxiang.stee", DISPATCH_QUEUE_CONCURRENT);
    printf("__FILE__值:%s\n" , __FILE__);
    for (int i = 0 ; i < 100;  i++) {
        dispatch_async(stee_queue, ^{
            NSMutableDictionary *dic = [[NSMutableDictionary alloc]init];
            dic[DXRiskManagerKeyUserId] = @"123456";
            dic[@"PRIVATE_CLEAR_TOKEN"] = @"clear";
            [DXRisk setup];
            NSString *constID1 = [DXRisk getToken:APPID extendsParams:dic];
            NSLog(@"----请求结果----\n%@", constID1);
        });
     }
}
@end
