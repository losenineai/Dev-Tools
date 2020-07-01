package com.xxlib.inter;

import android.app.Activity;
import android.view.ViewGroup;
import android.view.WindowManager;

/**
 * Created by rosejames on 15/10/16.
 */
public interface IFloatViewBase {
    /**
     * 提供给子类重写，用于重新设置界面的宽高，不需要重新创建view
     *
     * @description
     */
    void updateLayout();

    /**
     * 初始化该view的布局
     *
     * @description
     */
    void initViewParams();

    void onViewCreate();

    void onViewDestroy();

    int getFloatViewTag();

    void setFloatViewTag(int tag);

    void onActivityResume(Activity activity);

    /**
     * 返回自身
     * @return
     */
    ViewGroup getFloatViewBase();


    /**
     * 返回windowmanger.layoutparams用来布局
     * @return
     */
    WindowManager.LayoutParams getWinLayoutParams();

    boolean isFloat();

    boolean isReset();

    boolean isCanFinish();

    /**
     * 暂时SDK会用到的接口，叉叉不会用到
     * @return
     */
    boolean isCache();

    void hideActionBar();

    void showActionBar();
}
