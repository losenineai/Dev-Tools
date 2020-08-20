//
//  RiskViewController.m
//  STEEForiOSApp
//
//  Created by weaiken on 2018/11/19.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#import "RiskViewController.h"

@interface RiskViewController ()<UIGestureRecognizerDelegate>

@end

@implementation RiskViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationController.interactivePopGestureRecognizer.delegate = (id)self;
    // Do any additional setup after loading the view.

    
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
