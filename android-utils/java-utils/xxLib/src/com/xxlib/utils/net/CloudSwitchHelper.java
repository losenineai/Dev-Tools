package com.xxlib.utils.net;

import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by mushroom0417 on 11/9/15.
 * 统一获取云开关的辅助类
 */
public class CloudSwitchHelper {

    private static final String TAG = "CloudSwitchHelper";
    private static JSONObject configJsonObject = null;
    public static String KEY_KINGUSER = "ku_install"; // 替换kinguser root的开关。1表示推广ku的root，0表示不推广
    public static String KEY_KINGROOT = "ku_root"; // 安装kinguser的开关。1表示推广kinguser，无root去安装ku
    public static String KEY_GDT = "gdt"; // 广电通的开关。1表示用广电通的广告、0表示不用广电通的广告。
    public static String KEY_ASSIST_TEXT = "assist_text"; // 插件类脚本的icon文案
    public static String KEY_AUTO_ACCELERATOR = "auto_accelerator"; // 本地添加的app是否自动添加加速器插件
    public static String KEY_SILENT_INSTALL_GPGAME = "silent_install_gpgame"; // 是否静默安装果盘游戏app

    public interface RequestStateListener {
        void onRequestSuccess();
        void onRequestFail();
    }

    /**
     * 请求开关配置
     */
    public static void requestSwitchConfig(String urlString, RequestStateListener requestStateListener) {
        String result = HttpGetUtil.get(urlString);
        if (TextUtils.isEmpty(result)) {
            if (null != requestStateListener) {
                requestStateListener.onRequestFail();
            }
        } else {
            try {
                configJsonObject = new JSONObject(result);
                if (requestStateListener != null) {
                    requestStateListener.onRequestSuccess();
                }
            } catch (JSONException e) {
                e.printStackTrace();
                if (null != requestStateListener) {
                    requestStateListener.onRequestFail();
                }
            }
        }
    }

    public static String getString(String key, String defaultValue) {
        String result = defaultValue;
        if (configJsonObject != null) {
            try {
                result = configJsonObject.getString(key);
                LogTool.i(TAG, "get string success");
            } catch (JSONException e) {
                LogTool.i(TAG, "get string fail");
                e.printStackTrace();
            }
        }
        return result;
    }

    public static boolean getBoolean(String key, boolean defaultValue) {
        boolean result = defaultValue;
        if (configJsonObject != null) {
            try {
                result = configJsonObject.getBoolean(key);
                LogTool.i(TAG, "get boolean success");
            } catch (JSONException e) {
                LogTool.i(TAG, "get boolean fail");
                e.printStackTrace();
            }
        }
        return result;
    }

    public static int getInt(String key, int defaultValue) {
        int result = defaultValue;
        if (configJsonObject != null) {
            try {
                result = configJsonObject.getInt(key);
                LogTool.i(TAG, "get int success");
            } catch (JSONException e) {
                LogTool.i(TAG, "get int fail");
                e.printStackTrace();
            }
        }
        return result;
    }

    public static double getDouble(String key, double defaultValue) {
        double result = defaultValue;
        if (configJsonObject != null) {
            try {
                result = configJsonObject.getDouble(key);
                LogTool.i(TAG, "get double success");
            } catch (JSONException e) {
                LogTool.i(TAG, "get double fail");
                e.printStackTrace();
            }
        }
        return result;
    }

}
