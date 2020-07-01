package com.auric.intell.commonlib.connectivity.ap;

import java.io.Serializable;

/**
 * @author white
 * @description：连接参数类
 * @date 2017/9/26
 */

public class WifiApConnectParams implements Serializable {

    private int port;
    private String SSID;
    private String password;

    public int getPort() {
        return port;
    }

    public WifiApConnectParams setPort(int port) {
        this.port = port;
        return this;
    }

    public String getSSID() {
        return SSID;
    }

    public WifiApConnectParams setSSID(String SSID) {
        this.SSID = SSID;
        return this;
    }

    public String getPassword() {
        return password;
    }

    public WifiApConnectParams setPassword(String password) {
        this.password = password;
        return this;
    }

    public static WifiApConnectParams build() {
        return new WifiApConnectParams();
    }
}
