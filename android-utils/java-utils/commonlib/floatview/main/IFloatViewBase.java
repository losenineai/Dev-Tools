package com.auric.intell.commonlib.floatview.main;

import android.view.ViewGroup;
import android.view.WindowManager;

/**
 * @author white
 * @description：悬浮窗View 的接口
 * @date 2017/1/3
 */

public interface IFloatViewBase {

    /**
     * 获取WindowManager.LayoutParams参数
     * @return
     */
    WindowManager.LayoutParams getWinLayoutParams();

    /**
     * 获取MainView
     * @return
     */
    ViewGroup getMainView();

    /**
     * 当 View 加入到WindowManager
     */
    void onViewCreate();

    /**
     * 当 View 从WindownManager移除
     */
    void onViewDestroy();

    /**
     * View的标识(@see FloatViewManager)
     */
    int getFloatViewTag();
    void setFloatViewTag(int tag);
}
