//
//  ViewController.m
//  STEEForiOSApp
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "ViewController.h"
#import <JavaScriptCore/JavaScriptCore.h>
#import "NormalSaasTester.h"
#import "MultiAppTester.h"
#import "PrivateEvnTester.h"
#import "LooperEnvTester.h"
#import "MultiThreadTester.h"

@interface ViewController()


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [[[NormalSaasTester alloc] init] start];
    [[[MultiAppTester alloc]init]start];
    [[[PrivateEvnTester alloc]init]start];
    [[[LooperEnvTester alloc]init]start];
    [[[MultiThreadTester alloc]init]start];
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
