//
//  ViewController.h
//  STEEForiOSApp
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextView *textView;

+ (UIViewController *)getCurrentVC;
@end

