//
//  Test.m
//  STEEForiOSApp
//
//  Created by weaiken on 2019/2/27.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import "Test.h"
#include <mach-o/dyld.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <objc/runtime.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <WebKit/WebKit.h>
#import <ViewController.h>

@interface Test () <WKNavigationDelegate,WKUIDelegate,WKScriptMessageHandler>

@property (strong, nonatomic) WKWebView *wkwebView;
@end

@implementation Test



- (void) call_test{

     NSLog(@"---call_test");
//    JSVirtualMachine *vm = [[JSVirtualMachine alloc] init];
//    JSContext *ctx = [[JSContext alloc] initWithVirtualMachine:vm];
//    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"js"];
//    NSString *script = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
//    [ctx evaluateScript:script];
////    // 访问js对象的三种方式
//    JSValue * abc = ctx[@"s3"];
//    // 执行
//    JSValue * ret = [abc callWithArguments:@[@""]];
//    NSLog(@"ret: %@",[ret toString]);
    
    // js配置
    WKUserContentController *userContentController = [[WKUserContentController alloc] init];
    [userContentController addScriptMessageHandler:self name:@"Token_1"];
    [userContentController addScriptMessageHandler:self name:@"Token_2"];
    // WKWebView的配置
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    configuration.userContentController = userContentController;
    
       self.wkwebView = [[WKWebView alloc] initWithFrame: CGRectMake(0, 0, 300, 500) configuration:configuration];
    
        NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
        NSURL *fileURL = [NSURL fileURLWithPath:path];
//        fileURL = [NSURL URLWithString:@"https://www.baidu.com"];
        //    [_webView loadHTMLString:(nonnull NSString *) baseURL:(nullable NSURL *)];
        //    NSURL *url = [NSURL URLWithString:@"http://www.jianshu.com"];
        //    // 根据URL创建请求
        //    NSURLRequest *request = [NSURLRequest requestWithURL:url];
        // WKWebView加载请求
//        wkwebView.delegate = self;
   
        [self.wkwebView loadRequest:[NSURLRequest requestWithURL:fileURL]];
        [[[Test getCurrentVC] view] addSubview:self.wkwebView];
        self.wkwebView.navigationDelegate = self;
        self.wkwebView.hidden = YES;
//        [[UIApplication sharedApplication].keyWindow addSubview:wkwebView];
    
        NSLog(@"---call_test--end");
    
//        JSContext *jsContext = [[JSContext alloc] init];
    
//    [wkwebView evaluateJavaScript:@"s()" completionHandler:^(id _Nullable item, NSError * _Nullable error) {
//        NSLog(@"alert---%@" ,error);
//    }];
//    [wkwebView evaluateJavaScript:@"test()" completionHandler:^(id _Nullable item, NSError * _Nullable error) {
//                NSLog(@"alert---%@" ,(NSNumber*)item);
//    }];
    
    
//    NSString *inputValueJS = @"document.getElementById('token2').innerHTML";
//
//    //执行JS
//    [wkwebView evaluateJavaScript:inputValueJS completionHandler:^(id _Nullable response, NSError * _Nullable error) {
//        NSLog(@"value: %@ error: %@", response, error);
//    }];
//
}


- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message {
    NSLog(@"方法名:%@", message.name);
    NSLog(@"参数:%@", message.body);
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


- (void) webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation{
    NSLog(@"---didFinishNavigation");
}

- (void) webView:(WKWebView *)webView didStartProvisionalNavigation:(WKNavigation *)navigation{
     NSLog(@"---didStartProvisionalNavigation");
}

- (void) call_test222222{
    NSLog(@"---call_test222222");
}


+ (void)load
{
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
//        id obj = [[self alloc] init];
//         [obj swizzleMethod:@selector(call_test) withMethod:@selector(call_test222222)];
//        [obj swizzleMethod:@selector(addObject:) withMethod:@selector(safeAddObject:)];
//        [obj swizzleMethod:@selector(objectAtIndex:) withMethod:@selector(safeObjectAtIndex:)];
//        [obj swizzleMethod:@selector(insertObject:atIndex:) withMethod:@selector(safeInsertObject:atIndex:)];
//        [obj swizzleMethod:@selector(removeObjectAtIndex:) withMethod:@selector(safeRemoveObjectAtIndex:)];
//        [obj swizzleMethod:@selector(replaceObjectAtIndex:withObject:) withMethod:@selector(safeReplaceObjectAtIndex:withObject:)];
//    });
}


//- (void)swizzleMethod:(SEL)origSelector withMethod:(SEL)newSelector
//{
//    NSLog(@"swizzleMethod");
//    Class cls = [self class];
//
//    Method originalMethod = class_getInstanceMethod(cls, origSelector);
//    Method swizzledMethod = class_getInstanceMethod(cls, newSelector);
//
//    BOOL didAddMethod = class_addMethod(cls,
//                                        origSelector,
//                                        method_getImplementation(swizzledMethod),
//                                        method_getTypeEncoding(swizzledMethod));
//    if (didAddMethod) {
//        class_replaceMethod(cls,
//                            newSelector,
//                            method_getImplementation(originalMethod),
//                            method_getTypeEncoding(originalMethod));
//    } else {
//        method_exchangeImplementations(originalMethod, swizzledMethod);
//    }
//}

@end
