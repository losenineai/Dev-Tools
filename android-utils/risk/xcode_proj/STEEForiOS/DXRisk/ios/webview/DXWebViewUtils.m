//
//  DXWebViewUtils.m
//  STEEForiOS
//
//  Created by weaiken on 2019/4/10.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import "DXWebViewUtils.h"
#import <JavaScriptCore/JavaScriptCore.h>
#import <WebKit/WebKit.h>
#import "DXKeychainHelper.h"

@interface DXWebViewUtils () <WKNavigationDelegate,WKUIDelegate,WKScriptMessageHandler>
@property (strong, nonatomic) WKWebView *wkwebView;
@end

@implementation DXWebViewUtils


- (void) initData{
    // js配置
    
    NSString *HTML_1 = [DXKeyChainUtils get:@"DX-HTML-A" default:nil];
    NSString *HTML_2 = [DXKeyChainUtils get:@"DX-HTML-B" default:nil];
    NSString *HTML_3 = [DXKeyChainUtils get:@"DX-HTML-C" default:nil];
    if (HTML_1 != nil && HTML_2 != nil && HTML_3 != nil) {
        return;
    }
    
    WKUserContentController *userContentController = [[WKUserContentController alloc] init];
    [userContentController addScriptMessageHandler:self name:@"Token_1"];
    [userContentController addScriptMessageHandler:self name:@"Token_2"];
    [userContentController addScriptMessageHandler:self name:@"Token_3"];
    // WKWebView的配置
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    configuration.userContentController = userContentController;
    
    self.wkwebView = [[WKWebView alloc] initWithFrame: CGRectMake(0, 0, 300, 500) configuration:configuration];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
    NSURL *fileURL = [NSURL fileURLWithPath:path];
    
    [self.wkwebView loadRequest:[NSURLRequest requestWithURL:fileURL]];
    [[[DXWebViewUtils getCurrentVC] view] addSubview:self.wkwebView];
    self.wkwebView.navigationDelegate = self;
    self.wkwebView.hidden = YES;
    //        [[UIApplication sharedApplication].keyWindow addSubview:wkwebView];
//    NSLog(@"---call_test--end");
}

- (NSString*) getHtmlScore{

    NSString *HTML_1 = [DXKeyChainUtils get:@"DX-HTML-A" default:nil];
    NSString *HTML_2 = [DXKeyChainUtils get:@"DX-HTML-B" default:nil];
    NSString *HTML_3 = [DXKeyChainUtils get:@"DX-HTML-C" default:nil];
//    NSLog(@"--HTML_1---%@----HTML_2----%@" , HTML_1 , HTML_2);
    if (HTML_1 != nil && HTML_2 != nil && HTML_3 != nil) {
         return [NSString stringWithFormat:@"%@%@%@" , HTML_1 , HTML_2 , HTML_3];
    }
    
    return @"null";
}

- (void) Token_1:(NSString*) str{
    [DXKeyChainUtils set:str forkey:@"DX-HTML-A"];
}

- (void) Token_2:(NSString*) str{
    [DXKeyChainUtils set:str forkey:@"DX-HTML-B"];
}

- (void) Token_3:(NSString*) str{
    [DXKeyChainUtils set:str forkey:@"DX-HTML-C"];
}

- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message {
//    NSLog(@"方法名:%@", message.name);
//    NSLog(@"参数:%@", message.body);
    // 方法名
    NSString *methods = [NSString stringWithFormat:@"%@:", message.name];
    SEL selector = NSSelectorFromString(methods);
    // 调用方法
    if ([self respondsToSelector:selector]) {
        [self performSelector:selector withObject:message.body];
    } else {
        NSLog(@"未实行方法：%@", methods);
    }
}

+ (UIViewController*) getCurrentVC{
    UIWindow *keyWindow = [UIApplication sharedApplication].delegate.window;;
    UIViewController *vc = keyWindow.rootViewController;
    while (vc.presentedViewController) {
        vc = vc.presentedViewController;
        
        if ([vc isKindOfClass:[UINavigationController class]]) {
            vc = [(UINavigationController *)vc visibleViewController];
        } else if ([vc isKindOfClass:[UITabBarController class]]) {
            vc = [(UITabBarController *)vc selectedViewController];
        }
    }
    return vc;
}

@end

