//
// Created by white on 2018/5/15.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <anti_utils.h>
#include <anti_thread.h>
#include <anti_memdump.h>
#include <sys/wait.h>
#include <comm_utils.h>
#include "utils_anti_log.h"
#include "platform.h"
#include "file_utils.h"
#include "string_utils.h"
#include "anti_debug.h"
#include "dx_anti_stee.h"
#include "anti_crash_log_prefix.h"
#include "anti_result_handler.h"
#include "int_map.h"
#include "jni_comm_utils.h"
#include "jni_utils.h"

//#define OLD_STEE

#ifdef OLD_STEE

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
//    anti_info_log("anti_debug_detectByPort23946ByTcp");

    FILE *pfile = NULL;
    char buf[0x1000] = {0};
    bool ret = false;
    // 执行命令
    const char *strCatTcp = "cat /proc/net/tcp |grep :5D8A";
    //char* strNetstat="netstat |grep :23946";

    pfile = popen(strCatTcp, "r");
    if (NULL == pfile) {
        return ret;
    }

    // 获取结果
    if (fgets(buf, sizeof(buf), pfile)) {
        // 执行到这里，判定为调试状态
        anti_err_log("I'm debuged 1");
        ret = true;

    }//while

    pclose(pfile);

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
//    anti_info_log("anti_debug_detectByObjProcess");
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

        if (strA || strB || strC || strD) {
            // 执行到这里，判定为调试状态
            anti_err_log("I'm debuged 2");
            ret = true;
        }//if
    }//while

    pclose(pfile);

    anti_info_log("anti_debug_detectByObjProcess result: %d", ret);

    return ret;
}

/**
 * 父进程名检测
 * 原理: 有的时候不使用apk附加调试的方法进行逆向，而是写一个.out可执行文件直接加载so进行调试，
 * 这样程序的父进程名和正常启动apk的父进程名是不一样的。
 *
 * 测试发现:
 * (1)正常启动的apk程序:父进程是zygote
 * (2)调试启动的apk程序:在AS中用LLDB调试发现父进程还是zygote
 * (3)附加调试的apk程序:父进程是zygote
 * (4)vs远程调试 用可执行文件加载so:父进程名为gdbserver 结论:父进程名非zygote的，判定为调试状态。
 *
 * 做法: 读取/proc/pid/cmdline，查看内容是否为zygote
 * @return 调试中 true,非调试 false
 */
