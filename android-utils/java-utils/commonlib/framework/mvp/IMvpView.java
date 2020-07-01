package com.auric.intell.commonlib.framework.mvp;

/**
 * @author white
 * @description：MVPS 架构的View (自创新框架MVPS)
 * @date 2016/12/2
 */

public interface IMvpView<S extends IMvpStatus> {

    /**
     * 显示状态变化
     * @param status
     */
    void onShowStatus(S status);
}
