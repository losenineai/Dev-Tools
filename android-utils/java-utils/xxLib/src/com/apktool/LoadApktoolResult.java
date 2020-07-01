package com.apktool;

import com.apktool.access.IMultiApkHelper;

/**
 * Created by mushroom0417 on 12/16/15.
 */
public class LoadApktoolResult {

    public static final int LOAD_ASSIST_ERR_NONE = 200;
    public static final int LOAD_ASSIST_ERR_LOAD_CLASS_FAIL = 201;
    public static final int LOAD_ASSIST_ERR_NEW_INSTANCE_FAIL = 202;
    public static final int LOAD_ASSIST_ERR_WRONG_INSTANCE_FAIL = 203;
    public static final int LOAD_ASSIST_ERR_CONN_SOCKET_FAIL = 204;
    public static final int LOAD_ASSIST_ERR_GET_ASSIST_VIEW_FAIL = 205;

    public int mErrCode;
    public IMultiApkHelper iMultiApkHelper;

    public LoadApktoolResult(int errCode) {
        this.mErrCode = errCode;
    }

}
