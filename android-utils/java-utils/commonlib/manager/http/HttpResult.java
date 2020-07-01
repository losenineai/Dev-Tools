package com.auric.intell.commonlib.manager.http;

/**
 * Created by white on 16/7/19.
 */
public class HttpResult {

    public int resultCode = RESULT_CODE_NONE;

    public int responseCode = 0;

    public HttpHeader header;

    public Object dataObj;

    public static final int RESULT_CODE_NONE = 0;
    public static final int RESULT_CODE_SUCCESS = 1;
    public static final int RESULT_CODE_ERR_NOT_200 = 2;
    public static final int RESULT_CODE_ERR_NO_NET = 3;
    public static final int RESULT_CODE_ERR_IO = 4;
}
