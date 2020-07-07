/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 * Description: 
        This module is used for hooking function.
        And it supports armv7, arm64, x86 and x86_64 architectures.
 *************************************/
#ifndef needle_h
#define needle_h

typedef enum _RetStatus {
    ND_UNKOWN = -1,
    ND_DONE = 0,
    ND_SUCCESS,
    ND_FAILED,
    ND_DONE_HOOK,
    ND_DONE_INIT,
    ND_DONE_ENABLE,
    ND_ALREADY_HOOK,
    ND_ALREADY_INIT,
    ND_ALREADY_ENABLED,
    ND_NEED_INIT,
    ND_NO_BUILD_HOOK
} RetStatus;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
* NAME: 
    nd_hook

* DESCRIPTION
    This function is used for install hook.

* PARAMS:
    param[IN] symbol
        This is the source function address. It will jump to the replace function after calls nd_hook function.
    param[IN] replace: 
        This is the replace replace address. After calling nd_hook function, replace function will be called when someone calls the source function.
        used for replace 
    param[OUT] origin:
        This is a variable used for saving orignal function address. After calling nd_hook function, you can call the original function by the origin variable.

* RETURN VALUES
    The nd_hook function return ND_DONE or ND_SUCCESS on success or others on failure.
 *******************************************************************************/
RetStatus nd_hook(void *symbol, void *replace, void **origin);

/********************************************************************************
* NAME: 
    nd_debug

* DESCRIPTION
    This function is used for printing the debug log.

* PARAMS:
    param[IN] is_debug
        When is_debug is true, print the debug log; Otherwise, no printing.
 *******************************************************************************/
void nd_debug(bool is_debug);

#ifdef __cplusplus
}
#endif

#endif // needle_h
