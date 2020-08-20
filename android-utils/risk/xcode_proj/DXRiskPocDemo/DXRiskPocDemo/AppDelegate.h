//
//  AppDelegate.h
//  DXRiskPocDemo
//
//  Created by weaiken on 2019/3/9.
//  Copyright © 2019 weaiken. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (readonly, strong) NSPersistentContainer *persistentContainer;

- (void)saveContext;


@end

