package com.xxlib.utils.floatview;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.ComponentName;
import android.content.Context;
import android.text.TextUtils;

import com.xxlib.service.AccessibilityCheckRunning;
import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.base.LogTool;

import java.util.List;

/**
 * 检测指定游戏是否正在运行的工具类
 *
 * @author linbin 2014
 */
public class CheckAppRunning {

    private static final String TAG = "CheckAppRunning";
    public static int nowImportance = 0;
    public static final boolean isDebugRunning = false;		// 调试开关


    private static void log(String s) {
        if (isDebugRunning) {
            LogTool.i(TAG, s);
        }
    }

    private static int getImportanceByRunningTask(Context context, String packagename) {
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningTaskInfo> runningInfos = am.getRunningTasks(1);
        if (null != runningInfos && runningInfos.size() > 0) {
            ComponentName cn = runningInfos.get(0).topActivity;
            if (cn.getPackageName().equals(packagename)) {
                int importance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
                log("get importance by getRunningTasks");
                return importance;
            }
        }
        return RunningAppProcessInfo.IMPORTANCE_EMPTY;
    }

    private static int getImportanceByRunningAppProcesses(Context context, String packagename) {
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningAppProcessInfo> list = am.getRunningAppProcesses();
        if (list != null) {
            for (RunningAppProcessInfo info : list) {
                if (info.processName.equals(packagename)) {
                    int importance = info.importance;
                    log("get importance by getRunningAppProcesses pid " + info.pid + " importance is " + info.importance);
                    return importance;
                }
            }
        }
        return RunningAppProcessInfo.IMPORTANCE_EMPTY;
    }

    public static boolean isRunningAppProcessesValid(Context context) {
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<RunningAppProcessInfo> list = am.getRunningAppProcesses();
        if (null == list || list.size() == 0) {
            return false;
        }
        LogTool.i(TAG, "RunningAppProcessInfo size " + list.size());
        String pkgName = context.getPackageName();
        if (isDebugRunning) {
            for (RunningAppProcessInfo processInfo : list) {
                if (!processInfo.processName.contains(pkgName)) {
                    LogTool.i(TAG, "other process name : " + processInfo.processName);
                } else {
                    LogTool.i(TAG, "self process name : " + processInfo.processName);
                }
            }
        }
        // 如果可以获取到非本身的进程，表示可以用getRunningAppProcesses这个接口
        int otherProcessCnt = 0;
        for (int i=0; i<list.size(); ++i) {
            if (!list.get(i).processName.contains(pkgName)) {
                otherProcessCnt++;
                if (otherProcessCnt > 3) {  // 超过3个的其他进程，多于0也许可以？
                    return true;
                }
            }
        }
        return false;
    }

    private static int getImportanceByOomadjResult(int result) {
        int importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
        switch (result) {
            case CheckAppStateUtils.APP_CHECK_ERROR:
                // 错误的时候，当做是运行在后台
                // 可能是开始启动游戏oom_adj的值还没初始化完成；或者读取oomadj需要root权限
                importance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                break;
            case CheckAppStateUtils.APP_NO_RUNNING:
                importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                break;
            case CheckAppStateUtils.APP_RUNNING_BACKGROUND:
                importance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                break;
            case CheckAppStateUtils.APP_RUNNING_FRONT:
                importance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
                break;
            default:
                importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                break;
        }
        log("get importance by CheckAppStateUtils importance is " + importance + ", result " + result);
        return importance;
    }

    public static boolean isRunning(Context context, String packagename) {
        boolean isRunning = checkRunning(context, packagename);

        // 分享facebook的activity，正在分享时候，悬浮窗隐藏
        String topActivityName = CheckAppRunning.getRunningActivityName(ContextFinder.getApplication());
        if (nowImportance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
            if (topActivityName.equals("com.xxAssistant.DanMuKu.View.ShareFacebookActivity")
                    || topActivityName.equals("com.facebook.FacebookActivity")
                    || topActivityName.equals("com.facebook.composer.activity.ComposerActivity")){
                nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
            }
        }
        return isRunning;
    }

