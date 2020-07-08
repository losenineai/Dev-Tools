#include "HookUtils.h"
#include <string.h>
#include <stdlib.h>

#ifdef DX_RUN_ON_32
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Addr Elf_Addr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Dyn Elf_Dyn;
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Rel Elf_Rel;
typedef Elf32_Shdr Elf_Shdr;
#else
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Addr Elf_Addr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Dyn Elf_Dyn;
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Rela Elf_Rel;
typedef Elf64_Shdr Elf_Shdr;
#endif

struct MySoinfo{
    void* base;
    Elf_Ehdr *ehdr;
    Elf_Phdr *phdr;
    Elf_Dyn *dynamic;
    Elf_Sym *symtab;
    const char* strtab;
    Elf_Rel *plt_rel;
    size_t plt_rel_count;
};

void* get_library_address(const char*  libname)
{
    char path[256];
    char buff[256];
    FILE* file;
    size_t  addr = 0;

    snprintf(path, sizeof(path), "/proc/%d/maps", getpid());
    file = fopen(path, "rt");
    if (file == NULL)
        return 0;

    while (fgets(buff, sizeof buff, file) != NULL) {
        size_t  len = strlen(buff);
        if (len > 0 && buff[len-1] == '\n') {
            buff[--len] = '\0';
        }

        if (strstr(buff, libname) == NULL)
            continue;

        size_t start, end, offset;
        char flags[4];
        if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end,
                   &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7) {
            continue;
        }
        LOGI("line:%s", buff);
        if (flags[0] != 'r') {
            continue;
        }
        addr = start;
        break;
    }

    fclose(file);
    return (void*)addr;
}

uint32_t findDex(void* start, void* end, void *&addr){
    LOGI("in findDex start");

    if (start == NULL || end == NULL || start >= end)
        return 0;

    uint32_t *lookupPtr = (uint32_t*)start;

    bool isFound = false;

    while(lookupPtr < end){
        if(*lookupPtr == 0x3D5A3E1D){
            lookupPtr++;
            if (*lookupPtr == 0x3A5D3E7D){
                LOGI("src data magic found!");
                isFound = true;
                break;
            }
        }else {
            lookupPtr++;
        }
    }

    uint32_t dataLen = 0;

    if (isFound){
        lookupPtr++;
        dataLen = *lookupPtr;
        lookupPtr++;
        LOGI("dataLen:%x dataAddr:%p tmpAddr:%p", dataLen, lookupPtr, addr);
        addr = lookupPtr;
        char hex[256] = {0};
        getHex(hex, 256, addr, 128);
        LOGI("addr:%08x hex:%s", addr, hex);
    }

    LOGI("** in findDex dataLen:%d", dataLen);

    return dataLen;
}

void* get_dex_in_mem(const char *dexName, const char *pkgName, uint32_t &length)
{
    char path[256];
    char buff[256];

    snprintf(path, sizeof(path), "/proc/%d/maps", getpid());
    FILE *file = fopen(path, "rt");
    if (file == NULL)
        return 0;

    bool isfound = false;

    void * dexAddr = NULL;
    while (fgets(buff, sizeof(buff), file) != NULL) {
        int  len = strlen(buff);
        if (len > 0 && buff[len-1] == '\n') {
            buff[--len] = '\0';
        }

        if (!strstr(buff, pkgName)){
            continue;
        }

        if (strstr(buff, dexName)){
            isfound = true;
        }
        else if (strstr(buff, "base.apk") && strstr(buff, "/dev/ashmem/dalvik-classes")){
            isfound = true;
        }
        LOGI("line:%s", buff);
        if (isfound){
            size_t start  = 0,end = 0, offset = 0;
            char flags[4];
            if (sscanf(buff, "%zx-%zx %c%c%c%c %zx", &start, &end,
                       &flags[0], &flags[1], &flags[2], &flags[3], &offset) != 7) {
                continue;
            }
            if (flags[0] != 'r') {
                continue;
            }

            LOGI("in get_dex_in_mem buff:%s start:%p end:%p", buff, start, end);
            
            length = findDex((void*)start, (void*)end, dexAddr);
            
            LOGI("in get_dex_in_mem buff:%s start:%p end:%p addr:%p", buff, start, end, dexAddr);

            if(((length) > 0) && (dexAddr != NULL)){
                break;
            }

            isfound = false;
        }

    }
    fclose(file);
    return dexAddr;
}

