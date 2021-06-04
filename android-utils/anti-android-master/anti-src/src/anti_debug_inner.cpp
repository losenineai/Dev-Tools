//
// Created by white on 2018/11/28.
//

#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <linux/ptrace.h>
#include <sys/ptrace.h>
#include <comm_utils.h>
#include "anti_debug_inner.h"
#include "platform.h"
#include "dx_anti_stee.h"
#include "utils_anti_log.h"
#include "dx_anti_prefix.h"
#include "anti_result_handler.h"
#include "anti_crash_log_prefix.h"
#include "file_utils.h"
#include "anti_config.h"
#include "string_utils.h"
#include "anti_utils.h"


/**
 * 未完成：自身进程名检测
 * 原理: 和上条一样，也是写个.out加载so来脱壳的场景，正常进程名一般是apk的com.xxx这样的格式。
 * @return 调试中 true,非调试 false
 */

/**
 * 未完成：apk进程fd文件检测
 * 原理: 根据/proc/pid/fd/路径下文件的个数差异，判断进程状态。
 * (apk启动的进程和非apk启动的进程fd数量不一样)
 * (apk的debug启动和正常启动，进程fd数量也不一样)
 * @return 调试中 true,非调试 false
 */


/**
 * IDA调试端口检测
 * 原理: 调试器远程调试时，会占用一些固定的端口号。
 * 做法: 读取/proc/net/tcp，查找IDA远程调试所用的23946端口，若发现说明进程正在被IDA调试。
 * (也可以运行netstat ­apn结果中搜索23946端口)
 * @return 调试中 true,非调试 false
 */
STEE
bool anti_debug_detectByPort23946ByTcp() {
    bool ret = false;
    // 执行命令
    const char *strCatTcp = "cat /proc/net/tcp |grep :5D8A";
    //char* strNetstat="netstat |grep :23946";

    string idaExist = anti_util_executeCMD(strCatTcp);

    ret = !idaExist.empty();

    anti_info_log("anti_debug_detectByPort23946ByTcp result: %d", ret);

    return ret;
}


/**
 * 调试器进程名检测
 * 原理: 远程调试要在手机中运行android_server gdbserver gdb等进程。
 * 做法: 遍历进程，查找固定的进程名，找到说明调试器在运行。
 * @return 调试中 true,非调试 false
 */
STEE
bool anti_debug_detectByObjProcess() {
    FILE *pfile = NULL;
    char buf[0x1000] = {0};
    bool ret = false;

    // 执行命令
    //pfile=popen("ps | awk '{print $9}'","r"); // 部分不支持awk命令
    pfile = popen("ps", "r");
    if (NULL == pfile) {
        return ret;
    }

    // 获取结果
    while (fgets(buf, sizeof(buf), pfile)) {
        // 打印进程
        // anti_info_log("遍历进程:%s\n",buf);
        // 查找子串
        char *strA = strstr(buf, "android_server");
        char *strB = strstr(buf, "gdbserver");
        char *strC = strstr(buf, "gdb");
        char *strD = strstr(buf, "fuwu");
        char *strE = strstr(buf, "frida");

        if (strA || strB || strC || strD || strE) {
            ret = true;
            break;
        }
    }

    pclose(pfile);

    anti_info_log("anti_debug_detectByObjProcess result: %d", ret);

    return ret;
}


/**
 * @FIXME 如果是子进程启动，就不适用
 * 父进程名检测
 * 原理: 有的时候不使用apk附加调试的方法进行逆向，而是写一个.out可执行文件直接加载so进行调试，
 * 这样程序的父进程名和正常启动apk的父进程名是不一样的。
 *
 * 测试发现:
 * (1)正常启动的apk程序:父进程是zygote
 * (2)调试启动的apk程序:在AS中用LLDB调试发现父进程还是zygote
 * (3)附加调试的apk程序:父进程是zygote
 * (4)vs远程调试 用可执行文件加载so:父进程名为gdbserver
 *
 * 结论:父进程名非zygote的，判定为调试状态。
 *
 * 做法: 读取/proc/pid/cmdline，查看内容是否为zygote
 * @return 调试中 true,非调试 false
 */
