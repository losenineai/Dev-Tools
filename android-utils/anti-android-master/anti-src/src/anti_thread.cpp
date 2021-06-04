//
// Created by white on 2018/5/15.
//

#include <jni.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <pthread.h>
#include "dx_watcher.h"
#include <dirent.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include "anti_utils.h"
#include "anti_thread.h"
#include "dx_anti_stee.h"

//     DXAnti_detect_thread
//         anti_anti_thread_of_process_debug
//             anti_anti_thread_body
//                 anti_scan_process_threads
//                 anti_check_process_stopped

STEE
bool anti_thread_scan_process_threads(pid_t pid) {

    struct dirent *p_dirent;
    const char *subfilename;
    pid_t tid;
    FILE *fp;
    DIR *dirp;
    char sz_task_dir[256];
    char status_filepath[256];
    char line[1024];

    sprintf(sz_task_dir, "/proc/%d/task/", pid);

    dirp = opendir(sz_task_dir);
    if (NULL == dirp) {
        return false;
    }

    p_dirent = readdir(dirp);
    while (p_dirent)
    {
        subfilename = &p_dirent->d_name[0];

        if (0 != strcmp(subfilename, ".") || 0 != strcmp(subfilename, "..")) {
            tid = atol(subfilename);
            if (tid != 0 && pid != tid)
            {
                snprintf(status_filepath, 0x100u, "/proc/%d/task/%d/status", pid, tid);
                fp = fopen(status_filepath, "r");
                // LOGE("%s(%d), fp: %p, status_filepath: %s", __FUNCTION__, __LINE__, fp, status_filepath);
//                LOG_CRASH("- status_filepath: %s", status_filepath);
                if ( fp )
                {
                    while ( fgets(line, 1024, fp) )
                    {
//                        LOG_CRASH("line: %s", line);

                        if ( !strncmp(line, "State:", strlen("State:")) &&
                             (strcasestr(line, "T (stopped)") || strcasestr(line, "t (tracing stop)")) )
                        {
                            fclose(fp);
                            closedir(dirp);
                            return true;
                        }

                        /****************  blue add for 减少分析文件长度 start ****************/
                        if (0 == strncmp(line, "TracerPid:", strlen("TracerPid:"))) {
                            break;
                        }
                        /****************  blue add for 减少分析文件长度 end ****************/

                        /****************  matao add start ****************/
                        /*else if (0 == strncmp(line, "TracerPid:", strlen("TracerPid:")))
                        {
                            int tracer_pid = 0;
                            char nouse[64];
                            sscanf(line, "%s %d", nouse, &tracer_pid);

                            if (tracer_pid != 0 && tracer_pid != getpid()) {
                                fclose(fp);
                                closedir(dirp);
                                return true;
                            }
                            break;
                        }*/
                        /****************  matao add end ****************/
                    }
                    fclose(fp);
                }
            }
        }
        p_dirent = readdir(dirp);
    }

    closedir(dirp);
    return false;
}

STEE
bool anti_thread_check_process_stopped(int pid) {

    FILE *fp;
    char filepath[256] = {0};
    char line[1024];

    snprintf(filepath, sizeof(filepath), "/proc/%d/status", pid);
    fp = fopen(filepath, "r");

    if (NULL == fp) {
        return false;
    }

    while (fgets(line, sizeof(line), fp)) {

//        LOG_CRASH("anti_thread_check_process_stopped: line  %s", line);

        if (strncmp(line, "State:", strlen("State:")) == 0
            && (0 != strcasestr(line, "T (stopped)") ||
                0 != strcasestr(line, "(zombie)") ||
                0 != strcasestr(line, "t (tracing stop)"))) {

            /**************** libSecShell.so start ***************/
            fclose(fp);
            return true;
            /****************  libSecShell.so end ***************/

            /****************  libDexHelper.so start ***************/
            /*char nouse[64];
            pid_t tracer_pid = 0;
            while (fgets(line, sizeof(line), fp)) {
                // LOGE("%s(%d) line2: %s", __FUNCTION__, __LINE__, line);
                if (0 == strncmp(line, "TracerPid:", strlen("TracerPid:"))) {
                    sscanf(line, "%s %d", nouse, &tracer_pid);

                    LOG_CRASH("anti_thread_check_process_stopped: tracer_pid: %d  pid: %d", tracer_pid, getpid());

                    break;
                }
            }

            fclose(fp);

            return tracer_pid > getpid();*/
            /****************  libDexHelper end ***************/

            /****************  matao add start ****************/
            /*if (tracer_pid == 0 || tracer_pid == getpid()) {
                return false;
            }

            return true;
             */
            /****************  matao add end ****************/
        }

        /****************  blue add for 减少分析文件长度 start ****************/
        if (0 == strncmp(line, "TracerPid:", strlen("TracerPid:"))) {
            break;
        }
    }

    fclose(fp);
    return false;
}

STEE
void *DXAnti_detect_thread(void *arg) {

    pid_t pid = *(pid_t *) arg;
    free(arg);

    while (anti_thread_check_process_stopped(pid) != 1 && anti_thread_scan_process_threads(pid) != 1)
        sleep(1u);

    dx_kill(pid, SIGKILL);

    return NULL;
}

STEE
int DXAnti_detect_thread_async(pid_t pid) {

    pthread_t newthread;
    int ret = -1;

    dx_pthread_create(ret, &newthread, NULL, DXAnti_detect_thread, malloc_wraper(&pid, sizeof(pid)));

    return ret;
}


STEE
void DXAnti_detect_thread_entry() {
    pid_t pid = getpid();
    DXAnti_detect_thread_async(pid);
}