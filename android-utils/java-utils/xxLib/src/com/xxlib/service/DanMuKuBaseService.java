package com.xxlib.service;

import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;

import com.xxlib.config.JsonSpCenter;
import com.xxlib.reciever.HomeWatcherReciever;
import com.xxlib.utils.AppTool;
import com.xxlib.utils.SPCenter.XXLibSpkey;
import com.xxlib.utils.ThreadUtil;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.floatview.CheckAppRunning;

import java.lang.reflect.Method;

/**
 * Created by jammy on 15/10/23.
 */
public abstract class DanMuKuBaseService extends Service {

    private static final String TAG = "DanMuKuBaseService";
    /**
     * 游戏包名
     */
    public static String mGamePkgName = "";
    /**
     * 标识Service是否已创建
     */
    protected static boolean isCreate = false;
    /**
     * 循环检测时间
     */
    public static final int mCircleTime = 200;

    private HomeWatcherReciever mHomeWatcher;

    private JsonSpCenter mJsonSp;

//    /**
//     * 是否有“无障碍”的权限
//     */
//    protected boolean mIsHasAccessibilityPermission = false;
//    public static AccessibilityCheckRunning mAccessibilityCheckRunning = new AccessibilityCheckRunning();


    // 必须实现的一些方法
    public abstract void onGameForeground();
    public abstract void onGameBackground();
    public abstract void onGameExit();


    @Override
    public void onCreate() {
        super.onCreate();
        AccessibilityCheckRunning.checkIsHasAccessibilityPermission(this);
        LogTool.i(TAG, "mIsHasAccessibilityPermission:" + AccessibilityCheckRunning.mIsHasAccessibilityPermission);

        if (mHomeWatcher == null && Build.VERSION.SDK_INT >= 21) {
            mHomeWatcher = new HomeWatcherReciever();
            IntentFilter homeFilter = new IntentFilter(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
            registerReceiver(mHomeWatcher, homeFilter);
        }
        mJsonSp = new JsonSpCenter(this);
    }

    @Override
    public void onDestroy() {
        if (mHomeWatcher != null && Build.VERSION.SDK_INT >= 21) {
            try{
                unregisterReceiver(mHomeWatcher);
            }catch(Exception er){
            }
            mHomeWatcher = null;
        }
        super.onDestroy();
    }

    /**
     * 检测线程
     */
    protected Thread mWatchDog = new Thread() {
        public void run() {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
            while (isCreate) {
                if (mGamePkgName.equals("")) {
                    LogTool.w(TAG, "mGamePkgName is empty, handler send to kill service");
                    onGameExit();
                    break;
                }

                boolean isRunning = CheckAppRunning.isRunning(getApplicationContext(), mGamePkgName);
                int importance = CheckAppRunning.nowImportance;
                if (!isRunning) {
                    LogTool.i(TAG, "mGamePkgName is not running, importance " + importance);
                    onGameExit();
                    break;
                }

                if (importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
//                    boolean isScriptRunningFlag = SPCenter.getBoolean(XXLibSpkey.XXLIB_IS_SCRIPT_RUNNING, false, SPCenter.SCRIPT_PREFERENCE);
                    boolean isScriptRunningFlag = mJsonSp.getBoolean(XXLibSpkey.XXLIB_IS_SCRIPT_RUNNING, false);

                    LogTool.i(TAG, "isScriptProcessRunning " + AppTool.isScriptProcessRunning(DanMuKuBaseService.this));
                    if (isScriptRunningFlag && AppTool.isScriptProcessRunning(DanMuKuBaseService.this)) {
                        onGameBackground();// 脚本正在运行，不显示浮点
                    } else {
                        onGameForeground();
                    }
                    if (HomeWatcherReciever.mIsHomeOut) {
                        // 如果是home出去的话，重置标志位
                        HomeWatcherReciever.mIsHomeOut = false;
                    }
                    if (AccessibilityCheckRunning.mIsJumpOut) {
                        // 如果是游戏中的悬浮窗跳转出去的话，重置标志位
                        AccessibilityCheckRunning.mIsJumpOut = false;
                    }
                } else {
                    onGameBackground();
                }

                ThreadUtil.sleepForDetectGap(mCircleTime);
            } // end of while
        }
    };


    protected void cancelNotification(int notifyId) {
        LogTool.i(TAG, "cancelNotification " + notifyId);
        if (isShowNotification()) {
            doCancelNotification(notifyId);
        }
    }

    private void doCancelNotification(int notifyId) {
        try {
            LogTool.i(TAG, "doCancelNotification " + notifyId + ", pid " + android.os.Process.myPid());
            NotificationManager nMgr = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            nMgr.cancel(notifyId);
        } catch (Throwable t) {
            LogTool.w(TAG, t);
        }
    }

    protected void startNotification(Context context, int notifyId, int iconResId, String title, String content, Class<?> cls) {
        LogTool.i(TAG, "startNotification " + notifyId);
        if (isShowNotification()) {
            doStartNotification(context, notifyId, iconResId, title, content, cls);
        }
    }

    private void doStartNotification(Context context, int notifyId, int iconResId, String title, String content, Class<?> cls) {
        LogTool.i(TAG, "doStartNotification " + notifyId + ", pid " + android.os.Process.myPid());
        // 在通知栏上设置通知
        Notification notification = new Notification(iconResId, title, System.currentTimeMillis());
        Intent notificationIntent = new Intent(this, cls);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);
        if (Build.VERSION.SDK_INT < 23) {
            //notification.setLatestEventInfo(this, title, content, pendingIntent);
            try {
                Method method = Notification.class.getDeclaredMethod("setLatestEventInfo", Context.class, CharSequence.class, CharSequence.class, PendingIntent.class);
                method.invoke(notification, this, title, content, pendingIntent);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            notification = new Notification.Builder(this).setSmallIcon(iconResId)
                    .setContentTitle(title)
                    .setWhen(System.currentTimeMillis())
                    .setContentIntent(pendingIntent)
                    .setContentText(content)
                    .build();
        }
        // 设置Service为前台进程
        startForeground(notifyId, notification);
    }

    protected boolean isShowNotification() {
//        LogTool.i(TAG, "isShowNotification, pid " + android.os.Process.myPid());
//        if (mIsHasAccessibilityPermission) {
//            // 如果开启了无障碍，那么就不用通知栏了，不然会导致通知栏一直存在
//            return false;
//        }
        return true;
    }
}
