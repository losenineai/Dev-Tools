package com.xxlib.utils;

import android.app.ActivityManager;
import android.content.Context;
import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;

import java.util.List;

/**
 * Created by jammy on 15/11/19.
 */
public class AppTool {

    private static final String TAG = "AppTool_xxLib";

    public static boolean isFlaotViewProcessRunning(Context context) {
        return isPorcessRunning(context, context.getPackageName()+":floatview");
    }

    public static boolean isUploadProcessRunning(Context context) {
        return isPorcessRunning(context, context.getPackageName()+":upload");
    }

    public static boolean isScriptProcessRunning(Context context) {
        return isPorcessRunning(context, context.getPackageName()+":script");
    }

    public static boolean isPorcessRunning(Context context, String processName) {
        if (TextUtils.isEmpty(processName)) {
            return false;
        }
        boolean isRunning = false;
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> appProcessInfos = activityManager.getRunningAppProcesses();
        for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : appProcessInfos) {
            if (runningAppProcessInfo.processName.equals(processName)) {
                isRunning = true;
                break;
            }
        }
        return isRunning;
    }


    public static boolean killFloatViewProcess(Context context) {
        return killSelfProcess(context, context.getPackageName() + ":floatview");
    }

    public static boolean killScriptProcess(Context context) {
        return killSelfProcess(context, context.getPackageName() + ":script");
    }

    public static boolean killUploadProcess(Context context) {
        return killSelfProcess(context, context.getPackageName() + ":upload");
    }

    public static boolean killXXProcess(Context context) {
        return killSelfProcess(context, context.getPackageName());
    }

    public static boolean killSelfProcess(Context context, String processName) {
        boolean isKilled = false;
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> appProcessInfos = activityManager.getRunningAppProcesses();
        for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : appProcessInfos) {
            if (runningAppProcessInfo.processName.equals(processName)) {
                CheckExceptionUtil.getInstance().noneException();
                isKilled = true;
                LogTool.i("MulProcess", "kill : " + runningAppProcessInfo.pid);
                android.os.Process.killProcess(runningAppProcessInfo.pid);
            }
        }
        return isKilled;
    }
}
