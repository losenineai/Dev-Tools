//
// Created by blue on 2018/11/19.
//

#include "dx_anti.h"

#include <jni_utils.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <include/anti_utils.h>
#include <include/anti_crash_log_prefix.h>
#include "dx_anti_stee.h"
#include "anti_result_handler.h"
#include "jni_comm_utils.h"

#ifdef ANTI_RETURN_ON
static DXAnti_ResultCallback g_callback = NULL;

STEE
void DXAnti_set_monitor_callback(DXAnti_ResultCallback callback){
    if(callback != NULL){
        g_callback = callback;
    }
}
#endif

STEE
void antiResultCallbackImp(AntiResult *result) {
#ifdef ANTI_RETURN_ON
    if(result == NULL){
        return;
    }

    AntiType antiType = result->type;
    void *args = NULL;

    if(!result->emulatorStr.empty()){
        args = (void *)result->emulatorStr.c_str();
    }

    anti_info_log("antiResultCallbackImp pid:%d, antiType:%d g_callback:%p", getpid(), antiType, g_callback);
    int flag = -1;
    switch (antiType) {
        case AntiType::AT_DEBUG:
            flag = DXANTI_FLAG_DEBUG;
            break;
        case AntiType::AT_EMULATOR:
            flag = DXANTI_FLAG_EMULATOR;
            break;
        case AntiType::AT_FLAW_JANUS:
            flag = DXANTI_FLAG_FLAW_JANUS;
            break;
        case AntiType::AT_INJECT:
            flag = DXANTI_FLAG_INJECT;
            break;
        case AntiType::AT_LOG:
            flag = DXANTI_FLAG_LOG;
            break;
        case AntiType::AT_MEMDUMP:
            flag = DXANTI_FLAG_MEMDUMP;
            break;
        case AntiType::AT_MULTIRUN:
            flag = DXANTI_FLAG_MULTIRUN;
            break;
        case AntiType::AT_ROOT:
            flag = DXANTI_FLAG_ROOT;
            break;
        case AntiType::AT_VIRTUAL_APP:
            flag = DXANTI_FLAG_VIRTUAL_APP;
            break;
        default:
            break;
    }

    if (g_callback != NULL && flag > 0) {
        g_callback(flag, args);
    }
#endif
}

STEE
DXAntiRetStatus DXAnti_monitor_init(JavaVM *vm, jobject jContext) {

    static bool l_started = false;

    if (l_started) {
        LOG_WARNING("%d", ANTI_INIT_FAIL_CODE + INIT_FAIL_ALREADY_INIT);
        return INIT_FAIL_ALREADY_INIT;
    }
    l_started = true;


    DXAnti_AndroidRuntime::init(vm);

    /**
     * Context初始化，使用外部传入context，并 env->NewGlobalRef 方式保存
     * FIXME
     * 废弃使用 anti_jni_getGlobalContext。循环反射获取context实例，因4.2或以下版本在子线程下无法获取Context，导致一直死循环
     */
    DXAnti_JNIEnvGuard guard;
    JNIEnv *env = anti_get_jni_env();

    // 兼容入参jContext为空情况，尝试通过反射获取实例
    if (jContext == NULL) {
        jobject jcontext = anti_jni_getGlobalContext(env);
        if (jcontext == NULL) {
            LOG_WARNING("%d", ANTI_INIT_FAIL_CODE + INIT_FAIL_PARAMS_NULL);
            return INIT_FAIL_PARAMS_NULL;
        }
    }
    else{
        anti_jni_setContext(env, jContext);
    }


    // 设置可ptrace
    prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);

    AntiResultHandler::getInstance()->Init();
    AntiResultHandler::getInstance()->SetResultCallback(antiResultCallbackImp);

    return INIT_SUCCESS;
}


