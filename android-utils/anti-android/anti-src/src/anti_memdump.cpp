//
// Created by white on 2018/5/15.
//
#include <sys/inotify.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <pthread.h>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <anti_crash_log_prefix.h>
#include "utils_anti_log.h"
#include "comm_utils.h"
#include "platform.h"
#include "anti_memdump.h"
#include "dx_anti_stee.h"
#include "anti_utils.h"
#include "dx_watcher.h"
#include "anti_result_handler.h"
#include "dx-libc/dx_libc.h"

static std::list<std::string> l_process_paths(0);
static std::list<std::string> l_thread_paths(0);
static bool l_auto_rm_switch_on = false;

STEE
void anti_memdump_add_process_watch(pid_t pid) {

    char sz[256] = {0};

    l_process_paths.clear();

    dx_sprintf(sz, "/proc/%d/mem", pid);
    watcher_add(sz);
    l_process_paths.push_back(sz);

    dx_sprintf(sz, "/proc/%d/pagemap", pid);
    watcher_add(sz);
    l_process_paths.push_back(sz);
}

STEE
void anti_memdump_add_thread_watch(pid_t pid, pid_t tid) {

    char sz[256] = {0}; // [sp+44h] [bp-11Ch]@1

    dx_sprintf(sz, "/proc/%d/mem", pid);
    watcher_add(sz);
    if (l_auto_rm_switch_on) {
        l_thread_paths.push_back(sz);
    }

    dx_sprintf(sz, "/proc/%d/pagemap", pid);
    watcher_add(sz);
    if (l_auto_rm_switch_on) {
        l_thread_paths.push_back(sz);
    }

    dx_sprintf(sz, "/proc/%d/task/%d/mem", pid, tid);
    watcher_add(sz);
    if (l_auto_rm_switch_on) {
        l_thread_paths.push_back(sz);
    }

    dx_sprintf(sz, "/proc/%d/task/%d/pagemap", pid, tid);
    watcher_add(sz);
    if (l_auto_rm_switch_on) {
        l_thread_paths.push_back(sz);
    }
}

STEE
static void anti_memdump_thread_exit_handler(int s) {
    pthread_exit(0);
}

STEE
void * anti_memdump_monitor_pid(void* arg) {

    pid_t pid = *(pid_t* )arg;
    char sz[256] = {0};
    uintptr_t pass_time = 0;
    int auto_rm_time = 3600; // 适当调整

    free(arg);

    struct sigaction act = {};
    act.sa_handler = anti_memdump_thread_exit_handler;
    sigaction(SIGUSR1, (const struct sigaction *)&act, NULL);

    dx_sprintf(sz, "/proc/%d/task/", pid);
    bool flag = true;
    while ( flag )
    {
        struct dirent* dir;
        DIR* dirp = NULL;
        while (dirp == NULL) {
            dirp = opendir(sz);
        }

        if (pass_time >= auto_rm_time) {
            l_auto_rm_switch_on = true;
            l_thread_paths = l_process_paths;
        }

        while ( (dir = readdir(dirp)) != NULL )
        {
            const char *dirname = &dir->d_name[0];
            // LOGE("%s(%d) dirname: %s", __FUNCTION__, __LINE__, dirname);
            if ( 0 != dx_strcmp(dirname, ".") && 0 != strcmp(dirname, "..") )
            {
                struct stat status = {0};
                char tid_file[0x100] = {0};

                dx_sprintf(tid_file, "%s%s", sz, dirname);
                // LOGE("%s(%d) tid_file: %s", __FUNCTION__, __LINE__, tid_file);
                if ( lstat(tid_file, &status) != -1 && (status.st_mode & 0xF000) == 0x4000 ) // 0x4000目录
                {
                    pid_t tid = atoi(dirname);
                    anti_memdump_add_thread_watch(pid, tid);
                }
            }
        }

        closedir(dirp);

        if (pass_time >= auto_rm_time) {
            watcher_auto_remove(l_thread_paths);
            l_auto_rm_switch_on = false;
            pass_time = 0;
        }

        sleep(2u);
        pass_time += 2;
    }


    return NULL;
}


STEE
void *DXAnti_detect_memdump(void *arg) {
    pid_t pid = *(pid_t* )arg;
    free(arg);

    if ( watcher_init() )
    {
        int ret = 0;
        pthread_t newthread;
        anti_memdump_add_process_watch(pid);
        dx_anti_pthread_create(ret, &newthread, NULL, anti_memdump_monitor_pid, malloc_wraper((void* )&pid, sizeof(pid)));

        watcher_dispose();
        watcher_uninit();
        pthread_kill(newthread, SIGUSR1);

        // 统一anti结果处理
        AntiResult result = {AntiType::AT_MEMDUMP, ANTI_MEMDUMP_ERR_BASE};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    return NULL;
}

STEE
int DXAnti_detect_memdump_async(pthread_t &tid, pid_t pid) {
    int ret = -1;

    dx_anti_pthread_create(ret, &tid, NULL, DXAnti_detect_memdump, malloc_wraper((void* )&pid, sizeof(pid)));

    return ret;
}



STEE
void DXAnti_detect_memdump_entry() {
    pid_t pid = getpid();
    pthread_t tid;
    DXAnti_detect_memdump_async(tid, pid);
}