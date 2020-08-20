//
//  TestRisk.m
//  DXRiskStaticTest
//
//  Created by weaiken on 2019/9/6.
//  Copyright © 2019 dingxiang-inc. All rights reserved.
//

#import "TestRisk.h"
#import <DXPlatform.h>
#import <DXRiskInter.h>
@implementation TestRisk


+ (void) testCheckEvn{
    DXEnvCheck *check = new DXEnvCheck();
    dx_risk_check_init(*check);
    delete check;
}


+ (void) testRiskEvn{
    DXRiskInter *risk = new DXRiskInter();
    dx_risk_init(*risk);
    delete risk;
}

@end
