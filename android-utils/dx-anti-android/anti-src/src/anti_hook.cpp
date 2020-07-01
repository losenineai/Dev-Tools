
#include "anti_hook.h"
#include "dx_libc.h"
#include "anti_utils.h"
#include "anti_result_handler.h"
#include "utils_anti_log.h"
#include "anti_crash_log_prefix.h"
#include "jni_utils.h"
#include "jni_comm_utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void* DXAnti_detect_hook(void* arg){
    HookCheck hookCheck;
//    hookCheck.setLibname("libDxAntiTest.so");
    while(true){
        bool ret = hookCheck.doCheck() || checkXposed();

        anti_info_log("hook check:%d", ret);
        if (ret) {
            // 统一anti结果处理
            AntiResult result = {AntiType::AT_HOOK, ANTI_DEBUG_ERR_HOOK};
            AntiResultHandler::getInstance()->HandleResult(&result);
//            break;
        }
        dx_sleep(10);
    }

    return NULL;
}


int DXAnti_detect_hook_async(pthread_t &tid){
    int ret = -1;

    dx_anti_pthread_create(ret, &tid, NULL, DXAnti_detect_hook, NULL);

    return ret;
}


STEE
jobject getSystemClassLoader_loadClass(JNIEnv* env, const char* name){
    jclass class_obj = anti_jni_findClassByName(env, "java/lang/ClassLoader");
    if(class_obj == NULL){
        anti_info_log("class obj is NULL");
        return nullptr;
    }

    jobject obj = anti_jni_callStaticMethodReturnObj(env, class_obj, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

    if(obj == NULL){
        anti_info_log("obj is NULL");
        return nullptr ;
    }

    jstring jstr = anti_jni_string_to_jstring(env, name);

    obj = anti_jni_callMethodByName(env, obj, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", jstr);

    if(obj == NULL){
        anti_info_log("2 obj is NULL");
        return nullptr;
    }
    return obj;
}


STEE
bool checkXposed(){

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    anti_info_log("check xposed");

    jobject obj = getSystemClassLoader_loadClass(env, "de.robv.android.xposed.XposedBridge");

    if(obj == NULL){
        anti_info_log("2 obj is NULL");
        return false;
    }

    jobjectArray jobjarray = (jobjectArray)anti_jni_callMethodByName(env, obj, "getDeclaredFields", "()[Ljava/lang/reflect/Field;");

    if(jobjarray == NULL){
        anti_info_log("jobjarray is NULL");
        return false;
    }

    jint element_legnth = env->GetArrayLength(jobjarray);

    int flag = -1;
    jobject field_hookedMethodCallbacks = nullptr;

    for(int i=0; i<(int)element_legnth; i++){
        jobject field = env->GetObjectArrayElement(jobjarray, i);

        if(field == nullptr){
            return false;
        }

        jstring m_jstr = (jstring)anti_jni_callMethodByName(env, field, "getName", "()Ljava/lang/String;");

        if(m_jstr == nullptr){
            return false;
        }

        string m_str = anti_jni_jstring_to_string(env, m_jstr);

        if(m_str == "sHookedMethodCallbacks"){
            field_hookedMethodCallbacks = field;
            flag = 0;
            break;
        }
        if(m_str == "hookedMethodCallbacks"){
            field_hookedMethodCallbacks = field;
            flag = 1;
            break;
        }

    }

    if(field_hookedMethodCallbacks != nullptr){
        anti_info_log("begin anti_jni_callVoidMethodByName");
        jboolean m_jbool = JNI_TRUE;
        anti_jni_callVoidMethodByName(env, field_hookedMethodCallbacks, "setAccessible", "(Z)V", m_jbool);

        anti_info_log("after anti_jni_callVoidMethodByName");
        jobject meth_getSnapshot = nullptr;
        jobject clz_copyOnWriteSortedSet = nullptr;
        jobject hookedMethodCallbacks = anti_jni_callMethodByName(env, field_hookedMethodCallbacks, "get", "(Ljava/lang/Object;)Ljava/lang/Object;",
                                                                  nullptr);
        if(hookedMethodCallbacks == nullptr){
            anti_info_log("hooked method callbacks is NULL");
            return false;
        }

        if(flag == 0){
            clz_copyOnWriteSortedSet = getSystemClassLoader_loadClass(env, "de.robv.android.xposed.XposedBridge$CopyOnWriteSortedSet");
            if(clz_copyOnWriteSortedSet != nullptr){
                jstring m_jstr = anti_jni_string_to_jstring(env, "getSnapshot");

                meth_getSnapshot = anti_jni_callMethodByName(env, clz_copyOnWriteSortedSet,
                                                             "getDeclaredMethod",
                                                             "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;",
                                                             m_jstr, nullptr);

                if(meth_getSnapshot == nullptr){
                    anti_info_log("meth_getSnapshot is NULL");
                }

            }
        }

        jobject map_set = anti_jni_callMethodByName(env, hookedMethodCallbacks, "entrySet", "()Ljava/util/Set;");

        if(map_set == nullptr){
            return false;
        }

        jobject it = anti_jni_callMethodByName(env, map_set, "iterator", "()Ljava/util/Iterator;");

        if(it == nullptr){
            return false;
        }

        while(anti_jni_callBoolMethodByName(env, it, "hasNext", "()Z") == JNI_TRUE){
            jobject entry = anti_jni_callMethodByName(env, it, "next", "()Ljava/util/Map$Entry;");
            jobject hookMethod = anti_jni_callMethodByName(env, entry, "getKey", "()Ljava/lang/Object;");
            jobject callbacks = anti_jni_callMethodByName(env, entry, "getValue", "()Ljava/lang/Object;");

            jobjectArray snaps = nullptr;

            if(hookMethod == nullptr){
                anti_info_log("hook method is nullptr");
                break;
            }
            if(callbacks == nullptr){
                anti_info_log("callbacks is nullptr");
                break;
            }


            if(clz_copyOnWriteSortedSet != nullptr ){
                jboolean is_instance = anti_jni_callBoolMethodByName(env,
                                                                     clz_copyOnWriteSortedSet,
                                                                     "isInstance",
                                                                     "(Ljava/lang/Object;)Z",
                                                                     callbacks);
                if(is_instance == JNI_TRUE){
                    snaps = (jobjectArray)anti_jni_callMethodByName(env,
                                                                    meth_getSnapshot,
                                                                    "invoke",
                                                                    "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;",
                                                                    callbacks,
                                                                    nullptr);
                }
            }else{
                jstring m_jstr = anti_jni_string_to_jstring(env, "java.util.TreeSet");
                jclass m_jclass = anti_jni_findClassByName(env, "java/lang/Class");
                if(m_jstr == nullptr || m_jclass == nullptr){
                    return false;
                }
                jobject m_obj = anti_jni_callStaticMethodReturnObj(env,
                                                                   m_jclass,
                                                                   "forName",
                                                                   "(Ljava/lang/String;)Ljava/lang/Class;",
                                                                   m_jstr);

                jboolean is_instance = anti_jni_callBoolMethodByName(env,
                                                                     m_obj,
                                                                     "isInstance",
                                                                     "(Ljava/lang/Object;)Z",
                                                                     callbacks);
                if(is_instance == JNI_TRUE){
                    snaps = (jobjectArray)anti_jni_callMethodByName(env,
                                                                    callbacks,
                                                                    "toArray",
                                                                    "()[Ljava/lang/Object;");
                }
                anti_info_log("is TreeSet instance : %d", is_instance);
            }

            if(snaps == nullptr){
                return false;
            }

            jint snaps_legnth = env->GetArrayLength(snaps);

            anti_info_log("snap_length : %d", (int)snaps_legnth);

            for(int i=0; i<snaps_legnth; i++){
                jobject m_obj = env->GetObjectArrayElement(snaps, i);
                if(m_obj == nullptr){
                    return false;
                }
                m_obj = anti_jni_callMethodByName(env, m_obj, "getClass", "()Ljava/lang/Class;");

                if(m_obj == nullptr){
                    return false;
                }
                m_obj = anti_jni_callMethodByName(env, m_obj, "getClassLoader", "()Ljava/lang/ClassLoader;");
                if(m_obj == nullptr){
                    return false;
                }
                m_obj = anti_jni_callMethodByName(env, m_obj, "toString", "()Ljava/lang/String;");
                if(m_obj == nullptr){
                    return false;
                }
                string str = anti_jni_jstring_to_string(env, (jstring)m_obj);

                anti_info_log("=======module name : %s", str.c_str());

                //存在不包含"XposedBridge.jar"字符串的string，则为被hook。
                //"/data/app/com.wi.xposedDemo-HoG3hPf-TE71MJLd_tYUcg==/base.apk"
                if(str.find("XposedBridge.jar") == string::npos && str.find("base.apk") != string::npos){
                    return true;
                }
            }
        }
    }
    return false;
}

HookCheck::HookCheck() {
    //Got hook check
    this->funclist.push_back("fopen");
    this->funclist.push_back("strlen");
    this->funclist.push_back("strcmp");
    this->funclist.push_back("malloc");
    this->funclist.push_back("sprintf");
    this->funclist.push_back("mmap");
    this->funclist.push_back("fwrite");
    this->funclist.push_back("pthread_create");
    this->funclist.push_back("exit");
    this->funclist.push_back("fgets");
    this->funclist.push_back("mprotect");
    this->funclist.push_back("usleep");
    this->funclist.push_back("fseeko");
    this->funclist.push_back("fcntl");
    this->funclist.push_back("sscanf");
    this->funclist.push_back("strchr");
    this->funclist.push_back("mkdir");
    this->funclist.push_back("access");
    this->funclist.push_back("gettid");
    this->funclist.push_back("fprintf");
    this->funclist.push_back("__openat");
    this->funclist.push_back("fork");
    this->funclist.push_back("open");
    this->funclist.push_back("write");
    this->funclist.push_back("read");
    this->funclist.push_back("fread");
    this->funclist.push_back("__system_property_get");
//    this->funclist.push_back("dlsym");
//    this->funclist.push_back("dlopen");
//    this->funclist.push_back("dlclose");

    this->depliblist.push_back("libc.so");

    InitAllLibsAddr();
}

void HookCheck::setLibname(const char *libname) {
    this->libnamelist.push_back(libname);
}

void tmpFunc(){ };
typedef void (*fun)();

bool HookCheck::doCheck() {
    if(this->gotCheckList.size()<=0) {
        if(this->libnamelist.size() > 0){
            for(auto libname : this->libnamelist){
                std::string name = (std::string)libname;
                GotHook gotHook(name.c_str());
                for(auto funname : this->funclist){
                    gotHook.addFun(funname.c_str());
                }
                for(auto deplib : this->depliblist){
                    gotHook.addDepLib(deplib.c_str());
                }

                this->gotCheckList.push_back(gotHook);
            }
        }else if(this->libsRange.size() > 0){
            for(auto libRange : this->libsRange){
                fun funcAddr = tmpFunc;
                void* curaddr = (void*)funcAddr;
                anti_info_log("funcaddr:%p-->[%p, %p]", curaddr, libRange.first, libRange.second);
                if(libRange.first < curaddr && libRange.second > curaddr){
                    GotHook gotHook(libRange.first);
                    for(auto funname : this->funclist){
                        gotHook.addFun(funname.c_str());
                    }
                    for(auto deplib : this->depliblist){
                        gotHook.addDepLib(deplib.c_str());
                    }

                    this->gotCheckList.push_back(gotHook);
                    break;
                }
            }

        }
    }

    if(this->inlineCheckList.size()<=0 && this->depliblist.size() > 0){
        for(auto libname : this->depliblist){
            std::string name = (std::string)libname;
            InlineHook inlineCheck(name.c_str());
            for(auto funname : this->funclist){
                inlineCheck.addFun(funname.c_str());
            }
            this->inlineCheckList.push_back(inlineCheck);
        }
    }
    bool isHook = false;
    for(std::vector<GotHook>::iterator gotCheck_iter = this->gotCheckList.begin();
        gotCheck_iter < this->gotCheckList.end(); gotCheck_iter++){
        if(gotCheck_iter->checkHash()) {
            anti_info_log("gotCheck hash");
            isHook = true;
        }
        if(gotCheck_iter->checkIpRange()){
            anti_info_log("gotCheck ipRange");
            isHook = true;
        }
    }

    for(std::vector<InlineHook>::iterator inlineCheck_iter = this->inlineCheckList.begin();
    inlineCheck_iter < this->inlineCheckList.end(); inlineCheck_iter++){
        if(inlineCheck_iter->checkfeature()){
            anti_info_log("inlineCheck feature");
            isHook = true;
        }
        if(inlineCheck_iter->checkhash()){
            anti_info_log("inlineCheck hash");
            isHook = true;
        }
    }


    return isHook;
}


void HookCheck::InitAllLibsAddr(){
    int fd_maps = dx_open("/proc/self/maps", O_RDONLY);
    if(fd_maps <=0 ) {
//        __android_log_print(ANDROID_LOG_ERROR, tag, "failed to open maps");
        return;
    }

    int buffsize = 100000;
    char* buff = (char*)malloc(buffsize);
    if(buff == NULL){
        dx_close(fd_maps);
        return;
    }
//    void* module_load_addr = 0;
    char* lib_load_addr = 0;
    char* lib_end_addr = 0;
    char eachline[256] = {0};

    int retSize = 0;
    int ret = dx_read(fd_maps, buff, buffsize);
    retSize = ret;
    int expand_size = 200000;
    int times = 0;
    // LOGD("========retSize:%d, buffsize:%d\n", retSize, buffsize);
    while(ret > 0){
        if(retSize + expand_size > buffsize){
            buff = (char*)realloc(buff, buffsize+expand_size);
            buffsize += expand_size;
        }
        times++;
        ret = dx_read(fd_maps, buff+retSize, buffsize-retSize);
        retSize += ret;
    }
    // LOGD("+++++++++retSize:%d, buffsize:%d\n", retSize, buffsize);
    if(retSize > 0) {
        for(int i=0, j=0; i<retSize; i++){
            eachline[j] = buff[i];
            if(j >= sizeof(eachline)-1 || eachline[j] == '\n'){
                eachline[j+1] = '\0';
                j=0;
//                 LOGD("eachline:%s", eachline);

                if (dx_strstr(eachline, "   /data/") && dx_strstr(eachline, ".so")) {
//                    LOGD("find so :%s", eachline);
                    //just get the first line, which includes the base address.
                    lib_load_addr = 0;
                    lib_end_addr = 0;
                    anti_info_log("eachline:%s", eachline);
                    if(dx_sscanf(eachline, "%p-%p", &lib_load_addr, &lib_end_addr) == 2) {
                        anti_info_log("load_addr:%p, end_addr:%p", lib_load_addr, lib_end_addr);
                        if (lib_load_addr[0] == 0x7f && lib_load_addr[1] == 0x45 &&
                            lib_load_addr[2] == 0x4c && lib_load_addr[3] == 0x46) {
//                            module_load_addr = lib_load_addr;
#ifdef __arm__
                            Elf32_Ehdr* ehdr = (Elf32_Ehdr*)lib_load_addr;
                            if(ehdr->e_machine != EM_ARM){
                                continue;
                            }
#elif __i386__
                            Elf32_Ehdr* ehdr = (Elf32_Ehdr*)lib_load_addr;
                            if(ehdr->e_machine != EM_386){
                                continue;
                            }
#elif __aarch64__
                            Elf64_Ehdr* ehdr = (Elf64_Ehdr*)lib_load_addr;
                            if(ehdr->e_machine != EM_AARCH64){
                                continue;
                            }
#elif __x86_x64__
                            Elf64_Ehdr* ehdr = (Elf64_Ehdr*)lib_load_addr;
                            if(ehdr->e_machine != EM_X86_64){
                                continue;
                            }
#endif
//                            LOGD("============");
                            libsRange.insert(std::make_pair((void*)lib_load_addr, (void*)lib_end_addr));
                        }
                    }
                }
            }else{
                j++;
            }
        }
    }
    free(buff);
    dx_close(fd_maps);

}


GotHook::GotHook(const char *libname) {
    this->mbase = get_module_addr(libname);
    this->dyn_hashcode = 0;
    this->isInit = false;
}

GotHook::GotHook(void *libaddr) {
    this->mbase = libaddr;
    this->dyn_hashcode = 0;
    this->isInit = false;
}

void GotHook::addFun(const char *funcname) {
    if(this->mbase != NULL){
        this->got_maps.insert(std::make_pair(funcname, (void*)NULL));
    }
}

bool GotHook::addDepLib(const char *libname) {
    void* lib_base = get_module_addr(libname);
    if(lib_base == NULL){
        return false;
    }
    if(sizeof(void*) == 8){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)lib_base;
        Elf64_Phdr* phdr = (Elf64_Phdr*)(ehdr->e_phoff + (uint8_t*)lib_base);
        for(int i=0; i<ehdr->e_phnum; i++){
            if(phdr[i].p_type == PT_LOAD && phdr[i].p_memsz != 0){
                this->ipRange.insert(std::make_pair((void*)lib_base, (void*)((uint64_t)phdr[i].p_memsz+(uint8_t*)lib_base)));
                return true;
            }
        }
    }else{
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)lib_base;
        Elf32_Phdr* phdr = (Elf32_Phdr*)(ehdr->e_phoff + (uint8_t*)lib_base);
        for(int i=0; i<ehdr->e_phnum; i++){
            if(phdr[i].p_type == PT_LOAD && phdr[i].p_memsz != 0){
//                anti_info_log("iprange:[%x, %x]", (uint32_t)lib_base, (uint32_t)phdr[i].p_memsz+(uint32_t)lib_base);
                this->ipRange.insert(std::make_pair((void*)lib_base, (void*)((uint64_t)phdr[i].p_memsz+(uint8_t*)lib_base)));
                return true;
            }
        }
    }
    return false;
}

