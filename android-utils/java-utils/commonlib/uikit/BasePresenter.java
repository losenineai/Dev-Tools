package com.auric.intell.commonlib.uikit;

import android.app.Activity;
import android.content.Context;
import android.support.v4.app.Fragment;

/**
 * Created by zhangxiliang on 2017/3/22.
 */

public abstract class BasePresenter<V extends BaseUiView> {

    protected V mUiView;

    protected Context mContext;


    public BasePresenter(V uiView) {
        this.mUiView = uiView;
        try {
            mContext = ((Activity) uiView);
        } catch (Exception e) {
            mContext = ((Fragment) uiView).getContext();

        }
    }


    public void onResume() {

    }

    public void onPause() {

    }

    public void onDestroy() {
    }


    /**
     * 取消加载的方法
     */
    public void cancleLoading() {
        mUiView.hiddenLoading();
    }

    /**
     * 重新加载页面的方法
     */
    public void showLoading() {
        mUiView.showLoading();
    }

//    abstract public void getScanCode();

}
