package com.xxlib.view.list;

import android.view.View;
import android.view.ViewGroup;

/**
 * 悬浮窗使用fragment必须继承的接口
 * Created by jammy on 15/9/9.
 */
public interface IFloatFragment extends FloatViewObserver{

    /**
     * 悬浮窗的fragment严格意义上，不是fragment
     * 需要自己控制view的创建和获取
     *
     * @param container
     * @return
     */
    View getView(ViewGroup container);

}
