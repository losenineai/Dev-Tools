package com.xxlib.utils;

import android.content.Context;
import android.text.ClipboardManager;
import android.text.TextUtils;

import com.xxlib.utils.floatview.CheckAppRunning;

/**
 * 复制到剪切板工具类
 * @Description 
 *
 * 版本号    作者    日期    相关修改
 *  1.0   wxj   2014-10-20  创建
 *
 * @author wxj
 * 2014-10-20   --  下午3:03:21
 */
public class CopyToClipboard {

	/**
	 * 复制文本
	 * @description  
	 * @param context
	 * @param text
	 */
	public static void copy(Context context, String text){
		
		ClipboardManager cm = (ClipboardManager) context
                .getSystemService(Context.CLIPBOARD_SERVICE);
		cm.setText(text);

        ToastUtils.show(context, "已成功复制到黏贴板", !CheckAppRunning.isXxRunningOnForeground(context, context.getPackageName()));
	}

	public static void copyAndShowToastInApp(Context context, String text, int duration){

		ClipboardManager cm = (ClipboardManager) context
				.getSystemService(Context.CLIPBOARD_SERVICE);
		cm.setText(text);

		ToastUtils.show(context, "已成功复制到黏贴板", duration);
	}

    public static void copy(Context context, String text, String toastHint){

        ClipboardManager cm = (ClipboardManager) context
                .getSystemService(Context.CLIPBOARD_SERVICE);
        cm.setText(text);

        if (!TextUtils.isEmpty(toastHint)) {
            ToastUtils.show(context, toastHint, !CheckAppRunning.isXxRunningOnForeground(context, context.getPackageName()));
        }
    }

	public static void copy(Context context, String text, String toastHint, boolean isFloatView) {
		ClipboardManager cm = (ClipboardManager) context
				.getSystemService(Context.CLIPBOARD_SERVICE);
		cm.setText(text);

		if (!TextUtils.isEmpty(toastHint)) {
			ToastUtils.show(context, toastHint, isFloatView);
		}
	}
}
