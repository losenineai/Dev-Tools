//
//  MultiAppTester.m
//  DXRiskTestDemo
//
//  Created by weaiken on 2019/9/3.
//  Copyright © 2019 weaiken. All rights reserved.
//

#import "MultiAppTester.h"


#define APPID1     @"28babb1ef43fe9968d9fb88687f9d8e6"
#define APPID2     @"dxdxdxtest2017keyc3e83b6940835"
#define APPID3     @"0fd68c9f8f253db0b163b0aa345da747"

@interface MultiAppTester()<BaseTester>

@end

@implementation MultiAppTester

- (void)start {
    dispatch_queue_t stee_queue = dispatch_queue_create("com.dingxiang.stee", DISPATCH_QUEUE_CONCURRENT);
    dispatch_async(stee_queue, ^{
        NSMutableDictionary *dic = [[NSMutableDictionary alloc]init];
        dic[DXRiskManagerKeyUserId] = @"123456";
        dic[@"PRIVATE_CLEAR_TOKEN"] = @"clear";
        
        [DXRisk setup];
        
        NSString *constID1 = [DXRisk getToken:APPID1 extendsParams:dic];
        NSString *constID2 = [DXRisk getToken:APPID2 extendsParams:dic];
        NSString *constID3 = [DXRisk getToken:APPID3 extendsParams:dic];
        printf("__FILE__值:%s\n" , __FILE__);
        NSLog(@"----请求结果----\n%@", constID1);
        NSLog(@"----请求结果----\n%@", constID2);
        NSLog(@"----请求结果----\n%@", constID3);
    });
}

@end
