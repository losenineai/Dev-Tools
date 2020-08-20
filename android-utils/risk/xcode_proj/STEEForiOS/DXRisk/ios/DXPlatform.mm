//
//  platform.cpp
//  STEEForiOS
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "DXPlatform.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "STEECommonFunctions.h"
#import "STEEAppScanManager.h"
#import "DXRisk.h"
#include "stee.h"
#import "DXHttpRequest.h"
#import <SystemConfiguration/CaptiveNetwork.h>
#import <SystemConfiguration/SCNetworkConnection.h>
#import "DXReachability.h"
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import <sys/utsname.h>


#if defined(ENABLE_RISKMGR)
@interface NSObject(NSURLResponse)
- (NSDictionary *)allHeaderFields;
@end

STEE
string sync_http_request(const int type, const string &url, const string &data, const bool isSaas, const string &contentType )
{
    DXHttpRequest *request = [[DXHttpRequest alloc] init];
    [request setRequestType:DXHttpRequestTypePOST];
    [request setUrl:[NSURL URLWithString:CPPString2NSString(url)]];
    NSMutableDictionary *headerDictionary = [@{@"Content-Type":@"application/octet-stream",@"Accept":@"*/*",@"User-Agent":@"STEE-SDK"} mutableCopy];
    [request setRequestHeaders:headerDictionary];
    NSData *postData = [NSData dataWithBytes:data.c_str() length:data.length()];
    [request setPostData:[postData mutableCopy]];
    [request setTimeOutSeconds:10];
    [request startSynchronous];
    
    if (request.error) {
        LOGE("error : %s",NSString2CString([request.error localizedDescription]));
    }
    
    return NSData2CPPString(request.responseData);
}

STEE
void async_http_request(const int type, const string &url, const string &data, const bool isSaas , const string &contentType){
    DXHttpRequest *request = [[DXHttpRequest alloc] init];
    [request setRequestType:DXHttpRequestTypePOST];
    [request setUrl:[NSURL URLWithString:CPPString2NSString(url)]];
    NSMutableDictionary *headerDictionary = [@{@"Content-Type":@"application/octet-stream",@"Accept":@"*/*",@"User-Agent":@"STEE-SDK"} mutableCopy];
    [request setRequestHeaders:headerDictionary];
    NSData *postData = [NSData dataWithBytes:data.c_str() length:data.length()];
    [request setPostData:[postData mutableCopy]];
    [request setTimeOutSeconds:10];
    [request startAsynchronous];
}

