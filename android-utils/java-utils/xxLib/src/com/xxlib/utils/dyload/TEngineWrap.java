package com.xxlib.utils.dyload;

import android.os.Handler;

/**
 * Created by jammy on 15/11/6.
 */
public class TEngineWrap {

    public static final String TEngineClassName = "com.xxtengine.core.TEngine";
    public static final String TEngineUIManagerClassName = "com.xxtengine.core.TEngineUIManager";


    public static final int EVENT_NETWORK_ERR = 0;
    public static final int EVENT_DIRTY_DATA_ERR = 1;
    public static final int EVENT_VERIFY_LOOP_HAPPENDED = 2;
    public static final int EVENT_REVERIFY = 3;
    public static final int EVENT_LOGIN_STATE_ERR = 4;
    public static final int EVENT_FILE_NOT_FOUND = 5;
    public static final int EVENT_VERFIY_SUCCESSFULLY = 6;
    public static final int EVENT_SCRIPT_TYPE_ERR = 7;
    public static final int EVENT_RE_INIT_ERR = 8;
    public static final int EVENT_SCRIPT_VERITY_ERR = 9;
    public static final int EVENT_SHOULD_PAY_BUT_DO_NOTHING = 10;

    public static final int EVENT_NO_SCRIPT_BINDED_ERR = 0;
    public static final int EVENT_SCRIPT_LOADING_ERR = 1;
    public static final int EVENT_SCRIPT_PLAY_ERR = 2;
    public static final int EVENT_SCRIPT_PLAY_START = 3;
    public static final int EVENT_SCRIPT_PLAY_END = 4;
    public static final int EVENT_SCRIPT_PLAY_STOP = 5;
    public static final int EVENT_SCRIPT_PLAY_RESTART = 6;
    public static final int EVENT_RUNTIME_HAD_BEEN_SHUTDOWN = 7;

    public static boolean getEngineEnvInitState() {
        return false;
    }


    public static boolean engineEnvInit() {
        return false;
    }


    public static boolean isTEngineEnvInitException() {
        return true;
    }

    public static Object createInstance(String path) {
        return null;
    }

    public static Object createInstance(byte[] userByte, long uin, String loginkey, String scriptPath, int scriptId) {
        return null;
    }


    public static void playScript(Object mScriptEngine, Handler.Callback paramCallback) {

    }

    public static boolean init(Object mScriptEngine, Handler.Callback paramCallback) {
        return true;
    }


    public static boolean isRunning(Object mScriptEngine) {
        return false;
    }


    public static void stopScript(Object mScriptEngine) {
    }
    public static void hideAllViews() {
    }
    public static void showAllViews() {
    }
}
