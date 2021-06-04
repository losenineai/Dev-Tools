/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 * Description: 
        This module is used for hooking function.
        And it supports armv7, arm64, x86 and x86_64 architectures.
 *************************************/
#ifndef dx_hook_h
#define dx_hook_h

#include <stdbool.h>

#define dx_hook_hookfun                 IiO1llgLolI99IlO0oLilioiO9gIo1II
#define dx_hook_debug                   IOI1l0IOgoI0II11i0ooLLo190g1llIL

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
    DX_NO_BUILD_HOOK
} DXRetStatus;

#ifdef __cplusplus
extern "C" {
#endif

DXRetStatus dx_hook_hookfun(void *symbol, void *replace, void **origin);

void dx_hook_debug(bool is_debug);

#ifdef __cplusplus
}
#endif

#endif // dx_hook_h
