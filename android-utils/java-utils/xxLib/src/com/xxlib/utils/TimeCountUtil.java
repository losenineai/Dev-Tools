package com.xxlib.utils;

import com.xxlib.utils.base.LogTool;

public class TimeCountUtil {
	private static long beginTime;

	public static void begin() {
		beginTime = System.currentTimeMillis();
	}

	public static void end() {
		LogTool.i("TimeCountUtil", "Cast : " + (System.currentTimeMillis() - beginTime) + " millis");
	}

	public static void end(String task) {
		LogTool.i("TimeCountUtil", task + " Cast : " + (System.currentTimeMillis() - beginTime) + " millis");
	}
}
