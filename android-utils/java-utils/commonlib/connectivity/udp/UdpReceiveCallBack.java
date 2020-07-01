package com.auric.intell.commonlib.connectivity.udp;

/**
 * Created by sh on 2017/11/13.
 */

public interface UdpReceiveCallBack {
    void onReciverMessage(String serverIp,String message);
}
