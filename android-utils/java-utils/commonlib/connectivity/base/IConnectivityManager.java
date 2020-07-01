package com.auric.intell.commonlib.connectivity.base;

import android.content.Context;

/**
 * @author white
 * @description：端对端连接类接口
 * @date 2017/9/5
 */

public interface IConnectivityManager<T, C, R> {

    /**
     * 初始化服务
     * @param context
     */
    void init(Context context);

    /**
     * 建立服务，并且接受客户端
     * @param params
     * @param callback
     */
    void buildServerAndReceive(T params, IConnectivityCallback<C>  callback);

    /**
     * 客户端连接服务端
     * @param params
     * @param callback
     */
    void connectServer(T params, IConnectivityCallback<C> callback);

    /**
     * 发送数据
     * @param msg
     */
    void sendMessage(R msg);

    /**
     * 释放资源
     */
    void release();
}
