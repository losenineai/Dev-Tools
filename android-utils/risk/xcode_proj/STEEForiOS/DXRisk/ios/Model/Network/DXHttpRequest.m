//
//  DXHttpRequest.m
//  STEEForiOS
//
//  Created by ccc on 11/12/2017.
//  Copyright © 2017 dingxiang-inc. All rights reserved.
//

#import "DXHttpRequest.h"

static const CFOptionFlags kNetworkEvents =  kCFStreamEventHasBytesAvailable | kCFStreamEventEndEncountered | kCFStreamEventErrorOccurred;
static NSString *DXHTTPRequestRunLoopMode = @"DXHTTPRequestRunLoopMode";
static NSOperationQueue *dxHttpRequestQueue;

@interface DXHttpRequest()

@property (nonatomic, strong) NSInputStream *readStream;
@property (nonatomic, strong) NSInputStream *bodyStream;
@property (nonatomic, assign) BOOL complete;
@property (nonatomic, strong) NSDate *lastActivityDate;
@property (nonatomic, strong) NSTimer *statusTimer;
@property (nonatomic, assign) BOOL inProgress;
@property (nonatomic, strong) NSString *runloopMode;

- (void)handleNetworkEvent:(CFStreamEventType)type;
+ (NSOperationQueue *)requestQueue;

@end

static void ReadStreamClientCallBack(CFReadStreamRef readStream, CFStreamEventType type, void *clientCallBackInfo) {
    [((__bridge DXHttpRequest*)clientCallBackInfo) handleNetworkEvent: type];
}

@implementation DXHttpRequest

#pragma classMethods
+ (NSOperationQueue *)requestQueue{
    if (!dxHttpRequestQueue) {
        dxHttpRequestQueue = [[NSOperationQueue alloc] init];
    }
    return  dxHttpRequestQueue;
}

+ (void)requestEntryPoint:(id)__unused objec{
    @autoreleasepool {
        [[NSThread currentThread] setName:@"DXHttpRequestThread"];
        NSRunLoop *currentRunloop = [NSRunLoop currentRunLoop];
        [currentRunloop addPort:[NSPort port] forMode:NSDefaultRunLoopMode];
        [currentRunloop run];
    }
}

+ (NSThread *)sharedThread{
    static NSThread *shredThread = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        shredThread = [[NSThread alloc] initWithTarget:self selector:@selector(requestEntryPoint:) object:nil];
        [shredThread start];
    });
    return shredThread;
}

#pragma mark - LifeCycle
- (void)dealloc{
    [self cancelLoad];
}

#pragma mark - Methods

- (void)start{
    self.runloopMode = NSDefaultRunLoopMode;
    self.complete = NO;
    self.inProgress = YES;
    self.responseData = [NSMutableData data];
    if (![self isCancelled] && ![self complete]) {
        [self performSelector:@selector(main) onThread:[DXHttpRequest sharedThread] withObject:nil waitUntilDone:NO];
    }
}

- (void)startAsynchronous{
    [[DXHttpRequest requestQueue] addOperation:self];
}

- (void)startSynchronous
{
    self.runloopMode = DXHTTPRequestRunLoopMode;
    self.complete = NO;
    self.inProgress = YES;
    self.responseData = [NSMutableData data];
    if (![self isCancelled] && ![self complete]) {
        [self main];
        // when the date request complete,the autoreleasepool not release some object(like RunloopContext.etc), make another 5 runloop continue to ensure the RunloopContext has fully released
        int i=0;
        while (!self.complete || i < 5) {
            [[NSRunLoop currentRunLoop] runMode:self.runloopMode beforeDate:[NSDate distantFuture]];
            if (self.complete) {
                i++;
            }
        }
    }
    
    self.inProgress = NO;
}

