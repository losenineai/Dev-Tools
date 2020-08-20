//
//  proxy_detect.m
//  STEEForiOS
//
//  Created by ccc on 20/03/2018.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#import "UIDevice+STEE_Application.h"
#import <Foundation/Foundation.h>
#import <SystemConfiguration/SCNetworkConnection.h>
#import "safe_detect.h"

using namespace std;

DEFINE_CHECK_IMPL(proxy) {
    NSDictionary *proxySettings = (__bridge NSDictionary *) CFNetworkCopySystemProxySettings();
    NSNumber *proxyHttpEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesHTTPEnable];
    NSNumber *proxyAutoConfigEnable = [proxySettings valueForKey:(NSString *) kCFNetworkProxiesProxyAutoConfigEnable];
    if( [proxyHttpEnable boolValue] || [proxyAutoConfigEnable boolValue] ) {
        call((void *)NULL);
    }
}