bool GotHook::checkHash() {
    bool isHook = false;
    if(this->mbase == NULL){
        return isHook;
    }
    //判断dynamic段是否被修改。
    uint8_t* dyn_mem = NULL;
    size_t dyn_size = 0;
    std::map <void*, uint32_t>::iterator hash_iter;

    if((sizeof(void*) == 8)){
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)this->mbase;
        int phnum = ehdr->e_phnum;
        Elf64_Phdr* phdr = (Elf64_Phdr*)((uint8_t*)this->mbase + ehdr->e_phoff);
        for(int i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dyn_size = phdr[i].p_memsz;
                dyn_mem = (uint8_t*)this->mbase + phdr[i].p_vaddr;
            }
        }
    }else{
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)this->mbase;
        int phnum = ehdr->e_phnum;
        Elf32_Phdr* phdr = (Elf32_Phdr*)((uint8_t*)this->mbase + ehdr->e_phoff);
        for(int i=0; i<phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dyn_size = phdr[i].p_memsz;
                dyn_mem = (uint8_t*)this->mbase + phdr[i].p_vaddr;
            }
        }
    }
    uint32_t hash = hashUtils.calHashCode(dyn_mem, dyn_size);

    if(this->dyn_hashcode == 0){
        this->dyn_hashcode = hash;
    }else{
        if(hash != this->dyn_hashcode){
            isHook = true;
        }
    }

    return isHook;
}

