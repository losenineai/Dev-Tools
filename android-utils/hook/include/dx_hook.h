/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 * Description: 
        This module is used for hooking function.
        And it supports armv7, arm64, x86 and x86_64 architectures.
 *************************************/
#ifndef dx_hook_h
#define dx_hook_h

#include "dx_hook_config.h"
#include <stdbool.h>

typedef enum _DXRetStatus {
    DX_UNKOWN = -1,
    DX_DONE = 0,
    DX_SUCCESS,
    DX_FAILED,
    DX_DONE_HOOK,
    DX_DONE_INIT,
    DX_DONE_ENABLE,
    DX_ALREADY_HOOK,
    DX_ALREADY_INIT,
    DX_ALREADY_ENABLED,
    DX_NEED_INIT,
    DX_NO_BUILD_HOOK,
    DX_ABI_NOT_MATCH = 0x1000,
    DX_SIZE_NOT_ENOUGH
} DXRetStatus;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
* NAME: 
    dx_hook_fun

* DESCRIPTION
    This function is used for install hook.

* PARAMS:
    param[IN] symbol
        This is the source function address. It will jump to the replace function after calls dx_hook_fun function.
    param[IN] replace: 
        This is the replace replace address. After calling dx_hook_fun function, replace function will be called when someone calls the source function.
        used for replace 
    param[OUT] origin:
        This is a variable used for saving orignal function address. After calling dx_hook_fun function, you can call the original function by the origin variable.

* RETURN VALUES
    The dx_hook_fun function return DX_DONE or DX_SUCCESS on success or others on failure.
 *******************************************************************************/
DXRetStatus dx_hook_hookfun(void *symbol, void *replace, void **origin);

/********************************************************************************
* NAME: 
    dx_hook_fun

* DESCRIPTION
    This function is used for printing the debug log.

* PARAMS:
    param[IN] is_debug
        When is_debug is true, print the debug log; Otherwise, no printing.
 *******************************************************************************/
void dx_hook_debug(bool is_debug);

/********************************************************************************
* NAME: 
    dx_hook_make_as_system_caller

* DESCRIPTION
    This function must be called before [dx_hook_hookfun], when the lib is not called by apk.
 *******************************************************************************/
void dx_hook_make_as_system_caller(void);

#ifdef __cplusplus
}
#endif

#endif // dx_hook_h
