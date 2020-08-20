//
//  DeviceViewController.h
//  STEEForiOSApp
//
//  Created by weaiken on 2018/11/20.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DeviceViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *appIdTextView;

@property (weak, nonatomic) IBOutlet UITextField *appSecTextView;

@property (weak, nonatomic) IBOutlet UITextField *urlTextView;

@property (weak, nonatomic) IBOutlet UILabel *statusText;

@property (weak, nonatomic) IBOutlet UITextView *showTextView;

@property (weak, nonatomic) IBOutlet UISwitch *tokenSwitch;


@end

