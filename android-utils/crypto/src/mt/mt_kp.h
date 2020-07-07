//
//  mt_kp.h
//  
//  此安全算法是一套轻量、低能耗算法，算法内部无API调用、无乘除法使用。因此，算法内部的计算过程只能依赖参数的内存空间。
//  如果使用者希望在kp_decode函数执行后，对解密前的密文数组src仍然有使用需求，则可以调用kp_set_api来设置内存操作API。
//  算法内部会根据kp_set_api的被调用情况来选择是否申请新空间。
//
//  Created by mt on 2017/8/15.
//  Copyright © 2017年 mt. All rights reserved.
//

#ifndef kp_h
#define kp_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*PFUNC_MALLOC)(size_t size);
typedef void (*PFUNC_FREE)(void *ptr);

/**
 *  功能描述: 设置API
 *      此函数用于向加密算法传入malloc与free函数的函数指针，可选择性使用。
 *      如果使用此函数，则能保证解密函数kp_decode执行后，参数src数组内容不变；
 *      否则，解密函数kp_decode执行后，参数src数组内容可能会发生变化，不再建议使用。
 *  @param [in] pfunc_malloc
 *      malloc函数的函数指针
 *  @param [in] pfunc_free
 *      free函数的函数指针
 *  @return  
 *      true 表示API设置成功，后面再调用kp_decode函数，能保护src数组内容不会发生变化。
 *      false 表示API设置失败，请检查，传入的参数是不为NULL。
 */
bool kp_set_api(PFUNC_MALLOC pfunc_malloc, PFUNC_FREE pfunc_free);

/** 
 *  功能描述: 取消API设置
 *      调用此函数后，后面再执行kp_decode函数后src数组内容不再具有使用价值。
 *   
 *  @return  
 *      无
 */
void kp_unset_api();

/** 
 *  功能描述: 加密函数
 *  @param [out] dex 
 *      加密后的输出的数据数组，长度同src
 *  @param [in]  src 
 *      需要加密的数据数组
 *  @param [in]  data_len 
 *      加密数据数组src的长度
 *  @param [in]  k
 *      密钥数组
 *  @param [in]  k_len
 *      密钥数组的长度
 *   
 *  @return  
 *      无
 */

void kp_encode(uint8_t* des, uint8_t* src, uint32_t data_len, uint8_t* k, uint32_t k_len);

/** 
 *  功能描述: 加密函数
 *  @param [out] dex 
 *      解密后的输出的数据数组，长度同src
 *  @param [in]  src 
 *      需要解密的数据数组
 *  @param [in]  data_len 
 *      解密数据数组src的长度
 *  @param [in]  k
 *      密钥数组
 *  @param [in]  k_len
 *      密钥数组的长度
 *   
 *  @return  
 *      如果使用kp_set_api方法且kp_set_api返回值为true, 则此项有效：
 *          返回true为加密成功
 *          返回false为内存不足
 *      否则此项永远返回true。
 */  
bool kp_decode(uint8_t* des, uint8_t* src, uint32_t data_len, uint8_t* k, uint32_t k_len);
    
#ifdef __cplusplus
}
#endif

#endif /* kp_h */
