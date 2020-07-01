package com.xxlib.gpgame.inter;

/**
 * Created by rosejames on 16/5/25.
 */
public class GPConnectResult {
    public static int GP_CONNECT_RESULT_SUC = 1001;
    public static int GP_CONNECT_RESULT_FAILED_START_NO_SCHEME = 1002;
    public static int GP_CONNECT_RESULT_FAILED_JSON_EXCEPTION = 1003;
    public static int GP_CONNECT_RESULT_FAILED_PARAMS_ILLEGAL = 1004;
    public static int GP_CONNECT_RESULT_FAILED_UNKNOWN = 1005;

    private int mErrorCode;
    private String mErrMessage = "";

    public String getErrMessage() {
        return mErrMessage;
    }

    public GPConnectResult setErrMessage(String errMessage) {
        mErrMessage = errMessage;
        return this;
    }

    public int getErrorCode() {
        return mErrorCode;
    }

    public GPConnectResult setErrorCode(int errorCode) {
        mErrorCode = errorCode;
        return this;
    }
}
