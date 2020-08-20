//
//  DXRiskManager.h
//  STEEForiOS
//
//  Created by echonn on 04/07/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#define DXRISK_SDK_VERSION "5.5.6"

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

@protocol DXRiskManagerExport <JSExport>
//+ (NSString *)getToken;
////JSExportAs(getToken, + (NSString *) getToken);
//JSExportAs(setup,
//+ (BOOL)setup:(NSString *)appId extendsParams:(NSDictionary *)extendsParams
//);

+ (BOOL)setup;
//JSExportAs(getToken, + (NSString *) getToken);
JSExportAs(getToken,
           + (NSString *)getToken:(NSString*) appId extendsParams:(NSDictionary *)extendsParams
);


@end

// 风控组件：DXRiskManager类
@interface DXRiskManager : NSObject<DXRiskManagerExport>

// 字符串常量
extern NSString* const DXRiskManagerKeyUserId;
extern NSString* const DXRiskManagerKeyEmail;
extern NSString* const DXRiskManagerKeyPhone;
extern NSString* const DXRiskManagerKeyUserExtend1;
extern NSString* const DXRiskManagerKeyUserExtend2;
extern NSString* const DXRiskManagerKeyURL;
extern NSString* const DXRiskManagerKeyBackup;
extern NSString* const DXRiskManagerKeyDegradeNotify;
extern NSString* const DXRiskManagerKeyCountry;
extern NSString* const DXRiskManagerKeyBackupAppId;

extern NSString* const DXRiskManagerKeyDelayMsTime;

// NoticeDegrade参数
/* The NoticeDegrade Value. This value only be used pair with key:DXRiskManagerKeyDegradeNotify to notify token degrade. */
extern NSString* const DXRiskManagerKeyDegradeNotifyEnable;
// Backup参数
/* The Backup Value. This value only be used pair with key:DXRiskManagerKeyBackup to set data backup. */
extern NSString* const DXRiskManagerKeyBackupEnable;
// Country参数
/* The Country Value. This value only be used pair with key:DXRiskManagerKeyCountry to set country. */
extern NSString* const DXRiskManagerCountryChina;
/* The Country Value. This value only be used pair with key:DXRiskManagerKeyCountry to set country. */
extern NSString* const DXRiskManagerCountryIndonesia;

/**
 采集端的设备指纹信息，上传至风控后台，再由风控后台返回token。
 该API为耗时操作，因此必须在非主线程上调用。
 @param extendsParams 业务方用户唯一标识，用户名，用户ID，Email等等，常用Key值可参考DXRiskManagerKeyUserId, DXRiskManagerKeyEmail等等
 @param appId appId
 @return token
 */
+ (NSString *)getToken:(NSString*) appId extendsParams:(NSDictionary *)extendsParams;
   
/** 
 DXRiskManager -- 初始化方法
 */
+ (BOOL)setup;


@end
