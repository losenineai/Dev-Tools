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
#include "memory-linux.h"

zz_size_t MemoryHelperGetPageSize() { return zz_posix_vm_get_page_size(); }

zz_ptr_t MemoryHelperAllocatePage(zz_size_t n_pages) { return zz_posix_vm_allocate_pages(n_pages); }

zz_ptr_t MemoryHelperAllocateNearPage(zz_addr_t address, zz_size_t redirect_range_size, zz_size_t n_pages) {
    return zz_posix_vm_allocate_near_pages(address, redirect_range_size, n_pages);
}

zz_ptr_t MemoryAllocate(zz_size_t size) { return zz_posix_vm_allocate(size); }

bool MemoryHelperPatchCode(const zz_addr_t address, const zz_ptr_t codedata, zz_size_t codedata_size) {
    return zz_posix_vm_patch_code(address, codedata, codedata_size);
}

bool MemoryHelperProtectAsExecutable(const zz_addr_t address, zz_size_t size) {

    return zz_posix_vm_protect_as_executable(address, size);
}

bool MemoryHelperProtectAsWritable(const zz_addr_t address, zz_size_t size) {
    return zz_posxi_vm_protect_as_writable(address, size);
}

zz_ptr_t MemoryHelperSearchCodeCave(zz_addr_t address, zz_size_t redirect_range_size, zz_size_t size) {
    // return zz_darwin_vm_search_text_code_cave_via_dylibs(address, redirect_range_size, size);
    return zz_linux_vm_search_code_cave(address, redirect_range_size, size);
}

bool MemoryHelperIsSupportAllocateRXMemory() { return TRUE; }