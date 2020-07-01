package com.auric.intell.commonlib.robot;


import android.text.TextUtils;

import com.auric.intell.commonlib.utils.FileUtil;
import com.auric.intell.commonlib.utils.LogTool;

import org.json.JSONObject;

import java.io.File;

/**
 * @author white
 * @description：RBT内共享的key-value数据
 * @date 2016/10/9
 */
public class RBTSpCenter {

	private static final String TAG = "RBTSpCenter";

	private static final String JSON_FILE_PATH = "/private/rbt_json_sp.json";

    private static synchronized JSONObject getJsonObject(){
        File file = new File(JSON_FILE_PATH);
        if (!file.exists()) {
            return null;
        }
        JSONObject json = null;
        try {
            String content = FileUtil.readFile(file, FileUtil.CHARSET);
            json = new JSONObject(content);
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
            return null;
        }
        return json;
    }

	public static synchronized boolean getBoolean(String key, boolean def) {
        try {
			JSONObject json = getJsonObject();
			return json != null && json.has(key) ? json.getBoolean(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}

	public static synchronized long getLong(String key, long def) {
        try {
			JSONObject json = getJsonObject();
			return json != null && json.has(key) ? json.getLong(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}

	public static synchronized int getInt(String key, int def) {
        try {
			JSONObject json = getJsonObject();
			return json != null && json.has(key) ? json.getInt(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}

	public static synchronized String getString(String key, String def) {
        try {
			JSONObject json = getJsonObject();
			return json != null && json.has(key) ? json.getString(key) : def;
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return def;
	}

	public static synchronized void remove(String[] keys) {
        // 读取json sp 文件整个字符串到json结构体，然后进行修改，再把json的结构体写回去jsonSp文件
        try {
            File file = new File(JSON_FILE_PATH);
            if (!file.exists())
                return;

            // 读取原来json的字符串
            String content = FileUtil.readFile(file, FileUtil.CHARSET);
            if (TextUtils.isEmpty(content))
                return;

            // 修改json的值
            JSONObject json = new JSONObject(content);
            for (String key : keys)
                json.remove(key);

            // 保存json字符串到文件
            String contentMdf = json.toString();
            LogTool.d(TAG, "after revmoe " + contentMdf);
            FileUtil.saveContent(contentMdf, file, FileUtil.CHARSET);
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
        }
    }

    public static synchronized boolean putString(String key, String value) {
        return put(key, value);
    }

    public static synchronized boolean putInt(String key, int value) {
        return put(key, value);
    }

    public static synchronized boolean putLong(String key, long value) {
        return put(key, value);
    }

    public static synchronized boolean putBoolean(String key, boolean value) {
        return put(key, value);
    }

    public static synchronized boolean put(String key, Object value) {
        // 读取json sp 文件整个字符串到json结构体，然后进行修改，再把json的结构体写回去jsonSp文件
        try {
            File file = new File(JSON_FILE_PATH);
            if (!file.exists())
                file.createNewFile();

            // 读取原来json的字符串
            String content = FileUtil.readFile(file, FileUtil.CHARSET);
            // 修改json的值
            JSONObject json = (null==content||content.length()==0) ? new JSONObject() : new JSONObject(content);
            json.put(key, value);

            // 保存json字符串到文件
            String contentMdf = json.toString();
            FileUtil.saveContent(contentMdf, file, FileUtil.CHARSET);
            return true;
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
        }
        return false;
    }
}
