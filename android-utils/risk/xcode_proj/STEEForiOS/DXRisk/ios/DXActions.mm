//
//  actions.cpp
//  STEEForiOS
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#include <stdio.h>
#include "utils.h"
#include "DXRiskInter.h"
#import "DXPlatform.h"
#import <mach-o/dyld.h>
#import <sys/sysctl.h>
#import <ifaddrs.h>
#import <arpa/inet.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import <resolv.h>
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>
#import <CoreLocation/CoreLocation.h>
#import <AdSupport/AdSupport.h>
#import "DXKeychainHelper.h"
#import "DXRisk.h"
#import <sys/utsname.h>
#import <sys/sysctl.h>
#import <sys/mount.h>
#import <mach-o/arch.h>
#import <mach/mach_host.h>
#import <mach-o/ldsyms.h>
#include <objc/runtime.h>
#include <dlfcn.h>
#import <objc/runtime.h>
#import <objc/message.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import <SystemConfiguration/SCNetworkConnection.h>
#import <sys/socket.h>
#import <sys/utsname.h>
#import <sys/stat.h>
#import "DXReachability.h"
#import "md5.h"


#import "STEECommonFunctions.h"
#import "STEEAppScanManager.h"
#import "STEEApplicationProxy.h"

#import "UIDevice+STEE_Application.h"
#import "UIDevice+STEE_InfoDic.h"
#import "NSDictionary+STEE_Convert.h"
#import "NSArray+STEE_Convert.h"

#import "webview/DXWebViewUtils.h"

//MAC DXAddress
#import "DXICMP.h"
#import "DXARP.h"
#import "DXMDNS.h"
#import "DXAddress.h"

#import "DXGCDWebServer.h"
#import "DXGCDWebServerDataResponse.h"

#import <Metal/Metal.h>
#import <GLKit/GLKit.h>
#import <CheckEnv.h>


#define ACTION_IMPL(name) static void action_get_##name(map<string, string> &infos, void *args)
#define ADD_ACTION(name) sp.AddCollectionAction(#name, action_get_##name, NULL)

//#define ADD_TO_INFO_NSSTR(value) infos[(__FUNCTION__ + strlen("action_get_"))] = NSString2CPPString(value)
//#define ADD_TO_INFO(json) infos[(__FUNCTION__ + strlen("action_get_"))] = JSonUtils::JSon2string(json)
#define ADD_TO_INFO(key,value) if(NSString2CPPString(value) != "" ) infos[NSString2CPPString(key)] = NSString2CPPString(value)

#define KEY_MACADDRESS @"K1"
#define KEY_PROXYTYPE @"K2"
#define KEY_PROXYURL @"K3"
#define KEY_IDFA @"K4"
#define KEY_IDFV @"K5"
#define KEY_DXUUID @"K6"
#define KEY_CPUTYPE @"K7"
#define KEY_CPUSUBTYPE @"K8"
#define KEY_MAXCPUS @"K9"
#define KEY_AVAILCPUS @"K10"
#define KEY_TOTALSPACE @"K11"
#define KEY_FREESPACE @"K12"
#define KEY_MEMORY @"K13"
#define KEY_RESOLUTION @"K14"
#define KEY_NETWORKTYPE @"K15"
#define KEY_RSSI @"K16"
#define KEY_BATTERY @"K17"
#define KEY_OS @"K18"
#define KEY_NETWORKNODE @"K19"
#define KEY_RELEASE @"K20"
#define KEY_SYSVERSION @"K21"
#define KEY_HARDWARETYPE @"K22"
#define KEY_JAILBREAK @"K23" 
#define KEY_ASLR @"K24"
#define KEY_BOOTTIME @"K25"
#define KEY_CELLIP @"K26"
#define KEY_WIFIIP @"K27"
#define KEY_WIFINETMASK @"K28"
#define KEY_VPNIP @"K29"
#define KEY_VPNNETMASK @"K30"
#define KEY_LANGUAGE @"K31"
#define KEY_DEVICENAME @"K32"
#define KEY_CARRIER @"K33"
#define KEY_MNC @"K34"
#define KEY_MCC @"K35"
#define KEY_RADIOACCESSTYPE @"K36"
#define KEY_COUNTRYISO @"K37"
#define KEY_BSSID @"K38"
#define KEY_SSID @"K39"
#define KEY_DNS @"K40"
#define KEY_BRIGHTNESS @"K41"
#define KEY_JBPRINT @"K42"
#define KEY_GPSLOCATION @"K43"
#define KEY_GPSSTATUS @"K44"
#define KEY_TIMEZONE @"K45"
#define KEY_BUNDLE @"K46"
#define KEY_OSVERSION @"K47"
#define KEY_PLATFORM @"K48"
#define KEY_SDKVERSION @"K49"
#define KEY_HOOKINLINE @"K50"
#define KEY_HOOKOBJECTIVEC @"K51"
// 20180804 新增
#define KEY_DEBUG @"K52"
#define KEY_INJECT @"K53"
#define KEY_HOOK @"K54"
// 20190604
#define KEY_MUTIL_APP @"K55"

// 20190221
#define KEY_HWMODEL @"K60"
//个人热点IP
#define KEY_BRIDGEIP @"K61"
#define KEY_BRIDGENETMASK @"K62"
#define KEY_DISPLAY_NAME @"K63"
#define KEY_BUNDLE_NAME @"K64"
#define KEY_SYSTEM_AREA @"K65"
#define KEY_SYSTEM_UPTIME @"K66"

// 20190313
#define KEY_GPUINFO @"K70"
#define KEY_EMULATOR_INFO @"K71"
#define KEY_SYSTEM_STARTUP_TIME @"K72"

// 20190820
#define KEY_APP_VERSION @"K76"
#define KEY_BUILD_VERSION @"K77"

//20190930
#define KEY_APP_CRASH_FLAT @"K78"

//20191022
#define KEY_APP_LIST @"K80"

//20191016
static NSArray* KEY_APP_KOCK_INDEX = @[@"K1001" , @"K1002" ,
                                        @"K1003" , @"K1004" ,
                                        @"K1005" , @"K1006" ,
                                        @"K1007" , @"K1008" ,
                                        @"K1009" , @"K1010" ,];

static IMP ocPtrs[45];
static uintptr_t cPtrs[18];


@interface DXGPSHelper : NSObject

@property (nonatomic, strong)NSString *gpsInfo;
+ (NSThread *)gpsQueryThread;
+ (instancetype)sharedHelper;
- (void)getGPS;

@end

@implementation DXGPSHelper

+ (void)gpsQueryThreadEntryPoint:(id)__unused objec {
    @autoreleasepool {
        [[NSThread currentThread] setName:@"DXGPSHelper"];
        NSRunLoop *runloop = [NSRunLoop currentRunLoop];
        [runloop addPort:[NSMachPort port] forMode:NSDefaultRunLoopMode];
        [runloop run];
    }
}

+ (NSThread *)gpsQueryThread {
    static NSThread *_gpsQueryThread = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _gpsQueryThread = [[NSThread alloc] initWithTarget:self selector:@selector(gpsQueryThreadEntryPoint:) object:nil];
        [_gpsQueryThread start];
    });
    return _gpsQueryThread;
}

+ (instancetype)sharedHelper {
    static DXGPSHelper *_dxhelper = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _dxhelper = [[DXGPSHelper alloc] init];
    });
    return _dxhelper;
}

- (void)getGPS {
    [self performSelector:@selector(gpsQuery) onThread:[[self class] gpsQueryThread] withObject:nil waitUntilDone:YES];
}

- (void)gpsQuery {
    // get gps location and gps status
    CLLocationManager *locationManager = [[CLLocationManager alloc] init];
    locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    CLLocation *location = [locationManager location];
    if(location) {
        NSDictionary * locationDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                       [NSNumber numberWithDouble: (int)(location.coordinate.latitude * 1000000) / 1000000.0], @"lat",
                                       [NSNumber numberWithDouble: (int)(location.coordinate.longitude * 1000000) / 1000000.0], @"long",
                                       [NSNumber numberWithInt: location.horizontalAccuracy], @"accuracy",
                                       [NSNumber numberWithLongLong: [location.timestamp timeIntervalSince1970] * 1000000], @"time",
                                       nil];
        NSData *locationData = [NSJSONSerialization dataWithJSONObject:locationDict options:kNilOptions error:nil];
        self.gpsInfo = [[NSString alloc] initWithData:locationData encoding:NSUTF8StringEncoding];
    }else{
        self.gpsInfo = nil;
    }
}

@end





