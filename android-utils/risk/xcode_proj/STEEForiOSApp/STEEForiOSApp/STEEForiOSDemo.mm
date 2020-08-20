//
//  STEEForiOS.m
//  STEEForiOS
//
//  Created by echonn on 30/06/2017.
//  Copyright © 2017 echonn. All rights reserved.
//

#import "STEEForiOSDemo.h"
#import "DXRisk.h"
//#import "DXSafeManager.h"
#import <CommonCrypto/CommonCryptor.h>
#import "Test.h"
//#define ENABLE_RISKMGR
//#define ENABLE_CHECKMGR
//#define ENABLE_SAFE
//@interface STEEForiOSDemo()<DXSafeManagerStreamDelegate>
//@end

static NSData *originData;
static STEEForiOSDemo *sid;
@implementation STEEForiOSDemo

#ifdef DEBUG

#define NSLog(FORMAT, ...) fprintf(stderr, "%s:%zd\t%s\n", [[[NSString stringWithUTF8String: __FILE__] lastPathComponent] UTF8String], __LINE__, [[NSString stringWithFormat: FORMAT, ## __VA_ARGS__] UTF8String]);

#else

#define NSLog(FORMAT, ...) nil

#endif


#define APPID1 @"28babb1ef43fe9968d9fb88687f9d8e6"
#define APPID2 @"dxdxdxtest2017keyc3e83b6940835"
#define APPID3 @"0fd68c9f8f253db0b163b0aa345da747"

