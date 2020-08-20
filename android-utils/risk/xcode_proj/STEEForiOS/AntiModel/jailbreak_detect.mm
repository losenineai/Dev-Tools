//
//  jailbreak_detect.m
//  STEEForiOS
//
//  Created by ccc on 20/03/2018.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//

#import "UIDevice+STEE_Application.h"
#import <Foundation/Foundation.h>
#import "safe_detect.h"

using namespace std;

DEFINE_CHECK_IMPL(jailbreak) {
    if([[UIDevice currentDevice] stee_isJailbreak_1] || [[UIDevice currentDevice] stee_isJailbreak_2] || [[UIDevice currentDevice] stee_isJailbreak_3] || [[UIDevice currentDevice] stee_isJailbreak_4]) {
        call((void *)NULL);
    }
}
