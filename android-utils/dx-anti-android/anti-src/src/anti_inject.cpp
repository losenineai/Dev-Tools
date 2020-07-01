//
// Created by blue on 2018/7/11.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <include/anti_crash_log_prefix.h>
#include <src/utils/include/comm_utils.h>
#include "utils_anti_log.h"
#include "file_utils.h"
#include "string_utils.h"
#include "anti_utils.h"
#include "anti_inject.h"
#include "dx_anti_stee.h"
#include "dx_libc.h"
#include "anti_result_handler.h"

typedef struct _Hook_Module {
    const char* funcname;
    const char* modname;
} Hook_Module;

STEE
bool anti_inject_is_xposed_att(const char *filepath) {
    anti_info_log("anti_inject_is_xposed_att");
    if(NULL == filepath)
        return false;

    return ( strcasestr(filepath, "xposedbridge")
             || strcasestr(filepath, ".xposed.")
             || strcasestr(filepath, "xposed_art"));
}

STEE
bool anti_inject_jar_filter(const char * filepath, const char *cmdline) {

    anti_info_log("anti_inject_jar_filter");
    return (NULL != filepath && NULL != cmdline
            && 0 != dx_strncmp(filepath, "/dev/", dx_strlen("/dev/"))
            && 0 != dx_strncmp(filepath, "/vendor/", dx_strlen("/vendor/"))
            && 0 != dx_strncmp(filepath, "[vector]", dx_strlen("[vector]"))
            && 0 != dx_strncmp(filepath, "[vectors]", dx_strlen("[vectors]"))
            && 0 != dx_strncmp(filepath, "[sigpage]", dx_strlen("[sigpage]"))
            && 0 != dx_strncmp(filepath, "[vdso]", dx_strlen("[vdso]"))
            && 0 != dx_strncmp(filepath, "[vsyscall]", dx_strlen("[vsyscall]"))
            && NULL == dx_strstr(filepath, cmdline));
}

STEE
bool anti_inject_so_filter(const char* filepath, const char *cmdline) {

    return (NULL != filepath && NULL != cmdline
        && 0 != dx_strncmp(filepath, "/system/", dx_strlen("/system/"))
        && 0 != dx_strncmp(filepath, "/dev/", dx_strlen("/dev/"))
        && 0 != dx_strncmp(filepath, "/data/dalvik-cache/", dx_strlen("/data/dalvik-cache/"))
        && 0 != dx_strncmp(filepath, "/cache/dalvik-cache/", dx_strlen("/cache/dalvik-cache/"))
        && 0 != dx_strncmp(filepath, "/vendor/", dx_strlen("/vendor/"))
        && 0 != dx_strncmp(filepath, "[vector]", dx_strlen("[vector]"))
        && 0 != dx_strncmp(filepath, "[vectors]", dx_strlen("[vectors]"))
        && 0 != dx_strncmp(filepath, "[sigpage]", dx_strlen("[sigpage]"))
        && 0 != dx_strncmp(filepath, "[vdso]", dx_strlen("[vdso]"))
        && 0 != dx_strncmp(filepath, "[vsyscall]", dx_strlen("[vsyscall]"))
        && 0 != dx_strncmp(filepath, "[kuserhelpers]", dx_strlen("[kuserhelpers]"))
        && NULL == dx_strstr(filepath, cmdline));
}