- (void)main {
    CFStringRef requestMethod;
    switch (self.requestType) {
        case DXHttpRequestTypePOST:
            requestMethod = CFStringCreateWithCString(kCFAllocatorDefault, "POST", kCFStringEncodingUTF8);
            break;
        case DXHttpRequestTypeGET:
            requestMethod = CFStringCreateWithCString(kCFAllocatorDefault, "GET", kCFStringEncodingUTF8);
            break;
        case DXHttpRequestTypeNONE:
            NSLog(@"NOT SET HTTP TYPE");
            break;
    }
    
    CFHTTPMessageRef request = CFHTTPMessageCreateRequest(kCFAllocatorDefault, requestMethod, (__bridge CFURLRef)self.url, kCFHTTPVersion1_1);

    for (NSString *header in self.requestHeaders) {
        CFHTTPMessageSetHeaderFieldValue(request, (__bridge CFStringRef)header, (__bridge CFStringRef)[self.requestHeaders objectForKey:header]);
    }
    
    if (self.postData.length > 0) {
        self.bodyStream = [NSInputStream inputStreamWithData:self.postData];
    }
    
    [self setReadStream:(__bridge_transfer NSInputStream *)CFReadStreamCreateForStreamedHTTPRequest(kCFAllocatorDefault, request, (__bridge CFReadStreamRef)self.bodyStream)];

    [self.readStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:self.runloopMode];

    CFStreamClientContext ctxt = {0, (__bridge void *)(self), NULL, NULL, NULL};
    if (CFReadStreamSetClient((CFReadStreamRef)[self readStream], kNetworkEvents, ReadStreamClientCallBack, &ctxt)) {
        CFReadStreamOpen((CFReadStreamRef)[self readStream]);
    }

    CFRelease(request);
    //time out
    self.lastActivityDate = [NSDate date];
    self.statusTimer = [NSTimer timerWithTimeInterval:0.25 target:self selector:@selector(updateStatus:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:self.statusTimer forMode:self.runloopMode];
}

- (void)updateStatus:(NSTimer *)timer {
    [self checkRequestStatus];
    if (![self inProgress]) {
        [self setStatusTimer:nil];
    }
}

- (void)checkRequestStatus{
    if ([self shouldTimeOut]) {
        [self failWithError:[NSError errorWithDomain:@"request_timed_out" code:0 userInfo:nil]];
        [self cancelLoad];
        self.inProgress = NO;
        [self setComplete:YES];
        return;
    }
}

- (BOOL)shouldTimeOut{
    NSTimeInterval secondsSinceLastActivity = [[NSDate date] timeIntervalSinceDate:self.lastActivityDate];
    // See if we need to timeout
    if ([self readStream] && [self lastActivityDate] && [self timeOutSeconds] > 0 && secondsSinceLastActivity > [self timeOutSeconds]) {
        
        // ***Black magic warning***
        // We have a body, but we've taken longer than timeOutSeconds to upload the first small chunk of data
        // Since there's no reliable way to track upload progress for the first 32KB (iPhone) or 128KB (Mac) with CFNetwork, we'll be slightly more forgiving on the timeout, as there's a strong chance our connection is just very slow.
        if (secondsSinceLastActivity > [self timeOutSeconds]*1.5) {
            return YES;
        }
    }
    return NO;
}


- (void)cancelLoad{
    if ([self readStream]) {
        [self destroyReadStream];
        [self setReadStream:nil];
        [self setBodyStream:nil];
    }
    
    if (self.responseData) {
        if (![self complete]) {
            self.responseData = nil;
        }
    }
}

- (void)destroyReadStream
{
    if ([self readStream]) {
        CFReadStreamSetClient((CFReadStreamRef)[self readStream], kCFStreamEventNone, NULL, NULL);
        [[self readStream] removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:self.runloopMode];
        CFReadStreamClose((__bridge CFReadStreamRef)self.readStream);
        //when kCFStreamEventErrorOccurred bodyStream not removed by CFNetwork, so remeve it manual
        [[self bodyStream] removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:self.runloopMode];
        CFReadStreamClose((__bridge CFReadStreamRef)self.bodyStream);
    }
}

- (void)failWithError:(NSError *)error{
    self.error = error;
}

- (void)handleNetworkEvent:(CFStreamEventType)type {

    switch(type) {
        case kCFStreamEventHasBytesAvailable:
        {
            UInt8 buf[4096];
            NSInteger bytesRead = [self.readStream read:buf maxLength:sizeof(buf)];
            if (bytesRead > 0) {
                [self.responseData appendBytes:buf length:(NSUInteger)bytesRead];
            }
            break;
        }
        case kCFStreamEventErrorOccurred:
        {
            NSError *error = (__bridge_transfer NSError *)CFReadStreamCopyError((CFReadStreamRef)self.readStream);
            [self failWithError:error];
            [self cancelLoad];
            [self setStatusTimer:nil];
            self.complete = YES;
            break;
        }
        case kCFStreamEventEndEncountered:
        {
            CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty((CFReadStreamRef)self.readStream, kCFStreamPropertyHTTPResponseHeader);
            CFIndex statusCode = CFHTTPMessageGetResponseStatusCode(response);
            self.responseHeaders = (__bridge_transfer NSDictionary *)CFHTTPMessageCopyAllHeaderFields(response);
            if(statusCode == 200) {
                NSData *data = self.responseData;
                CFRelease(response);
                self.responseData = [data mutableCopy];
            } else {
                CFRelease(response);
                NSError *error = [NSError errorWithDomain:@"http_bad_status" code:statusCode userInfo:nil];
                [self failWithError:error];
            }
            
            [self destroyReadStream];
            
            [self setStatusTimer:nil];
            self.complete = YES;
            self.inProgress = NO;
            break;
        }
        default:
            break;
    }
}

#pragma mark - Setter&Getter

- (void)setStatusTimer:(NSTimer *)timer
{
    if (_statusTimer && timer != _statusTimer) {
        [_statusTimer invalidate];
    }
    _statusTimer = timer;
}

@end