ACTION_IMPL(mac_from_arp){
    @autoreleasepool {
#if TARGET_IPHONE_SIMULATOR
        return;
#else
        NSString *ip = [DXAddress currentIPAddressOf:@"en0"];
        if (ip.length <= 0) {
            return;
        }

        NSString *mac;
        [DXICMP sendICMPEchoRequestTo:ip];
        mac = [DXARP walkMACAddressOf:ip];
        if (!mac) {
            [DXICMP sendICMPEchoRequestTo:ip];
            mac = [DXARP MACAddressOf:ip];
        }

        if (mac) {
            ADD_TO_INFO(KEY_MACADDRESS, mac);
        }
#endif
    }
}

BOOL checkAPPIsExist(NSString* URLScheme){
    NSURL* url;
    if ([URLScheme containsString:@"://"]) {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@",URLScheme]];
    } else {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@://",URLScheme]];
    }
    if ([[UIApplication sharedApplication] canOpenURL:url]){
        return YES;
    } else {
        return NO;
    }
}


ACTION_IMPL(network_info){
    
    Json network_info;
    
//    map<string, string> header_infos;
//    string respone = http_request(HTTP_GET, "https://ip.taobao.com/service/getIpInfo2.php?ip=myip", STRING_NULL, header_infos);
//    if(respone.size() > 0){
//        network_info["ip_info"] = JSonUtils::string2JSon(respone);
//    }
    
    //得到代理
    NSDictionary *proxySettings = (__bridge_transfer NSDictionary *) CFNetworkCopySystemProxySettings();
    if (proxySettings != nil && ![proxySettings isEqual:[NSNull null]]) {
        NSNumber *proxyHttpEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesHTTPEnable];
        NSNumber *proxyAutoConfigEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesProxyAutoConfigEnable];
        if([proxyAutoConfigEnable boolValue]) {
            ADD_TO_INFO(KEY_PROXYTYPE, @"auto");
            NSString *autoConfigURL = [proxySettings valueForKey:(NSString *)kCFNetworkProxiesProxyAutoConfigURLString];
            ADD_TO_INFO(KEY_PROXYURL, autoConfigURL);
        } else if([proxyHttpEnable boolValue]) {
            ADD_TO_INFO(KEY_PROXYTYPE, @"http");
            NSString *httpProxyIp = [proxySettings valueForKey:(NSString *)kCFNetworkProxiesHTTPProxy];
            NSNumber *httpProxyPort = [proxySettings valueForKey:(NSString *)kCFNetworkProxiesHTTPPort];
            NSNumber *httpProxyAuthenticated = [proxySettings valueForKey:@"HTTPProxyAuthenticated"];
            NSString *proxyUrl;
            if([httpProxyAuthenticated boolValue]) {
                NSString *httpProxyUsername = [proxySettings valueForKey:@"HTTPProxyUsername"];
                proxyUrl = [[NSString alloc] initWithFormat:@"http://%@@%@:%@", httpProxyUsername, httpProxyIp, httpProxyPort];
            } else {
                proxyUrl = [[NSString alloc] initWithFormat:@"http://%@:%@", httpProxyIp, httpProxyPort];
            }
            ADD_TO_INFO(KEY_PROXYURL, proxyUrl);
        } else {
            ADD_TO_INFO(KEY_PROXYTYPE, @"none");
        }
    }else{
        ADD_TO_INFO(KEY_PROXYTYPE, @"none");
    }
}

// 应用信息 : 注意，使用私有API
ACTION_IMPL(apps_info){
    @autoreleasepool {
        NSString *appDisplayName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
        NSString *appBundleName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
        NSString *appVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
        NSString *appBuildVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];

        ADD_TO_INFO(KEY_DISPLAY_NAME, appDisplayName);
        ADD_TO_INFO(KEY_BUNDLE_NAME, appBundleName);
        ADD_TO_INFO(KEY_APP_VERSION, appVersion);
        ADD_TO_INFO(KEY_BUILD_VERSION, appVersion);

        NSArray<STEEApplicationProxy *> *appList = [[STEEAppScanManager sharedAppScanManager] getAllAppsInfo];
        NSMutableArray *sysApps = @[].mutableCopy;
        NSMutableArray *userApps = @[].mutableCopy;

        for (STEEApplicationProxy *proxy in appList) {
            if (proxy.is_sys) {
                [sysApps addObject:[proxy dictionary]];
            }
            else {
                [userApps addObject:[proxy dictionary]];
            }
        }

        if (sysApps.count > 0 || userApps.count > 0) {
            NSDictionary *appInfos = @{@"sys":sysApps, @"usr":userApps};
            NSString *jsonStr = [appInfos stee_convertToJsonString];
    //        ADD_TO_INFO_NSSTR(jsonStr);
    //        NSLog(@"--jsonStr--%@" , jsonStr);
        }
    }
}

ACTION_IMPL(device_ids){
    @autoreleasepool {
        ADD_TO_INFO(KEY_APP_KOCK_INDEX[0], @"true");
#ifdef STEE_IDFA
        typedef NSUUID* (*advertisingIdentifier)(id,SEL);
        advertisingIdentifier func_advertisingIdentifier = (advertisingIdentifier)ocPtrs[0];
        {
#if defined(__arm__)
            uint32_t *method = (uint32_t *)func_advertisingIdentifier;
            if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
            if(*method == 0xf000f8df) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0xdf4f03b5) {
                    func_advertisingIdentifier = (advertisingIdentifier)(ptr+7);
                }
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)func_advertisingIdentifier;
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
            if(*method == 0xd61f020058000050) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0x580000f0580000d1) {
                    func_advertisingIdentifier = (advertisingIdentifier)(ptr+6);
                }
            }
#endif
        }
        NSString *idfa = [func_advertisingIdentifier([ASIdentifierManager sharedManager],@selector(advertisingIdentifier)) UUIDString];
        ADD_TO_INFO(KEY_IDFA, idfa);
#endif

        typedef NSUUID* (*identifierForVendor)(id,SEL);
        identifierForVendor func_identifierForVendor = (identifierForVendor)ocPtrs[24];
        {
#if defined(__arm__)
            uint32_t *method = (uint32_t *)func_identifierForVendor;
            if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
            if(*method == 0xf000f8df) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0xdf4f03b5) {
                    func_identifierForVendor = (identifierForVendor)(ptr+7);
                }
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)func_identifierForVendor;
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
            if(*method == 0xd61f020058000050) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0x580000f0580000d1) {
                    func_identifierForVendor = (identifierForVendor)(ptr+6);
                }
            }
#endif
        }
//          NSLog(@"UUIDString---5");
//        NSString *idfv = [func_identifierForVendor([UIDevice currentDevice],@selector(identifierForVendor)) UUIDString];
        NSString * idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        ADD_TO_INFO(KEY_IDFV, idfv);

        NSString *uuid = [DXKeyChainUtils get:@"DX-UUID-A" default:nil];

        if(uuid == nil){
            CFUUIDRef puuid = CFUUIDCreate( kCFAllocatorDefault );
            CFStringRef uuidString = CFUUIDCreateString( kCFAllocatorDefault, puuid );
            uuid = [(NSString *)CFBridgingRelease(CFStringCreateCopy(NULL, uuidString)) uppercaseString];
            CFRelease(puuid);
            CFRelease(uuidString);
            unsigned char check_sum1 = 0xff;
            int check_sum2 = 0;
            for(int i=0; i<uuid.length; i++) {
                unsigned char c = [uuid characterAtIndex:i];
                if(c == '-') continue;
                check_sum1 ^= c;
                check_sum2 += c;
                if(i>uuid.length - 3) {
                    check_sum2 -= c;
                    check_sum1 ^= c;
                }
            }
            uuid = [NSString stringWithFormat:@"%@%X%X", [uuid substringToIndex:uuid.length - 2], check_sum1 % 16, check_sum2 % 16];
            [DXKeyChainUtils set:uuid forkey:@"DX-UUID-A"];
        }
        ADD_TO_INFO(KEY_DXUUID, uuid);
        ADD_TO_INFO(KEY_APP_KOCK_INDEX[1], @"true");
    }

}

ACTION_IMPL(bt_info){
    NSString *jsonStr = [[[UIDevice currentDevice] stee_bluetoothInfo] stee_convertToJsonString];
//    ADD_TO_INFO_NSSTR(jsonStr);
}

