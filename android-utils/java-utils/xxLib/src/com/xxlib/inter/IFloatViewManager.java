package com.xxlib.inter;

import android.view.View;

/**
 * 悬浮窗管理器
 * Created by rosejames on 15/10/15.
 */
public interface IFloatViewManager {

    void addViewAbs(int viewTag, Object obj);

    void backToLastViewAbs();

    void backToViewByTagAbs(int viewTag);

    void createCommonDialogAbs(String title, String content, String negaString, String posString,
                               View.OnClickListener negListener, View.OnClickListener posListener, boolean singleBtn);

    void createWarningLoginViewAbs();

    boolean isSwitchOrientation();
    
    IFloatViewBase getBackLastView(int viewTag);
    
}