STEE
bool anti_debug_detectByParents() {

    bool result = false;
    // 设置buf
    char cmdlineChar[100] = {0};
    snprintf(cmdlineChar, sizeof(cmdlineChar), "/proc/%d/cmdline", getppid());

    string pProcessName;
    anti_file_readAll(cmdlineChar, pProcessName);

    if (pProcessName.size() > 0) {
        // 未匹配，返回0
        if (strstr(pProcessName.c_str(), "zygote") == NULL) {
            result = true;
        }
    }

    anti_info_log("anti_debug_detectByParents result: %d", result);
    return result;
}


/**
 * apk线程检测
 * 原理: 同样.out加载so来脱壳的场景，正常apk进程一般会有十几个线程在运行(比如会有jdwp线程)，
 * 自己写可执行文件加载so一般只有一个线程， 可以根据这个差异来进行调试环境检测。
 * @return 调试中 true,非调试 false
 */
STEE
bool anti_debug_detectByTaskCount() {

    bool ret = false;
    char buf[0x100] = {0};
    const char *str = "/proc/%d/task";
    snprintf(buf, sizeof(buf), str, getpid());

    // 打开目录:
    DIR *pdir = opendir(buf);
    if (!pdir) {
//        perror("CheckTaskCount open() fail.\n");
        return ret;
    }

    // 查看目录下文件个数:
    struct dirent *pde = NULL;
    int Count = 0;
    while ((pde = readdir(pdir))) {
        // 字符过滤
        if ((pde->d_name[0] <= '9') && (pde->d_name[0] >= '0')) {
            ++Count;
        }
    }

    closedir(pdir);

    ret = 1 >= Count;

    anti_info_log("anti_debug_detectByTaskCount Count: %d", Count);

    return ret;
}


/**
 * 安卓系统自带调试检测函数
 * // android.os.Debug.isDebuggerConnected();
 * 原理: 分析android自带调试检测函数isDebuggerConnected()在native的实现， 尝试在native使用。
 * 做法:
 * (1)dalvik模式下:
 * 找到进程中libdvm.so中的dvmDbgIsDebuggerConnected()函数， 调用他就能得知程序是否被调试。
 * dlopen(/system/lib/libdvm.so) dlsym(_Z25dvmDbgIsDebuggerConnectedv)
 * (2)art模式下:
 * art模式下，结果存放在libart.so中的全局变量gDebuggerActive中， 符号名为_ZN3art3Dbg15gDebuggerActiveE。
 * 但是貌似新版本android不允许使用非ndk原生库，dlopen(libart.so)会失败。 所以无法用dalvik那样的方法了。
 * 有一种麻烦的方法，手动在内存中搜索libart模块，然后手工寻找该全局变量符号。
 * @return 调试中 true,非调试 false
 */
// 只写了dalvik的代码，art的就不写了
typedef unsigned char wbool;

typedef wbool (*PPP)();
STEE
bool anti_debug_detectByNativeIsDBGConnected() {

    bool result = false;
    void *handle = NULL;
    handle = dlopen("/system/lib/libdvm.so", RTLD_LAZY);

    if (NULL == handle) {
        return result;
    }

    PPP Fun = (PPP) dlsym(handle, "_Z25dvmDbgIsDebuggerConnectedv");

    if ((NULL != Fun) && (1 == (wbool)Fun())) {
        result = true;
    }

    dlclose(handle);

    anti_info_log("anti_debug_detectByNativeIsDBGConnected result: %d", result);

    return result;
}




/**
 * 通过java api
 * @return
 */
STEE
bool anti_debug_detectByApi() {
    bool result = anti_platform_isDebuggerConnected();

    anti_info_log("anti_debug_detectByJava result: %d", result);
    return result;
}

/**
 * 通过捕捉信号SIGTRAP
 * 会有兼容问题
 * @return
 */
STEE
bool anti_debug_catch_SIGTRAP() {
    bool result = false;
    static bool is_debuged = false;
    static auto sigtrap_handler = [](int sig) {
        is_debuged = true;
    };

    signal(SIGTRAP, sigtrap_handler);

    if (is_debuged) {
        anti_info_log("anti_debug_catch_SIGTRAP");
        result = true;
    }

    return result;
}