ACTION_IMPL(cpu_info){
    @autoreleasepool {
        host_basic_info_data_t info;
        mach_msg_type_number_t infoCount;

        infoCount = HOST_BASIC_INFO_COUNT;
        host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&info, &infoCount);
        NSString *cpuType = [NSString stringWithFormat:@"%d", info.cpu_type];
        NSString *cpuSubtype = [NSString stringWithFormat:@"%d", info.cpu_subtype];
        NSString *maxCPUs = [NSString stringWithFormat:@"%d", info.max_cpus];
        NSString *availCPUS = [NSString stringWithFormat:@"%d", info.avail_cpus];
        ADD_TO_INFO(KEY_CPUTYPE, cpuType);
        ADD_TO_INFO(KEY_CPUSUBTYPE, cpuSubtype);
        ADD_TO_INFO(KEY_MAXCPUS, maxCPUs);
        ADD_TO_INFO(KEY_AVAILCPUS, availCPUS);
    }

}

ACTION_IMPL(screen_info){
     @autoreleasepool {
         CGSize screenSize = [UIScreen mainScreen].bounds.size;
         CGFloat scale = [UIScreen mainScreen].scale;
         NSString *resolution = [NSString stringWithFormat:@"%.0fX%.0f", screenSize.width * scale, screenSize.height * scale];
         ADD_TO_INFO(KEY_RESOLUTION, resolution);
     }
}

ACTION_IMPL(mem_info){
    @autoreleasepool {
        NSDictionary *fattributes = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
        NSNumber *totalSize = [fattributes objectForKey:NSFileSystemSize];
        NSString *totalSpace;
        if (totalSize) {
            totalSpace = [NSString stringWithFormat:@"%lld",totalSize.longLongValue];
            ADD_TO_INFO(KEY_TOTALSPACE, totalSpace);
        }else{
            struct statfs buf1;
            unsigned long long totalSizeNumber = -1;
            if (statfs("/var", &buf1) >= 0)
            {
                totalSizeNumber = (unsigned long long)(buf1.f_bsize * buf1.f_blocks);
            }
            totalSpace = [NSString stringWithFormat:@"%lld",totalSizeNumber];
            ADD_TO_INFO(KEY_TOTALSPACE, totalSpace);
        }

        NSNumber *freeSize = [fattributes objectForKey:NSFileSystemFreeSize];
        NSString *freeSpace;
        if (freeSize) {
            freeSpace=[NSString stringWithFormat:@"%lld", freeSize.longLongValue];
            ADD_TO_INFO(KEY_FREESPACE, freeSpace);
        }else{
            struct statfs buf;
            unsigned long long freeSizeNumber = -1;
            if (statfs("/var", &buf) >= 0)
            {
                freeSizeNumber = (unsigned long long)(buf.f_bsize * buf.f_bavail);
            }
            NSString *freeSpace = [NSString stringWithFormat:@"%lld",freeSizeNumber];
            ADD_TO_INFO(KEY_FREESPACE, freeSpace);
        }


        NSString *memory =[NSString stringWithFormat:@"%lld",[NSProcessInfo processInfo].physicalMemory];
        if (memory) {
            ADD_TO_INFO(KEY_MEMORY, memory);
        }else{
            host_basic_info_data_t info;
            mach_msg_type_number_t infoCount;

            infoCount = HOST_BASIC_INFO_COUNT;
            host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&info, &infoCount);
            memory = [NSString stringWithFormat:@"%lld", (unsigned long long)info.memory_size];
            ADD_TO_INFO(KEY_MEMORY, memory);
        }
    }

}

ACTION_IMPL(battery_info){
    @autoreleasepool {
        UIDevice *currentDevice = [UIDevice currentDevice];
        currentDevice.batteryMonitoringEnabled = true;
        UIDeviceBatteryState batteryState = [currentDevice batteryState];
        float batteryLevel = [currentDevice batteryLevel];
        NSString *battery_info = [NSString stringWithFormat:@"%@%@%@%@", batteryState == UIDeviceBatteryStateUnknown ? @"unknown" : batteryState == UIDeviceBatteryStateUnplugged ? @"unplugged" : @"charging", @"/", [NSNumber numberWithFloat: batteryLevel > 0 ? batteryLevel * 100 : 0.0], @"%"];
        ADD_TO_INFO(KEY_BATTERY, battery_info);
    }
}

ACTION_IMPL(kernal_info){
     @autoreleasepool {
         struct utsname systemInfo;
         uname(&systemInfo);
         ADD_TO_INFO(KEY_OS, CString2NSString(systemInfo.sysname));
         ADD_TO_INFO(KEY_NETWORKNODE, CString2NSString(systemInfo.nodename));
         ADD_TO_INFO(KEY_RELEASE, CString2NSString(systemInfo.release));
         ADD_TO_INFO(KEY_SYSVERSION, CString2NSString(systemInfo.version));
         ADD_TO_INFO(KEY_HARDWARETYPE, CString2NSString(systemInfo.machine));

         NSString* machine = CString2NSString(systemInfo.machine);
         if ([machine isEqualToString:@"x86_64"] || [machine isEqualToString:@"i386"]) {
              ADD_TO_INFO(KEY_EMULATOR_INFO, @"true");
         }else{
              ADD_TO_INFO(KEY_EMULATOR_INFO, @"false");
         }
     }

}

//TODO 移往线程检测
ACTION_IMPL(detect_jailbreak){
    NSString *jailbreak = ([[UIDevice currentDevice] stee_isJailbreak_1] || [[UIDevice currentDevice] stee_isJailbreak_2] || [[UIDevice currentDevice] stee_isJailbreak_3] || [[UIDevice currentDevice] stee_isJailbreak_4]) ? @"true" : @"false";
    ADD_TO_INFO(KEY_JAILBREAK, jailbreak);
}

ACTION_IMPL(aslr_info){
    @autoreleasepool {
        typedef uint32_t (*dyldimagecount)();
        dyldimagecount func_dyldimagecount = (dyldimagecount)cPtrs[3];
        {
#if defined(__arm__)
            uint32_t *method = (uint32_t *)func_dyldimagecount;
            if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
            if(*method == 0xf000f8df) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0xdf4f03b5) {
                    func_dyldimagecount = (dyldimagecount)(ptr+7);
                }
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)func_dyldimagecount;
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
            if(*method == 0xd61f020058000050) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0x580000f0580000d1) {
                    func_dyldimagecount = (dyldimagecount)(ptr+6);
                }
            }
#endif
        }
        uint32_t imageCount = func_dyldimagecount();
        NSUInteger most_aslr_count = 0;
        intptr_t most_aslr = -1;
        typedef intptr_t (*dyldgetimagevmaddrslide)(uint32_t);
        dyldgetimagevmaddrslide func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)cPtrs[2];
        {
#if defined(__arm__)
            uint32_t *method = (uint32_t *)func_dyldgetimagevmaddrslide;
            if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
            if(*method == 0xf000f8df) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0xdf4f03b5) {
                    func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)(ptr+7);
                }
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)func_dyldgetimagevmaddrslide;
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
            if(*method == 0xd61f020058000050) {
                uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
                if (*ptr == 0x580000f0580000d1) {
                    func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)(ptr+6);
                }
            }
#endif
        }
        for (int i = 0; i < imageCount; i++) {
            intptr_t aslr = func_dyldgetimagevmaddrslide(i);
            if (most_aslr_count == 0) {
                most_aslr = aslr;
                most_aslr_count = 1;
                continue;
            }

            if (most_aslr == aslr) {
                most_aslr_count++;
            }else{
                most_aslr_count--;
            }
        }

        NSString *aslr_info = [NSString stringWithFormat:@"%ld", most_aslr];
        ADD_TO_INFO(KEY_ASLR, aslr_info);
    }

}

