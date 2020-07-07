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

#ifndef SUBSTRATE_H_
#define SUBSTRATE_H_

#include "dx_hook.h"
#include <dlfcn.h>
#include <stdlib.h>

#define _finline \
    inline __attribute__((__always_inline__))
#define _disused \
    __attribute__((__unused__))


//#include <android/log.h>
#ifdef SubstrateInternal
struct SubstrateHookMemory {
    void *address_;
    size_t width_;
    bool is_created_;

    bool SubstrateMemoryCreate(void *data, size_t size);
    void SubstrateMemoryRelease();

    SubstrateHookMemory(void *data, size_t size)
    {
        is_created_ = SubstrateMemoryCreate(data, size);
//        __android_log_print(ANDROID_LOG_ERROR, "dx-native", "is_created_: %d", is_created_);
    }

    ~SubstrateHookMemory() {
        if (is_created_) {
            SubstrateMemoryRelease();
        }
    }
};
#endif

#define MSHook(type, name, args...) \
    _disused static type (*_ ## name)(args); \
    static type $ ## name(args)

#ifdef __cplusplus
#define MSHake(name) \
    &$ ## name, &_ ## name
#else
#define MSHake(name) \
    &$ ## name, (void **) &_ ## name
#endif


#endif//SUBSTRATE_H_