bool GotHook::initGot(){
    bool success = false;
    if(this->mbase == NULL){
        return success;
    }
    if(sizeof(void*) == 8){
        //arm64, x86_64使用Elf32_Rela结构体。
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)this->mbase;
        Elf64_Phdr* phdr = (Elf64_Phdr*)(ehdr->e_phoff + (uint8_t*)this->mbase);
        void* dynamic = NULL;
//    LOGD("ehdr addr:%p, phdr addr:%p, phnum:%d", ehdr, phdr, ehdr->e_phnum);
        for(int i=0; i<ehdr->e_phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynamic = (uint8_t*)this->mbase + phdr[i].p_vaddr;
            }
        }
//    LOGD("dynamic address:%p", dynamic);
        if(dynamic == NULL){
            return success;
        }
        Elf64_Rela* rela_ = NULL;
        Elf64_Word rela_count_ = 0;
        Elf64_Sym* symtab_ = NULL;
        const char* strtab_ = NULL;
        Elf64_Rela* plt_rela_ = NULL;
        Elf64_Word plt_rela_count_ = 0;
        Elf64_Rel* rel_ = NULL;
        Elf64_Word rel_count_ = 0;
        for(Elf64_Dyn* dyn = (Elf64_Dyn*)dynamic; dyn->d_tag != DT_NULL; dyn++ ){
//        LOGD("dynamic tag:%d", dyn->d_tag);
            switch(dyn->d_tag){
                case DT_RELA:
                    rela_ = (Elf64_Rela*)((uint8_t*)this->mbase + (dyn->d_un.d_ptr));  // + load_bias  ????
                    break;
                case DT_RELASZ:
                    //LOGD("relasz...");
                    rela_count_ =  (Elf64_Word)dyn->d_un.d_val / sizeof(Elf64_Rela);
                    //LOGD("relasz : %ld", rel_count_);
                    break;
                case DT_SYMTAB:
                    //LOGD("symtab...");
                    symtab_ = (Elf64_Sym*)((uint8_t*)this->mbase + (Elf64_Addr)(dyn->d_un.d_ptr));  // + load_bias   ???
                    break;
                case DT_STRTAB:
                    //LOGD("strtab...");
                    strtab_ = (const char*)((uint8_t*)this->mbase + (Elf64_Addr)dyn->d_un.d_ptr);  // + load_bias   ???
                    break;
                case DT_JMPREL:
                    plt_rela_ = (Elf64_Rela*)((uint8_t*)this->mbase + (Elf64_Addr)dyn->d_un.d_ptr);
                    break;
                case DT_PLTRELSZ:
                    plt_rela_count_ = (Elf64_Word)dyn->d_un.d_val / sizeof(Elf64_Rela);
                    break;
                case DT_REL:
                    rel_ = (Elf64_Rel*)((uint8_t*)this->mbase + (Elf64_Addr)dyn->d_un.d_ptr);
                    break;
                case DT_RELSZ:
                    rel_count_ = (Elf64_Word)dyn->d_un.d_val / sizeof(Elf64_Rela);
                    break;
                default:
                    break;
            }
        }


        if(rela_ != NULL && rela_count_ > 0){
            for(int i=0; i<rela_count_; i++){
                Elf64_Xword sym_index = ELF64_R_SYM(rela_[i].r_info);
                if(sym_index == 0){
                    continue;
                }
                const char* symbolname = symtab_[sym_index].st_name + strtab_;
                std::map <std::string, void*>::iterator got_iter= this->got_maps.begin();
                for(;got_iter != got_maps.end(); got_iter++){
                    if(got_iter->second != 0){
                        continue;
                    }
                    std::string name = (got_iter->first);
                    if(!strcmp(symbolname, name.c_str())){
                        got_iter->second = (void*)(rela_[i].r_offset + (uint8_t*)this->mbase);
//                    LOGD("===[%s]:%lx", name.c_str(), got_iter->second);
                    }
                }

            }
        }


        if(plt_rela_ != NULL && plt_rela_count_ > 0){
            for(int i=0; i<plt_rela_count_; i++){
                Elf64_Xword sym_index = ELF64_R_SYM(plt_rela_[i].r_info);
                if(sym_index == 0){
                    continue;
                }
                const char* symbolname = symtab_[sym_index].st_name + strtab_;
                std::map <std::string, void*>::iterator got_iter= this->got_maps.begin();
                for(;got_iter != got_maps.end(); got_iter++){
                    if(got_iter->second != 0){
                        continue;
                    }
                    std::string name = (got_iter->first);
                    if(!strcmp(symbolname, name.c_str())){
                        got_iter->second = (void*)(plt_rela_[i].r_offset + (uint8_t*)this->mbase);
                    }
                }
            }
        }
    }else{
        //arm, x86使用Elf32_Rel结构体。
        Elf32_Ehdr* ehdr = (Elf32_Ehdr*)this->mbase;
        Elf32_Phdr* phdr = (Elf32_Phdr*)(ehdr->e_phoff + (uint8_t*)this->mbase);
        void* dynamic = NULL;
//    LOGD("ehdr addr:%p, phdr addr:%p, phnum:%d", ehdr, phdr, ehdr->e_phnum);
        for(int i=0; i<ehdr->e_phnum; i++){
            if(phdr[i].p_type == PT_DYNAMIC){
                dynamic = (uint8_t*)this->mbase + phdr[i].p_vaddr;
            }
        }
        if(dynamic == NULL){
            return success;
        }
        Elf32_Rel* rel_ = NULL;
        Elf32_Word rel_count_ = 0;
        Elf32_Sym* symtab_ = NULL;
        const char* strtab_ = NULL;
        Elf32_Rel* plt_rel_ = NULL;
        Elf32_Word plt_rel_count_ = 0;

        for(Elf32_Dyn* dyn = (Elf32_Dyn*)dynamic; dyn->d_tag != DT_NULL; dyn++ ){
            switch(dyn->d_tag){
                case DT_RELA:
                    rel_ = (Elf32_Rel*)((uint8_t*)this->mbase + (dyn->d_un.d_ptr));  // + load_bias  ????
                    break;
                case DT_RELASZ:
                    //LOGD("relasz...");
                    rel_count_ =  (Elf32_Word)dyn->d_un.d_val / sizeof(Elf32_Rela);
                    //LOGD("relasz : %ld", rel_count_);
                    break;
                case DT_SYMTAB:
                    //LOGD("symtab...");
                    symtab_ = (Elf32_Sym*)((uint8_t*)this->mbase + (dyn->d_un.d_ptr));  // + load_bias   ???
                    break;
                case DT_STRTAB:
                    //LOGD("strtab...");
                    strtab_ = (const char*)((uint8_t*)this->mbase + (Elf32_Addr)(dyn->d_un.d_ptr));  // + load_bias   ???
                    break;
                case DT_JMPREL:
                    plt_rel_ = (Elf32_Rel*)((uint8_t*)this->mbase + (Elf32_Addr)(dyn->d_un.d_ptr));
//                    anti_info_log("plt_rel addr ")
                    break;
                case DT_PLTRELSZ:
                    plt_rel_count_ = (Elf32_Word)dyn->d_un.d_val / sizeof(Elf32_Rela);
                    break;
                case DT_REL:
                    rel_ = (Elf32_Rel*)((uint8_t*)this->mbase + (Elf32_Addr)dyn->d_un.d_ptr);
                    break;
                case DT_RELSZ:
                    rel_count_ = (Elf32_Word)dyn->d_un.d_val / sizeof(Elf32_Rela);
                    break;
                default:
                    break;
            }
        }

        if(rel_ != NULL && rel_count_ > 0){
            for(int i=0; i<rel_count_; i++){
                Elf32_Word sym_index = ELF32_R_SYM(rel_[i].r_info);
                if(sym_index == 0){
                    continue;
                }
                const char* symbolname = symtab_[sym_index].st_name + strtab_;
                std::map <std::string, void*>::iterator got_iter= this->got_maps.begin();
                for(;got_iter != got_maps.end(); got_iter++){
                    if(got_iter->second != 0){
                        continue;
                    }
                    std::string name = (got_iter->first);
                    if(!strcmp(symbolname, name.c_str())){
                        got_iter->second = (void*)(rel_[i].r_offset + (uint8_t*)this->mbase);
                    }
                }

            }
        }

        if(plt_rel_ != NULL && plt_rel_count_ > 0){
            for(int i=0; i<plt_rel_count_; i++){
                Elf32_Word sym_index = ELF32_R_SYM(plt_rel_[i].r_info);
                if(sym_index == 0){
                    continue;
                }

                Elf32_Word st_name = symtab_[sym_index].st_name;
                if(st_name == 0){
                    continue;
                }
                const char* symbolname = st_name + strtab_;

                std::map <std::string, void*>::iterator got_iter= this->got_maps.begin();
                for(;got_iter != got_maps.end(); got_iter++){
                    if(got_iter->second != 0){
                        continue;
                    }
                    std::string name = (got_iter->first);
                    if(!strcmp(symbolname, name.c_str())){
                        got_iter->second = (void*)(plt_rel_[i].r_offset + (uint8_t*)this->mbase);
                    }
                }
            }
        }
    }

    return success;
}


