//
//  ViewController.m
//  STEEForiOSApp
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "ViewController.h"
#import "STEEForiOSDemo.h"
//#import "STEEiOSManager.h"
//#import <CoreBluetooth/CoreBluetooth.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "DXRisk.h"
#import <WebKit/WebKit.h>
#import "Test.h"
@import CoreLocation;
@interface ViewController ()<WKUIDelegate,WKNavigationDelegate>//<CBCentralManagerDelegate, CBPeripheralManagerDelegate>
{
    //    CBCentralManager *_manager;
    CLLocationManager *manager;
}

@property (strong, nonatomic) UIWebView *webView;
@property (strong, nonatomic) WKWebView *wkwebView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    self.webView = [[UIWebView alloc]initWithFrame:CGRectMake(0, 20, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)];
//    self.webView.delegate = self;
//    [self.view addSubview:_webView];
//    NSString* path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
//    NSURL* url = [NSURL fileURLWithPath:path];
//    NSURLRequest* request = [NSURLRequest requestWithURL:url] ;
//    [self.webView loadRequest:request];
//
//    JSContext *jsContext = [self.webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
//    jsContext[@"DXRiskManager"] = [DXRiskManager class];
//    jsContext[@"DXRiskManagerKeyUserId"] = DXRiskManagerKeyUserId;
////    jsContext[@"DXRiskManagerKeyEmail"] = DXRiskManagerKeyEmail;
////    jsContext[@"DXRiskManagerKeyPhone"] = DXRiskManagerKeyPhone;
////    jsContext[@"DXRiskManagerKeyURL"] = DXRiskManagerKeyURL;
//    jsContext.exceptionHandler = ^(JSContext *context, JSValue *exceptionValue) {
//        context.exception = exceptionValue;
//        NSLog(@"异常信息：%@", exceptionValue);
//    };

//    manager = [[CLLocationManager alloc] init];
//    [manager requestWhenInUseAuthorization];
//    [manager startUpdatingLocation];
//
    NSString* str = [STEEForiOSDemo main];
    NSLog(@"str----%@" , str);
   _textView.text = str;
    
    
    
//    self.wkwebView = [[WKWebView alloc] initWithFrame: CGRectMake(0, 0, 300, 300)];
//    NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
//    NSURL *fileURL = [NSURL fileURLWithPath:path];
//    //    [_webView loadHTMLString:<#(nonnull NSString *)#> baseURL:<#(nullable NSURL *)#>];
//    //    NSURL *url = [NSURL URLWithString:@"http://www.jianshu.com"];
//    //    // 根据URL创建请求
//    //    NSURLRequest *request = [NSURLRequest requestWithURL:url];
//    // WKWebView加载请求
//    self.wkwebView.UIDelegate = self;
//    self.wkwebView.navigationDelegate = self;
//    [self.wkwebView loadRequest:[NSURLRequest requestWithURL:fileURL]];
//    [self.view addSubview:self.wkwebView];
//        UIWindow *window = [UIApplication sharedApplication].keyWindow;
//        window.windowLevel =UIWindowLevelNormal;
//        [window addSubview:self.wkwebView];
    
//    WKWebView *webView1 = [[WKWebView alloc] initWithFrame: CGRectMake(0, 0, 300, 300)];
//    NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
//    NSURL *fileURL = [NSURL fileURLWithPath:path];
//    webView1.UIDelegate = self;
//    webView1.navigationDelegate = self;
//    [webView1 loadRequest:[NSURLRequest requestWithURL:fileURL]];
//
//    UIViewController *uiview = [ViewController getCurrentVC];
//    [[uiview view]addSubview:webView1];
//    [self.view addSubview:webView1];
    
    
    
//    UIWindow *window = [UIApplication sharedApplication].keyWindow;
//    window.windowLevel =UIWindowLevelNormal;
//    [window addSubview:webView1];
//    NSLog(@"---call_test--end");
//    Test *test = [[Test alloc] init];
//    [test call_test];
//
    
 
    
//    [self.navigationController presentViewController: ickImageViewController animated:true];

}

- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    NSLog(@"加载完成1");
}


+ (UIViewController *)getCurrentVC
{
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


- (IBAction)clickGetToken:(id)sender {
    NSString* str = _textView.text;
    str = [NSString stringWithFormat:@"%@\n%@" , str , [STEEForiOSDemo main]];
    _textView.text = str;
}



- (IBAction)clickTestCrash:(id)sender {
    
       
            //常见异常1---不存在方法引用
    //        [self performSelector:@selector(thisMthodDoesNotExist) withObject:nil];
            //常见异常2---键值对引用nil
        //    [[NSMutableDictionary dictionary] setObject:nil forKey:@"nil"];
            //常见异常3---数组越界
    //        [[NSArray array] objectAtIndex:1];
            //常见异常4---memory warning 级别3以上
        //    [self performSelector:@selector(killMemory) withObject:nil];
        
        
    //    UIView *tempView = [[UIView alloc]init];
    //    [tempView release];
    ////    //对象已经被释放，内存不合法，此块内存地址又没被覆盖，所以此内存内垃圾内存，所以调用方法的时候会导致SIGSEGV的错误
    //    [tempView setNeedsDisplay];
    //
    //    或者说 我在堆内存中找栈内存地址
    //
    //    id x_id = [self performSelector:@selector(createNum)];
    //
    //    - (int)createNum {
    //        return 10;
    //    }
    //
    //    这种情况也是会导致SIGSEGV的错误的
    //
    //    如果在内存中释放不存在的空间，就会导致SIGABRT错误
    //
    //    Test * test = {1, 2};
    //    free(test);
    //
    //
    //    内存地址不对齐会导致SIGBUS错误
    //
        char *s = "hello world";
        *s = 'H';
//    [self killMemory];
}

#pragma mark - custom method

- (void) killMemory
{
    for (int i = 0; i < 300; i ++)
    {
        UILabel *tmpLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 320, 200)];
        tmpLabel.layer.masksToBounds = YES;
        tmpLabel.layer.cornerRadius = 10;
        tmpLabel.backgroundColor = [UIColor redColor];
        [self.view addSubview:tmpLabel];
    }
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
