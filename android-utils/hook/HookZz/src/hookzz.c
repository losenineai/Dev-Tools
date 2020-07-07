
#include "hookzz.h"

DXRetStatus dx_hook_hookfun(void *symbol, void *replace, void **origin) {
    return (DXRetStatus )ZzHookReplace(symbol, replace, origin);
}

void dx_hook_debug(bool is_debug) {

}

void dx_hook_make_as_system_caller(void) {

}