STEE
void send_degrade_info( const string &url, const string &appid){

     @autoreleasepool {
         DXReachability *internetReachability = [DXReachability reachabilityForInternetConnection];
         [internetReachability startNotifier];
         NetworkStatus status = [internetReachability currentReachabilityStatus];
         [internetReachability stopNotifier];

         NSArray *typeStrings2G = @[CTRadioAccessTechnologyEdge,
                                    CTRadioAccessTechnologyGPRS,
                                    CTRadioAccessTechnologyCDMA1x];

         NSArray *typeStrings3G = @[CTRadioAccessTechnologyHSDPA,
                                    CTRadioAccessTechnologyWCDMA,
                                    CTRadioAccessTechnologyHSUPA,
                                    CTRadioAccessTechnologyCDMAEVDORev0,
                                    CTRadioAccessTechnologyCDMAEVDORevA,
                                    CTRadioAccessTechnologyCDMAEVDORevB,
                                    CTRadioAccessTechnologyeHRPD];

         NSArray *typeStrings4G = @[CTRadioAccessTechnologyLTE];

         NSString *networkType = @"offline";
         switch (status)
         {
             case ReachableViaWWAN:
             {
                 CTTelephonyNetworkInfo *info = [CTTelephonyNetworkInfo new];
                 NSString *radioAccessTechnology = info.currentRadioAccessTechnology;

                 if ([typeStrings4G containsObject:radioAccessTechnology])
                 {
                     networkType = @"4G";
                 }
                 else if ([typeStrings3G containsObject:radioAccessTechnology])
                 {
                     networkType = @"3G";
                 }
                 else if ([typeStrings2G containsObject:radioAccessTechnology])
                 {
                     networkType = @"2G";
                 }
                 else
                 {
                     //通信行业向5G演进过程中，可能会出现LTE-Advanced，apple更新了currentRadioAccessTechnology，用户又没有更新SDK版本的话
                     networkType = @"4G";
                 }
                 break;
             }
             case ReachableViaWiFi:
             {
                 networkType = @"Wifi";
                 break;
             }
             default:
                 break;
         }

         struct utsname systemInfo;
         uname(&systemInfo);

         CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
         CTCarrier *carrier = [netinfo subscriberCellularProvider];
         NSString *carrierName = [carrier carrierName];


         NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:CPPString2NSString(url)]];
         [request setHTTPMethod:@"POST"];


         NSMutableDictionary *deviceInfo = [NSMutableDictionary dictionary];
         [deviceInfo setValue:@"iOS" forKey:@"tp"];
         [deviceInfo setValue:CPPString2NSString(appid) forKey:@"appId"];
         [deviceInfo setValue:[NSBundle mainBundle].bundleIdentifier forKey:@"pn"];
         [deviceInfo setValue:CString2NSString(DXRISK_SDK_VERSION) forKey:@"sv"];
         [deviceInfo setValue:networkType forKey:@"nt"];
         [deviceInfo setValue:[[UIDevice currentDevice] systemVersion] forKey:@"ov"];
         [deviceInfo setValue:[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] forKey:@"dt"];
         [deviceInfo setValue:carrierName forKey:@"opt"];
         NSData *postData = [NSJSONSerialization dataWithJSONObject:deviceInfo options:NSJSONWritingPrettyPrinted error:nil];

         [request setHTTPBody:postData];
         [request setValue:[NSString stringWithFormat:@"%lu",(unsigned long)postData.length] forHTTPHeaderField:@"Content-Length"];
         [request setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
         NSURLSession *session = [NSURLSession sharedSession];
         NSURLSessionTask *task = [session dataTaskWithRequest:request];
         [task resume];

     }
}

#endif

#ifdef ENABLE_RISKMGR

extern int dx_risk_init_ios_impl(DXRiskInter &);
extern int dx_risk_check_ios_impl(DXEnvCheck &);
extern int dx_risk_init(DXRiskInter &);

int dx_risk_check_init(DXEnvCheck &sp)
{
    dx_risk_check_ios_impl(sp);
    return 0;
}

int dx_risk_init(DXRiskInter &sp)
{
    dx_risk_init_ios_impl(sp);
    return 0;
}

#endif

//extern int check_point_entry_ios_impl(DXCheckCompt &sp, uint32_t  flags, function<void(void *)> &callback);
//
//int check_point_entry(DXCheckCompt &sp, uint32_t  flags, function<void(void *)> &callback)
//{
//    return check_point_entry_ios_impl(sp, flags, callback);
//}





string get_app_version()
{
    NSDictionary *info = [[NSBundle mainBundle] infoDictionary];
    NSString *appVersion = [info objectForKey:@"CFBundleShortVersionString"];
    
    return NSString2CPPString(appVersion);
}

int get_app_version_code()
{
    return 0;
}

string get_os_version()
{
    NSString *sysVersion = [[UIDevice currentDevice] systemVersion];
    
    return NSString2CPPString(sysVersion);
}

