/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#include "dx_hook.h"
#include "utils.h"
#include <elf.h>
#include "elf_reader.h"

#ifdef __aarch64__
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Addr Elf_Addr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Dyn Elf_Dyn;
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Rela Elf_Rel;
typedef Elf64_Shdr Elf_Shdr;
#else
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Addr Elf_Addr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Dyn Elf_Dyn;
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Rel Elf_Rel;
typedef Elf32_Shdr Elf_Shdr;
#endif

void* utils::mmap(size_t len)
{
    void *buffer = NULL;
    while ( 1 )
    {
        buffer = ::mmap(NULL, 
                    len, 
                    PROT_READ | PROT_WRITE, 
                    MAP_ANON | MAP_PRIVATE, 
                    -1, 
                    0);
        if (buffer != MAP_FAILED) {
            return buffer;
        }
        else if (errno != 4) {
            log(2, "MS:Error:mmap() = %d", errno);
            return NULL;
        }
    }
    
}

bool utils::munmap(void *addr, size_t len)
{
    if (addr == NULL) {
        return true;
    }

    do
    {
        if ( ::munmap(addr, len) != -1 ) {
            return true;
        }
    }
    while (errno == 4);
    return false;
}

void* utils::dx_hook_memcpy(void* src, void* des, size_t size)
{
    uint8_t* s = (uint8_t* )src;
    uint8_t* d = (uint8_t* )des;

    for (size_t i = 0; i < size; ++i, s++, d++)
    {
        *s = *d;
    }
    return s;
}

bool utils::is_addr_executable(uintptr_t va) {
    bool is_executable = false;
    char buff[512] = {0};
    int abi = HKABI_UNKNOWN;

    if (0 == va) {
        return 0;
    }

    FILE* file = fopen("/proc/self/maps", "rt");
    if (file == NULL)
        return 0;

    char flags[4] = {0};
    size_t start = 0, end = 0, offset = 0;
    while (fgets(buff, sizeof buff, file) != NULL) {
        int len = (int )strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';

        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        if (va >= start && va < end) {
            if (flags[2] == 'x') {
                is_executable = true;
            }
            break;
        }
    }
    fclose(file);

    return is_executable;
}

int utils::get_loaded_lib_abi(const char* sofile) {
    char buff[512] = {0};
    int abi = HKABI_UNKNOWN;

    if (NULL == sofile || '\0' == sofile[0]) {
        return 0;
    }
    int libNameLen = strlen(sofile);

    FILE* file = fopen("/proc/self/maps", "rt");
    if (file == NULL)
        return 0;

    char flags[4] = {0};
    size_t addr = 0;
    size_t start = 0, end = 0, offset = 0;
    while (fgets(buff, sizeof buff, file) != NULL) {
        int len = (int )strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';
        if (len <= libNameLen || memcmp(buff + len - libNameLen, sofile, libNameLen))
            continue;
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        if (flags[0] != 'r')
            continue;
        addr = start - offset;
        break;
    }
    fclose(file);

    if (0 != addr) {
        Elf_Ehdr* ehdr = (Elf_Ehdr* )addr;
        switch (ehdr->e_machine)
        {
        case EM_ARM:
            abi = HKABI_ARM;
            break;
        case EM_AARCH64:
            abi = HKABI_ARM64;
            break;
        case EM_386:
            abi = HKABI_I386;
            break;
        case EM_X86_64:
            abi = HKABI_X86_64;
            break;
        default:
            break;
        }

    }

    return abi;
}

int utils::get_curenv_abi(void) {
#if defined(__arm__)
    return HKABI_ARM;
#elif defined(__i386__)
    return HKABI_I386;
#elif defined(__x86_64__)
    return HKABI_X86_64;
#elif defined(__arm64__) || defined(__aarch64__)
    return HKABI_ARM64;
#else
    return HKABI_UNKNOWN;
#endif
}

#if defined(__arm__)
size_t get_module_base(const char *moduleName, pid_t pid = -1) {
    char buff[512] = {0};
    int libNameLen = 0;
    char filename[128] = {0};

    if (0 == moduleName || 0 == (libNameLen = strlen(moduleName))) {
        return 0;
    }

    if (pid < 0) {
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    }
    else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    }

    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return 0;
    }

    char flags[4] = {0};
    size_t addr = 0;
    size_t start, end, offset;
    while (fgets(buff, sizeof buff, file) != NULL)
    {
        int len = (int)strlen(buff);
        if (len > 0 && buff[len-1] == '\n')
            buff[--len] = '\0';
        if (len <= libNameLen || memcmp(buff + len - libNameLen, moduleName, libNameLen))
            continue;
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end, &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7)
            continue;
        if (flags[0] != 'r' || flags[2] != 'x')
            continue;
        addr = start - offset;
        break;
    }
    fclose(file);

    return addr;
}

bool utils::is_hookable(void *addr) {
    static bool is_init = false;
    static bool is_valid = false;
    static void *image = NULL;
    static IElfReader *libc_elf = NULL;

    if (!is_init) {
        image = (void* )get_module_base("/system/lib/libc.so");
        libc_elf = IElfReader_new("/system/lib/libc.so");
        if (NULL != image && NULL != libc_elf && libc_elf->valid()) {
            is_valid = true;
        } else {
            IElfReader_delete(libc_elf);
        }
        is_init = true;
    }

    if (is_valid) {
        uintptr_t sym_size = libc_elf->get_symbol_size((uintptr_t )addr - (uintptr_t )image);
        if (sym_size > 0 && sym_size < 8 + (sym_size & 0x1) * 4) {
            return false;
        }
    }

    return true;
}
#endif