ACTION_IMPL(boottime){
    @autoreleasepool {
        struct kinfo_proc info_proc;
        size_t size = sizeof(info_proc);
        int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, 0};
        bzero(&info_proc, sizeof(info_proc));
        int sysctlResult = sysctl(mib, 4, &info_proc, &size, NULL, 0);
        long long bootTime = 0;
        if(sysctlResult != -1) {
            const struct timeval *timeVal = &(info_proc.kp_proc.p_starttime);
            bootTime = timeVal->tv_sec *(double)1000000 + timeVal->tv_usec;
        }
        if (bootTime == 0) {
            struct timeval boottime;
            mib[0] = CTL_KERN;
            mib[1] = KERN_BOOTTIME;
            size = sizeof(boottime);
            sysctlResult = sysctl(mib, 2, &boottime, &size, NULL, 0);
            if(sysctlResult != -1) {
                bootTime = boottime.tv_sec;
            }
        }

        NSString *boottime = [NSString stringWithFormat:@"%lld",bootTime];
        ADD_TO_INFO(KEY_BOOTTIME, boottime);
    }
}
ACTION_IMPL(network_interfaces){
     @autoreleasepool {
    ADD_TO_INFO(KEY_APP_KOCK_INDEX[8], @"true");
    auto ipAddress = [&](struct in_addr in) {
        char *ret = (char *)calloc(1, 18);
        //区别于系统的[inet_ntoa error]，此处用[dnet_ntoa error]标识
        strcpy(ret, "[dnet_ntoa error]");
        (void) inet_ntop(AF_INET, &in, ret, 18);
        NSString *addr = [NSString stringWithUTF8String:ret];
        free(ret);
        return addr;
    };
    
    struct ifaddrs *interfaces = nil;
    struct ifaddrs *temp_addr = nil;
    NSMutableArray* networkNames = [NSMutableArray array];
    int success = 0;
    success = getifaddrs(&interfaces);
    if (success == 0) {
        temp_addr = interfaces;
        while(temp_addr != nil) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *temp_addr_in = (struct sockaddr_in *)temp_addr->ifa_addr;
                NSString *addr =  ipAddress((temp_addr_in)->sin_addr);
                NSString *netmask = ipAddress(((struct sockaddr_in *)temp_addr->ifa_netmask)->sin_addr);
                NSString *interfaceName = [NSString stringWithUTF8String:temp_addr->ifa_name];
                [networkNames addObject:interfaceName];
#ifdef DEBUG
                NSLog(@"interfaceName: %@ addr: %@  netmask:%@", interfaceName, addr , netmask);
#endif
                // cell
                if([interfaceName isEqualToString:@"pdp_ip0"]) {
                    ADD_TO_INFO(KEY_CELLIP, addr);
                }
                // wifi
                else if([interfaceName isEqualToString:@"en0"]) {
                    ADD_TO_INFO(KEY_WIFIIP, addr);
                    ADD_TO_INFO(KEY_WIFINETMASK, netmask);
                }
                // 个人热点
                else if([interfaceName isEqualToString:@"bridge100"]){
                    ADD_TO_INFO(KEY_BRIDGEIP, addr);
                    ADD_TO_INFO(KEY_BRIDGENETMASK , netmask);
                }
                // vpn
                else if([interfaceName rangeOfString:@"tap"].location != NSNotFound || [interfaceName rangeOfString:@"tun"].location != NSNotFound || [interfaceName rangeOfString:@"ppp"].location != NSNotFound || [interfaceName rangeOfString:@"ipsec"].location != NSNotFound){
                        ADD_TO_INFO(KEY_VPNIP, addr);
                        ADD_TO_INFO(KEY_VPNNETMASK, netmask);
                }
            }
            // mac address for wifi(en0) only
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);
    ADD_TO_INFO(KEY_APP_KOCK_INDEX[9], @"true");
     }
}

ACTION_IMPL(languages){
    NSArray *preferredLanguages = [NSLocale preferredLanguages];
    NSString *langs;
    if (preferredLanguages == nil || ![preferredLanguages isKindOfClass:[NSArray class]] || [preferredLanguages count] == 0) {
        langs = @"en";
    }
    else {
        langs =  [[preferredLanguages subarrayWithRange:NSMakeRange(0, MIN(10, [preferredLanguages count]))] componentsJoinedByString:@":"];
    }
    ADD_TO_INFO(KEY_LANGUAGE, langs);
}

ACTION_IMPL(device_name){
    typedef id (*name)(id,SEL);
    name func_name = (name)ocPtrs[26];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_name;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_name = (name)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_name;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_name = (name)(ptr+6);
            }
        }
#endif
    }
    NSString *deviceName = func_name([UIDevice currentDevice],@selector(name));
    ADD_TO_INFO(KEY_DEVICENAME, deviceName);
}

ACTION_IMPL(carrier_info){
    CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = [netinfo subscriberCellularProvider];
    NSString *carrierName = [carrier carrierName];
    NSString *mobileNetworkCode = [carrier mobileNetworkCode];
    NSString *mobileCountryCode = [carrier mobileCountryCode];
    NSString *countryCode = [carrier isoCountryCode];
#ifdef DEBUG
    NSLog(@"%@  %@  %@", carrierName,mobileNetworkCode,mobileCountryCode);
#endif

    ADD_TO_INFO(KEY_CARRIER, carrierName);
    ADD_TO_INFO(KEY_MNC, mobileNetworkCode);
    ADD_TO_INFO(KEY_MCC, mobileCountryCode);
    ADD_TO_INFO(KEY_COUNTRYISO, countryCode);
}

ACTION_IMPL(radio_access_type){
    CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
    NSString *radioType = [netinfo currentRadioAccessTechnology];
    if(radioType != nil && radioType.length != 0) {
        if (radioType.length <= 23){
            ADD_TO_INFO(KEY_RADIOACCESSTYPE, radioType);
        }else{
            ADD_TO_INFO(KEY_RADIOACCESSTYPE, [radioType substringFromIndex:23]);
        }
    }
}

ACTION_IMPL(country_iso){
    NSString *country_iso = [[NSLocale currentLocale] objectForKey: NSLocaleCountryCode];
    ADD_TO_INFO(KEY_COUNTRYISO, country_iso);
    ADD_TO_INFO(KEY_SYSTEM_AREA, [[NSLocale currentLocale] displayNameForKey:NSLocaleIdentifier value:[[NSLocale currentLocale] localeIdentifier]]);
}

ACTION_IMPL(wifi_info){
    typedef CFDictionaryRef (*CNCopyCurrentNetworkInfo)(CFStringRef);
    CNCopyCurrentNetworkInfo func_CNCopyCurrentNetworkInfo = (CNCopyCurrentNetworkInfo)cPtrs[5];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_CNCopyCurrentNetworkInfo;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_CNCopyCurrentNetworkInfo = (CNCopyCurrentNetworkInfo)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_CNCopyCurrentNetworkInfo;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_CNCopyCurrentNetworkInfo = (CNCopyCurrentNetworkInfo)(ptr+6);
            }
        }
#endif
    }
    NSDictionary *SSIDInfo = CFBridgingRelease(func_CNCopyCurrentNetworkInfo((__bridge CFStringRef)@"en0"));
    if (SSIDInfo != NULL) {
        ADD_TO_INFO(KEY_BSSID, [SSIDInfo valueForKey:@"BSSID"]);
        ADD_TO_INFO(KEY_SSID, [SSIDInfo valueForKey:@"SSID"]);
    }
}

ACTION_IMPL(dns_info){
    @autoreleasepool {
        auto ipAddress = [&](struct in_addr in) {
            char *ret = (char *)calloc(1, 18);
            //区别于系统的[inet_ntoa error]，此处用[dnet_ntoa error]标识
            strcpy(ret, "[dnet_ntoa error]");
            (void) inet_ntop(AF_INET, &in, ret, 18);
            NSString *addr = [NSString stringWithUTF8String:ret];
            free(ret);
            return addr;
        };

        NSMutableArray *addresses = [[NSMutableArray alloc] init];
        res_state res = (res_state) calloc(1, sizeof(struct __res_state));
        int result = res_ninit(res);
        if (result == 0)
        {
            for ( int i = 0; i < res->nscount; i++ )
            {
                NSString *s = ipAddress(res->nsaddr_list[i].sin_addr);
                [addresses addObject:s];
            }
        }
        res_ndestroy(res);
        free(res);
        ADD_TO_INFO(KEY_DNS, [addresses componentsJoinedByString:@","]);
    }

}

ACTION_IMPL(brightness_info){
     @autoreleasepool {
    float brightness = [UIScreen mainScreen].brightness;
    if (brightness >= 0.0 && brightness <= 1.0) {
        NSString *brightness_info = [NSString stringWithFormat:@"%f",brightness];
        ADD_TO_INFO(KEY_BRIGHTNESS, brightness_info);
    }
     }
}

ACTION_IMPL(jbprint_info){
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray<NSString *> * files = [fileManager contentsOfDirectoryAtPath:@"/private/var/stash" error:nil];
    if (files == nil) {
        files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:@"/private/var/db/stash" error:nil];
    }
    
    if (files == nil) {
        files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:@"/Developer" error:nil];
    }
    
    if (files != nil && files.count > 0) {
        NSString* filesString = [files componentsJoinedByString:@"^"];
        const char* filesCString = [filesString UTF8String];
        string ff = CryptUtils::MD5(string(filesCString));
        NSMutableString *filesDigest = [NSMutableString stringWithCapacity:ff.length() * 2];
        for(int i = 0; i<ff.length(); i++) {
            [filesDigest appendFormat:@"%02x",ff[i]];
        }
        ADD_TO_INFO(KEY_JBPRINT, filesDigest);
#ifdef DEBUG
        NSLog(@"jbPrint: %@", filesDigest);
#endif
    }
}

