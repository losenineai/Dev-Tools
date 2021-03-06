package com.xxlib.utils.floatview;

import android.annotation.TargetApi;
import android.app.AppOpsManager;
import android.app.usage.UsageStats;
import android.app.usage.UsageStatsManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.provider.Settings;
import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Created by xufu on 15-9-15.
 */

@TargetApi(Build.VERSION_CODES.LOLLIPOP)
public class LollipopUtil {

    private static final String TAG = "LollipopUtil";
    private static boolean isDebug = CheckAppRunning.isDebugRunning;

    public static final int STAT_RESULT_RUNNING_TOP = 1;
    public static final int STAT_RESULT_RUNNING_BACKGROUND = 2;
    public static final int STAT_RESULT_GOT_EMPTY_LIST_WITH_PERMISSION = 3;
    public static final int STAT_RESULT_GOT_EMPTY_LIST_WITHOUT_PERMISSION = 4;


    public static boolean hasStatAccessPermission(Context context) {
        try {
            PackageManager pm = context.getPackageManager();
            ApplicationInfo info = pm.getApplicationInfo(context.getPackageName(), 0);
            AppOpsManager appOpsManager = (AppOpsManager) context.getSystemService(Context.APP_OPS_SERVICE);
            return appOpsManager.checkOpNoThrow(AppOpsManager.OPSTR_GET_USAGE_STATS, info.uid, info
                    .packageName) == AppOpsManager.MODE_ALLOWED;
        } catch (Exception e) {
        }

        return false;
    }

    public static void setStatAccessPermission(Context context) {
        try {
            context.startActivity(new Intent(Settings.ACTION_USAGE_ACCESS_SETTINGS));
        } catch (Exception e) {
            LogTool.i(TAG, e.toString());
        }
    }

    public static int isAppRunningOnTop(Context context, String packageName) {
        if (isDebug)
            LogTool.i(TAG, "PackageName->" + packageName);
        String topAppPackageName = getTopActivity(context);
        if (TextUtils.isEmpty(topAppPackageName)) {
            return STAT_RESULT_RUNNING_BACKGROUND;  // ?????????????????????????????????
        }

        boolean isTop = topAppPackageName.equals(packageName);
        return isTop ? STAT_RESULT_RUNNING_TOP : STAT_RESULT_RUNNING_BACKGROUND;
    }

    /**
     * ?????????top???activity??????
     */
    public static String getTopActivity(Context context) {
        List<UsageStats> appList = getUsageStatsList(context);
        if (appList == null || appList.isEmpty()) {
            if (isDebug)
                LogTool.e(TAG, appList == null ? "appList is null" : "appList.isEmpty()");
            return "";
        }

        Collections.sort(appList, new Comparator<UsageStats>() {
            @Override
            public int compare(UsageStats lhs, UsageStats rhs) {
                return (int) (rhs.getLastTimeStamp() - lhs.getLastTimeStamp());
            }
        });
        if (isDebug)
            LogTool.i(TAG, "UsageStats->" + appList.get(0).getPackageName());
        return appList.get(0).getPackageName();
    }


    public static final String[] mLauncherClassName = new String[] {
            "com.miui.home.launcher.Launcher",                  // ??????
            "com.oneplus.hydrogen.launcher.Launcher",           // ??????
            "com.huawei.android.launcher.Launcher",             // ??????
            "com.jiubang.ggheart.apps.desks.diy.GoLauncher",    // go??????
            "com.google.android.launcher.GEL",                  // ????????????
            "com.android.launcher2.Launcher",                   // ??????
            "com.android.launcher.Launcher",                    // ??????1
            "com.meizu.flyme.launcher",                         // ??????
            "com.android.launcher3.Launcher",                   // cm rom
    };
    public static final String[] mLauncherPkgNames = new String[] {
            "com.miui.home",                            // ??????
            "com.oneplus.hydrogen.launcher",            // ??????
            "com.huawei.android.launcher",              // ??????
            "com.gau.go.launcherex",                    // go??????
            "com.google.android.googlequicksearchbox",  // ????????????
            "com.sec.android.app.launcher",             // ??????
            "com.sec.android.app.launcher",             // ??????1
            "com.meizu.flyme.launcher",                 // ??????
            "com.cyanogenmod.trebuchet",                // cm rom
    };
    /**
     * ?????????launcher??????
     * @return
     */
    public static boolean isOnLauncher(Context context) {
        String topAppPkgName = getTopActivity(context);
        return isOnLauncher(context, topAppPkgName);
    }

    public static boolean isOnLauncher(Context context, String topAppPkgName) {
        if (CheckAppRunning.isDebugRunning) {
            LogTool.i(TAG, "topAppPkgName " + topAppPkgName);
        }
        if (TextUtils.isEmpty(topAppPkgName)) {
            return false;
        }

        // ????????????launcher????????????
        for (String launcherPkgName : mLauncherPkgNames) {
            if (launcherPkgName.equals(topAppPkgName)) {
                return true;
            }
        }

        // ??????launcher????????????????????????
        if (topAppPkgName.toLowerCase().contains("launcher")) {
            LogTool.i(TAG, "topAppPkgName " + topAppPkgName);
            return true;
        }

        return false;
    }


    private static List getUsageStatsList(Context context) {
        UsageStatsManager usm = (UsageStatsManager) context.getSystemService(Context.USAGE_STATS_SERVICE);
        if (usm != null) {
            long now = System.currentTimeMillis();
            return usm.queryUsageStats(UsageStatsManager.INTERVAL_DAILY, now - 60*1000, now + 60 * 1000);
        }
        return null;
    }

}
