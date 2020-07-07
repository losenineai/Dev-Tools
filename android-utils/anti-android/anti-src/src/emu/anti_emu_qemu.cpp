//
// Created by blue on 2018/2/4.
//

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string>
#include <unistd.h>

#include <vector>
#include <file_utils.h>
#include <properties.h>
#include <include/anti_utils.h>

#include "anti_emu_qemu.h"
#include "utils_anti_log.h"
#include "dx_anti_stee.h"
#include "anti_config.h"

using namespace std;

void* anti_emu_checkQemuArm_get_thread(void *);
void anti_emu_checkQemuArm_exec_thread(void);

bool checkQemuArm_result = true;
bool checkQemuArm_finish = false;

uint32_t global_value = 0;

class DXProperty{
    // todo : how to code destructor
public:
    DXProperty(string name, string seek_value){
        this->name = name;
        this->seek_value = seek_value;
    }
    string name;
    string seek_value;


};

STEE
bool anti_emu_qemu_checkQemuProps(){
    anti_info_log("anti_emu_qemu_checkQemuProps");
    DXProperty PROPERTIES[] = {
            DXProperty("init.svc.qemud", "none"),
            DXProperty("init.svc.qemu-props", "none"),
            DXProperty("qemu.hw.mainkeys", "none"),
            DXProperty("qemu.sf.fake_camera", "none"),
            DXProperty("qemu.sf.lcd_density", "none"),
            DXProperty("ro.bootloader", "unknown"),
            DXProperty("ro.bootmode", "unknown"),
            DXProperty("ro.hardware", "goldfish"),
            DXProperty("ro.kernel.android.qemud", "none"),
            DXProperty("ro.kernel.qemu.gles", "none"),
            DXProperty("ro.kernel.qemu", "1"),
            DXProperty("ro.product.device", "generic"),
            DXProperty("ro.product.model", "sdk"),
            DXProperty("ro.product.name", "sdk"),
            DXProperty("ro.product.name", "ChangWan"),
            DXProperty("ro.product.name", "lgshouyou"),
            DXProperty("ro.product.name", "nox"),
            DXProperty("ro.product.name", "ttVM_Hdragon"),

            DXProperty("ro.serialno", "none")
    };

    int MIN_PROPERTIES_THRESHOLD = 0x5;

    int found_props = 0;

    for (DXProperty property : PROPERTIES){

        char buf[256];
        anti_property_get(property.name.c_str(), buf, "none");
        string strbuf = string(buf);
        if((property.seek_value == "none" && strbuf != "none")
           || (property.seek_value !="none" && strbuf.find(property.seek_value) !=  string::npos)){
            found_props ++;
        }

    }

    if (found_props >= MIN_PROPERTIES_THRESHOLD) {
        return true;
    }
    return false;

}

STEE
bool anti_emu_qemu_checkQemuDriver(){

    string QEMU_DRIVERS[] = {"goldfish"};
    string DRIVERS_FILE_PATHS[] = {
            "/proc/tty/drivers",
            "/proc/cpuinfo"
    };
    for (string driver_file_path : DRIVERS_FILE_PATHS){

        string driver_data;
        anti_file_readAll(driver_file_path, driver_data);
        for (string known_qemu_driver : QEMU_DRIVERS){
            anti_info_log("anti_emu_qemu_checkQemuDriver known_qemu_driver:%s",
                          known_qemu_driver.c_str());
            if (driver_data.find(known_qemu_driver.c_str()) != string::npos){
                return true;
            }

        }

    }
    return false;

}

STEE
bool anti_emu_qemu_checkQemuFiles(){

    string const knownFiles[5] = {"/system/lib/libc_malloc_debug_qemu.so", "/sys/qemu_trace",
                                  "/system/bin/qemu-props", "/dev/socket/qemud", "/dev/qemu_pipe"};

    for (string pipe:knownFiles){
        if (anti_file_exist(pipe)){
            return true;
        }
    }

    return false;

}

NO_STEE
NO_BCF
NO_FLA
NO_SPL
NO_SUB
void* anti_emu_checkQemuArm_get_thread(void *) {

    for(int j = 0; j < 10000; j++) {
        if(global_value != 0x20) {
            checkQemuArm_result = false;
            break;
        }
    }

    checkQemuArm_finish = true;

    return nullptr;
}


