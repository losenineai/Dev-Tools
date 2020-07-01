package com.xxlib.config;

import android.content.Context;
import android.text.TextUtils;

import com.xxlib.utils.FileUtils;
import com.xxlib.utils.base.LogTool;

import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;

/**
 * 多进程设置sp会导致sp被重置，搞一个json写文件来用于多进程。尽量多用sp，实在有多进程的标记位才用这个。
 * <p> 修改的时候，要考虑多进程的处理细节
 */
public class JsonSpCenter {
	
	private static final String TAG = "JsonSpCenter";
	private static final String JSON_FILE_NAME = "json_sp";
	private Context context;
	
	
	public JsonSpCenter(Context context) {
		this.context = context;
	}
	
	
	public synchronized void putBoolean(String key, boolean b) {
		if (null == context) {
			LogTool.i(TAG, "context == null");
			return;
		}
		
		// 读取json sp 文件整个字符串到json结构体，然后进行修改，再把json的结构体写回去jsonSp文件
		try {
			File file = new File(context.getFilesDir()+File.separator+JSON_FILE_NAME);
			if (!file.exists())
				file.createNewFile();
			
			// 读取原来json的字符串
			FileInputStream inputStream = context.openFileInput(JSON_FILE_NAME);
			String content = FileUtils.readFile(inputStream, FileUtils.CHARSET);

			// 修改json的值
			JSONObject json = (null==content||content.length()==0) ? new JSONObject() : new JSONObject(content);
			json.put(key, b);
			
			// 保存json字符串到文件
			String contentMdf = json.toString();
			FileUtils.saveContent(contentMdf, context.openFileOutput(JSON_FILE_NAME, Context.MODE_PRIVATE), FileUtils.CHARSET);
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
	}
	
	
	public synchronized boolean getBoolean(String key, boolean def) {
		if (null == context) {
			LogTool.i(TAG, "context == null");
			return def;
		}
		
		try {
			FileInputStream inputStream = context.openFileInput(JSON_FILE_NAME);
			String content = FileUtils.readFile(inputStream, FileUtils.CHARSET);
			
			JSONObject json = new JSONObject(content);
			return json.has(key) ? json.getBoolean(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}
	
	
	public synchronized String getString(String key, String def) {
		if (null == context) {
			LogTool.i(TAG, "context == null");
			return def;
		}
		
		try {
			FileInputStream inputStream = context.openFileInput(JSON_FILE_NAME);
			String content = FileUtils.readFile(inputStream, FileUtils.CHARSET);
			
			JSONObject json = new JSONObject(content);
			return json.has(key) ? json.getString(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}
	
	
	public synchronized void remove(String[] keys) {
		// 读取json sp 文件整个字符串到json结构体，然后进行修改，再把json的结构体写回去jsonSp文件
		try {
			File file = new File(context.getFilesDir()+File.separator+JSON_FILE_NAME);
			if (!file.exists())
				return;
				
			// 读取原来json的字符串
			FileInputStream inputStream = context.openFileInput(JSON_FILE_NAME);
			String content = FileUtils.readFile(inputStream, FileUtils.CHARSET);
			if (TextUtils.isEmpty(content))
				return;
			
			// 修改json的值
			JSONObject json = new JSONObject(content);
			for (String key : keys)
				json.remove(key);
			
			// 保存json字符串到文件
			String contentMdf = json.toString();
			LogTool.i(TAG, "after revmoe " + contentMdf);
			FileUtils.saveContent(contentMdf, context.openFileOutput(JSON_FILE_NAME, Context.MODE_PRIVATE), FileUtils.CHARSET);
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
	}
	
	
	public synchronized void putString(String key, String str) {
		if (null == context) {
			LogTool.i(TAG, "context == null");
			return;
		}
		
		// 读取json sp 文件整个字符串到json结构体，然后进行修改，再把json的结构体写回去jsonSp文件
		try {
			File file = new File(context.getFilesDir()+File.separator+JSON_FILE_NAME);
			if (!file.exists())
				file.createNewFile();
			
			// 读取原来json的字符串
			FileInputStream inputStream = context.openFileInput(JSON_FILE_NAME);
			String content = FileUtils.readFile(inputStream, FileUtils.CHARSET);

			// 修改json的值
			JSONObject json = (null==content||content.length()==0) ? new JSONObject() : new JSONObject(content);
			json.put(key, str);
			
			// 保存json字符串到文件
			String contentMdf = json.toString();
			FileUtils.saveContent(contentMdf, context.openFileOutput(JSON_FILE_NAME, Context.MODE_PRIVATE), FileUtils.CHARSET);
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
	}
	
	
	
	/***************************** 以下是json sp用到的一些key定义 ********************/
	// 悬浮窗加速器首次提示标记
	public final static String FLOAT_SPEED_TIPS_HAS_SHOW = "FLOAT_SPEED_TIPS_HAS_SHOW_";
	public final static String HAS_UPDATE_USER_FROM_OLD_VERSION = "UP_FROM_OLD";			// 2.1.0加入
	public final static String HAS_UPDATE_USER_FROM_OLD_VERSION_211 = "UP_FROM_OLD_211";	// 2.1.2加入
	public final static String LOGIN_KEY = "LOGIN_KEY";
	public final static String UIN = "UIN";
}
