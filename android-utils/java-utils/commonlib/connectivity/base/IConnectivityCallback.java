package com.auric.intell.commonlib.connectivity.base;

/**
 * @author white
 * @description：连接回调
 * @date 2017/9/5
 */

public interface IConnectivityCallback<T> {

    void onResult(T result);
}
