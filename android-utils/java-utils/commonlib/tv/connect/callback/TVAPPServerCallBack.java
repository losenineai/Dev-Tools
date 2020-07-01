package com.auric.intell.commonlib.tv.connect.callback;

/**
 * Created by sh on 2018/1/30.
 */

public interface TVAPPServerCallBack {
    void onConnect();
    void onDisConnect();
    void onReceivedTcpData(String data);
    void onReceivedUdpData(String ip,String data);
}