ACTION_IMPL(gps_info){
     @autoreleasepool {
    NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString *locationAlwaysUsageDescription = infoDict[@"NSLocationAlwaysUsageDescription"];
    NSString *locationWhenInUseUsageDescription = infoDict[@"NSLocationWhenInUseUsageDescription"];
    if(locationAlwaysUsageDescription || locationWhenInUseUsageDescription) {
        [[DXGPSHelper sharedHelper] getGPS];
        if ([DXGPSHelper sharedHelper].gpsInfo) {
            ADD_TO_INFO(KEY_GPSLOCATION, [DXGPSHelper sharedHelper].gpsInfo);
        }
        NSString *gpsStatus = [NSString stringWithFormat:@"%@",[NSNumber numberWithInt:([CLLocationManager locationServicesEnabled] << 4) | [CLLocationManager authorizationStatus]]];
        ADD_TO_INFO(KEY_GPSSTATUS, gpsStatus);
    }
     }
}

//ACTION_IMPL(location_data_network_info){
//    dispatch_block_t mainThreadBlock = ^(){
//        // get gps location and gps status
//        UIDevice *currentDevice = [UIDevice currentDevice];
//        NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
//        NSString *locationAlwaysUsageDescription = infoDict[@"NSLocationAlwaysUsageDescription"];
//        NSString *locationWhenInUseUsageDescription = infoDict[@"NSLocationWhenInUseUsageDescription"];
//        if([[currentDevice systemVersion] floatValue] < 8.0 ||
//           ([[currentDevice systemVersion] floatValue] >= 8.0
//            && ( locationAlwaysUsageDescription || locationWhenInUseUsageDescription))) { // check if app using gps
//               CLLocationManager *locationManager = [[CLLocationManager alloc] init];
//               locationManager.desiredAccuracy = kCLLocationAccuracyBest;
//               CLLocation *location = [locationManager location];
//#ifdef DEBUG
//               NSLog(@"gpsLocation: %@", location);
//#endif
//               if(location) {
//                   NSDictionary * locationDict = [NSDictionary dictionaryWithObjectsAndKeys:
//                                                  [NSNumber numberWithDouble: (int)(location.coordinate.latitude * 1000000) / 1000000.0], @"lat",
//                                                  [NSNumber numberWithDouble: (int)(location.coordinate.longitude * 1000000) / 1000000.0], @"long",
//                                                  [NSNumber numberWithInt: location.horizontalAccuracy], @"accuracy",
//                                                  [NSNumber numberWithLongLong: [location.timestamp timeIntervalSince1970] * 1000000], @"time",
//                                                  nil];
//                   NSData *locationData = [NSJSONSerialization dataWithJSONObject:locationDict options:kNilOptions error:nil];
//                   ADD_TO_INFO(KEY_GPSLOCATION, [[NSString alloc] initWithData:locationData encoding:NSUTF8StringEncoding]);
//               }
//           }
//        
//        @try{
//            NSArray *subviews = [[[[UIApplication sharedApplication] valueForKey:@"statusBar"] valueForKey:@"foregroundView"] subviews];
//            for (id subview in subviews) {
//                if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
//                    NSArray *networkTypes = [[NSArray alloc] initWithObjects:@"offline", @"2G", @"3G", @"4G", @"LTE", @"Wifi", nil];
//                    NSInteger dataNetwotkType = [[subview valueForKey:@"dataNetworkType"] integerValue];
//                    if(dataNetwotkType >= 0 && dataNetwotkType < [networkTypes count]) {
//                        ADD_TO_INFO(KEY_NETWORKTYPE, [networkTypes objectAtIndex:dataNetwotkType]);
//                    }
//                    NSString *rssi = [subview valueForKey:@"_stringForRSSI"];
//                    ADD_TO_INFO(KEY_RSSI, rssi);
//                    break;
//                }
//            }
//        } @catch (NSException *e) {
//            // [UIView subviews] may cause NSGenericException:
//            // Collection <CALayerArray: > was mutated while being enumerated
//        }
//    };
//    
//    if([NSThread isMainThread]) {
//        mainThreadBlock();
//    } else {
//        dispatch_sync(dispatch_get_main_queue(), mainThreadBlock);
//    }
//    NSString *gpsStatus = [NSString stringWithFormat:@"%@",[NSNumber numberWithInt:([CLLocationManager locationServicesEnabled] << 4) | [CLLocationManager authorizationStatus]]];
//    ADD_TO_INFO(KEY_GPSSTATUS, gpsStatus);
//}

ACTION_IMPL(network_type){
    @autoreleasepool {
     ADD_TO_INFO(KEY_APP_KOCK_INDEX[6], @"true");
    DXReachability *internetReachability;
    if (internetReachability == nil)
    {
        internetReachability = [DXReachability reachabilityForInternetConnection];
        [internetReachability startNotifier];
    }
    NetworkStatus status = [internetReachability currentReachabilityStatus];
    
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
    ADD_TO_INFO(KEY_NETWORKTYPE, networkType);
    ADD_TO_INFO(KEY_APP_KOCK_INDEX[7], @"true");
    }
}

ACTION_IMPL(timezone_info){
    NSTimeZone *timeZoneLocal = [NSTimeZone localTimeZone];
    ADD_TO_INFO(KEY_TIMEZONE, [timeZoneLocal name]);
}

ACTION_IMPL(system_info){
    @autoreleasepool {
        NSString *str =[NSString stringWithFormat:@"%f", (double)[[NSProcessInfo processInfo] systemUptime]];
//        NSLog(@"system_info---%@" , str);
        ADD_TO_INFO(KEY_SYSTEM_UPTIME, str);
        
        size_t v11;
        sysctlbyname("kern.boottime", 0, &v11, 0, 0);
        int *v10 = (int *) calloc(1, v11);
        sysctlbyname("kern.boottime", v10, &v11, 0LL, 0LL);
        int v9 = *v10;
        NSString *startup_time =[NSString stringWithFormat:@"%d", v9];
        ADD_TO_INFO(KEY_SYSTEM_STARTUP_TIME, startup_time);
    }
}

ACTION_IMPL(gpu_info){
    @autoreleasepool {
        EAGLContext *ctx = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:ctx];
        NSString* vensor = [NSString stringWithCString:(const char*)glGetString(GL_VENDOR) encoding:NSASCIIStringEncoding];
        NSString *str =[NSString stringWithFormat:@"%@;%@", vensor , [MTLCreateSystemDefaultDevice() name] ];
        GLenum glError = glGetError();
//                NSLog(@"gpu_info---%@" , str);
        ADD_TO_INFO(KEY_GPUINFO, str);
    }
}


ACTION_IMPL(bundle_info){
 @autoreleasepool {
      ADD_TO_INFO(KEY_APP_KOCK_INDEX[4], @"true");
    typedef CFTypeRef (*bundleGetValueForInfoDicKey)(CFBundleRef, CFStringRef);
    bundleGetValueForInfoDicKey func_bundleGetValueForInfoDicKey = (bundleGetValueForInfoDicKey)cPtrs[16];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_bundleGetValueForInfoDicKey;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_bundleGetValueForInfoDicKey = (bundleGetValueForInfoDicKey)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_bundleGetValueForInfoDicKey;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_bundleGetValueForInfoDicKey = (bundleGetValueForInfoDicKey)(ptr+6);
            }
        }
#endif
    }
    typedef CFBundleRef (*mainBundle)();
    mainBundle func_mainBundle = (mainBundle)cPtrs[17];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_mainBundle;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_mainBundle = (mainBundle)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_mainBundle;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_mainBundle = (mainBundle)(ptr+6);
            }
        }
#endif
    }
    CFBundleRef mb = func_mainBundle();
    if (mb) {
        CFRetain(mb);
        CFStringRef bundleID = (CFStringRef)func_bundleGetValueForInfoDicKey(mb,kCFBundleIdentifierKey);
        CFStringRef bundleShortVersion = (CFStringRef)func_bundleGetValueForInfoDicKey(mb, (__bridge CFStringRef)@"CFBundleShortVersionString");
        NSString *bundle = [NSString stringWithFormat:@"%@_%@", (__bridge NSString *)bundleID, (__bridge NSString *)bundleShortVersion];
        CFRelease(mb);
        ADD_TO_INFO(KEY_BUNDLE, bundle);
    }
    ADD_TO_INFO(KEY_OSVERSION, [[UIDevice currentDevice] systemVersion]);
    // get platform
    struct kinfo_proc info_proc;
    size_t size = sizeof(info_proc);
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char *)calloc(1, size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    free(machine);

    // get HW.Model
    size_t size1 = sizeof(info_proc);
    sysctlbyname("hw.model", NULL, &size1, NULL, 0);
    char *hwmodel = (char *)calloc(1, size1);
    sysctlbyname("hw.model", hwmodel, &size1, NULL, 0);
    NSString *hwmodelStr = [NSString stringWithCString:hwmodel encoding:NSUTF8StringEncoding];
    free(hwmodel);

    ADD_TO_INFO(KEY_PLATFORM, platform);
    ADD_TO_INFO(KEY_HWMODEL, hwmodelStr);
    ADD_TO_INFO(KEY_SDKVERSION, CString2NSString(DXRISK_SDK_VERSION));
      ADD_TO_INFO(KEY_APP_KOCK_INDEX[5], @"true");
 }
}