STEE
string get_pkg_name()
{
    NSBundle *bundle = [NSBundle mainBundle];
    NSDictionary *info = [bundle infoDictionary];
    NSString *pkgName1 = [info objectForKey:@"CFBundleIdentifier"];
    NSString *pkgName2 = [bundle bundleIdentifier];
    info = [NSDictionary dictionaryWithContentsOfFile:[bundle pathForResource:@"Info" ofType:@"plist"]];
    NSString *pkgName3 = [info objectForKey:@"CFBundleIdentifier"];
    
    if([pkgName1 isEqualToString:pkgName2] && [pkgName1 isEqualToString: pkgName3])
        return NSString2CPPString(pkgName1);
    else
        return NSString2CPPString(@"");
}

STEERequestHeader_OSArch get_os_arch() {
#if defined(__i386__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__X86;
#elif defined(__x86_64__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__X86_64;
#elif defined(__arm__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__ARM;
#elif defined(__aarch64__)
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSARCH__ARM64;
#else
# error Unsupported architecture
#endif
}

STEERequestHeader_OSType get_os_type() {
    return COM__DINGXIANG__MOBILE__STEEREQUEST_HEADER__OSTYPE__iOS;
}

#ifdef ENABLE_RISKMGR
vector<string> get_all_app_pkg_name()
{
    vector<string> pkgNames;
    NSArray<NSString *> *pkgNameList = [[STEEAppScanManager sharedAppScanManager] getAllAppsPackageName];
    for (NSString *pkgName in pkgNameList) {
        pkgNames.push_back(NSString2CPPString(pkgName));
    }
    return pkgNames;
}

string get_installed_apk_path()
{
    return "";
}
#endif

string get_res_by_name(const string &name)
{
    NSString *resPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:CPPString2NSString(name)];
    if (FileUtils::Exists(NSString2CPPString(resPath))) {
        return FileUtils::ReadAll(NSString2CPPString(resPath));
    }

    return string("");
}

//这段先注释掉，后续分拆DXSafe、DXRisk
//STEE
//bool check_license_validity(const License &lc){
//    
//    if(lc.has_ios() && (lc.ios().flags() & iOSSpec_Flags_PKGS_CHECK) != 0){
//        
//    string app_pkg = get_pkg_name();
//        
//        for(auto &pkg : lc.ios().pkgs()){
//            if(pkg == app_pkg){
//                return true;
//            }
//        }
//        
//        err_log("pkg name is invalid for current license");
//        return false;
//        
//    }
//        
//    return true;
//}

string get_dx_risk_sdk_version(){
    return DXRISK_SDK_VERSION;
}

#ifdef ENABLE_RISKMGR
string get_property(const string &appId, const string &key)
{
    @autoreleasepool {
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
        string xxtea_key = CryptUtils::SHA1(get_pkg_name());
        NSString *value = [defaults valueForKey:CPPString2NSString(string(appId + ":" + key))];
        string base64_encrypted_property =  (value == nil ? string("") : string([value cStringUsingEncoding:NSISOLatin1StringEncoding]));
        string encrypted_property = CryptUtils::Base64Decode(base64_encrypted_property);
        string property = CryptUtils::XXTeaDecrypt(xxtea_key, encrypted_property);
        return property;
    }
}

void set_property(const string &appId, const string &key, const string &value)
{
    @autoreleasepool {
        NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
        string xxtea_key = CryptUtils::SHA1(get_pkg_name());
        string encrypted_property = CryptUtils::XXTeaEncrypt(xxtea_key, value);
        string base64_encrypted_property = CryptUtils::Base64Encode(encrypted_property);
        [defaults setValue:[NSString stringWithCString:base64_encrypted_property.c_str() encoding:NSISOLatin1StringEncoding] forKey:CPPString2NSString(string(appId + ":" + key))];
    }
}

bool isCheckDangerEvn(map<string , string> &infoResultMap){
     map<string,string>::iterator mit = infoResultMap.begin();
    for( mit = infoResultMap.begin();mit!=infoResultMap.end();mit++)
    {
        if (mit->second == string("true")) {
            return true;
        }
    }
    return false;
}

string get_app_list()
{
    return string("");
}

#endif