bool GotHook::checkIpRange() {
    bool isHook = false;
    if(this->isInit == false){
        this->isInit = this->initGot();
    }
    if(this->ipRange.size() <= 0){
        return isHook;
    }
    for(std::map <std::string, void*>::iterator got_iter= this->got_maps.begin();got_iter != got_maps.end(); got_iter++){
//        anti_info_log("[%s]:%lx", got_iter->first.c_str(), got_iter->second);
        if(got_iter->second == 0){
            continue;
        }
#if defined(__arm__) || defined(__i386__)
        void* ip_addr = (void*)*(uint32_t*)got_iter->second;
#elif defined(__x86_64__) || defined(__aarch64__)
        void* ip_addr = (void*)*(uint64_t*)got_iter->second;
#endif

        bool isMatch = false;
//        anti_info_log("got_maps size:%d, ipRange size:%d", got_maps.size(), ipRange.size());
        for(std::map<void*, void*>::iterator ipRange_iter = this->ipRange.begin(); ipRange_iter != this->ipRange.end(); ipRange_iter++){
//            anti_info_log("ipaddr:%lx, iprange[%p, %p]", ip_addr, (ipRange_iter->first), (ipRange_iter->second));
            if(ip_addr > ipRange_iter->first && ip_addr < ipRange_iter->second){
//                anti_info_log("ipRange match");
                isMatch = true;
            }
        }
        if(isMatch == false){
//            anti_info_log("[%s]:%lx", got_iter->first.c_str(), got_iter->second);
            isHook = true;
        }
    }
    return isHook;
}


