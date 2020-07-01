package com.auric.intell.commonlib.utils;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

/**
 * @author white
 * @description：
 * @date 2016/11/24
 */

public class AppUtil {

    /**
     * 获取当前进程名字
     * @param context
     * @return
     */
    public static String getCurProcessName(Context context) {
        int pid = android.os.Process.myPid();
        ActivityManager mActivityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningAppProcessInfo appProcess : mActivityManager.getRunningAppProcesses()) {
            if (appProcess.pid == pid) {
                return appProcess.processName;
            }
        }
        return null;
    }


    /**
     * 获取当前 APP versionName
     * @param ctx
     * @return
     */
    public static String getAPPVersionNameFromAPP(Context ctx) {
        String appVersionName = "";
        PackageManager manager = ctx.getPackageManager();
        try {
            PackageInfo info = manager.getPackageInfo(ctx.getPackageName(), 0);
            appVersionName = info.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return appVersionName;
    }


    /**
     * 获取当前 APP versionName
     * @param ctx
     * @return
     */
    public static int getAPPVersionCode(Context ctx) {
        int appVersionCode=0 ;
        PackageManager manager = ctx.getPackageManager();
        try {
            PackageInfo info = manager.getPackageInfo(ctx.getPackageName(), 0);
            appVersionCode = info.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return appVersionCode;
    }

    /**
     * 获取当前包名
     * @return
     */
    public static String getPkgName() {
        return ContextFinder.getApplication().getPackageName();
    }
}