STEE
bool anti_debug_detectByParents() {
//    anti_info_log("anti_debug_detectByParents");

    bool result = false;
    // 设置buf
    char strPpidCmdline[0x100] = {0};
    snprintf(strPpidCmdline, sizeof(strPpidCmdline), "/proc/%d/cmdline", getppid());

    // 打开文件
    int file = open(strPpidCmdline, O_RDONLY);
    if (file < 0) {
        return result;
    }
    // 文件内容读入内存
    memset(strPpidCmdline, 0, sizeof(strPpidCmdline));
    ssize_t ret = read(file, strPpidCmdline, sizeof(strPpidCmdline));
    if (-1 == ret) {
        return result;
    }
    // 没找到返回0
    const char *sRet = strstr(strPpidCmdline, "zygote");
    if (NULL == sRet) {
        // 执行到这里，判定为调试状态
        anti_err_log("I'm debuged 3");
        result = true;
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
//    anti_info_log("anti_debug_detectByTaskCount");

    bool ret = false;
    char buf[0x100] = {0};
    const char *str = "/proc/%d/task";
    snprintf(buf, sizeof(buf), str, getpid());

    // 打开目录:
    DIR *pdir = opendir(buf);
    if (!pdir) {
        perror("CheckTaskCount open() fail.\n");
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

    ret = 1 >= Count;

    anti_info_log("anti_debug_detectByTaskCount result: %d", ret);
    if (ret) {
        anti_err_log("I'm debuged 5");
    }

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
    void *Handle = NULL;
    Handle = dlopen(dx_get_sys_lib_path("libdvm.so"), RTLD_LAZY);

    if (NULL == Handle) {
        return result;
    }

    PPP Fun = (PPP) dlsym(Handle, "_Z25dvmDbgIsDebuggerConnectedv");
    if (NULL == Fun) {
        return result;
    } else {
        wbool ret = Fun();
        if (1 == ret) {
            result = true;
        }
    }

    anti_info_log("anti_debug_detectByNativeIsDBGConnected result: %d", result);
    if (result) {
        anti_err_log("I'm debuged 6");
    }
    return result;
}

/**
 * 通过/proc/%d/status 获取TracerPid检测
 * @return
 */
STEE
bool anti_debug_detectByTrace() {

    static bool is_first_time = true;

    if (is_first_time) {
        is_first_time = false;

        string pkg = anti_platform_getPkgName();
        string flag_file = anti_platform_getWorkDir() + "/.cf";
        anti_file_deleteIfExist(flag_file);

        pid_t pid = fork();
        if (pid == 0) {

            prctl(PR_SET_NAME, (pkg + ":dx").c_str());

            while (true) {
                pid_t ppid = getppid();
                anti_info_log("anti_debug_detectByTrace ppid:%d", getpid());

                if (ppid == 1) {
                    goto EXIT;
                }

                for (string &line : anti_file_readLines(
                        anti_str_format("/proc/%d/status", ppid))) {
                    if (!line.empty() && line.find("TracerPid") != line.npos) {

                        int pos = line.find(':');
                        int tpid = atoi(line.substr(pos + 1).c_str());

                        if (tpid != 0) {
                            anti_file_writeAll(flag_file, "fuck");
                            goto EXIT;
                        }
                    }
                }

                sleep(5);
            }

            EXIT:
            exit(0);
        }
    } else {

        string flag_file = anti_platform_getWorkDir() + "/.cf";
        if (anti_file_exist(flag_file)) {
            anti_file_deleteIfExist(flag_file);
            anti_err_log("I'm debuged 8");
            return true;
        }
    }
    anti_info_log("anti_debug_detectByTrace result: 0");

    return false;
}


/**
 * 通过java api
 * @return
 */
STEE
bool anti_debug_detectByApi() {
//    anti_info_log("anti_debug_detectByJava");

    bool result = anti_platform_isDebuggerConnected();

    anti_info_log("anti_debug_detectByJava result: %d", result);
    if (result) {
        anti_err_log("I'm debuged 9");
    }
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

STEE
void* anti_debug_check_thread(void* arg) {

    if (anti_debug_detectByApi()
        //           || anti_debug_detectByNativeIsDBGConnected()
        || anti_debug_detectByObjProcess()
        || anti_debug_detectByParents()
        || anti_debug_detectByPort23946ByTcp()
        || anti_debug_detectByTaskCount()
        || anti_debug_detectByTrace()
        //           || anti_debug_catch_SIGTRAP()
            ) {
        AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_PTRACE2};
        AntiResultHandler::getInstance()->HandleResult(&result);
    } else {
        syscall(__NR_ptrace, PTRACE_TRACEME);
        ptrace(PTRACE_TRACEME);
    }

    return NULL;
}

STEE
void DXAnti_detect_debug(pid_t main_pid) {

    pthread_t newthread;
    int ret = 0;

    dx_anti_pthread_create(ret, &newthread, NULL, anti_debug_check_thread, NULL);

}

#else


struct FD_PID_PAIR {
    int fildes;
    pid_t pid;
    pid_t main_pid;
};

static pid_t l_main_pid = -1;

int (*dx_orignal_ptrace)(int request, pid_t pid, caddr_t addr, int data) = NULL;

typedef void* (*fun)(void*);

int subprocessPid = 0;

int TEST_PTRACE = 0x20;

uint32_t get_random_Pid() {
    fun f = DXAnti_detect_debug;
    uint8_t* mem = (uint8_t*)f;
    size_t size = 20;
    uint32_t b = 378551;
    uint32_t a = 63689;
    uint32_t hash = 0;
    for(size_t i=0; i<size; i++){
//        LOGD("%x", mem[i]);
        hash = hash * a + mem[i];
        a *= b;
    }
    return (hash & 0xFFFFF + 0x65535);
}


STEE
int anti_debug_replace_ptrace(int request, pid_t pid, caddr_t addr, int data) {

    if(request == TEST_PTRACE && pid == get_random_Pid()){
        return pid + getpid();
    }

    if (request == PTRACE_DETACH && pid != -1 && pid == l_main_pid) {
        // 统一anti结果处理
        AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_PTRACE1};
        AntiResultHandler::getInstance()->HandleResult(&result);
        return -1;
    }
    return dx_orignal_ptrace(request, pid, addr, data);
}

static void (*orig_dvmDbgActive)(void);
STEE
void anti_debug_replace_dvmDbgActive(void) {
    orig_dvmDbgActive();

    // 统一anti结果处理
    AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_DBG_ACTIVE};
    AntiResultHandler::getInstance()->HandleResult(&result);
}