#define IIMP(class_name, method_name) class_getMethodImplementation(objc_getClass(class_name),sel_registerName(method_name))
#define CIMP(class_name, method_name) class_getMethodImplementation(objc_getMetaClass(class_name),sel_registerName(method_name))
#ifdef __LP64__
#define mach_header mach_header_64
#define segment_command segment_command_64
#define LCSEGMENT LC_SEGMENT_64
#else
#define mach_header mach_header
#define segment_command segment_command
#define LCSEGMENT LC_SEGMENT
#endif

ACTION_IMPL(hook_info){
     @autoreleasepool {
#if TARGET_IPHONE_SIMULATOR
    return;
#endif
    typedef uint32_t (*dyldimagecount)();
    dyldimagecount func_dyldimagecount = (dyldimagecount)cPtrs[3];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_dyldimagecount;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_dyldimagecount = (dyldimagecount)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_dyldimagecount;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_dyldimagecount = (dyldimagecount)(ptr+6);
            }
        }
#endif
    }
    typedef intptr_t (*dyldgetimagevmaddrslide)(uint32_t);
    dyldgetimagevmaddrslide func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)cPtrs[2];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_dyldgetimagevmaddrslide;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_dyldgetimagevmaddrslide;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_dyldgetimagevmaddrslide = (dyldgetimagevmaddrslide)(ptr+6);
            }
        }
#endif
    }
    typedef const char* (*dyldgetimagename)(uint32_t);
    dyldgetimagename func_dyldgetimagename = (dyldgetimagename)cPtrs[1];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_dyldgetimagename;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_dyldgetimagename = (dyldgetimagename)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_dyldgetimagename;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_dyldgetimagename = (dyldgetimagename)(ptr+6);
            }
        }
#endif
    }
    typedef const struct mach_header* (*dyldgetimageheader)(uint32_t);
    dyldgetimageheader func_dyldgetimageheader = (dyldgetimageheader)cPtrs[0];
    {
#if defined(__arm__)
        uint32_t *method = (uint32_t *)func_dyldgetimageheader;
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1);
        if(*method == 0xf000f8df) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0xdf4f03b5) {
                func_dyldgetimageheader = (dyldgetimageheader)(ptr+7);
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)func_dyldgetimageheader;
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1);
        if(*method == 0xd61f020058000050) {
            uintptr_t *ptr = reinterpret_cast<uintptr_t *>(*(method + 1));
            if (*ptr == 0x580000f0580000d1) {
                func_dyldgetimageheader = (dyldgetimageheader)(ptr+6);
            }
        }
#endif
    }

    NSMutableArray <NSNumber *>*customerStart = [NSMutableArray array];
    NSMutableArray <NSNumber *>*customerLength = [NSMutableArray array];
    NSMutableArray *customerDYLIBs = [NSMutableArray array];
    
    //add origin image into customerDYLIBS as occupied not hooked address
    NSString *execBin = [[NSString stringWithCString:func_dyldgetimagename(0) encoding:NSUTF8StringEncoding] lastPathComponent];
    [customerDYLIBs addObject:execBin];

#ifdef DXRISK_DY
    const struct mach_header *mh_exe_header;
    for (int i = 0; i < _dyld_image_count(); i++) {
        if ([[NSString stringWithUTF8String:_dyld_get_image_name(i)].lastPathComponent isEqualToString:execBin]) {
            mh_exe_header = (const struct mach_header *)_dyld_get_image_header(i);
            break;
        }
    }
    
    if (!mh_exe_header) {
        return;
    }
    
    const uint8_t *command = (const uint8_t *)(mh_exe_header + 1);
#else
    const uint8_t *command = (const uint8_t *)(&_mh_execute_header + 1);
#endif
    NSUInteger loadDYLIB = 0;
#ifdef DXRISK_DY
    for (uint32_t idx = 0; idx < mh_exe_header->ncmds; ++idx) {
#else
    for (uint32_t idx = 0; idx < _mh_execute_header.ncmds; ++idx) {
#endif
        if (((const struct load_command *)command)->cmd == LC_LOAD_DYLIB){
            const char *content = (const char*)(command + sizeof(struct dylib_command));
            NSString *dylibPath = [NSString stringWithCString:content encoding:NSUTF8StringEncoding];
            [customerDYLIBs addObject:[dylibPath lastPathComponent]];
            loadDYLIB++;
#ifdef DEBUG
//            NSLog(@"dylib %@", [NSString stringWithCString:content encoding:NSUTF8StringEncoding]);
#endif
        } else {
            //假定加载的动态库在load_command内全为连续，若中断，之后的动态库不做计入，判定为注入库
            if (loadDYLIB > 0) {
                break;
            }
        }
        command += ((const struct load_command *)command)->cmdsize;
    }
    
    uint32_t imageCount = func_dyldimagecount();
    NSUInteger most_aslr_count = 0;
    uintptr_t most_aslr = -1;
    for (int i = 0; i < imageCount; i++) {
        uintptr_t aslr = func_dyldgetimagevmaddrslide(i);
        if (most_aslr_count == 0) {
            most_aslr = aslr;
            most_aslr_count = 1;
            continue;
        }
        
        if (most_aslr == aslr) {
            most_aslr_count++;
        }else{
            most_aslr_count--;
        }
    }
    
    uintptr_t minSharedDynamicLibraryAddress = UINTPTR_MAX;
    for (int i = 0; i< imageCount; i++) {
        if (most_aslr == func_dyldgetimagevmaddrslide(i)) {
            if (minSharedDynamicLibraryAddress > (uintptr_t)func_dyldgetimageheader(i)) {
                minSharedDynamicLibraryAddress = (uintptr_t)func_dyldgetimageheader(i);
            }
            continue;
        }
        
        NSString * dylib = [[NSString stringWithCString:func_dyldgetimagename(i) encoding:NSUTF8StringEncoding] lastPathComponent];
        if ([customerDYLIBs containsObject:dylib]) {
            const struct mach_header* header = (const struct mach_header*)func_dyldgetimageheader(i);
            const uint8_t *command = (const uint8_t *)(header + 1);
            uint64_t vmsize = 0;
            for (uint32_t idx = 0; idx < header->ncmds; ++idx) {
                if (((const struct load_command *)command)->cmd == LCSEGMENT){
                    const struct segment_command *seg_cmd = (const struct segment_command *)command;
                    if (![[NSString stringWithCString:seg_cmd->segname encoding:NSUTF8StringEncoding] isEqualToString:@"__PAGEZERO"] || i != 0) {
                        vmsize += seg_cmd->vmsize;
                    }
                }
                command += ((const struct load_command *)command)->cmdsize;
            }
            
            [customerStart addObject:[NSNumber numberWithUnsignedLongLong:(uintptr_t)func_dyldgetimageheader(i)]];
            [customerLength addObject:[NSNumber numberWithUnsignedLongLong:vmsize]];
        }
    }
    
    for (int i = 1; i < customerStart.count; i++) {
        for (int j = 0; j < customerStart.count - i; j++) {
            if ([customerStart[j] compare:customerStart[j+1]] == NSOrderedDescending) {
                [customerStart exchangeObjectAtIndex:j withObjectAtIndex:j+1];
                [customerLength exchangeObjectAtIndex:j withObjectAtIndex:j+1];
            }
        }
    }
    
    CFMutableBitVectorRef inlineHookedArray = CFBitVectorCreateMutable(NULL, sizeof(uint64_t)*8);
    CFBitVectorSetCount(inlineHookedArray, 64);
    CFBitVectorSetAllBits(inlineHookedArray, 0);
    
    //use tempPtr set value to avoid compiler set fun a const address
    uintptr_t tempPtr;
    uintptr_t cmethods[] = {
        tempPtr = (uintptr_t)_dyld_get_image_header,
        tempPtr = (uintptr_t)_dyld_get_image_name,
        tempPtr = (uintptr_t)_dyld_get_image_vmaddr_slide,
        tempPtr = (uintptr_t)_dyld_image_count,
        tempPtr = (uintptr_t)CFNetworkCopySystemProxySettings,
        tempPtr = (uintptr_t)CNCopyCurrentNetworkInfo,
        tempPtr = (uintptr_t)dladdr,
        tempPtr = (uintptr_t)dlsym,
        tempPtr = (uintptr_t)fopen,
        tempPtr = (uintptr_t)getenv,
        tempPtr = (uintptr_t)getifaddrs,
        tempPtr = (uintptr_t)stat,
        tempPtr = (uintptr_t)sysctl,
        tempPtr = (uintptr_t)sysctlbyname,
        tempPtr = (uintptr_t)uname,
        tempPtr = (uintptr_t)SCNetworkReachabilityGetFlags,
        tempPtr = (uintptr_t)CFBundleGetValueForInfoDictionaryKey,
        tempPtr = (uintptr_t)CFBundleGetMainBundle,
    };
    
    size_t cmethod_count = sizeof(cmethods) / sizeof(uintptr_t);
    for(size_t i=0; i<cmethod_count; i++) {
        uintptr_t ptr = cmethods[i];
        if (ptr == 0) {
            continue;
        }
        
#if defined(__arm__)
        uint32_t *method = (uint32_t *)cmethods[i];
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1); //fix address alignment
        if (method != nil) {
            if(*method == 0xf000f8df || *method == 0xe51ff004) { // inline hook instruction
                CFBitVectorSetBitAtIndex(inlineHookedArray, i, 1);
                continue;
            }
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)cmethods[i];
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1); //fix address alignment
        if (method != nil) {
            if(*method == 0xd61f020058000050) { // inline hook instruction
                CFBitVectorSetBitAtIndex(inlineHookedArray, i, 1);
                continue;
            }
        }