NO_STEE
NO_BCF
NO_FLA
NO_SPL
NO_SUB
void anti_emu_checkQemuArm_exec_thread(void) {

    anti_info_log("checkQemuArm_get_thread");
    for(int i = 0; i < 10000; i++) {

        if (checkQemuArm_finish) {
            return;
        }
// FIXME 会编译报错，因为虚机不支持armv5，所以先不用
//#ifdef __arm__
//
//        __asm__ __volatile__(
//                "push {r0};"
//                "push {r1};"
//                :::);
//
//        __asm__ __volatile__ (
//            "mov r0, %0;"
//            "mov r1, #1;"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            "add r1, r1, #1;" "str r1, [r0];"
//            :
//            : "r" (&global_value)
//            :
//        );
//
//        __asm__ __volatile__(
//                "pop {r1};"
//                "pop {r0};"
//                :::);

#ifdef __i386__

            // 执行add之前要先push，最后要pop，保证eax，ebx寄存器环境保存，否则会导致checkQemuArm_finish 变量取值错误
            __asm__ __volatile__(
                "push %%eax;"
                "push %%ebx;"
                "pushfd;"
                :::);

            __asm__ __volatile__ (
                "mov $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"

                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                "add $1, %%eax;" "mov %%eax, (%%ebx);"
                :
                : "b" (&global_value)
                :
            );

            __asm__ __volatile__(
                "popfd;"
                "pop %%ebx;"
                "pop %%eax;"
                :::);

#endif
    }
}


STEE
bool anti_emu_doCheckQemuArm_pc() {

    pthread_t thread_checkQemuArm_get;
    int status;

    status = pthread_create(&thread_checkQemuArm_get, NULL, anti_emu_checkQemuArm_get_thread, NULL);

    if(status){
        perror("pthread_create()");
    }

    anti_emu_checkQemuArm_exec_thread();

    while(!checkQemuArm_finish);

    return checkQemuArm_result;
}



STEE
bool anti_emu_qemu_checkArm_pc(){

    bool result = false;
#ifdef __arm__
//    anti_info_log("start __arm__ checkQemuArm_pc");
//    result = anti_emu_doCheckQemuArm_pc();
//    anti_info_log("checkQemuArm_pc result  %d", result);
#elif __i386__
    anti_info_log("start __i386__ checkQemuArm_pc");
    result = anti_emu_doCheckQemuArm_pc();
    anti_info_log("checkQemuArm_pc result  %d", result);
#endif
    return result;
}





void (*asmcheck)(void);
/**
 * 检测arm的cache特性
 * todo：该方法检测模拟器会不准，且部分真机会出现 signal 11(SIGSEGV) 的 Fatal
 * @return
 */
STEE
bool anti_emu_qemu_checkArm_cache() {

#ifdef __arm__


//     __asm__ __volatile__(
//        "push {r0-r8};"
//        :::
//    );

//
//
//     __asm__ __volatile__(
//        "stmfd sp!, {r4-r8, lr};"
//        "mov r6, #0;"
//        "mov r7, #0;"
//        "mov r8, pc;"
//        "mov r4, #0;"
//        "mov r7, #1;"
//        "mov r5, [r8];"
//        "code:"
//        "add r4, #1;"
//        "mov r8, #12;"
//        "sub r8, #12;"
//        "str r5, [r8];"
//        "add r6, #1;"
//        "cmp r4, #10;"
//        "bge out;"
//        "b code;"
//        "out:"
//        "mov r0, r4;"
//        "ldmfd sp!, {r4-r8, pc};"
//        :"=r"(a)
//        :"r"(a)
//        :"memory"
//     );
//
//   __asm __volatile (
//        "pop {r4-r8};"
//        :::
//    );

    int a = 0;

    //可执行二进制代码
    char code[] =
            "\xF0\x41\x2D\xE9"
            "\x00\x70\xA0\xE3"
            "\x0F\x80\xA0\xE1"
            "\x00\x40\xA0\xE3"
            "\x01\x70\x87\xE2"
            "\x00\x50\x98\xE5"
            "\x01\x40\x84\xE2"
            "\x0F\x80\xA0\xE1"
            "\x0C\x80\x48\xE2"
            "\x00\x50\x88\xE5"
            "\x0A\x00\x54\xE3"
            "\x02\x00\x00\xAA"
            "\x0A\x00\x57\xE3"
            "\x00\x00\x00\xAA"
            "\xF6\xFF\xFF\xEA"
            "\x04\x00\xA0\xE1"
            "\xF0\x81\xBD\xE8";

    // 映射一块可执行内存 PROT_EXEC
    void *exec = mmap(NULL, (size_t) 4096, PROT_EXEC|PROT_WRITE|PROT_READ, MAP_ANONYMOUS | MAP_SHARED, -1, (off_t) 0);
    memcpy(exec, code, sizeof(code) + 1);
    //强制赋值到函数
    asmcheck = (void (*)(void)) exec;

    anti_info_log("exec address 0x%x", exec);
    //执行函数
    asmcheck();
    __asm __volatile (
    "mov %0,r0;"
    :"=r"(a)
    ::"memory"
    );
    munmap(exec, 4096);

//     __asm__ __volatile__(
//        "push {r0-r8};"
//        :::
//    );


    anti_info_log("checkQemuArm_cache a: %d", a);
    // todo: 判断方法会不准，部分真机会返回非10的值
    // 非10就是模拟器
    return !(a == 10);
#else
    return false;
#endif

}



