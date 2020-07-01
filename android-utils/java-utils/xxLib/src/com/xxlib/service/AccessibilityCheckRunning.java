package com.xxlib.service;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.text.TextUtils;

import com.xxlib.reciever.HomeWatcherReciever;
import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.FileUtils;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.floatview.CheckAppRunning;
import com.xxlib.utils.floatview.LollipopUtil;

import org.json.JSONObject;

import java.io.FileInputStream;

/**
 * Created by jammy on 15/10/27.
 */
public class AccessibilityCheckRunning {

    private static final String TAG = "AccessibilityCheckRunning";

    public static String mCurPackageName;
    public static String mCurClassName;
    public static String mLastPackageName;
    public static String mLastClassName;
    public static String mCurActivityPackageName;
    public static String mCurActivityClassName;
    public static boolean mIsServiceConnected = false;

    public static boolean mIsHasAccessibilityPermission = false;

    public static boolean mIsJumpOut = false;

    public static void checkIsHasAccessibilityPermission(Context context) {
        mIsHasAccessibilityPermission = AccessibilityCheckRunning.isAccessibilitySettingsOn(context);
    }


    public static void readJsonFromFile(Context context) {
        if (!mIsHasAccessibilityPermission) {
            return;
        }
        try {
            String jsonPath = String.format(XAccessibilityService.JSON_PATH, context.getPackageName());
            String content = FileUtils.readFile(new FileInputStream(jsonPath), FileUtils.CHARSET);

            JSONObject json = new JSONObject(content);
            if (json.has(XAccessibilityService.CUR_PKG_NAME)) {
                mCurPackageName = json.getString(XAccessibilityService.CUR_PKG_NAME);
            }
            if (json.has(XAccessibilityService.CUR_CLS_NAME)) {
                mCurClassName = json.getString(XAccessibilityService.CUR_CLS_NAME);
            }
            if (json.has(XAccessibilityService.CUR_ACTIVITY_PKG_NAME)) {
                mCurActivityPackageName = json.getString(XAccessibilityService.CUR_ACTIVITY_PKG_NAME);
            }
            if (json.has(XAccessibilityService.IS_CONNECT)) {
                mIsServiceConnected = json.getBoolean(XAccessibilityService.IS_CONNECT);
            }
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
        }
    }


    public static boolean isOnFloatView(String gamePackageName) {
        if (CheckAppRunning.isDebugRunning) {
            LogTool.i(TAG, "isOnFloatView, mCurPackageName " + mCurPackageName + ",mCurClassName " + mCurClassName + ", mCurActivityPackageName " + mCurActivityPackageName);
        }
        if (TextUtils.isEmpty(mCurPackageName) || TextUtils.isEmpty(mCurClassName) || TextUtils.isEmpty(gamePackageName) || !mIsServiceConnected) {
            return false;
        }

        // 1、游戏的包名
        if (mCurPackageName.equals(gamePackageName)) {
            return true;
        }

        // 2、xmod和xx的包名,进入面板页面是FrameLayout
        String selfPackageName = ContextFinder.getApplication().getPackageName();
        if (!mIsJumpOut && !HomeWatcherReciever.mIsHomeOut && mCurPackageName.equals(selfPackageName)
                && mCurClassName.equals("android.widget.FrameLayout")) {
            return true;
        }

        // 3、xmod和xx的包名,进入引导页，是RelativeLayout
        // && mCurPackageName.equals(selfPackageName) && mCurClassName.equals("android.widget.RelativeLayout")
        if (!mIsJumpOut && !HomeWatcherReciever.mIsHomeOut && !TextUtils.isEmpty(mCurActivityPackageName)
                && mCurActivityPackageName.equals(gamePackageName)) {
            return true;
        }

        return false;
    }

    /**
     * 是否在launcher界面
     * @return
     */
    public static boolean isOnLauncher(Context context) {
        return LollipopUtil.isOnLauncher(context, mCurActivityPackageName);
    }

    // 该接口还没测试
    public static void setAccessibilityPermission(Activity activity) {
        Intent intent = new Intent(android.provider.Settings.ACTION_ACCESSIBILITY_SETTINGS);
        // TODO 是否有for result的东西
        activity.startActivityForResult(intent, 0); //
    }

    public static boolean gotoAccessibilitySettings(Context context) {
        Intent settingsIntent = new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS);
        if (!(context instanceof Activity)) {
            settingsIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        }
        boolean isOk = true;
        try {
            context.startActivity(settingsIntent);
        } catch (Exception e) {
            isOk = false;
        }
        return isOk;
    }

    // To check if service is enabled
    public static boolean isAccessibilitySettingsOn(Context mContext) {
        int accessibilityEnabled = 0;
        String pkg = mContext.getPackageName();
        final String service = pkg + "/" + XAccessibilityService.class.getName();
        boolean accessibilityFound = false;
        try {
            accessibilityEnabled = Settings.Secure.getInt(
                    mContext.getApplicationContext().getContentResolver(),
                    android.provider.Settings.Secure.ACCESSIBILITY_ENABLED);
            LogTool.i(TAG, "accessibilityEnabled = " + accessibilityEnabled);
        } catch (Exception e) {
            LogTool.e(TAG, "Error finding setting, default accessibility to not found: " + e.getMessage());
        }
        TextUtils.SimpleStringSplitter mStringColonSplitter = new TextUtils.SimpleStringSplitter(':');

        if (accessibilityEnabled == 1) {
            LogTool.i(TAG, "***ACCESSIBILIY IS ENABLED*** -----------------");
            String settingValue = Settings.Secure.getString(
                    mContext.getApplicationContext().getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
            if (settingValue != null) {
                TextUtils.SimpleStringSplitter splitter = mStringColonSplitter;
                splitter.setString(settingValue);
                while (splitter.hasNext()) {
                    String accessabilityService = splitter.next();

                    LogTool.i(TAG, "-------------- > accessabilityService :: " + accessabilityService);
                    if (accessabilityService.equalsIgnoreCase(service)) {
                        LogTool.i(TAG, "We've found the correct setting - accessibility is switched on!");
                        return true;
                    }
                }
            }
        } else {
            LogTool.i(TAG, "***ACCESSIBILIY IS DISABLED***");
        }

        return accessibilityFound;
    }
}
