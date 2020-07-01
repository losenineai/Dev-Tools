#include<string>
#include<map>
#include<vector>
#include <pthread.h>
#ifndef DX_ANTI_ANDROID_ANTI_HOOK_H
#define DX_ANTI_ANDROID_ANTI_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 反调试检测
 * @return
 */
// void *DXAnti_detect_hook(void *arg);

//int DXAnti_detect_hook_async();

int DXAnti_detect_hook_async(pthread_t &tid);

// void DXAnti_detect_hook_entry();


#ifdef __cplusplus
}
#endif

bool checkXposed();

class HashUtils{
public:
    HashUtils():hash(0){};
    uint32_t getHashCode();
    void setHashCode(uint32_t hash);
    uint32_t calHashCode(const uint8_t* mem, size_t size);
private:
    uint32_t hash;

};

class GotHook{
public:
    GotHook(const char* libname);
    GotHook(void* libaddr);
    bool checkHash();
    bool checkIpRange();
    bool initGot();
    void addFun(const char* funcname);
    bool addDepLib(const char* libname);
private:
    std::map<std::string, void* > got_maps;
    std::map<void*, void*> ipRange;
    HashUtils hashUtils;
    void* mbase;
    uint32_t dyn_hashcode;
    bool isInit;
};

class InlineHook{
public:
    InlineHook(const char* libname);
    bool checkhash();
    bool checkfeature();

    void addFun(const char* funcname);
private:
    void* mbase;
    std::map<void*, uint32_t > hash_maps;
    HashUtils hashUtils;
    int checkSize;
};

class HookCheck{
public:
    HookCheck();
    void setLibname(const char* libname);
    bool doCheck();

private:
    std::vector<std::string> funclist;
    std::vector<std::string> depliblist;

    std::vector<std::string> libnamelist;

    std::vector<GotHook> gotCheckList;
    std::vector<InlineHook> inlineCheckList;
    std::map<void*, void*> libsRange;
    void InitAllLibsAddr();
};

#endif