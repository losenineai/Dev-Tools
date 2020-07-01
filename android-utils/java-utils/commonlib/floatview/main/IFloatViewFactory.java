package com.auric.intell.commonlib.floatview.main;

/**
 * 悬浮窗工厂接口, 生产悬浮窗
 * @author white
 * @date 16/7/27
 */
public interface IFloatViewFactory {

    /**
     * 创建悬浮窗
     * @param viewTag
     * @param params
     * @return
     */
    IFloatViewBase createFloatView(int viewTag, Object params);
}
