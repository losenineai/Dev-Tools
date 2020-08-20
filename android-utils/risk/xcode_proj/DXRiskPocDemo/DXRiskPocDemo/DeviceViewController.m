//
//  DeviceViewController.m
//  STEEForiOSApp
//
//  Created by weaiken on 2018/11/20.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#import "DeviceViewController.h"
#import <DXRisk/DXRiskManager.h>
#import "MD5Utils.h"
#import "UnityUtil.h"


//#define DEFAULT_URL @"http://10.10.192.16:7775"
//#define DEFAULT_APPID @"1c654a23d1b38450d59a6ca62aa86a52"
//#define DEFAULT_APPSEC @"83f92122d54e873fa9dbbde49b3b2f5f"

#define DEFAULT_URL @"https://constid.dingxiang-inc.com"
#define DEFAULT_APPID @"5cfb563ec02ea30cf53f026ef76ce397"
#define DEFAULT_APPSEC @"46d4ba9009d2477b12573d586f226005"

@interface DeviceViewController ()

@end

@implementation DeviceViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _tokenSwitch.on = NO;
    _urlTextView.text = @"http://";
    
    _appIdTextView.text = @"64c9679d763408aea29b80a5d63ecfe5";
    _appSecTextView.text = @"3413247204074578c40a59bf8379c1e1";
    _urlTextView.text = @"http://10.21.16.168:7775";
}


