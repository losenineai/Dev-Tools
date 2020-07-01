package com.auric.intell.commonlib.utils;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

/**
 * 控制输入法工具
 * 
 * @author wxj 2014
 */
public class InputMethodUtil {

	/**
	 * 打开输入法
	 * 
	 * @param context
	 * @param anchor
	 *            父视图
	 */
	public static void openInputMethod(final Context context, final View anchor) {
		if (context == null) {
			return;
		}
		// 延迟500ms执行，为了防止调用view还没绘制成功
        Handler handler = new Handler(context.getMainLooper());
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                anchor.setFocusable(true);
                anchor.setFocusableInTouchMode(true);
                anchor.requestFocus();
                InputMethodManager inputMethodManager = (InputMethodManager) context
                        .getSystemService(Context.INPUT_METHOD_SERVICE);
                inputMethodManager.showSoftInput(anchor, InputMethodManager.SHOW_IMPLICIT);
            }
        }, 500);

	}

	/**
	 * 打开输入法,为悬浮窗定制
	 * 
	 * @param context
	 * @param anchor
	 *            父视图
	 */
	public static void openInputMethodForFloatView(Context context, final View anchor) {
		if (context == null) {
			return;
		}
		anchor.setFocusable(true);
		anchor.setFocusableInTouchMode(true);
		anchor.requestFocus();
		InputMethodManager inputMethodManager = (InputMethodManager) context
				.getSystemService(Context.INPUT_METHOD_SERVICE);
		inputMethodManager.showSoftInput(anchor, InputMethodManager.SHOW_FORCED);
	}

	/**
	 * 关闭输入法
	 * 
	 * @param acti
	 */
	public static boolean closeInputMethod(Activity acti) {
		if (acti == null) {
			return false;
		}
		InputMethodManager inputMethodManager = (InputMethodManager) acti
				.getSystemService(Context.INPUT_METHOD_SERVICE);
		// try catch 防止在没有可获取焦点的界面出现null的错误
		try {
			if (inputMethodManager.isActive()) {
				inputMethodManager.hideSoftInputFromWindow(acti.getCurrentFocus().getWindowToken(),
						InputMethodManager.HIDE_NOT_ALWAYS);
			}
			return true;
		} catch (Exception err) {
			// err.printStackTrace();
			return false;
		}
	}

	/**
	 * 关闭输入法
	 *
	 */
	public static boolean closeInputMethod(Context context, View anchor, int flag) {
		if (context == null || anchor == null) {
			return false;
		}
		anchor.setFocusable(true);
		anchor.setFocusableInTouchMode(true);
		anchor.requestFocus();
		InputMethodManager inputMethodManager = (InputMethodManager) context
				.getSystemService(Context.INPUT_METHOD_SERVICE);
		// try catch 防止在没有可获取焦点的界面出现null的错误
		return inputMethodManager.hideSoftInputFromWindow(anchor.getWindowToken(), flag);
	}

	public static boolean closeInputMethod(Context context, View anchor) {
		return closeInputMethod(context, anchor, 0);
	}

	/**
	 * Whether the input method is opened, true is opened, false is closed
	 * 
	 * @description
	 * @param context
	 * @return
	 */
	public static boolean isInputMethodOpen(Context context, View v) {
		InputMethodManager imm = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
		try {
			return imm.hideSoftInputFromWindow(v.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
		} catch (Exception err) {
			return false;
		}
	}

}
