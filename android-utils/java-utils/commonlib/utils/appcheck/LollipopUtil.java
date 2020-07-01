package com.auric.intell.commonlib.utils.appcheck;

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

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import com.auric.intell.commonlib.utils.LogTool;

/**
 * Created by xufu on 15-9-15.
 */

@TargetApi(Build.VERSION_CODES.LOLLIPOP)
public class LollipopUtil {

    private static final String TAG = "LollipopUtil";
    private static boolean isDebug = CheckAppRunningUtil.isDebugRunning;

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
            LogTool.d(TAG, e.toString());
        }
    }

    public static int isAppRunningOnTop(Context context, String packageName) {
        if (isDebug)
            LogTool.d(TAG, "PackageName->" + packageName);
        String topAppPackageName = getTopActivity(context);
        if (TextUtils.isEmpty(topAppPackageName)) {
            return STAT_RESULT_RUNNING_BACKGROUND;  // 空列表当做是在后台运行
        }

        boolean isTop = topAppPackageName.equals(packageName);
        return isTop ? STAT_RESULT_RUNNING_TOP : STAT_RESULT_RUNNING_BACKGROUND;
    }

    /**
     * 获取最top的activity包名
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
            LogTool.d(TAG, "UsageStats->" + appList.get(0).getPackageName());
        return appList.get(0).getPackageName();
    }


    public static final String[] mLauncherClassName = new String[] {
            "com.miui.home.launcher.Launcher",                  // 小米
            "com.oneplus.hydrogen.launcher.Launcher",           // 一加
            "com.huawei.android.launcher.Launcher",             // 华为
            "com.jiubang.ggheart.apps.desks.diy.GoLauncher",    // go桌面
            "com.google.android.launcher.GEL",                  // 谷歌系列
            "com.android.launcher2.Launcher",                   // 三星
            "com.android.launcher.Launcher",                    // 三星1
            "com.meizu.flyme.launcher",                         // 魅族
            "com.android.launcher3.Launcher",                   // cm rom
    };
    public static final String[] mLauncherPkgNames = new String[] {
            "com.miui.home",                            // 小米
            "com.oneplus.hydrogen.launcher",            // 一加
            "com.huawei.android.launcher",              // 华为
            "com.gau.go.launcherex",                    // go桌面
            "com.google.android.googlequicksearchbox",  // 谷歌系列
            "com.sec.android.app.launcher",             // 三星
            "com.sec.android.app.launcher",             // 三星1
            "com.meizu.flyme.launcher",                 // 魅族
            "com.cyanogenmod.trebuchet",                // cm rom
    };
    /**
     * 是否在launcher界面
     * @return
     */
    public static boolean isOnLauncher(Context context) {
        String topAppPkgName = getTopActivity(context);
        return isOnLauncher(context, topAppPkgName);
    }

    public static boolean isOnLauncher(Context context, String topAppPkgName) {
        if (CheckAppRunningUtil.isDebugRunning) {
            LogTool.d(TAG, "topAppPkgName " + topAppPkgName);
        }
        if (TextUtils.isEmpty(topAppPkgName)) {
            return false;
        }

        // 更常见的launcher包名比较
        for (String launcherPkgName : mLauncherPkgNames) {
            if (launcherPkgName.equals(topAppPkgName)) {
                return true;
            }
        }

        // 包含launcher字眼就当做在桌面
        if (topAppPkgName.toLowerCase().contains("launcher")) {
            LogTool.d(TAG, "topAppPkgName " + topAppPkgName);
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