/**
 * 通过/proc/%d/status 获取TracerPid检测
 * @return
 */
STEE
bool anti_debug_detectByTrace() {

    bool result = false;

    // TODO 用处？
    string flag_file = anti_platform_getWorkDir() + "/.cf";
    anti_file_deleteIfExist(flag_file);

    // 主线程内
    pid_t pid = getpid();
    anti_info_log("anti_debug_detectByTrace pid:%d", pid);

    char status_path[56] = {0};
    sprintf(status_path, "/proc/%d/status", pid);

    vector<string> lineVect;
    anti_file_readLines(string(status_path), lineVect);

    for (string &line : lineVect) {
        if (!line.empty() && line.find("TracerPid:") != line.npos) {
            anti_info_log("anti_debug_detectByTrace line:%s", line.c_str());

            int pos = line.find(':');
            int tpid = atoi(line.substr(pos + 1).c_str());
            anti_info_log("anti_debug_detectByTrace tpid:%d", tpid);

            if (tpid != 0) {
                result = true;
                break;
            }
        }
    }


//    static bool is_first_time = true;
//    if (is_first_time) {
//        is_first_time = false;
//
//        string pkg = anti_platform_getPkgName();
//        string flag_file = anti_platform_getWorkDir() + "/.cf";
//        anti_file_deleteIfExist(flag_file);
//
//        pid_t pid = fork();
//        if (pid == 0) {
//
//            // 父进程退出，内核会通知子进程退出
//            prctl(PR_SET_PDEATHSIG, SIGKILL);
//            prctl(PR_SET_NAME, (pkg + ":dx").c_str());
//
//            while (true) {
//                pid_t ppid = getppid();
//                anti_info_log("anti_debug_detectByTrace ppid:%d", getpid());
//
//                if (ppid == 1) {
//                    goto EXIT;
//                }
//
//                for (string &line : anti_file_readLines(
//                        anti_str_format("/proc/%d/status", ppid))) {
//                    if (!line.empty() && line.find("TracerPid") != line.npos) {
//
//                        int pos = line.find(':');
//                        int tpid = atoi(line.substr(pos + 1).c_str());
//
//                        if (tpid != 0) {
//                            anti_file_writeAll(flag_file, "fuck");
//                            goto EXIT;
//                        }
//                    }
//                }
//
//                sleep(5);
//            }
//
//            EXIT:
//            exit(0);
//        }
//    } else {
//
//        string flag_file = anti_platform_getWorkDir() + "/.cf";
//        if (anti_file_exist(flag_file)) {
//            anti_file_deleteIfExist(flag_file);
//            anti_err_log("I'm debuged 8");
//            return true;
//        }
//    }

    anti_info_log("anti_debug_detectByTrace result: %d", result);

    return result;
}

STEE
void *DXAnti_detect_debug_inner(void *arg) {
    while (1) {
        bool result =  anti_debug_detectByApi()
//                       || anti_debug_detectByNativeIsDBGConnected()
//                       || anti_debug_catch_SIGTRAP()
//                       || anti_debug_detectByParents()
                       || anti_debug_detectByTrace()
                       || anti_debug_detectByObjProcess()
                       || anti_debug_detectByPort23946ByTcp()
                       || anti_debug_detectByTaskCount()
        ;

        anti_info_log("DXAnti_detect_debug_inner result :%d", result);
        if (result) {
            // 统一anti结果处理
            AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_INNER};
            AntiResultHandler::getInstance()->HandleResult(&result);
            break;
        }

        sleep(5);
    }

    return NULL;
}

STEE
int DXAnti_detect_debug_inner_async(pthread_t &tid) {
    int ret = -1;

    dx_pthread_create(ret, &tid, NULL, DXAnti_detect_debug_inner, NULL);

    return ret;
}

STEE
void DXAnti_detect_debug_inner_entry() {
    pthread_t tid;
    DXAnti_detect_debug_inner_async(tid);
}
