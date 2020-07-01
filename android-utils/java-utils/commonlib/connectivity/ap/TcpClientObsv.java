package com.auric.intell.commonlib.connectivity.ap;

/**
 * @author white
 * @description：TCP Client观察者
 * @date 2017/9/6
 */

public interface TcpClientObsv {

    void onStateChange(TcpClient client, TcpClient.TcpClientState state);
    void onReceiveData(TcpClient client, String data);
}
