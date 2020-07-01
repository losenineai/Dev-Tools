package com.auric.intell.commonlib.connectivity.ap;


/**
 * @author white
 * @description：TCP Client观察者
 * @date 2017/9/6
 */

public interface TcpServerObsv {

    void onStateChange(TcpServer.TcpServerState state);
    void onReceiveData(TcpClient client, String data);
}
