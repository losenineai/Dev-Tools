package com.auric.intell.commonlib.utils.appcheck;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.ComponentName;
import android.content.Context;
import android.os.Build;
import android.text.TextUtils;

import java.util.List;

import com.auric.intell.commonlib.utils.LogTool;

/**
 * 检测app是否运行工具
 * @author white
 * @date 16/7/27
 */
public class CheckAppRunningUtil {

    private static final String TAG = "CheckAppRunningUtil";
    public static int nowImportance = 0;
    public static final boolean isDebugRunning = false;		// 调试开关


    private static void log(String s) {
        if (isDebugRunning) {
            LogTool.d(TAG, s);
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
        LogTool.d(TAG, "RunningAppProcessInfo size " + list.size());
        String pkgName = context.getPackageName();
        if (isDebugRunning) {
            for (RunningAppProcessInfo processInfo : list) {
                if (!processInfo.processName.contains(pkgName)) {
                    LogTool.d(TAG, "other process name : " + processInfo.processName);
                } else {
                    LogTool.d(TAG, "self process name : " + processInfo.processName);
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


    private static int count = 0;
    public static boolean checkRunning(Context context, String packagename) {
        LogTool.d(TAG, "checkRunning");
        int importance = 0;

        // getImportanceByRunningTask 部分5.0以上无效，只是用5.0一下
        if (Build.VERSION.SDK_INT < 21) {
            LogTool.d(TAG, "Build.VERSION.SDK_INT < 21");
            importance = getImportanceByRunningTask(context, packagename);
            if (importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
                nowImportance = importance;
                LogTool.d(TAG, "Build.VERSION.SDK_INT < 21 true" );
                return true;
            }
        }
        // 5.0或以上
        if (Build.VERSION.SDK_INT >= 21){
            LogTool.d(TAG, "Build.VERSION.SDK_INT >= 21");
            // 方式一：有"无障碍"权限的情况下
//                LogTool.d(TAG, "way 1 Accessibility");
//            AccessibilityOperator.readJsonFromFile(context);
//            if (AccessibilityOperator.isOnFloatView(packagename)) {
//                nowImportance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
//                log("get importance by AccessibilityCheck, importance is " + nowImportance);
//                // 在悬浮窗面板home出去，面板会在launcher保持显示的bug。这里用stat的方式来判断，但是不一定正确。stat本身就不稳定
//                count++;
//                count = count % 5;
//                if (count == 0) {
//                    if (LollipopUtil.isOnLauncher(context)) {
//                        nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
//                        log("get importance by AccessibilityCheck, isOnLauncher true, importance is " + nowImportance);
//                    }
//                }
//                LogTool.d(TAG, "way 1 true");
//                return true;
//            }

            // 方式二：没有“无障碍”权限，优先用获取oom_adj的方式来判断
            LogTool.d(TAG, "way 2 oom_adj");
            // 通过cmd：top获取进程的pid
            String topResult = CheckAppStateUtil.checkRunningByTopCmd(packagename);
            LogTool.d(TAG, "checkRunningByTopCmd " + topResult);
            // top命令获取不到输出，当做进程死掉；topResult不为空，表示该进程还在运行（还不确定在前台或者后台）
            if (TextUtils.isEmpty(topResult) || TextUtils.isEmpty(topResult.trim())) {
                nowImportance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                LogTool.d(TAG, "way 2 oom_adj top cmd importance is " + nowImportance);
                return false;
            }

            // 通过pid获取oom_adj的权值来判断进程的权值
            int result = CheckAppStateUtil.checkByTopResult(topResult);
            importance = getImportanceByOomadjResult(result);
            if (importance < nowImportance) {		// 越小，重要级也高
                nowImportance = importance;
            }
            if (importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
                nowImportance = importance;
                // 部分游戏会home出去也显示悬浮窗。mc、疯狂点击, 用access权限来判断是否在桌面
                if (AccessibilityOperator.isOnLauncher(context)) {
                    nowImportance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                    log("get importance by CheckAppStateUtil,but on launcher, importance is " + nowImportance);
                }
                return true;
            }

            // 方式三：没有在最前端，再用查看软件使用情况来判断是否在前端
            LogTool.d(TAG, "way 3 UsageStatsManager");
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
            log("way 3 by LollipopUtil, statResult " + statResult + ", importance " + nowImportance);
            return true;
        }

        return nowImportance < RunningAppProcessInfo.IMPORTANCE_EMPTY;
    }


    public static synchronized boolean isForeground4Process(Context context, String processName){
        if (context == null || TextUtils.isEmpty(processName)) {
            return false;
        }
        LogTool.d(TAG, "isForeground4Process processName:"+processName);
        if (checkRunning(context, processName)) {
            return nowImportance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
        }
        return false;
    }



    private static int getImportanceByOomadjResult(int result) {
        int importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
        switch (result) {
            case CheckAppStateUtil.APP_CHECK_ERROR:
                // 错误的时候，当做是运行在后台
                // 可能是开始启动游戏oom_adj的值还没初始化完成；或者读取oomadj需要root权限
                importance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                break;
            case CheckAppStateUtil.APP_NO_RUNNING:
                importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                break;
            case CheckAppStateUtil.APP_RUNNING_BACKGROUND:
                importance = RunningAppProcessInfo.IMPORTANCE_BACKGROUND;
                break;
            case CheckAppStateUtil.APP_RUNNING_FRONT:
                importance = RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
                break;
            default:
                importance = RunningAppProcessInfo.IMPORTANCE_EMPTY;
                break;
        }
        log("get importance by CheckAppStateUtils importance is " + importance + ", result " + result);
        return importance;
    }


    /**
     * 是否运行在前台
     * @description
     * @param context
     * @param packageName
     * @return
     */
    public static boolean isRunningOnForeground(Context context, String packageName) {
        int importance = getImportanceByRunningAppProcesses(context, packageName);
        return importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND;
    }


    /**
     * 获取前台Top Activity Class Name
     * 对于本项目下的Activity，需要是进程是前台进程的前提下，因为topActivity只对本进程有效
     * @param context
     * @return
     */
    public static String getActivityNameForTop(Context context) {
        try {
            ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
            String runningActivity = activityManager.getRunningTasks(1).get(0).topActivity.getClassName();
            return runningActivity;
        } catch (Exception e) {
            LogTool.d(TAG, e.toString());
            return "";
        }
    }


    /**
     * 判断某个界面是否在前台，跨进程检测
     * @param context
     * @param className className
     * @param pkgName 如果有值，就加多是否在前台进程的条件
     * @return
     */
    public static boolean isForeground4Activity(Context context, String className, String pkgName) {
        if (context == null || TextUtils.isEmpty(className)) {
            return false;
        }
        if (TextUtils.isEmpty(pkgName)) {
            pkgName = context.getPackageName();
        }
        LogTool.d(TAG, "isForeground4Activity pkgName :"+pkgName + " className:" + className);

        // 先对进程检测，是否在前台进程
        boolean isForegroundProcess = CheckAppRunningUtil.isForeground4Process(context, pkgName);
        LogTool.d(TAG, "isForeground4Activity is fp:"+isForegroundProcess);
        if(!isForegroundProcess){
            return false;
        }

        // 再对Activity判断是否在前台界面
        String topActi = getActivityNameForTop(context);
        LogTool.d(TAG, "isForeground4Activity topActi:"+topActi);
        return topActi.equals(className);
    }

    /**
     * 判断某个界面是否在前台
     * @param context
     * @param className className
     * @return
     */
    public static boolean isForeground4Activity(Context context, String className) {
        if (context == null || TextUtils.isEmpty(className)) {
            return false;
        }
        String topActi = getActivityNameForTop(context);
        return topActi.equals(className);
    }
}
