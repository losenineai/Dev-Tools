//
// Created by white on 2018/5/14.
//

#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <anti_utils.h>
#include <anti_crash_log_prefix.h>
#include <platform.h>
#include <jni_utils.h>
#include <jni_comm_utils.h>
#include <properties.h>
#include "utils_anti_log.h"
#include "string_utils.h"
#include "comm_utils.h"
#include "anti_multirun.h"
#include "dx_anti_stee.h"

#include "cutils/sockets.h"
#include "anti_result_handler.h"

int _multirun_crash_code = 0;

typedef struct _Socket_Data {
    char *port;
    char *data;
} Socket_Data;

void* anti_multirun_start_server(void *arg) {

    Socket_Data *socket_data = (Socket_Data *)arg;

    int socket = -1;

    int server_fd = socket_local_server (socket_data->port, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);

    while((socket = accept(server_fd, NULL, NULL)) > 0) {

        if(send(socket, socket_data->data, strlen(socket_data->data), 0) == -1)
        {
            perror("send");
            close(socket);
            exit(0);
        } else {
            close(socket);
        }

        //send(scokfd, buff, buff_size, MSG_DONTWAIT);
    }

    return NULL;
}

bool anti_multirun_start_client(void *arg) {

    Socket_Data *socket_data = (Socket_Data *)arg;

    int socket = -1;

    ssize_t socket_size;
    unsigned char socket_buffer[15];
    memset(socket_buffer,0, sizeof(socket_buffer));

    int flag = true;
    while(flag)
    {
        while(socket < 0)
        {
            socket = socket_local_client(socket_data->port, ANDROID_SOCKET_NAMESPACE_ABSTRACT, SOCK_STREAM);
            sleep(1);
        }

        sleep(1);

        socket_size = recv(socket, socket_buffer, sizeof(socket_buffer), 0);

        flag = false;
    }

    if (memcmp(socket_buffer, socket_data->data, socket_size)) {
        _multirun_crash_code = ANTI_MULTIRUN_ERR_SOCKET;
        return true;

    }

    return false;

}

bool anti_multirun_socket_check() {

    pthread_t newthread;
    int ret = 0;

    string socketPort = anti_platform_getPkgName() + "_5af66a00be5bb647";

    /* init socket data */
    Socket_Data *socket_data = new Socket_Data();
    socket_data->port = new char[socketPort.length() + 1];
    socket_data->data = new char[15];
    strcpy(socket_data->port, socketPort.data());
    socket_data->port[socketPort.length()] = '\0';
    //sprintf(socket_data->data, "%d_%d\n", getuid(), getpid());
    sprintf(socket_data->data, "%d_\n", getuid());

//    LOGE("anti_multirun_check: port: %s", socket_data->port);
//    LOGE("anti_multirun_check: data: %s", socket_data->data);

    /* start server to send data */
    dx_pthread_create(ret, &newthread, NULL, anti_multirun_start_server, socket_data);

    /* wait  */
    anti_util_sleepSEC(1);

    /* start client to recv data */
    return anti_multirun_start_client(socket_data);

}

/**
 * FIXME 逻辑有问题，容易会误判
 * @return
 */
bool anti_multirun_pkg_check() {

    vector<string> directoryExitsList;
    string filter = anti_str_format("u0_a%d", getuid() - 10000);
    string cmd_result = anti_util_executeCMD("ps", filter);
    string word;

    int lastIndex = 0;

    for (int i = 0; i < cmd_result.length(); ++i) {
        if (cmd_result.at(i) == '\n') {
            string stringin = cmd_result.substr(lastIndex, i - lastIndex + 1);
            word = anti_str_trim_copy(stringin);
            anti_info_log("word.insert(0, \"/data/data/\"):%s", word.insert(0, "/data/data/").c_str());
            if (access(word.insert(0, "/data/data/").c_str(), F_OK) == 0) {
                anti_info_log("word:%s", word.c_str());
                bool isExit = false;
                for (auto &it:directoryExitsList) {
                    if (strcmp(it.c_str(), word.c_str()) == 0) {
                        isExit = true;
                        break;
                    }
                }
                if (!isExit) {
                    directoryExitsList.push_back(word);
                }
            }

            lastIndex = i;
        }
    }

    return directoryExitsList.size() > 1;
}

STEE
bool anti_multirun_xiaomi_check() {

    char value[256] = { 0 };
    if (anti_property_get("ro.miui.ui.version.name", value, nullptr) <= 0)
        return false;

    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();
    jobject jcontext = anti_jni_getGlobalContext(env);

    // ctx.getExternalFilesDir(null).getPath();
    jobject jfile = anti_jni_callMethodByName(env, jcontext, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;", nullptr);
    jstring jpath = (jstring)anti_jni_callMethodByName(env, jfile, "getPath", "()Ljava/lang/String;");
    string path = anti_jni_jstring_to_string(env, jpath);

    anti_jni_delete_ref(env, jfile);
    anti_jni_delete_ref(env, jpath);

    if(path.find("/storage/emulated/999/") != string::npos) {
        _multirun_crash_code = ANTI_MULTIRUN_ERR_XIAOMI_MULTI;
        return true;
    }

    if(path.find("/storage/emulated/10") != string::npos || path.find("/storage/emulated/11") != string::npos) {
        _multirun_crash_code = ANTI_MULTIRUN_ERR_XIAOMI_SYSTEM;
        return true;
    }

    return false;
}

STEE
void *DXAnti_detect_multirun(void *arg) {

    if(anti_multirun_xiaomi_check() || anti_multirun_socket_check()) {

        // 统一anti结果处理
        AntiResult result = {AntiType::AT_MULTIRUN, _multirun_crash_code};
        AntiResultHandler::getInstance()->HandleResult(&result);
    }

    return NULL;
}

STEE
int DXAnti_detect_multirun_async() {

    pthread_t newthread;
    int ret = -1;

    dx_pthread_create(ret, &newthread, NULL, DXAnti_detect_multirun, NULL);

    return ret;
}


STEE
void DXAnti_detect_multirun_entry() {
    DXAnti_detect_multirun_async();
}