STEE
void anti_debug_dwp() {

    void (*orig1)(void) = NULL;
    void (*orig2)(void) = NULL;

    dlopen(dx_get_sys_lib_path("libdvm.so"), RTLD_NOW);

    // FIXME: arm so无法hookx86的设备。解决：dx_hook2 要使用 dlsym获取到地址
    dx_hook2(dx_get_sys_lib_path("libdvm.so"), "dvmDbgActive", (void* )anti_debug_replace_dvmDbgActive, (void **)&orig1);
    dx_hook2(dx_get_sys_lib_path("libdvm.so"), "_Z12dvmDbgActivev", (void* )anti_debug_replace_dvmDbgActive, (void **)&orig2);

    orig_dvmDbgActive = orig1 != NULL ? orig1 : orig2;
}

STEE
void* anti_debug_keep_pipe_connect(void* arg) {

    FD_PID_PAIR *pair = (FD_PID_PAIR* )arg;
    int fildes;
    pid_t cur_pid;
    pid_t main_pid;
    char buf;

    fildes = pair->fildes;
    cur_pid = pair->pid;
    main_pid = pair->main_pid;
    free(pair);
    prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
    int ret = 0;
    do {
        errno = 0;
        ret = read(fildes, &buf, 1u);
        if(ret > 1){
            subprocessPid = ret;
        }
        anti_info_log("keep pipe connect ret:%d", ret);
    } while (ret == -1 && errno == EAGAIN);
    close(fildes);

    // 统一anti结果处理
    int crash_reason = cur_pid == main_pid ? ANTI_DEBUG_ERR_PIPE_PARENT_DIS : ANTI_DEBUG_ERR_PIPE_SON_DIS;
    AntiResult result = {AntiType::AT_DEBUG, crash_reason};
    AntiResultHandler::getInstance()->HandleResult(&result);

    return 0;
}

STEE
int anti_debug_handle_exception_sig(pid_t pid, int sig) {

    // 前0x10忽略处理。后0x10正常处理。5是调试，要崩溃
    // 5 SIGTRAP 由断点指令或其它trap指令产生. 由debugger使用.
    if ( sig == SIGTRAP )
    {
        // 统一anti结果处理
        AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_RAP_SIG};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    // 取消信号拦截，打印堆栈信息  by blue 2018.12.03
    /*
     * else if (sig == SIGSEGV || sig == SIGILL) {

        static clock_t startTime = 0;
        clock_t curTime = clock();
        static map_t processMap = map_allocate();

        // 10毫秒
        if (curTime - startTime > 10 * 1000) {
            startTime = curTime;
            map_trunk(&processMap);
        }

        int times = map_get(&processMap, pid);

        if(times > 9) {
            map_delete(&processMap, pid);
            return dx_orignal_ptrace(PTRACE_CONT, pid, NULL, sig);
        }

        map_set(&processMap, pid, ++times);

    return dx_orignal_ptrace(PTRACE_CONT, pid, NULL, sig <= 0x10 ? 0 : sig);
    */

    return ptrace(PTRACE_CONT, pid, NULL, sig);
}

STEE
void anti_debug_handle_catched_signal(pid_t pid) {

    while ( 1 )
    {
        int stat_loc = 0;
        int ret = waitpid(pid, &stat_loc, 0);

        if ( ret == -1 || WIFEXITED(stat_loc) || WIFSIGNALED(stat_loc)) {
            return;
        } else {
            anti_debug_handle_exception_sig(pid, WEXITSTATUS(stat_loc));
        }
    }
}

