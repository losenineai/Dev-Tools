package com.auric.intell.commonlib.connectivity.base;


/**
 * @author white
 * @description：客户端接口
 * @date 2017/9/1
 */

public interface IClient<D> {

    /**
     * 连接
     */
    void connect();

    /**
     * 关闭连接
     */
    void closeConnection();

    /**
     * 发送数据
     * @param data
     */
    void sendData(D data);


}