- (IBAction)clickGetToken:(id)sender {

    _showTextView.text = @"";

    NSString *appid = [_appIdTextView.text isEqualToString:@""] ? DEFAULT_APPID : _appIdTextView.text;
    NSString *appSec = [_appSecTextView.text isEqualToString:@""] ? DEFAULT_APPSEC : _appSecTextView.text;
    NSString *url = ([_urlTextView.text isEqualToString:@""] || [_urlTextView.text isEqualToString:@"http://"]) ? DEFAULT_URL:_urlTextView.text;
    BOOL isCacheToken = _tokenSwitch.on;


    NSString *mTokenUrl = [NSString stringWithFormat:@"%@%@", url, @"/udid/m1"];
    NSString *mEngineUrl = [NSString stringWithFormat:@"%@%@", url, @"/udid/api/getDeviceInfo"];

    NSMutableDictionary *dic = [NSMutableDictionary new];
    dic[DXRiskManagerKeyUserId] = @"123456";
    dic[DXRiskManagerKeyURL] = mTokenUrl;
    if (!isCacheToken) {
         dic[@"PRIVATE_CLEAR_TOKEN"] = @"clear";
    }

    NSString *requestInfo = [NSString stringWithFormat:@"%@%@" , @"----请求信息----" , @"\r\n" ];
    requestInfo = [requestInfo stringByAppendingFormat:@"%@%@%@" ,@"appId:" , appid , @"\r\n"];
    requestInfo = [requestInfo stringByAppendingFormat:@"%@%@%@" ,@"appSec:" , appSec , @"\r\n"];
    requestInfo = [requestInfo stringByAppendingFormat:@"%@%@%@" ,@"risk url:" , mTokenUrl , @"\r\n"];
    requestInfo = [requestInfo stringByAppendingFormat:@"%@%@%@" ,@"info url:" , mEngineUrl , @"\r\n\n"];

    _showTextView.text = requestInfo;

    double nowtime = [self currentTime];

    BOOL isSuccess = [DXRiskManager setup];
    NSString *constID = [DXRiskManager getToken:appid extendsParams:dic];
    NSString *response = [NSString stringWithFormat:@"%@%@" , @"----请求结果----" , @"\r\n" ];
    response = [response stringByAppendingFormat:@"%@%@%@" ,@"token:" , constID , @"\r\n"];

    NSString *reponseDevice = [self getDeviceInfo:constID andAppId:appid andAppSecret:appSec andRequestUrl:mEngineUrl];

//    NSDictionary *dict = [self dictionaryWithJsonString:reponseDevice];
//    reponseDevice = [self jsonStringWithDict:dict];

    response = [response stringByAppendingFormat:@"%@%@"  , reponseDevice , @"\r\n\n"];

    response = [response stringByAppendingFormat:@"%@%@"   , @"----性能消耗信息----" ,@"\r\n"];

    NSString *memuse = [NSString stringWithFormat:@"%.1fKB\r",[UnityUtil memoryUsage]];
    NSString *cpuuse = [NSString stringWithFormat:@"%.2f%%\r",[UnityUtil cpuUsage]];
    NSString *memshow = [NSString stringWithFormat:@"%@%@\n",@"内存使用:",memuse];
    NSString *cpushow = [NSString stringWithFormat:@"%@%@\n",@"CPU使用:",cpuuse];
    NSString *showtime = [NSString stringWithFormat:@"%@%.0f ms\n",@"采集耗时:",[self currentTime]-nowtime];

     NSString *showText = [NSString stringWithFormat:@"%@%@%@",memshow,cpushow,showtime];
    response = [response stringByAppendingFormat:@"%@"   , showText];

    _showTextView.text = [NSString stringWithFormat:@"%@%@" , requestInfo , response ];;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{

    [self.view endEditing:YES];
}

- (NSString *)jsonStringWithDict:(NSDictionary *)dict {
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error];
    NSString *jsonString;
    if (!jsonData) {
        NSLog(@"%@",error);
    }else{
        jsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    return jsonString;
}


- (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString
{
    if (jsonString == nil) {
        return nil;
    }

    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err)
    {
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    return dic;
}


//获取当前时间戳
-(double)currentTime{
    NSDate* date = [NSDate dateWithTimeIntervalSinceNow:0];//获取当前时间0秒后的时间
    NSTimeInterval time=[date timeIntervalSince1970]*1000;// *1000 是精确到毫秒，不乘就是精确到秒
    return time;
}


- (NSString*) getDeviceInfo:(NSString*) token andAppId:(NSString*) AppID andAppSecret:(NSString*) AppSecret andRequestUrl:(NSString*) requestUrl {
    //    NSString* url = requestUrl + @"?appId=" + AppID + @"&token=" + token + @"&sign=" ;
    NSString* mdStr = [MD5Utils getmd5WithString:[NSString stringWithFormat:@"%@%@%@", AppSecret, token , AppSecret]];
    NSString* url = [requestUrl stringByAppendingString:@"?appId="];
    url = [url stringByAppendingString:AppID];
    url = [url stringByAppendingString:@"&token="];
    url = [url stringByAppendingString: [NSString stringWithString:[token stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]];
    url = [url stringByAppendingString:@"&sign="];
    url = [url stringByAppendingString:mdStr];

    return [self httpGet:url];
}


- (NSString*) httpGet:(NSString*) url{
    NSMutableDictionary *header = @{@"Content-Type":@"charset=utf-8",@"Accept":@"*/*",@"User-Agent":@"STEE-SDK" , @"connection":@"keep-Alive"} ;

    // NSURL *url = [NSURL URLWithString:CPPString2NSString(url)];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]];
    // 设置请你去方法
    //        NSString *requestData = [CPPString2NSString(data) stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    //        [request setHTTPBody:[requestData dataUsingEncoding:NSUTF8StringEncoding]];
    //        [request setHTTPBody:[CPPString2NSString(data) dataUsingEncoding:NSUTF8StringEncoding]];
    request.HTTPMethod = @"get";
    // 设置请求体
    //        [request setValue:@"application/x-www-form-urlencoded;charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    [request setValue:@"*/*" forHTTPHeaderField:@"Accept"];
    [request setValue:@"STEE-SDK" forHTTPHeaderField:@"User-Agent"];
    [request setValue:@"keep-Alive" forHTTPHeaderField:@"connection"];
    //        [request setValue:@"testtest----test" forUndefinedKey:@"ccbParam"];
    NSLog(@"request--url-----%@" , url);
    NSError *error = nil;
    NSURLResponse *response = nil;
    //发送同步请求(sendSynchronousRequest)
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];

    if (!error) {
        //打印的服务端返回的信息以及错误信息
        NSLog(@"%@",[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding]);
        NSLog(@"request-reponse--%@",error);
        return [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];

    }else {
        NSLog(@"request-reponse--%@",error);
        return [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
    }
}



@end
