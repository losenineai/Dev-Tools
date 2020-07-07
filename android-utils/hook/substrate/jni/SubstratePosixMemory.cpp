/* Cydia Substrate - Powerful Code Insertion Platform
 * Copyright (C) 2008-2011  Jay Freeman (saurik)
*/

/* GNU Lesser General Public License, Version 3 {{{ */
/*
 * Substrate is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Substrate is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Substrate.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */

#define SubstrateInternal
#include "dx_hook.h"
#include "CydiaSubstrate.h"
#include "SubstrateLog.hpp"

#include <sys/mman.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
//#include <android/log.h>

extern "C" void __clear_cache (void *beg, void *end);

bool SubstrateHookMemory::SubstrateMemoryCreate(void *data, size_t size) {
    if (size == 0)
        return false;

    long page(sysconf(_SC_PAGESIZE)); // Portable applications should employ sysconf(_SC_PAGESIZE) instead of getpagesize

//    __android_log_print(ANDROID_LOG_ERROR, "dx-native", "SubstrateMemoryCreate(%p, %ld)", data, size);
    uintptr_t base = (uintptr_t )data / page * page;
    size_t width = (((uintptr_t )data + size - 1) / page + 1) * page - base;
    void *address = (void* )base;

//    __android_log_print(ANDROID_LOG_ERROR, "dx-native", "SubstrateMemoryCreate-mprotect(%p, %d)", address, width);
    if (mprotect(address, width, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        // MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);
        return false;
    }

    address_ = address;
    width_ = width;
    return true;
}

void SubstrateHookMemory::SubstrateMemoryRelease() {
//    __android_log_print(ANDROID_LOG_ERROR, "dx-native", "SubstrateMemoryRelease-mprotect(%p, %d)", address_, width_);
    if (mprotect(address_, width_, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        // MSLog(MSLogLevelError, "MS:Error:mprotect() = %d", errno);
	}

//    __android_log_print(ANDROID_LOG_ERROR, "dx-native", "clear-cache[%p, %p]", address_, (char* )address_ + width_);
    __clear_cache((char* )address_, (char* )address_ + width_);
}
