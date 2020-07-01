package com.auric.intell.commonlib.utils;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;


/**
 * @author white
 * @description：闹钟工具类
 * @date 2017/6/22
 */

public class AlarmUtil {

    private static final String TAG = "AlarmUtil";

    /**
     * 启动闹钟
     * @param action
     * @param triggerAtMills 绝对时间
     */
    public static void startAlarm(String action, long triggerAtMills) {
        LogTool.d(TAG, "startAlarm " + action + " triggerAtMills:"+triggerAtMills);
        Context context = ContextFinder.getApplication();
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Service.ALARM_SERVICE);
        Intent intent = new Intent(action);
        PendingIntent pIntent = PendingIntent.getBroadcast(context, 0 , intent, PendingIntent.FLAG_UPDATE_CURRENT);
//        alarmManager.set(AlarmManager.RTC_WAKEUP, triggerAtMills, pIntent);
        // api >= 19
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            //参数2是开始时间、参数3是允许系统延迟的时间
            alarmManager.setWindow(AlarmManager.RTC_WAKEUP, triggerAtMills, 0, pIntent);
        } else {
            alarmManager.set(AlarmManager.RTC_WAKEUP, triggerAtMills, pIntent);
        }
    }

    /**
     * 启动重复闹钟
     * @param action
     * @param triggerAtMills 第一次触发绝对时间
     * @param intervalMillis 间隔的时间
     */
    public static void startRepeatAlarm(String action, long triggerAtMills, long intervalMillis) {
        LogTool.d(TAG, "startRepeatAlarm " + action + " triggerAtMills :"+triggerAtMills + " intervalMillis:"+intervalMillis);
        Context context = ContextFinder.getApplication();
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Service.ALARM_SERVICE);
        Intent intent = new Intent(action);
        PendingIntent pIntent = PendingIntent.getBroadcast(context, 0 , intent, PendingIntent.FLAG_UPDATE_CURRENT);
//        alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, triggerAtMills, intervalMillis, pIntent);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            // 参数2是开始时间、参数3是允许系统延迟的时间
            alarmManager.setWindow(AlarmManager.RTC_WAKEUP, triggerAtMills, 0, pIntent);
        } else {
            alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, triggerAtMills, intervalMillis, pIntent);
        }
    }

    /**
     * 取消闹钟
     * @param action
     */
    public static void cancelAlarm(String action) {
        LogTool.d(TAG, "cancelAlarm " + action);
        Context context = ContextFinder.getApplication();
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Service.ALARM_SERVICE);
        Intent intent = new Intent(action);
        PendingIntent pIntent = PendingIntent.getBroadcast(context, 0 , intent, PendingIntent.FLAG_CANCEL_CURRENT);
        alarmManager.cancel(pIntent);
        pIntent.cancel();
    }
}