#endif
        
        if (ptr > minSharedDynamicLibraryAddress) {
#ifdef DEBUG
//            NSLog(@"find symbol %zu in shared dynamic library", i);
#endif
            continue;
        }
        
        BOOL inCustomerImage = NO;
        for (int j = 0; j < customerStart.count; j++) {
            if (ptr > customerStart[j].unsignedLongLongValue && ptr < (customerStart[j].unsignedLongLongValue + customerLength[j].unsignedLongLongValue)) {
                inCustomerImage = YES;
                break;
            }
        }
        
        if(!inCustomerImage) {
            CFBitVectorSetBitAtIndex(inlineHookedArray, i, 1);
        }
    }
    
    CFIndex inlineHookedArrayLength = CFBitVectorGetCount(inlineHookedArray);
    u_int64_t inlineHookedNum = 0;
    for (CFIndex i = 0 ; i < inlineHookedArrayLength ; i++) {
        if (CFBitVectorGetBitAtIndex(inlineHookedArray, i)) {
            inlineHookedNum += pow(2, i);
        }
    }
    NSString *inlineHookedString = [NSString stringWithFormat:@"%llx",inlineHookedNum];
    if (inlineHookedNum>0) {
        ADD_TO_INFO(KEY_HOOKINLINE, inlineHookedString);
    }
    CFRelease(inlineHookedArray);

    
    CFMutableBitVectorRef objectiveCHookedArray = CFBitVectorCreateMutable(NULL, sizeof(uint64_t)*8);
    CFBitVectorSetCount(objectiveCHookedArray, 64);
    CFBitVectorSetAllBits(objectiveCHookedArray, 0);

    IMP ocmethods[] = {
        IIMP("ASIdentifierManager", "advertisingIdentifier"),
        IIMP("ASIdentifierManager", "isAdvertisingTrackingEnabled"),
        IIMP("CLLocation", "coordinate"),
        IIMP("CLLocationManager", "location"),
        IIMP("CTCarrier", "carrierName"),
        IIMP("CTCarrier", "mobileCountryCode"),
        IIMP("CTCarrier", "mobileNetworkCode"),
        IIMP("CTTelephonyNetworkInfo", "subscriberCellularProvider"),
        IIMP("NSArray", "containsObject:"),
        IIMP("NSBundle", "bundleIdentifier"),
        CIMP("NSBundle", "mainBundle"),
        IIMP("NSFileManager", "attributesOfFileSystemForPath:error:"),
        IIMP("NSFileManager", "attributesOfItemAtPath:error:"),
        IIMP("NSFileManager", "contentsOfDirectoryAtPath:error:"),
        IIMP("NSFileManager", "fileExistsAtPath:"),
        CIMP("NSLocale", "currentLocale"),
        CIMP("NSLocale", "preferredLanguages"),
        CIMP("NSProcessInfo", "processInfo"),
        IIMP("NSString", "initWithUTF8String:"),
        IIMP("NSString", "rangeOfString:"),
        CIMP("NSTimeZone", "localTimeZone"),
        IIMP("UIDevice", "batteryLevel"),
        IIMP("UIDevice", "batteryState"),
        IIMP("UIDevice", "identifierForVendor"),
        IIMP("UIDevice", "model"),
        IIMP("UIDevice", "name"),
        IIMP("UIDevice", "systemVersion"),
        IIMP("UIScreen", "brightness"),
        IIMP("UIStatusBarDataNetworkItemView", "dataNetworkType"),
        IIMP("CLLocationManager", "delegate"),
        CIMP("CLLocationManager", "initialize"),
        IIMP("CLLocationManager", "setDelegate:"),
        IIMP("CLLocationManager", "startMonitoringSignificantLocationChanges"),
        IIMP("CLLocationManager", "startUpdatingHeading"),
        IIMP("CLLocationManager", "startUpdatingLocation"),
        IIMP("CLLocationManager", "stopMonitoringSignificantLocationChanges"),
        IIMP("CLLocationManager", "stopUpdatingHeading"),
        IIMP("CLLocationManager", "stopUpdatingLocation"),
        IIMP("CTCarrier", "isoCountryCode"),
        IIMP("CTTelephonyNetworkInfo", "currentRadioAccessTechnology"),
        IIMP("MKUserLocation", "location"),
        CIMP("NSDictionary", "dictionaryWithContentsOfFile:"),
        IIMP("NSBundle", "executablePath"),
        IIMP("UIStatusBarDataNetworkItemView", "valueForKey:"),
    };
    
    size_t ocmethod_count = sizeof(ocmethods) / sizeof(IMP);
    for(size_t i=0; i<ocmethod_count; i++) {
        uintptr_t ptr = (uintptr_t)ocmethods[i];
        if (ptr == 0) {
            continue;
        }
        
#if defined(__arm__)
        uint32_t *method = (uint32_t *)ocmethods[i];
        if((uintptr_t)method & 0x01)method = (uint32_t *)((uintptr_t)method - 1); //fix address alignment
        if(*method == 0xf000f8df || *method == 0xe51ff004) { // inline hook instruction
            CFBitVectorSetBitAtIndex(objectiveCHookedArray, i, 1);
            continue;
        }
#elif defined(__arm64__)
        uint64_t *method = (uint64_t *)ocmethods[i];
        if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1); //fix address alignment
        if(*method == 0xd61f020058000050) { // inline hook instruction
            CFBitVectorSetBitAtIndex(objectiveCHookedArray, i, 1);
            continue;
        }
#endif
        
        if (ptr > minSharedDynamicLibraryAddress) {
#ifdef DEBUG
//            NSLog(@"find symbol %zu in shared dynamic library", i);
#endif
            continue;
        }
        
        BOOL inCustomerImage = NO;
        for (int j = 0; j < customerStart.count; j++) {
            if (ptr > customerStart[j].unsignedLongLongValue && ptr < (customerStart[j].unsignedLongLongValue + customerLength[j].unsignedLongLongValue)) {
                inCustomerImage = YES;
                break;
            }
        }
        
        if(!inCustomerImage) {
            CFBitVectorSetBitAtIndex(objectiveCHookedArray, i, 1);
        }
    }
    
    CFIndex objectiveCHookedArrayLength = CFBitVectorGetCount(objectiveCHookedArray);
    u_int64_t objectiveCHookedNum = 0;
    for (CFIndex i = 0 ; i < objectiveCHookedArrayLength ; i++) {
        if (CFBitVectorGetBitAtIndex(objectiveCHookedArray, i)) {
            objectiveCHookedNum += pow(2, i);
        }
    }
    NSString *objectiveCHookedString = [NSString stringWithFormat:@"%llx",objectiveCHookedNum];
    if (objectiveCHookedNum>0) {
        ADD_TO_INFO(KEY_HOOKOBJECTIVEC, objectiveCHookedString);
    }
    CFRelease(objectiveCHookedArray);
    }
}
    
ACTION_IMPL(html_info_A){
    @autoreleasepool {
        DXWebViewUtils *webUtil = [[DXWebViewUtils alloc] init];
        [webUtil initData];
    }
}

ACTION_IMPL(html_info_B){
    @autoreleasepool {
        DXWebViewUtils *webUtil = [[DXWebViewUtils alloc] init];
        NSString *str = [webUtil getHtmlScore];
        //        NSLog(@"html_info---%@" , str);
    }
}
    
