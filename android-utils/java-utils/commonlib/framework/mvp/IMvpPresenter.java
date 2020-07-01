package com.auric.intell.commonlib.framework.mvp;

/**
 * @author white
 * @description：MVPS 框架中的Presenter (自创新框架MVPS)
 * @date 2016/12/2
 */

public interface IMvpPresenter<S extends IMvpStatus, V extends IMvpView> {

    /**
     * 关联View
     * @param view
     */
    void attachView(V view);

    /**
     * 移除View
     * @param view
     */
    void detachView(V view);

}
