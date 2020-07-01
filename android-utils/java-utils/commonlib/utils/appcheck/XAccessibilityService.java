package com.auric.intell.commonlib.utils.appcheck;

import android.accessibilityservice.AccessibilityService;
import android.content.ComponentName;
import android.content.pm.ActivityInfo;
import android.text.TextUtils;
import android.view.accessibility.AccessibilityEvent;

import org.json.JSONObject;

import java.io.File;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.FileUtil;
import com.auric.intell.commonlib.utils.LogTool;

/**
 * 此服务由系统自动启动,不需要手动启动
 * onServiceConnected方法回调就是服务被启动了
 * Created by white on 15/11/5.
 */
public class XAccessibilityService extends AccessibilityService {

    private static final String TAG = "XAccessibilityService";

//    public static final String JSON_PATH = "/data/data/%s/accessibilityServiceInfo.json";
    public static final String JSON_PATH = "/private/accessibilityServiceInfo.json";
    public static final String CUR_PKG_NAME = "cur_pkg_name";
    public static final String CUR_CLS_NAME = "cur_cls_name";
    public static final String CUR_ACTIVITY_PKG_NAME = "cur_activity_pkg_name";
    public static final String CUR_ACTIVITY_CLS_NAME = "cur_activity_cls_name";
    public static final String IS_CONNECT = "is_connect";

    private String mCurPkgName;
    private String mCurClassName;
    private String mCurActivityClassName;
    private String mCurActivityPkgName;
    private boolean mIsServiceConnected = false;

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        LogTool.d(TAG, "onAccessibilityEvent type " + event.getEventType());
        if (event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED) {
            mCurPkgName = (null != event.getPackageName()) ? event.getPackageName().toString() : "";
            mCurClassName = (null != event.getClassName()) ? event.getClassName().toString() : "";
            LogTool.d(TAG, "TYPE_WINDOW_STATE_CHANGED pkgName " + mCurPkgName + " className " + mCurClassName);

            // activity界面的变化
            if (isActivity(new ComponentName(mCurPkgName, mCurClassName))) {
                mCurActivityClassName = mCurClassName;
                mCurActivityPkgName = mCurPkgName;
                LogTool.d(TAG, mCurActivityClassName + " is activity");
            }

//            if (null!= mCurClassName && !mCurClassName.equals("android.widget.FrameLayout") &&
//                    !mCurClassName.equals("android.widget.RelativeLayout") && !mCurClassName.equals("android.view.View")) {
//                // 表示这不是一个悬浮窗，也不是一个pop window的
//                mCurActivityPkgName = mCurPkgName;
//            }
            writeJsonToFile();
        }
    }

    private boolean isActivity(ComponentName cName) {
        if (cName == null) {
            return false;
        }
        try {
            ActivityInfo actiInfo = getPackageManager().getActivityInfo(cName, 0);
            return actiInfo != null;
        } catch (Exception e) {
        }
        return false;
    }

    private synchronized void writeJsonToFile() {
        String jsonPath = String.format(JSON_PATH, getPackageName());
        File file = new File(jsonPath);
        try {
            if (!file.exists()){
                file.createNewFile();
            }

            // json的值
            JSONObject json = new JSONObject();
            if (!TextUtils.isEmpty(mCurPkgName)) {
                json.put(CUR_PKG_NAME, mCurPkgName);
            }
            if (!TextUtils.isEmpty(mCurClassName)) {
                json.put(CUR_CLS_NAME, mCurClassName);
            }
            if (!TextUtils.isEmpty(mCurActivityPkgName)) {
                json.put(CUR_ACTIVITY_PKG_NAME, mCurActivityPkgName);
            }
            if (!TextUtils.isEmpty(mCurActivityClassName)) {
                json.put(CUR_ACTIVITY_CLS_NAME, mCurActivityClassName);
            }
            json.put(IS_CONNECT, mIsServiceConnected);

            // 保存json字符串到文件
            String content = json.toString();
            FileUtil.saveContent(content, file, FileUtil.CHARSET);
        } catch (Exception e) {
//            file.deleteOnExit();
            e.printStackTrace();
        }
    }

    @Override
    public void onInterrupt() {
        LogTool.d(TAG, "onInterrupt, pid " + android.os.Process.myPid());
    }


    @Override
    public void onServiceConnected() {
        LogTool.d(TAG, "onServiceConnected, pid: " + android.os.Process.myPid());
        mIsServiceConnected = true;
        // 读取当前缓存, 防止服务重启, top界面会重置的情况
        mCurActivityClassName = AccessibilityOperator.getTopActiClassName(ContextFinder.getApplication());
        mCurActivityPkgName = AccessibilityOperator.getTopActiPkgName(ContextFinder.getApplication());
    }
}
