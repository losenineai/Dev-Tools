/**
 *    Copyright 2017 jmpews
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "dx_hook_config.h"
#include "regs-arm.h"

void arm_register_describe(ARMReg reg, ARMRegInfo *ri) {
    if (reg >= ARM_REG_R0 && reg <= ARM_REG_R12) {
        ri->width = 32;
        ri->meta  = reg;
    } else if (reg == ARM_REG_SP) {
        ri->width = 32;
        ri->meta  = reg;
    } else if (reg == ARM_REG_LR) {
        ri->width = 32;
        ri->meta  = reg;
    } else if (reg == ARM_REG_PC) {
        ri->width = 32;
        ri->meta  = reg;
    } else {
        ERROR_LOG_STR("arm64_register_describe error.");
#if defined(DEBUG_MODE)
        debug_break();
#endif
        ri->index = 0;
    }
    ri->index = reg - ARM_REG_R0;
}