STEE
bool anti_inject_find_hook_feature(const char *filepath, const void* addr) {
    anti_info_log("anti_inject_find_hook_feature:%s", filepath);
    if (dx_strlen(filepath) == 0) {
        return false;
    }

    // check path from sdcard
    if (dx_strstr(filepath, "/sdcard/")){
        return true;
    }

    // skip check
    static auto check_path_is_skip = [=]() {
        bool result = true;

        // check .so suffix
        int len = dx_strlen(filepath);
        bool is_so_suf = dx_strcmp(&filepath[len - dx_strlen(".so")], ".so") == 0;

        // check file elf
        if (is_so_suf) {
            // FILE *fp = fopen(filepath, "rb");
            int fd = dx_open(filepath, O_RDONLY);
            if (fd > 0) {
                char file_type[5] = {0};
                // size_t retSize = fread(file_type, sizeof(char), 4, fp);
                size_t retSize = dx_read(fd, file_type, 4);
                if (retSize == 4) {
                    if (file_type[0] == 0x7f
                        && file_type[1] == 0x45
                        && file_type[2] == 0x4c
                        && file_type[3] == 0x46) {
                        result = false;
                    }
                }
                // fclose(fp);
                dx_close(fd);
            }
        }

        return result;

    };

    if(check_path_is_skip()){
        return false;
    }

    bool result = false;

    Hook_Module hookModules[22];
    hookModules[0]  = {"MSHookFunction",              "substrate"};
    hookModules[1]  = {"MSFindSymbol",                "substrate"};
    hookModules[2]  = {"MSCloseFunction",             "substrate"};
    hookModules[3]  = {"hook_postcall",               "adbi_hook"};
    hookModules[4]  = {"hook_precall",                "adbi_hook"};
    hookModules[5]  = {"dalvik_java_method_hook",     "ALLINONEs_arthook"};
    hookModules[6]  = {"art_java_method_hook",        "ALLINONEs_arthook"};
    hookModules[7]  = {"art_quick_call_entrypoint",   "ALLINONEs_arthook"};
    hookModules[8]  = {"artQuickToDispatcher",        "ALLINONEs_arthook"};
    hookModules[9]  = {"dexstuff_defineclass",        "ddi_hook"};
    hookModules[10] = {"dexstuff_loaddex",            "ddi_hook"};
    hookModules[11] = {"dexstuff_resolv_dvm",         "ddi_hook"};
    hookModules[12] = {"DexposedBridge",              "dexposed"};
    hookModules[13] = {"dexposedIsHooked",            "dexposed"};
    hookModules[14] = {"dexposedCallHandler",         "dexposed"};
    hookModules[15] = {"frida_agent_main",            "frida-agent"};
    hookModules[16] = {"AKHookFunction",              "AndHook"};
    hookModules[17] = {"AKWrapFunction",              "AndHook"};
    hookModules[18] = {"AKJavaHookMethod",            "AndHook"};
    hookModules[19] = {"doInitHookCap",               "yahfa"};
    hookModules[20] = {"genTrampoline",               "yahfa"};
    hookModules[21] = {"setupTrampoline",             "yahfa"};

    if (strcasestr(filepath, "substrate") ||    // substrate
        strcasestr(filepath, "libAKCompat") ||  // AndHook
        strcasestr(filepath, "andfix") ||       // AndFix
        strcasestr(filepath, "yahfa") ||        // yahfa
        strcasestr(filepath, "legend")){         // legend
        return true;
    }

    // FIXME 华为部分arm64手机运行arm平台时，dlclose 某些so导致 crash，如 libwebviewchromium.so
    // FIXME 特征堆栈 /system/lib/libc.so (offset 0x21000)
//    LOG_WARNING("anti %s", filepath);
//    if (strcasestr(filepath, "libwebviewchromium")) {
//        return false;
//    }

    void *handle = dx_dlopen(filepath, RTLD_NOW);

    if(handle == NULL) {
        return false;
    }

    for (int i = 0; i < sizeof(hookModules) / sizeof(Hook_Module); ++i) {

        if (dx_dlsym(handle, hookModules[i].funcname)) {
            result = true;
            break;
        }
    }

    dx_dlclose(handle);

    return result;
}

STEE
void anti_inject_get_cmdline(char *cmdline) {

    // FIXME mumu模拟器上fscanf会crash
//    FILE *fp;
//
//    fp = fopen("/proc/self/cmdline", "r");
//    if ( fp )
//    {
//        fscanf(fp, "%s[^\n]", cmdline);
//        fclose(fp);
//    }

    pid_t pid = getpid();

    char path[32] = {0};
    if (dx_sprintf(path, "/proc/%d/cmdline", pid) == -1) {
        return;
    }

     dxFILE *fp = dx_fopen(path, "rb");
//    int fd = open(path, O_RDONLY);
    if (fp != NULL) {
         dx_fread(cmdline, sizeof(char), 127, fp);
//        read(fd, cmdline, 127);
         dx_fclose(fp);
//        close(fd);
    }
}

//STEE
//char* anti_inject_trim_left(char *buf) {
//    char *p = buf;
//    int len = (int )strlen(buf);
//
//    while ( *p != '\0' && isspace(*p) )
//    {
//        ++p;
//        --len;
//    }
//    if ( buf != p ) {
//        memmove(buf, p, len + 1);
//    }
//    return buf;
//}
//
//STEE
//char* anti_inject_trim_right(char *buf) {
//    int i = 0;
//
//    for (i = (int )strlen(buf) - 1; i > 0 && isspace(buf[i]); --i);
//
//    if (isspace(buf[i])) {
//        buf[i] = '\0';
//    } else {
//        buf[i + 1] = '\0';
//    }
//
//    return buf;
//}

//
inline void readMaps(vector<string> &mapsVec){
    pid_t pid = getpid();
    char path[32] = {0};
    anti_info_log("=========readMaps");
    if (dx_sprintf(path, "/proc/%d/maps", pid) == -1){
        anti_info_log("=======path sprintf failed");
        return ;
    }


    string data;
    anti_file_readAll(path, data);
    anti_str_splitString(data, mapsVec, "\n");
}


