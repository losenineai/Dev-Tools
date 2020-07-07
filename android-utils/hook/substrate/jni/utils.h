/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#ifndef utils_h
#define utils_h


#include "CydiaSubstrate.h"
#include <sys/mman.h>

#define _trace() do { \
    MSLog(MSLogLevelNotice, "_trace(%u)", __LINE__); \
} while (false)

#if defined(__i386__) || defined(__x86_64__)
#include "hde64.h"
#endif

#include "SubstrateDebug.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "SubstrateARM.hpp"
#include <stddef.h>

enum HOOK_ABI {
    HKABI_UNKNOWN = 0,
    HKABI_ARM,
    HKABI_I386,
    HKABI_X86_64,
    HKABI_ARM64
};

#define log(...)
class utils
{
public:
    static void *mmap(size_t len);
    static bool munmap(void *addr, size_t len);
    static void* dx_hook_memcpy(void* src, void* des, size_t size);

    // HOOK库环境ABI？
    static int get_curenv_abi(void);

    // libc的ABI？
    static int get_loaded_lib_abi(const char* sofile);

    // 目标地址有x属性？
    static bool is_addr_executable(uintptr_t addr);

#if defined(__arm__)
    // 止标地址是否可被 hook
    static bool is_hookable(void *addr);
#endif
};

#endif // utils_h