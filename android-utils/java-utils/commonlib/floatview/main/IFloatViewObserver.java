package com.auric.intell.commonlib.floatview.main;


/**
 * 悬浮窗观察者
 * @author white
 * @date 16/7/27
 */
public interface IFloatViewObserver {

    void refreshUI(int type, Object... objs);
}