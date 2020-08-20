//
//  hook_detect.m
//  STEEForiOS
//
//  Created by ccc on 20/03/2018.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//


#import <mach-o/ldsyms.h>
#include <mach-o/dyld.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <objc/runtime.h>
#include <dlfcn.h>
#import <objc/runtime.h>
#import <objc/message.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import <SystemConfiguration/SCNetworkConnection.h>
#import <sys/socket.h>
#import <ifaddrs.h>
#import <sys/utsname.h>
#import <sys/stat.h>
#import <Foundation/Foundation.h>
#import "safe_detect.h"

using namespace std;

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

DEFINE_CHECK_IMPL(hook) {
#if TARGET_IPHONE_SIMULATOR
    return;
#endif
    NSMutableArray <NSNumber *>*customerStart = [NSMutableArray array];
    NSMutableArray <NSNumber *>*customerLength = [NSMutableArray array];
    NSMutableArray *customerDYLIBs = [NSMutableArray array];
    
    //add origin image into customerDYLIBS as occupied not hooked address
    NSString *execBin = [[NSString stringWithCString:_dyld_get_image_name(0) encoding:NSUTF8StringEncoding] lastPathComponent];
    [customerDYLIBs addObject:execBin];
    
#ifdef DXSAFE_DY
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
#ifdef DXSAFE_DY
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
        
        uint32_t imageCount = _dyld_image_count();
        NSUInteger most_aslr_count = 0;
        uintptr_t most_aslr = -1;
        for (int i = 0; i < imageCount; i++) {
            uintptr_t aslr = _dyld_get_image_vmaddr_slide(i);
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
            if (most_aslr == _dyld_get_image_vmaddr_slide(i)) {
                if (minSharedDynamicLibraryAddress > (uintptr_t)_dyld_get_image_header(i)) {
                    minSharedDynamicLibraryAddress = (uintptr_t)_dyld_get_image_header(i);
                }
                continue;
            }
            
            NSString * dylib = [[NSString stringWithCString:_dyld_get_image_name(i) encoding:NSUTF8StringEncoding] lastPathComponent];
            if ([customerDYLIBs containsObject:dylib]) {
                const struct mach_header* header = (const struct mach_header*)_dyld_get_image_header(i);
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
                
                [customerStart addObject:[NSNumber numberWithUnsignedLongLong:(uintptr_t)_dyld_get_image_header(i)]];
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
                    call((void *)NULL);
                }
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)cmethods[i];
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1); //fix address alignment
            if (method != nil) {
                if(*method == 0xd61f020058000050) { // inline hook instruction
                    call((void *)NULL);
                }
            }
#endif
            
            if (ptr > minSharedDynamicLibraryAddress) {
#ifdef DEBUG
                NSLog(@"find symbol %zu in shared dynamic library", i);
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
                call((void *)NULL);
            }
        }
        
        IMP ocmethods[] = {
#ifdef STEE_IDFA
            IIMP("ASIdentifierManager", "advertisingIdentifier"),
            IIMP("ASIdentifierManager", "isAdvertisingTrackingEnabled"),
#endif
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
            IIMP("UIApplication", "canOpenURL:"),
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
                call((void *)NULL);
            }
#elif defined(__arm64__)
            uint64_t *method = (uint64_t *)ocmethods[i];
            if((uintptr_t)method & 0x01)method = (uint64_t *)((uintptr_t)method - 1); //fix address alignment
            if(*method == 0xd61f020058000050) { // inline hook instruction
                call((void *)NULL);
            }
#endif
            
            if (ptr > minSharedDynamicLibraryAddress) {
#ifdef DEBUG
                NSLog(@"find symbol %zu in shared dynamic library", i);
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
                call((void *)NULL);
            }
        }        
}

