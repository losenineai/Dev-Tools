package com.auric.intell.commonlib.framework.mvp;

import android.os.Handler;

import java.lang.ref.Reference;
import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;

/**
 * @author white
 * @description：MVPS presenterBase
 * @date 2016/12/2
 */

public abstract class MvpBasePresenter<S extends IMvpStatus, V extends IMvpView> implements IMvpPresenter<S, V>{

    protected Map<Integer, Reference<V>> mViewMap = new HashMap<>();
    protected Handler mHandler;

    protected MvpBasePresenter(){
        mHandler = new Handler();
    }

    @Override
    public void attachView(V view) {
        if (view == null) {
            return;
        }
        int viewHashCode = view.hashCode();
        if (mViewMap.containsKey(viewHashCode)) {
            return;
        }

        Reference<V> viewReference = new WeakReference<V>(view);
        mViewMap.put(view.hashCode(), viewReference);
    }

    @Override
    public void detachView(V view) {
        if (view == null) {
            return;
        }
        int hashCode = view.hashCode();
        if (mViewMap.containsKey(hashCode)) {
            mViewMap.get(hashCode).clear();
            mViewMap.remove(hashCode);
        }
    }

    /**
     * 在UI线程显示状态变化
     * @param status
     */
    protected void onShowStatusOnUiThread(final S status){
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                for (Reference<V> reference : mViewMap.values()) {
                    if (reference != null && reference.get() != null) {
                        reference.get().onShowStatus(status);
                    }
                }
            }
        });
    }


}
