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
#include "dx_hook.h"
#include "memhelper.h"

#include <stdlib.h>
#include <string.h>

void *malloc0(zz_size_t size) {
    void *tmp = (void *)malloc(size);
    memset(tmp, 0, size);
    return tmp;
}

RetStatus ZzRuntimeCodePatch(void *address, void *codedata, unsigned long codedata_size) {
    //    zz_addr_t address_aligned = (zz_addr_t)address & ~(zz_addr_t)1;
    if (!MemoryHelperPatchCode((zz_addr_t)address, codedata, codedata_size))
        return RS_FAILED;
    return RS_SUCCESS;
}