STEE
void anti_inject_detect_inject_libs(pid_t pid) {
    anti_info_log("=======anti_inject_detect_inject_libs");
    char cmdline[1024] = {0};
    anti_inject_get_cmdline(cmdline);


    char sz[128] = {0}; //"/proc/self/maps";
    snprintf(sz, sizeof(sz), "/proc/%d/maps", pid);
    dxFILE *fp = dx_fopen(sz, "rb");

    if(fp == NULL)
        return;

    char module_path[1024] = {0};
    char property3 = '\0';
    void* addr = 0;

    char line[1024] = {0};
    while (dx_fgets(line, sizeof(line), fp) != NULL) {
        anti_info_log("self maps line:%s", line);
        usleep(100);
        memset(module_path, 0, sizeof(module_path));

        if (dx_sscanf(line, "%p-%*p %*c%*c%c%*c %*s %*s %*d%1023s", &addr, &property3,
                   module_path) == 3) {
            anti_str_trim_c(module_path);
            if (property3 == 'x' && module_path[0]
                && anti_inject_so_filter(module_path, cmdline)
                && anti_inject_find_hook_feature(module_path, addr)) {

                anti_info_log("detect inject in anti_inject_detect_inject_libs");

                // 统一anti结果处理
                AntiResult result = {AntiType::AT_INJECT, ANTI_INJECT_ERR_LIBS};
                AntiResultHandler::getInstance()->HandleResult(&result);
            }
        }
    }

    dx_fclose(fp);

    // FIXME mumu模拟器上fscanf会crash，使用上述sscanf替代
//    fp = fopen(sz, "r");
//    if (fp == NULL) {
//        return;
//    }
//    // a7bb000-ea7f8000 r-xp 00000000 103:0d 964                               /system/fonts/NotoSerif-Bold.ttf
//    while (fscanf(fp, "%p-%*p %*c%*c%c%*c %*s %*s %*d%1023[^\n]", &addr, &property3, module_path) == 3) {
//        anti_str_trim_c(module_path);
//
//        if (property3 == 'x' && module_path[0]
//            && anti_inject_so_filter(module_path, cmdline)
//            && anti_inject_find_hook_feature(module_path, addr)) {
//
//            // 统一anti结果处理
//            AntiResult result = {AntiType::AT_INJECT, ANTI_INJECT_ERR_LIBS};
//            AntiResultHandler::getInstance()->HandleResult(&result);
//        }
//    }
//    fclose(fp);


}

STEE
void anti_inject_detect_inject_jars(pid_t pid) {

    char cmdline[1024] = {0};
    anti_inject_get_cmdline(cmdline);

    char sz[128] = {0};
    snprintf(sz, sizeof(sz), "/proc/%d/maps", pid);
    dxFILE *fp = dx_fopen(sz, "rb");

    if(fp == NULL)
        return;

    char module_path[1024] = {0};

    char line[1024] = {0};
    while (dx_fgets(line, sizeof(line), fp) != NULL) {
        memset(module_path, 0, sizeof(module_path));
        usleep(100);
        anti_info_log("anti_inject_detect_inject_jars:%s", module_path);
        if (dx_sscanf(line, "%*p-%*p %*c%*c%*c%*c %*s %*s %*d%1023s", module_path) == 1) {
            anti_str_trim_c(module_path);
            anti_info_log("module path:%s", module_path);
            if ('\0' != module_path[0]
                && anti_inject_jar_filter(module_path, cmdline)
                && anti_inject_is_xposed_att(module_path)) {

                anti_info_log("=======111=detect inject");
                // 统一anti结果处理
                AntiResult result = {AntiType::AT_INJECT, ANTI_INJECT_ERR_JARS};
                AntiResultHandler::getInstance()->HandleResult(&result);
            }
        }
//        anti_info_log("===== out if");
    }

    anti_info_log("=1111=======out while");
    // FIXME mumu模拟器上fscanf会crash，使用上述sscanf
//    FILE *fp = NULL;
//    char sz[256] = {0};
//    snprintf(sz, sizeof(sz), "/proc/%d/maps", pid);
//    if (NULL == (fp = fopen(sz, "r")))
//        return;
//    // 40753000-4075b000 r-xp 00000000 b3:10 1970       /system/lib/libremotedesktop_client.so
//    while (fscanf(fp, "%*x-%*x %*c%*c%*c%*c %*s %*s %*d%1023[^\n]", modpath) == 1) {
//        anti_str_trim_c(modpath);
//
//        if ('\0' != modpath[0]
//            && anti_inject_jar_filter(modpath, cmdline)
//            && anti_inject_is_xposed_att(modpath)) {
//
//            // 统一anti结果处理
//            AntiResult result = {AntiType::AT_INJECT, ANTI_INJECT_ERR_JARS};
//            AntiResultHandler::getInstance()->HandleResult(&result);
//        }
//    }
//
    dx_fclose(fp);
}

STEE
void* DXAnti_detect_inject(void *arg) {

    anti_info_log("DXAnti_detect_inject, tid:%d", gettid());
    pid_t pid = *(pid_t* )arg;
    free(arg);

    anti_inject_detect_inject_libs(pid);

    anti_info_log("anti_inject_detect_inject_jars");
    anti_inject_detect_inject_jars(pid);

    anti_info_log("DXAnti_detect_inject ok");

    return NULL;
}

STEE
int DXAnti_detect_inject_async() {

    pid_t cur_pid = getpid();
    pthread_t newthread;
    int ret = -1;

    anti_info_log("=======inject async cur_pid:%d", cur_pid);
    dx_anti_pthread_create(ret, &newthread, NULL, DXAnti_detect_inject, malloc_wraper((void* )&cur_pid, sizeof(pid_t)));
    return ret;
}


STEE
void DXAnti_detect_inject_entry() {
    DXAnti_detect_inject_async();
}