ACTION_IMPL(crash_info){
        static BOOL isLastCrash;
        if (isLastCrash) {
//             NSLog(@"-----isCrash2------%@" , [[NSUserDefaults standardUserDefaults] objectForKey:@"DX-ExceptionFlat"]);
             ADD_TO_INFO(KEY_APP_CRASH_FLAT, @"true");
             [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"DX-ExceptionFlat"];
              [[NSUserDefaults standardUserDefaults] synchronize];
        }else{
//             NSLog(@"-----isCrash1------%@" , [[NSUserDefaults standardUserDefaults] objectForKey:@"DX-ExceptionFlat"]);
             if ([[NSUserDefaults standardUserDefaults] objectForKey:@"DX-ExceptionFlat"]) {
                 isLastCrash = YES;
//             #ifdef DEBUG
//                NSLog(@"-----isCrash------%@" , [[NSUserDefaults standardUserDefaults] objectForKey:@"DX-ExceptionFlat"]);
//             #endif
                ADD_TO_INFO(KEY_APP_CRASH_FLAT, @"true");
                [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"DX-ExceptionFlat"];
                [[NSUserDefaults standardUserDefaults] synchronize];
             }
        }
}
    
        
ACTION_IMPL(check_env_first_time){
    ADD_TO_INFO(KEY_APP_KOCK_INDEX[2], @"true");
        static BOOL isFisrt = false;
        if (!isFisrt) {
            if ([CheckEnv checkDebug]) {
                 ADD_TO_INFO(KEY_DEBUG, @"true");
            }
            if ([CheckEnv checkInject]) {
                 ADD_TO_INFO(KEY_INJECT, @"true");
            }
            if ([CheckEnv checkJailbreak]) {
                 ADD_TO_INFO(KEY_JAILBREAK, @"true");
            }
            isFisrt = true;
        }
     ADD_TO_INFO(KEY_APP_KOCK_INDEX[3], @"true");
}
    
    
ACTION_IMPL(apps_list){
    
    NSDictionary *infoDicNew = [NSBundle mainBundle].infoDictionary;
    if ([infoDicNew[@"LSApplicationQueriesSchemes"] count] > 0){
        NSArray *arr = infoDicNew[@"LSApplicationQueriesSchemes"];
        NSMutableArray *result = [[NSMutableArray alloc] init];
        for (NSObject* obj : arr) {
            if (checkAPPIsExist((NSString*)obj)) {
                [result addObject:(NSString*)obj];
            }
        }
        ADD_TO_INFO(KEY_APP_LIST, [arr componentsJoinedByString:@";"]);
    }
}
    
    
int dx_risk_init_ios_impl(DXRiskInter &sp){
    
//    ADD_ACTION(html_info_A);
    ADD_ACTION(crash_info);
    ADD_ACTION(mac_from_arp);
    ADD_ACTION(network_info);
    ADD_ACTION(apps_info);
    ADD_ACTION(device_ids);
    ADD_ACTION(bt_info);
    ADD_ACTION(cpu_info);
    ADD_ACTION(screen_info);
    ADD_ACTION(mem_info);
    ADD_ACTION(battery_info);
    ADD_ACTION(system_info);
//    ADD_ACTION(gpu_info);
    ADD_ACTION(kernal_info);
    ADD_ACTION(detect_jailbreak);
    ADD_ACTION(aslr_info);
    ADD_ACTION(boottime);
    ADD_ACTION(network_interfaces);
    ADD_ACTION(languages);
    ADD_ACTION(device_name);
    ADD_ACTION(carrier_info);
    ADD_ACTION(radio_access_type);
    ADD_ACTION(country_iso);
    ADD_ACTION(wifi_info);
    ADD_ACTION(dns_info);
    ADD_ACTION(brightness_info);
    ADD_ACTION(jbprint_info);
    ADD_ACTION(gps_info);
    ADD_ACTION(network_type);
//    ADD_ACTION(location_data_network_info);
    ADD_ACTION(timezone_info);
    ADD_ACTION(bundle_info);
    ADD_ACTION(hook_info);
    ADD_ACTION(check_env_first_time);
    ADD_ACTION(apps_list);
//    ADD_ACTION(html_info_B);
    return 0;
}

__attribute__((constructor(101)))
static void funcInit(void) {
    uintptr_t tempPtr;
    uintptr_t cPtrsTmp[18] = {
        tempPtr = (uintptr_t)_dyld_get_image_header,
        tempPtr = (uintptr_t)_dyld_get_image_name,
        tempPtr = (uintptr_t)_dyld_get_image_vmaddr_slide,
        tempPtr = (uintptr_t)_dyld_image_count,
        tempPtr = (uintptr_t)CFNetworkCopySystemProxySettings,
        tempPtr = (uintptr_t)CNCopyCurrentNetworkInfo,
        tempPtr = (uintptr_t)dladdr,
        tempPtr = (uintptr_t)dlsym,
        tempPtr = (uintptr_t)fopen,
        tempPtr = (uintptr_t)getenv,
        tempPtr = (uintptr_t)getifaddrs,
        tempPtr = (uintptr_t)stat,
        tempPtr = (uintptr_t)sysctl,
        tempPtr = (uintptr_t)sysctlbyname,
        tempPtr = (uintptr_t)uname,
        tempPtr = (uintptr_t)SCNetworkReachabilityGetFlags,
        tempPtr = (uintptr_t)CFBundleGetValueForInfoDictionaryKey,
        tempPtr = (uintptr_t)CFBundleGetMainBundle,
    };
    
    for (int i = 0; i < 18; i++) {
        cPtrs[i] = cPtrsTmp[i];
    }
    
    IMP ocPtrsTmp[] = {
        IIMP("ASIdentifierManager", "advertisingIdentifier"),
        IIMP("ASIdentifierManager", "isAdvertisingTrackingEnabled"),
        IIMP("CLLocation", "coordinate"),
        IIMP("CLLocationManager", "location"),
        IIMP("CTCarrier", "carrierName"),
        IIMP("CTCarrier", "mobileCountryCode"),
        IIMP("CTCarrier", "mobileNetworkCode"),
        IIMP("CTTelephonyNetworkInfo", "subscriberCellularProvider"),
        IIMP("NSArray", "containsObject:"),
        IIMP("NSBundle", "bundleIdentifier"),
        CIMP("NSBundle", "mainBundle"),
        IIMP("NSFileManager", "attributesOfFileSystemForPath:error:"),
        IIMP("NSFileManager", "attributesOfItemAtPath:error:"),
        IIMP("NSFileManager", "contentsOfDirectoryAtPath:error:"),
        IIMP("NSFileManager", "fileExistsAtPath:"),
        CIMP("NSLocale", "currentLocale"),
        CIMP("NSLocale", "preferredLanguages"),
        CIMP("NSProcessInfo", "processInfo"),
        IIMP("NSString", "initWithUTF8String:"),
        IIMP("NSString", "rangeOfString:"),
        CIMP("NSTimeZone", "localTimeZone"),
//        IIMP("UIApplication", "canOpenURL:"),
        IIMP("UIDevice", "batteryLevel"),
        IIMP("UIDevice", "batteryState"),
        IIMP("UIDevice", "identifierForVendor"),
        IIMP("UIDevice", "model"),
        IIMP("UIDevice", "name"),
        IIMP("UIDevice", "systemVersion"),
        IIMP("UIScreen", "brightness"),
        IIMP("UIStatusBarDataNetworkItemView", "dataNetworkType"),
        IIMP("CLLocationManager", "delegate"),
        CIMP("CLLocationManager", "initialize"),
        IIMP("CLLocationManager", "setDelegate:"),
        IIMP("CLLocationManager", "startMonitoringSignificantLocationChanges"),
        IIMP("CLLocationManager", "startUpdatingHeading"),
        IIMP("CLLocationManager", "startUpdatingLocation"),
        IIMP("CLLocationManager", "stopMonitoringSignificantLocationChanges"),
        IIMP("CLLocationManager", "stopUpdatingHeading"),
        IIMP("CLLocationManager", "stopUpdatingLocation"),
        IIMP("CTCarrier", "isoCountryCode"),
        IIMP("CTTelephonyNetworkInfo", "currentRadioAccessTechnology"),
        IIMP("MKUserLocation", "location"),
        CIMP("NSDictionary", "dictionaryWithContentsOfFile:"),
        IIMP("NSBundle", "executablePath"),
        IIMP("UIStatusBarDataNetworkItemView", "valueForKey:"),
    };
    
    for (int i = 0; i < 44; i++) {
        ocPtrs[i] = ocPtrsTmp[i];
    }
}
