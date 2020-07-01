package com.xxlib.utils;

import android.content.Context;
import android.os.Handler;
import android.widget.Toast;

import com.xxlib.config.LibParams;
import com.xxlib.utils.base.LogTool;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class CheckExceptionUtil {

	/**
	 * false 为 开启检测，，true为关闭检测，，记得发布前设置为true
	 */
	public static boolean checkClose = true;

	public static final int FLOAT_CLOSE_EXCEPTION = -1;
	public static final int FLOAT_CLOSE_NORMAL = 1;

	private Context mContext;
	private static CheckExceptionUtil mCheckExceptionUtil;

	private CheckExceptionUtil() {

	}

	public void init(Context context) {
		mContext = context;
	}

	public void initException() {
		if (checkClose) {
			return;
		}
		try {
			FileWriter file = new FileWriter(LibParams.EXCEPTION_PATH);
			BufferedWriter b = new BufferedWriter(file);
			b.write(FLOAT_CLOSE_EXCEPTION + "");
			b.close();
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void checkException() {
		if (checkClose) {
			return;
		}
		try {
			FileReader file = new FileReader(LibParams.EXCEPTION_PATH);
			BufferedReader b = new BufferedReader(file);
			int exception = Integer.valueOf(b.readLine());
			LogTool.i("daemon", "exception:" + exception);
			if (exception == FLOAT_CLOSE_EXCEPTION) {
				LogTool.i("daemon", "上次悬浮窗异常关闭");
				showToast("上次悬浮窗异常关闭");
			} else if (exception == FLOAT_CLOSE_NORMAL) {
				LogTool.i("daemon", "上次悬浮窗正常关闭");
				showToast("上次悬浮窗正常关闭");
			}
			b.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void noneException() {
		if (checkClose) {
			return;
		}
		try {
			FileWriter file = new FileWriter(LibParams.EXCEPTION_PATH);
			BufferedWriter b = new BufferedWriter(file);
			b.write(FLOAT_CLOSE_NORMAL + "");
			b.close();
			file.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	public static CheckExceptionUtil getInstance() {
		if (mCheckExceptionUtil == null) {
			mCheckExceptionUtil = new CheckExceptionUtil();
		}
		return mCheckExceptionUtil;
	}

	private void showToast(final String text) {
		if (checkClose) {
			return;
		}
		Handler handler = new Handler(mContext.getMainLooper());
		handler.post(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(mContext, text, Toast.LENGTH_LONG).show();
			}
		});
	}

}