STEE
void *DXAnti_detect_debug(void *arg){
    pid_t main_pid = getpid();

    pid_t test_pid = get_random_Pid();
    anti_info_log("======test_pid:%d", test_pid);
    if(ptrace(TEST_PTRACE, test_pid, 0, 0) == get_random_Pid()+main_pid){
        return NULL;
    }

    anti_debug_dwp();

    pthread_t newthread;
    int ret = 0;

    int fildes_sub_to_parent[2];
    int fildes_sub_keep_pipe_connect[2];
    int fildes_parent_keep_pipe_connect[2];

    l_main_pid = main_pid;
    pipe(fildes_sub_to_parent);
    pipe(fildes_sub_keep_pipe_connect);
    pipe(fildes_parent_keep_pipe_connect);
    pid_t fork_pid = fork();

    if ( fork_pid > 0 )
    {
        // parent process
        uint8_t buf;

        close(fildes_sub_to_parent[1]);
        close(fildes_parent_keep_pipe_connect[1]);
        close(fildes_sub_keep_pipe_connect[0]);

        dlopen(dx_get_sys_lib_path("libc.so"), RTLD_NOW);
        // FIXME: arm so无法hookx86的设备。解决：dx_hook2 要使用 dlsym获取到地址
        if (!dx_hook2(dx_get_sys_lib_path("libc.so"), "ptrace", (void* )anti_debug_replace_ptrace, (void **)&dx_orignal_ptrace)) {
            return NULL;
        }

        DXAnti_detect_thread_async(fork_pid);
        pthread_t dump_tid;
        DXAnti_detect_memdump_async(dump_tid, fork_pid);
        pid_t subpid = 0;
        if(read(fildes_sub_to_parent[0], &subpid, sizeof(pid_t)) != -1 && subpid != 0){
            subprocessPid = subpid;
            anti_info_log("read subprocess pid:%d", subpid);
        }

        if ( read(fildes_sub_to_parent[0], &buf, 1u) != -1 && buf == 0)
        {
            FD_PID_PAIR* pair = (FD_PID_PAIR* )malloc(sizeof(FD_PID_PAIR));
            pair->fildes = fildes_parent_keep_pipe_connect[0];
            pair->pid = main_pid;
            pair->main_pid = main_pid;

            dx_anti_pthread_create(ret, &newthread, NULL, anti_debug_keep_pipe_connect, (void* )pair);
        }
    } else if (fork_pid < 0) // if ( fork_pid )
    {
        // fork failed
        DXAnti_detect_thread_async(main_pid);
        pthread_t dump_tid;
        DXAnti_detect_memdump_async(dump_tid, main_pid);

    } else {
        // sub process
        int ptrace_ret;

        close(fildes_sub_to_parent[0]);
        close(fildes_parent_keep_pipe_connect[0]);
        close(fildes_sub_keep_pipe_connect[1]);
        anti_info_log("transform subpid");

        pid_t subPid = getpid();
        write(fildes_sub_to_parent[1], &subPid, sizeof(pid_t));

        DXAnti_detect_thread_async(main_pid);
        pthread_t dump_tid;
        DXAnti_detect_memdump_async(dump_tid, main_pid);

//        dlopen(dx_get_sys_lib_path("libc.so"), RTLD_NOW);
//
//        // FIXME: arm so无法hookx86的设备。解决：dx_hook2 要使用 dlsym获取到地址
//        if (!dx_hook2(dx_get_sys_lib_path("libc.so"), "ptrace", (void* )anti_debug_replace_ptrace, (void **)&dx_orignal_ptrace)) {
//            return NULL;
//        }

        if ( ptrace(PTRACE_ATTACH, main_pid, 0, 0) < 0 ) {
            ptrace_ret = -1;
        } else {
            int stat_loc;
            ptrace_ret = 0;
            wait(&stat_loc);
            ptrace(PTRACE_CONT, main_pid, 0, 0);
        }

        write(fildes_sub_to_parent[1], &ptrace_ret, 1u);

        FD_PID_PAIR* pair = (FD_PID_PAIR* )malloc(sizeof(FD_PID_PAIR));
        pair->fildes = fildes_sub_keep_pipe_connect[0];
        pair->pid = getpid();
        pair->main_pid = main_pid;

        if ( ptrace_ret != 0 ) {
            anti_debug_keep_pipe_connect(pair);
        } else {
            dx_anti_pthread_create(ret, &newthread, NULL, anti_debug_keep_pipe_connect, (void* )pair);
            anti_debug_handle_catched_signal(main_pid);
            ptrace(PTRACE_DETACH, main_pid, 0, 0);

            // 统一anti结果处理
            AntiResult result = {AntiType::AT_DEBUG, ANTI_DEBUG_ERR_PTRACE2};
            AntiResultHandler::getInstance()->HandleResult(&result);
        }
    }

    return NULL;
}

STEE
bool isApkDebuggable(){
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    jobject jobjCxt = anti_jni_getGlobalContext(env);

    jobject japplicationInfo = anti_jni_callMethodByName(env,
                                                          jobjCxt,
                                                          "getApplicationInfo",
                                                          "()Landroid/content/pm/ApplicationInfo;");

    jint flags = (jint)anti_jni_getIntField(env, japplicationInfo, "flags", "I");

    if(japplicationInfo != nullptr){
        anti_jni_delete_ref(env, japplicationInfo);
    }

    int m_flags = flags;
    int debuggable_flag = (1 << 1);

    if((m_flags & debuggable_flag) != 0){
        return true;
    }

    return false;
}
STEE
int DXAnti_detect_debug_async() {
    if(isApkDebuggable()){
        return 0;
    }

    pthread_t tid;
    int ret = -1;

    dx_anti_pthread_create(ret, &tid, NULL, DXAnti_detect_debug, NULL);
    return ret;
}

STEE
void DXAnti_detect_debug_entry() {
    DXAnti_detect_debug_async();
}

#endif