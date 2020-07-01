package com.auric.intell.commonlib.connectivity.ap;

import com.auric.intell.commonlib.connectivity.base.ConnectivityResult;

/**
 * @author white
 * @description：
 * @date 2017/9/5
 */

public class WifiApResult extends ConnectivityResult<String> {

    public static final int AP_OPEN_SUCCESS = BASE_BUILD_SERVER + 1;
    public static final int AP_OPEN_ERROR_TIME_OUT = BASE_BUILD_SERVER + 11;
    public static final int AP_OPEN_ERROR_OTHERS = BASE_BUILD_SERVER + 12;
    public static final int SERVER_BUILD_SUCCESS = BASE_BUILD_SERVER + 20;
    public static final int SERVER_BUILD_ERROR_HOST = BASE_BUILD_SERVER + 30;
    public static final int SERVER_BUILD_ERROR_OTHERS = BASE_BUILD_SERVER + 31;

    public static final int WIFI_OPEN_SUCCESS = BASE_CONNECT_CLIENT + 1;
    public static final int WIFI_OPEN_ERROR = BASE_CONNECT_CLIENT + 11;
    public static final int WIFI_CONNECTED_SUCCESS = BASE_CONNECT_CLIENT + 101;
    public static final int WIFI_CONNECTED_ERROR_AUTHENTICATING = BASE_CONNECT_CLIENT + 102;
    public static final int WIFI_CONNECTED_ERROR_SSID_NOT_FOUND = BASE_CONNECT_CLIENT + 103;
    public static final int WIFI_CONNECTED_ERROR_OTHERS = BASE_CONNECT_CLIENT + 104;
    public static final int CLIENT_CONNECTED_SUCCESS = BASE_CONNECT_CLIENT + 201;
    public static final int CLIENT_CONNECTED_ERROR = BASE_CONNECT_CLIENT + 202;

    public static final int DATA_RECEIVED = BASE_DATA + 1;

    public WifiApResult(int code, String data) {
        super(code, data);
    }

}
