package com.xxlib.utils.xxPlugin;

import com.xxAssistant.DanMuKu.plugin.apk.IXXPlugin;

/**
 * Created by jammy on 15/11/2.
 */
public class LoadXXPluginResult {

    public static final int LOAD_ASSIST_ERR_NONE = 200;
    public static final int LOAD_ASSIST_ERR_LOAD_CLASS_FAIL = 201;
    public static final int LOAD_ASSIST_ERR_NEW_INSTANCE_FAIL = 202;
    public static final int LOAD_ASSIST_ERR_WRONG_INSTANCE_FAIL = 203;
    public static final int LOAD_ASSIST_ERR_CONN_SOCKET_FAIL = 204;
    public static final int LOAD_ASSIST_ERR_GET_ASSIST_VIEW_FAIL = 205;

    public int mErrCode;
    public IXXPlugin mXXPlugin;

    public LoadXXPluginResult(int errCode) {
        this.mErrCode = errCode;
    }
}