InlineHook::InlineHook(const char *libname) {
    this->mbase = dx_dlopen(libname, 0);
    this->checkSize = 20;
}


void InlineHook::addFun(const char *funcname) {
    if(this->mbase != NULL){
        void* func_addr = dx_dlsym(this->mbase, funcname);
        if(func_addr != NULL){
//            LOGD("[%s]addr:%p", funcname, func_addr);
            this->hash_maps.insert(std::make_pair(func_addr, 0));
        }
    }
}

bool InlineHook::checkhash() {
    bool isHook = false;
    //判断load段是否被修改。
    std::map <void*, uint32_t>::iterator hash_iter;
    for(hash_iter = this->hash_maps.begin(); hash_iter != this->hash_maps.end(); hash_iter++){
        uint8_t* func_addr = (uint8_t*)hash_iter->first;
        uint32_t hash = hashUtils.calHashCode(func_addr, this->checkSize);
        uint32_t origHash = hash_iter->second;
//        LOGD("orighash:%d, hash:%d", origHash, hash);
        if(origHash == 0){
//            LOGD("update hashcode");
            hash_iter->second = hash;
        }else{
            if(hash != origHash){
                isHook = true;
            }
        }
    }
    return isHook;
}

bool InlineHook::checkfeature(){
    bool isHook = false;
#ifdef __i386__
    uint8_t hooked_magic[] = {0xe9, 0x00, 0x00, 0x00, 0x00}; //jmp 0xoffset
#elif __x86_64__
    uint8_t hooked_magic[] = {0xe9, 0x00, 0x00, 0x00, 0x00}; //jmp 0xoffset
#elif __aarch64__
    uint8_t hooked_magic[] = {0x00, 0x00, 0x00, 0x58,  //ldr X, #num
                              0x00, 0x00, 0x00, 0xd6};  //br X
#elif __arm__
    uint8_t hooked_magic[] = {0x04, 0xF0, 0x1F, 0xE5}; //ldr pc, [pc, #-4]
#endif

    //判断load段是否被修改。
    std::map <void*, uint32_t>::iterator hash_iter;
    for(hash_iter = this->hash_maps.begin(); hash_iter != this->hash_maps.end(); hash_iter++){
        uint8_t* func_addr = (uint8_t*)hash_iter->first;
        uint64_t* magic = (uint64_t*)hooked_magic;
        for(int i=0; i<this->checkSize; i=i+4){
//            LOGD("magic:%lld", *magic);
//            LOGD("func_addr:0x%lx", *(uint64_t*)(&func_addr[i]));
//            LOGD("*magic & *(uint64_t*)(&func_addr[i]):0x%lx", *magic & *(uint64_t*)(&func_addr[i]));
            if((*magic & *(uint64_t*)(&func_addr[i])) == *magic){
//                LOGD("check hook ,address:%p", func_addr);
                isHook = true;
                break;
            }
        }
    }

    return isHook;

}

uint32_t HashUtils::getHashCode() {
    return this->hash;
}

void HashUtils::setHashCode(uint32_t hash) {
    this->hash = hash;
}

uint32_t HashUtils::calHashCode(const uint8_t *mem, size_t size) {
    uint32_t b = 378551;
    uint32_t a = 63689;
    uint32_t hash = 0;
    for(size_t i=0; i<size; i++){
//        LOGD("%x", mem[i]);
        hash = hash * a + mem[i];
        a *= b;
    }
    return (hash & 0x7FFFFFFF);
}