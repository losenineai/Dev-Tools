//
//  ViewController.m
//  STEEForiOSApp
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "ViewController.h"
//#import "STEEiOSManager.h"
//#import <CoreBluetooth/CoreBluetooth.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "DeviceViewController.h"
#import "RiskViewController.h"
@import CoreLocation;
@interface ViewController ()<UITableViewDelegate, UITableViewDelegate>
{
//    CBCentralManager *_manager;
    CLLocationManager *manager;
}

//@property (strong, nonatomic) UIWebView *webView;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    manager = [[CLLocationManager alloc] init];
    [manager requestWhenInUseAuthorization];
//    [manager requestLocation];
    [manager startUpdatingLocation];




//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(10 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [STEEForiOSDemo main];
//    });
}




- (IBAction)clickRisk:(id)sender {
}

- (IBAction)clickDevice:(id)sender {
    DeviceViewController *controller = [[DeviceViewController alloc] init];
//    controller.navigationItem.title = @"设备指纹";
    [self.navigationController pushViewController:controller animated:YES];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
