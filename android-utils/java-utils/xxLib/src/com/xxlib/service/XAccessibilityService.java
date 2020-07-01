package com.xxlib.service;

import android.accessibilityservice.AccessibilityService;
import android.text.TextUtils;
import android.view.accessibility.AccessibilityEvent;

import com.xxlib.utils.RegisterModule.FileUtil;
import com.xxlib.utils.base.LogTool;

import org.json.JSONObject;

import java.io.File;

/**
 * Created by jammy on 15/11/5.
 */
public class XAccessibilityService extends AccessibilityService {

    private static final String TAG = "XAccessibilityService";

    public static final String JSON_PATH = "/data/data/%s/accessibilityServiceInfo.json";
    public static final String CUR_PKG_NAME = "cur_pkg_name";
    public static final String CUR_CLS_NAME = "cur_cls_name";
    public static final String CUR_ACTIVITY_PKG_NAME = "cur_activity_pkg_name";
    public static final String IS_CONNECT = "is_connect";

    private String mCurPackageName;
    private String mCurClassName;
    private String mCurActivityPackageName;
    private boolean mIsServiceConnected = false;

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        LogTool.i(TAG, "onAccessibilityEvent type " + event.getEventType());
        if (event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED) {
            mCurPackageName = (null != event.getPackageName()) ? event.getPackageName().toString() : "";
            mCurClassName = (null != event.getClassName()) ? event.getClassName().toString() : "";
            LogTool.i(TAG, "TYPE_WINDOW_STATE_CHANGED pkgName " + mCurPackageName + " className " + mCurClassName);

            if (null!= mCurClassName && !mCurClassName.equals("android.widget.FrameLayout") &&
                    !mCurClassName.equals("android.widget.RelativeLayout") && !mCurClassName.equals("android.view.View")) {
                // 表示这不是一个悬浮窗，也不是一个pop window的
                mCurActivityPackageName = mCurPackageName;
            }
            writeJsonToFile();
        }
    }

    private void writeJsonToFile() {
        try {
            String jsonPath = String.format(JSON_PATH, getPackageName());
            File file = new File(jsonPath);
            if (!file.exists())
                file.createNewFile();

            // json的值
            JSONObject json = new JSONObject();
            if (!TextUtils.isEmpty(mCurPackageName)) {
                json.put(CUR_PKG_NAME, mCurPackageName);
            }
            if (!TextUtils.isEmpty(mCurClassName)) {
                json.put(CUR_CLS_NAME, mCurClassName);
            }
            if (!TextUtils.isEmpty(mCurActivityPackageName)) {
                json.put(CUR_ACTIVITY_PKG_NAME, mCurActivityPackageName);
            }
            json.put(IS_CONNECT, mIsServiceConnected);

            // 保存json字符串到文件
            String content = json.toString();
            FileUtil.saveContent(content, file, FileUtil.CHARSET);
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
        }
    }

    @Override
    public void onInterrupt() {
        LogTool.i(TAG, "onInterrupt, pid " + android.os.Process.myPid());
    }


    @Override
    public void onServiceConnected() {
        LogTool.i(TAG, "onServiceConnected, pid: " + android.os.Process.myPid());
        mIsServiceConnected = true;
    }
}