#ifdef ENABLE_RISKMGR
+ (NSString*) testDXRiskManagerComponent
{
//    STEEiOSManager *mgr = [STEEiOSManager sharedInstance];
//    [mgr setup];
    
//    NSDictionary *dic = @{DXRiskManagerKeyUserId:@"value1", DXRiskManagerKeyEmail:@"value2"};
//    NSString *constID = [DXRiskManager fetchSecConstIDWithAppKey:@"d11260edf481c5b334fc3e83b6940835" extendParams:dic];
//    NSLog(@"constID: %@", constID);
   __block NSString *reponseStr;
    // 整个过程由于是耗时操作，必须要在非主线程上执行，否则会阻塞UI。如果本身已经在非UI线程上执行，则不需要另开线程
    dispatch_queue_t stee_queue = dispatch_queue_create("com.dingxiang.stee", DISPATCH_QUEUE_CONCURRENT);
    dispatch_sync(stee_queue, ^{
        // 根据业务逻辑，填充自定义字段
        NSDictionary *dic = @{DXRiskManagerKeyUserId: @"123456" , DXRiskManagerKeyDelayMsTime:@"4000"};//,DXRiskManagerKeyURL:@"http://10.0.1.47:8080/udid/m1"};
        // 如需自定义URL，填充DXRiskManagerKeyURL字段，如下注释
//        NSDictionary *dic = @{DXRiskManagerKeyUserId: @"123456",DXRiskManagerKeyURL:@"http://10.1.2.9:8190/udid/m1" , DXRiskManagerKeyBackupAppId:@"adfadfadfadsfads" , DXRiskManagerKeyBackup:@"BACKUP"};
        // 获取constID
        // 注意：constid最好不要保存在某个局部变量或者字段，每次使用时，都通过API获取。
        
        BOOL isSuccess = [DXRisk setup];//de88505f968de9730ee99fe84b8a2552
//        NSLog(@"setup success: %@" , isSuccess ? @"YES":@"NO");//
        NSString *constID1 = [DXRisk getToken:APPID1 extendsParams:dic];
        NSString *constID2 = [DXRisk getToken:APPID2 extendsParams:dic];
        NSString *constID3 = [DXRisk getToken:APPID3 extendsParams:dic];


        NSLog(@"constID1: %@", constID1);
        NSLog(@"constID2: %@", constID2);
        NSLog(@"constID3: %@", constID3);

        reponseStr = [NSString stringWithFormat:@"%@ , %@ , %@" ,[NSString stringWithFormat:@"constID1: %@" , constID1]
                                , [NSString stringWithFormat:@"constID2: %@" , constID2] , [NSString stringWithFormat:@"constID3: %@" , constID3]];

          NSLog(@"reponseStr---%@" , reponseStr);
//        return reponseStr;
//        NSLog(@"constID: %@", constID);
        // TODO 把constid通过Post请求，传到业务后台。
        // 下面是模拟频繁调用的过程
//        while(TRUE) {
//            NSLog(@"constID: %@", [DXRiskManager getToken:@"de88505f968de9730ee99fe84b8a2552" extendsParams:dic]);
////            [DXRiskManager setup:@"de88505f968de9730ee99fe84b8a2552"];
//            [NSThread sleepForTimeInterval:.5];
//        }
     
         //         [DXRiskManager setup:@"de88505f968de9730ee99fe84b8a2552"];
    });
    NSString *str = reponseStr;
//    Test *test = [[Test alloc] init];
//    [test call_test];
    return str;
}
#endif
//
//#ifdef ENABLE_SAFE
//+ (void) testDXSafeManager
//{
//    // 获取STEEManager单例（如果没有初始化，需要进行初始化[mgr setup];）
//
//    // 待加密/加签的数据
//    NSData *testData = [@"HelloWorld" dataUsingEncoding:NSUTF8StringEncoding];
//    // 测试k0， 只能用于加解密
//    NSString *key = @"k0";
//    NSData *testDataEncrypted = [DXSafeManager encrypt:testData withKey:key];
//    NSData *testDataDecrypted = [DXSafeManager decrypt:testDataEncrypted withKey:key];
//    NSAssert([testData isEqualToData:testDataDecrypted], @"key[%@] encrypt/decrypt fails: '%@' != '%@'.", key, testDataDecrypted, testData);
//
//    // 测试k1， k1为单向密钥，由k1加密/加签的数据只能由服务端解密/验签，反之亦然
//    key = @"k1";
//    testDataEncrypted = [DXSafeManager encrypt:testData withKey:key];
//    NSAssert(testDataEncrypted != nil, @"key[%@] encrypt fails.", key);
//    NSAssert([DXSafeManager sign:testData withKey:key] != nil, @"key[%@] sign fails.", key);
//
//    // 测试k2-k9
//    NSArray<NSString *> *keys = @[@"k2", @"k3", @"k4", @"k5", @"k6", @"k7", @"k8", @"k9"];
//    for(NSString* key : keys) {
//        NSString *sig = [DXSafeManager sign:testData withKey:key];
//        NSAssert([DXSafeManager verify:testData withKey:key andSig:sig], @"key[%@] sign/verify fails.", key);
//    }
//
//    // 测试k1服务端解密，key需要替换为license中的k1, 以下代码不要贴在集成文档中
//    const char keyCString[] = "e7f4f336197350e70fbc4e5dd905adc8", *pos = keyCString;
//    unsigned char aesKey[sizeof(keyCString)/2];
//    for(size_t count = 0; count < sizeof(aesKey)/sizeof(aesKey[0]); count++) {
//        sscanf(pos, "%2hhx", &aesKey[count]);
//        pos += 2;
//    }
//    NSUInteger dataLength = [testDataEncrypted length];
//    size_t bufferSize = dataLength + kCCBlockSizeAES128;
//    void *buffer = malloc(bufferSize);
//    size_t numBytesEncrypted = 0;
//    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt,
//                                          kCCAlgorithmAES128,
//                                          kNilOptions,
//                                          aesKey,
//                                          kCCKeySizeAES128,
//                                          NULL,
//                                          [testDataEncrypted bytes],
//                                          dataLength,
//                                          buffer,
//                                          bufferSize,
//                                          &numBytesEncrypted);
//    NSAssert(cryptStatus == kCCSuccess && *((unsigned char *)buffer + numBytesEncrypted - 1) <= kCCBlockSizeAES128, @"key[%@] decrypt fails.", @"k1");
//    testDataDecrypted = [NSData dataWithBytes:buffer length:numBytesEncrypted - *((unsigned char *)buffer + numBytesEncrypted - 1)];
//    NSAssert([testData isEqualToData:testDataDecrypted], @"key[%@] encrypt/decrypt fails: '%@' != '%@'.", @"k1", testDataDecrypted, testData);
//    free(buffer);
//
//    NSURL *source = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/WXWork_2.4.5.213.dmg"];
//    NSData *sd = [NSData dataWithContentsOfURL:source];
//    originData = sd;
//    sid = [[self alloc] init];
//    NSURL *dK0 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek0"];
//    NSURL *dK4 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek4"];
//    NSURL *dK5 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek5"];
//    NSURL *dK6 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek6"];
//    NSURL *dK9 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek9"];
////    NSURL *validation = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/validation"];
////    [DXSafeManager encryptWithSourceData:sd toDestinationURL:dK0 withKey:@"k0" andDelegate:sid];
////    [DXSafeManager encryptWithSourceData:sd toDestinationURL:dK4 withKey:@"k4" andDelegate:self];
////    [DXSafeManager encryptWithSourceData:sd toDestinationURL:dK5 withKey:@"k5" andDelegate:self];
////    [DXSafeManager encryptWithSourceData:sd toDestinationURL:dK6 withKey:@"k6" andDelegate:self];
////    [DXSafeManager encryptWithSourceData:sd toDestinationURL:dK9 withKey:@"k9" andDelegate:self];
//    NSLog(@"start");
////    [DXSafeManager dncryptWithSourceURL:dK0 withKey:@"k0" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK0 withKey:@"k0" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK0 withKey:@"k1" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK4 withKey:@"k4" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK5 withKey:@"k5" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK6 withKey:@"k6" andDelegate:sid];
////    [DXSafeManager dncryptWithSourceURL:dK9 withKey:@"k9" andDelegate:sid];
//}
//#endif

//- (void)encryptSuccess:(BOOL)isSuccess WithDestinationURL:(NSURL *)destinationURL error:(NSError *)error{
//    NSLog(@"encrypt %i %@ %@",isSuccess,destinationURL,error);
//    NSURL *dK0 = [[NSURL alloc] initFileURLWithPath:@"/Users/ccc/Downloads/enfilek0"];
//    [DXSafeManager decryptWithSourceURL:dK0 withKey:@"k0" andDelegate:sid];
//}
//
//- (void)decryptSuccess:(BOOL)isSuccess outPutData:(NSData *)data WithSourceURL:(NSURL *)sourceURL error:(NSError *)error{
//    NSLog(@"start");
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        NSLog(@"decrypt %i",isSuccess);
//        NSLog(@"decrypt %@",data);
//        NSLog(@"decrypt %@",sourceURL);
//        NSLog(@"decrypt %@",error);
//        NSLog(@"decrypt %i",[originData isEqual:data]);
//        NSLog(@"start");
//    });
//}


+ (NSString* )main
{
    return [STEEForiOSDemo testDXRiskManagerComponent];
}

@end
