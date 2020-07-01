package com.auric.intell.commonlib.floatview.base;

import android.content.Context;
import android.graphics.PixelFormat;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;

import com.auric.intell.commonlib.utils.LogTool;

/**
 * @author white
 * @description：
 * @date 2017/1/3
 */

public abstract class FloatViewMidBase extends FloatViewBase{

    private static final String TAG = "FloatViewMidBase";

    public FloatViewMidBase(Context context) {
        super(context);
    }

    public FloatViewMidBase(Context context, Object obj) {
        super(context, obj);
    }

    @Override
    protected void initViewParams() {
        super.initViewParams();
        this.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                LogTool.d(TAG, "onClickOutside");
                onClickOutside();
            }
        });
    }

    /**
     * 可以拦截点击事件的params
     * @return
     */
    @Override
    public WindowManager.LayoutParams getWinLayoutParams() {
        WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
        lp.width = WindowManager.LayoutParams.MATCH_PARENT;
        lp.height = WindowManager.LayoutParams.MATCH_PARENT;
        lp.type = WindowManager.LayoutParams.TYPE_PHONE;
        lp.format = PixelFormat.RGBA_8888;
        lp.gravity = Gravity.CENTER;
        return lp;
    }

    /**
     * 拦截返回按键
     * @param event
     * @return
     */
    public boolean dispatchKeyEvent(KeyEvent event) {
        LogTool.d(TAG, "dispatchKeyEvent");
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {
            onClickBackKey();
            return true;
        }
        return super.dispatchKeyEvent(event);
    }

    /**
     * 点击空白处
     */
    abstract protected void onClickOutside();

    /**
     * 点击返回键
     */
    abstract protected void onClickBackKey();

}
