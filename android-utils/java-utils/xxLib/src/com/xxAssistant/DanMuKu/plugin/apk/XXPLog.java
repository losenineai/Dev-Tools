package com.xxAssistant.DanMuKu.plugin.apk;

import android.util.Log;

import com.xxlib.utils.base.LogTool;

public class XXPLog {
	
	public static final boolean isLogOpen = LogTool.isLogOpen;
	
	public static void i(String tag, String msg) {
		if (isLogOpen) {
			Log.i(tag, msg);
		}
	}
	
	public static void e(String tag, String msg) {
		if (isLogOpen) {
			Log.e(tag, msg);
		}
	}
	
	public static void w(String tag, String msg) {
		if (isLogOpen) {
			Log.w(tag, msg);
		}
	}
	
	public static void d(String tag, String msg) {
		if (isLogOpen) {
			Log.d(tag, msg);
		}
	}
}
