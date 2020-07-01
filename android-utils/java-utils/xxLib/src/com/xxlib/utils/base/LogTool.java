package com.xxlib.utils.base;

import android.annotation.SuppressLint;
import android.util.Log;

import com.xxlib.config.LibParams;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Log控制器，通过isLogOpen控制开关 分为两种模式： 1.普通的字符串 log 输出 2.后台返回的协议 byte[] 输出
 * 
 * @author linbin 2014
 */
public class LogTool {
    public static boolean isLogOpen = true;
	private static boolean isLogFileOpen = false;
	private static boolean isLogFileEncodeOpen = false;
	/**
	 * LogTool加密的key
	 */
	public static final String Key = "#%$*)&*M<><vance";
	// 写log到文件的线程池
	private static ExecutorService cachedThreadPool = null;

	public static void setLogOn(boolean isOn) {
		isLogOpen = isOn;
	}

	public static void setLogFileOn(boolean isOn) {
		isLogFileOpen = isOn;
	}

	public static void setLogFileEncodeOn(boolean isOn) {
		isLogFileEncodeOpen = isOn;
	}

	public static boolean getLogFileOn() {
		return isLogFileOpen;
	}


	public static void normalShow(String tag, String msg) {
		if (isLogOpen)
			Log.e(tag, ""+msg);
		if (isLogFileOpen) {
			writeLog(tag, msg);
		}
	}

	public static void normalShow(boolean isOpen, String tag, String msg) {
		if (isOpen)
			Log.e(tag, ""+msg);
		if (isLogFileOpen) {
			writeLog(tag, msg);
		}
	}

	public static void byteShow(byte[] bytes) {
        if (null == bytes) {
            return;
        }
		if (isLogOpen) {
			StringBuilder sb = new StringBuilder();
			// 返回的数据进行base64解密
			for (int i = 0; i < bytes.length; i++) {
				sb.append((char) (bytes[i] & 0xff) + "");
			}
			Log.i("byte", sb.toString());
		}
	}

	public static void i(String tag, String msg) {
		if (isLogOpen) {
			Log.i(tag, ""+msg);
		}
		if (isLogFileOpen) {
			writeLog(tag, msg);
		}
	}

	public static void e(String tag, String msg) {
		if (isLogOpen) {
			Log.e(tag, ""+msg);
		}
		if (isLogFileOpen) {
			writeLog(tag, msg);
		}
	}

	public static void w(String tag, String msg) {
		if (isLogOpen) {
			Log.w(tag, ""+msg);
		}
		if (isLogFileOpen) {
			writeLog(tag, msg);
		}
	}

	public static void w(String tag, Throwable t)
	{
		if (isLogOpen)
		{
			String msg = getStackTraceString(t);
			Log.w(tag, ""+msg);
		}
		if (isLogFileOpen)
		{
			String msg = getStackTraceString(t);
			writeLog(tag, msg);
		}
	}
	@SuppressLint("SimpleDateFormat")
	public static void writeLogToFile(String log) {
		PrintStream p = null;
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd-HH");
		String time = dateFormat.format(new Date(System.currentTimeMillis()));
		try {
			File des = new File(LibParams.NATIVE_LOG_PATH);
			if (!des.exists()) {
				des.mkdirs();
			}
			File file = new File(LibParams.NATIVE_LOG_PATH, time);

			p = new PrintStream(new BufferedOutputStream(new FileOutputStream(file, true)));
			p.write(log.getBytes());
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != p)
					p.close();
			} catch (Exception e2) {
			}
		}
	}

	@SuppressLint("SimpleDateFormat")
	public static String changeLogToString(String tag, String msg) {
		SimpleDateFormat dateFormat = new SimpleDateFormat("MM-dd HH:mm:ss:SSS");
		String time = dateFormat.format(new Date(System.currentTimeMillis()));
		String log = "[" + time + "]  [" + tag + "]  [" + msg + "]";
		if (isLogFileEncodeOpen) {
			byte[] bytes = XXTea.XXDoTeaEncrypt(log, Key);
			log = Base64.encode(bytes);
		}
		return log + "\r\n";
	}

	public static void writeLog(final String tag, final String msg) {
		if (cachedThreadPool == null) {
			cachedThreadPool = Executors.newFixedThreadPool(3);
		}
		cachedThreadPool.execute(new Runnable() {
			@Override
			public void run() {
				writeLogToFile(changeLogToString(tag, msg));
			}
		});
	}

	/**
	 * 打印堆栈信息
	 * 
	 * @description
	 * @param t
	 * @return
	 */
	public static String getStackTraceString(Throwable t) {
		if (t == null) {
			return "";
		}
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append(t.toString() + "\n");
		StackTraceElement[] traceEles = t.getStackTrace();
		if (null == traceEles) {
			return "";
		}
		for (StackTraceElement stackTraceEle : traceEles) {
			if (null == stackTraceEle) {
				continue;
			}
			sBuilder.append(stackTraceEle.toString() + "\n");
		}
		return sBuilder.toString();
	}

}
