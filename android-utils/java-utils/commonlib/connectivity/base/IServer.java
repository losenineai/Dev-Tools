package com.auric.intell.commonlib.connectivity.base;

/**
 * @author white
 * @description：服务端接口
 * @date 2017/9/1
 */

public interface IServer<D> {

    /**
     * 建立服务
     */
    void buildServer();

    /**
     * 关闭服务
     */
    void closeServer();

    /**
     * 发送数据
     * @param data
     */
    void sendData(D data);

}
