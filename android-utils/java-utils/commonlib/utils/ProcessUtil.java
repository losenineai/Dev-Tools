package com.auric.intell.commonlib.utils;

import android.app.ActivityManager;
import android.content.Context;
import android.text.TextUtils;

import java.util.List;


/**
 * 进程管理工具
 * @author white
 * @date 16/7/27
 */
public class ProcessUtil {

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


    public static boolean killSelfProcess(Context context, String processName) {
        boolean isKilled = false;
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> appProcessInfos = activityManager.getRunningAppProcesses();
        for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : appProcessInfos) {
            if (runningAppProcessInfo.processName.equals(processName)) {
                isKilled = true;
                LogTool.d("MulProcess", "kill : " + runningAppProcessInfo.pid);
                android.os.Process.killProcess(runningAppProcessInfo.pid);
            }
        }
        return isKilled;
    }

    public static String getCurProcessName(Context context){
        int pid = android.os.Process.myPid();
        ActivityManager mActivityManager = (ActivityManager) context
                .getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningAppProcessInfo appProcess : mActivityManager
                .getRunningAppProcesses()) {
            if (appProcess.pid == pid) {
                return appProcess.processName;
            }
        }
        return null;
    }

    public static boolean inMainProcess(Context context) {
        String packageName = context.getPackageName();
        String processName = getCurProcessName(context);
        return packageName.equals(processName);
    }



}
