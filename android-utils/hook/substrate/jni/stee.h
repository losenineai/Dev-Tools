#ifndef _SECURITYSTACK_STEE_H_
#define _SECURITYSTACK_STEE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ANDROID
#include <jni.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
#ifdef __STEEC__
#define FLA __attribute((__annotate__(("fla"))))
#define BCF __attribute((__annotate__(("bcf"))))
#define SUB __attribute((__annotate__(("sub"))))
#define SPL __attribute((__annotate__(("spli"))))
#define NO_FLA __attribute((__annotate__(("no-fla"))))
#define NO_BCF __attribute((__annotate__(("no-bcf"))))
#define NO_SUB __attribute((__annotate__(("no-sub"))))
#define NO_SPL __attribute((__annotate__(("no-spli"))))
    
#ifdef __APPLE__
#define STEE SPL FLA SUB BCF
#define NO_STEE
#else
//#define STEE __attribute((__annotate__(("stee"))))
//#define NO_STEE __attribute((__annotate__(("no-stee"))))
#define STEE
#define NO_STEE
#endif
#else
#define STEE
#define FLA
#define BCF
#define SUB
#define SPL
#define NO_STEE
#define NO_FLA
#define NO_BCF
#define NO_SUB
#define NO_SPL

#endif
#define STEE_EXPORT __attribute__((visibility("default")))

/*
 *
 * 为每一个STEE实例创建随机值, 达到一机一密的效果
 *
 * 例如：
 * switch(stee_random_per_instance_uint32("alg_choise", 3)) {
 *   case 0:
 *       return fa();
 *   case 1:
 *       return fb();
 *   case 2:
 *       return fc();
 * }
 * 编译后：
 * 1. 在最终生成的某个stee实例中等价代码如下
 *    return fa(); // 仅有fa函数，其他的fb、fc函数在编译期间被清除
 * 2. 对应的在这个实例的数据库记录中可以看到
 *    alg_choise = 0
 * 3. 最终在服务器端参考数据库记录与终端上传的数据，进行计算与校验
 */
uint32_t stee_random_per_instance_uint32(const char* code, uint32_t maximum);
// 特别的， 可以生成一个随机数组， 数据库保存其base64值
void stee_random_per_instance_uint8p(const char* code, uint8_t* out, uint32_t out_size);
// 特别的， 可以生成一个随机AES密钥， 数据库保存其base64值
void stee_random_per_instance_aes_key(const char* code, uint8_t* out, uint32_t out_size);

#ifdef __cplusplus
}
#endif

#endif
