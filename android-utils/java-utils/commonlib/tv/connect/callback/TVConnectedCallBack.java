package com.auric.intell.commonlib.tv.connect.callback;

/**
 * Created by sh on 2018/1/10.
 */

public interface TVConnectedCallBack {
    void onSuccess();
    void onSendMessage();
    void onFail();
    void onClosed();
    void onReceiveTcpData(String data);
    void onReceiveUdpData(String ip,String data);
}