void* getTargetSI(const char *soname) {
    void *handle = get_library_address(soname);
    if (!handle) {
        return 0;
    }

    Elf_Ehdr *ehdr = (Elf_Ehdr *) handle;
    void *base = handle;
    Elf_Phdr *phdr = (Elf_Phdr *) ((char*)base + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            base = (void *) ((char *) base - phdr[i].p_vaddr);
            break;
        }
    }

    void *dynamicAddr = 0;
    for (int i = 0; i < ehdr->e_phnum; i++) {
    	if (phdr[i].p_type == PT_DYNAMIC) {
            dynamicAddr = (char *) base + phdr[i].p_vaddr;
            LOGI("dynamicAddr: %p", dynamicAddr);
            break;
        }
    }

	Elf_Dyn *d;
    Elf_Dyn *dynamic = (Elf_Dyn*) dynamicAddr;
    Elf_Sym *symtab = 0;
    const char *strtab = 0;
    Elf_Rel *plt_rel = 0;
    size_t plt_rel_count = 0;

    for (d = dynamic; d->d_tag != DT_NULL; ++d) {
        switch (d->d_tag) {
            case DT_SYMTAB:
                symtab = (Elf_Sym*)(d->d_un.d_ptr + (char*)base);
                break;
            case DT_STRTAB:
                strtab = (char*)(d->d_un.d_ptr + (char*)base);
                break;
            case DT_JMPREL:
                plt_rel = (Elf_Rel*)(d->d_un.d_ptr + (char*)base);
                break;
            case DT_PLTRELSZ:
                plt_rel_count = d->d_un.d_val / sizeof(Elf_Rel);
                break;
            default:
                printf("");
        }
    }

    MySoinfo* si = (MySoinfo*)malloc(sizeof(MySoinfo));
    si->base = base;
    si->ehdr = ehdr;
    si->phdr = phdr;
    si->dynamic = dynamic;
    si->symtab = symtab;
    si->strtab = strtab;
    si->plt_rel = plt_rel;
    si->plt_rel_count = plt_rel_count;

    return (void*)si;
}

void replaceFunc(void *handle, const char *name, void *pNewFun, void **pOldFun) {

	MySoinfo* si = (MySoinfo*) handle;

	Elf_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;

    Elf_Rel *rel = si->plt_rel;
    size_t count = si->plt_rel_count;

	unsigned long idx;
    int fit = 0;
	void* base = 0;

    base = si->base;

    for (idx = 0; idx < count; idx++) {
#ifdef DX_RUN_ON_32
        uint32_t sym = ELF32_R_SYM(rel->r_info);
#else
        uint64_t sym = ELF64_R_SYM(rel->r_info);
#endif
    	void* reloc = (void*) (rel->r_offset + (size_t)base);
    	char *sym_name = (char *) (strtab + symtab[sym].st_name);
    	if (strcmp(sym_name, name) == 0) {
    		size_t page_size = (size_t)getpagesize();
    		size_t entry_page_start = (unsigned long)reloc & (~(page_size - 1));
    		mprotect((size_t *) entry_page_start, page_size,
    				PROT_READ | PROT_WRITE);

    		*pOldFun = (void *) *((unsigned long*) reloc); //pOldFun = &g_OriginalFunc *pOldFun = g_OriginalFunc
    		*((unsigned long*) reloc) = (unsigned long) pNewFun;
    		fit = 1;
    		break;
    	}

    	rel++;
    }

    if (!fit) {
    	LOGE("replaceFunc function not find");
    }
}