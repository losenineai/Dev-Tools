#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "YSHRequestInterceptorProtocol.h"
#import "YSHResponseInterceptorProtocol.h"
#import "YSHttpConfigure.h"
#import "YSHttpConstant.h"
#import "YSHttpLogger.h"
#import "YSHttpManager.h"
#import "YSHttpManagerHeader.h"
#import "YSHttpRequest.h"
#import "YSHttpResponse.h"

FOUNDATION_EXPORT double YSHttpManagerVersionNumber;
FOUNDATION_EXPORT const unsigned char YSHttpManagerVersionString[];