STEE
void anti_emu_handler_signal(int signo) {
    anti_info_log("bkpt handler_signal no: %d", signo);
    exit(-1);
}

STEE
void anti_emu_setupSigTrap() {
    anti_info_log("bkpt anti_emu_setupSigTrap");
    // BKPT throws SIGTRAP on nexus 5 / oneplus one (and most devices)
    signal(SIGTRAP, anti_emu_handler_signal);
}
STEE
void anti_emu_setupSigSegv() {
    anti_info_log("bkpt anti_emu_setupSigSegv");
    signal(SIGSEGV, anti_emu_handler_signal);
}

STEE
void anti_emu_setupSigBus(){
    anti_info_log("bkpt anti_emu_setupSigBus");
    // BKPT throws SIGBUS on nexus 4
    signal(SIGBUS, anti_emu_handler_signal);
}

// for arm. This will cause a SIGSEGV on some QEMU or be properly respected
STEE
void anti_emu_tryBKPT(){
    anti_info_log("bkpt tryBKPT");
#ifdef __arm__
    __asm__ __volatile__ ("bkpt 255");
#endif
}



/**
 * 通过arm bkpt设置断点来检测模拟器
 * 但是存在部分真机会被系统抛出信号
 * SIGTRAP on nexus 5 / oneplus one
 * SIGBUS on nexus 4
 * @return
 */
STEE
int anti_emu_doCheckQemuArm_bkpt() {
// start check，fork process to asm bkpt
    int status = 0;

    pid_t child = fork();
    int child_status= 0;

    anti_info_log("bkpt child: %d ", child);

    // fork process run
    if (child == 0) {
//        anti_emu_setupSigTrap();
//        anti_emu_setupSigBus();
        anti_emu_setupSigSegv();
        anti_emu_tryBKPT();
    }
        // fork process failed
    else if (child == -1) {
        status = -1;
    }
        // current process run
    else {

        anti_info_log("bkpt waitpid start");
        /**
         * 一直等待子进程状态返回
         * option: WNOHANG 表示立刻返回
         * option: 0 默认只等待终止状态的子进程
         * 返回: 返回子进程pid
         */
        // 会有一直阻塞的风险
        pid_t waitpidResult = waitpid(child, &child_status, 0);
        (void )waitpidResult;

        anti_info_log("bkpt waitpid end waitpidResult: %d", waitpidResult);
        anti_info_log("bkpt child_status: %d", child_status);

        // 检测进程是否调用exit函数退出
        bool invokeExit = WIFEXITED(child_status);

        anti_info_log("bkpt invokeExit: %s", invokeExit ? "true" : "false");

        // 子进程正常退出
        if (!invokeExit) {
            status = 0;
        }
            // Didn't exit properly - very likely an emulator
        else {
            status = 2;
        }

        // Ensure child is dead
        dx_kill(child, SIGKILL);
    }
    anti_info_log("bkpt result status: %d", status);

    return status;
}



STEE
bool anti_emu_qemu_checkArm_bkpt(){

    anti_info_log("bkpt start");

    int status = 0;
    // 只针对arm平台的模拟器

    //#ifdef __arm64__
#ifdef __arm__
    anti_info_log("bkpt start arm, armv7");
    status = anti_emu_doCheckQemuArm_bkpt();
#elif __x86_x64__
    anti_info_log("bkpt start __x86_x64__");
#elif __aarch64__
    anti_info_log("bkpt start arm64-v8a");
#elif __i386__
    anti_info_log("bkpt start __x86__");
#else
    anti_info_log("bkpt start other");
#endif

    // status 大于 0 表示模拟器
    bool result = status > 0;

    return result;
}





