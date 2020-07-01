package com.auric.intell.commonlib.common.view;

import android.content.Context;
import android.support.v4.widget.NestedScrollView;
import android.util.AttributeSet;

/**
 * Created by jingbin on 2016/12/6.
 * 系统的滚动监听只能api23以上用，这为了兼容
 */

public class CompatNestedScrollView extends NestedScrollView {

    private ScrollInterface scrollInterface;

    /**
     * 定义滑动接口
     */
    public interface ScrollInterface {

        void onScrollChange(int scrollX, int scrollY, int oldScrollX, int oldScrollY);
    }

    public CompatNestedScrollView(Context context) {
        super(context);
    }

    public CompatNestedScrollView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public CompatNestedScrollView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void onScrollChanged(int l, int t, int oldl, int oldt) {
        if (scrollInterface != null) {
            scrollInterface.onScrollChange(l, t, oldl, oldt);
        }
        super.onScrollChanged(l, t, oldl, oldt);
    }

    public void setOnMyScrollChangeListener(ScrollInterface t) {
        this.scrollInterface = t;
    }
}
