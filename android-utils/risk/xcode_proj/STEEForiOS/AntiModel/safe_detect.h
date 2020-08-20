//
//  safe_detect.h
//  STEEForiOS
//
//  Created by ccc on 20/03/2018.
//  Copyright © 2018 dingxiang-inc. All rights reserved.
//
#include <unistd.h>
#include <thread>

#ifndef safe_detect_h
#define safe_detect_h

#define DEFINE_CHECK_IMPL(name) void check_is_##name(function<void(void*)> &call)

#endif /* safe_detect_h */