    private static int count = 0;
    public static boolean checkRunning(Context context, String packagename) {
        int importance = getImportanceByRunningTask(context, packagename);		// 部分5.0这个方法不行
        if (importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
            nowImportance = importance;
            return true;
        }

        importance = getImportanceByRunningAppProcesses(context, packagename);	// 部分5.0这个方法不行
        nowImportance = importance;
        if (importance != RunningAppProcessInfo.IMPORTANCE_EMPTY) {
            return true;
        }

        if (android.os.Build.VERSION.SDK_INT >= 21) {
            String topResult = CheckAppStateUtils.checkRunningByTopCmd(packagename);
//            log("top cmd topResult " + topResult);
            if (TextUtils.isEmpty(topResult) || TextUtils.isEmpty(topResult.trim())) {
                // top命令获取不到输出，当做游戏死掉；topResult不为空，表示该游戏还在运行（还不确定在前台或者后台）
                nowImportance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                LogTool.i(TAG, "get importance by top cmd, importance is " + nowImportance);
                return false;
            }

            // 有"无障碍"权限的情况下
            AccessibilityCheckRunning.readJsonFromFile(context);
            if (AccessibilityCheckRunning.isOnFloatView(packagename)) {
                nowImportance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
                log("get importance by AccessibilityCheck, importance is " + nowImportance);
                // 在悬浮窗面板home出去，面板会在launcher保持显示的bug。这里用stat的方式来判断，但是不一定正确。stat本身就不稳定
                count++;
                count = count % 5;
                if (count == 0) {
                    if (LollipopUtil.isOnLauncher(context)) {
                        nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                        log("get importance by AccessibilityCheck, isOnLauncher true, importance is " + nowImportance);
                    }
                }
                return true;
            }

            // 没有“无障碍”权限，优先用获取oom_adj的方式来判断
            int result = CheckAppStateUtils.checkByTopResult(topResult);
            importance = getImportanceByOomadjResult(result);
            if (importance < nowImportance) {		// 越小，重要级也高
                nowImportance = importance;
            }
            if (importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
                nowImportance = importance;
                // 部分游戏会home出去也显示悬浮窗。mc、疯狂点击, 用access权限来判断是否在桌面
                if (AccessibilityCheckRunning.isOnLauncher(context)) {
                    nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                    log("get importance by CheckAppStateUtils,but on launcher, importance is " + nowImportance);
                }
                return true;
            }
            // 没有在最前端，再用查看软件使用情况来判断是否在前端
            int statResult = LollipopUtil.isAppRunningOnTop(context, packagename);
            switch (statResult) {
                case LollipopUtil.STAT_RESULT_GOT_EMPTY_LIST_WITHOUT_PERMISSION:
                    nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                    break;
                case LollipopUtil.STAT_RESULT_RUNNING_TOP:
                case LollipopUtil.STAT_RESULT_GOT_EMPTY_LIST_WITH_PERMISSION:
                    nowImportance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
                    break;
                case LollipopUtil.STAT_RESULT_RUNNING_BACKGROUND:
                    nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                    break;
                default:
                    break;
            }
            log("get importance by LollipopUtil, statResult " + statResult + ", importance " + nowImportance);
            return true;
        }

        return nowImportance < RunningAppProcessInfo.IMPORTANCE_EMPTY;
    }


    /**
     * wxj, 是否运行在前台
     * @description
     * @param context
     * @param packageName
     * @return
     */
    public static boolean isXxRunningOnForeground(Context context, String packageName) {
        int importance = getImportanceByRunningAppProcesses(context, packageName);
        return importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
    }


    public static String getRunningActivityName(Context context) {
        try {
            ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
            @SuppressWarnings("deprecation")
            String runningActivity = activityManager.getRunningTasks(1).get(0).topActivity.getClassName();
            return runningActivity;
        } catch (Exception e) {
            LogTool.i(TAG, e.toString());
            return "";
        }
    }
}
