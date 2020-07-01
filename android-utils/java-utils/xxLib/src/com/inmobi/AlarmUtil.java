package com.inmobi;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.util.Calendar;

/**
 * 闹钟定时器相关
 * Created by kim on 1/11/16.
 */
public class AlarmUtil {

    /**
     * 设置一个每天启动一次的闹钟
     */
    public static void setAlarmPerHalfHour(Context ctx, Class<? extends BroadcastReceiver> receiverCls,
                                      Calendar calendar, String action) {
        Intent intent = new Intent(ctx, receiverCls);
        intent.setAction(action);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(ctx, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);

        AlarmManager alarmManager = (AlarmManager) ctx.getSystemService(Context.ALARM_SERVICE);
        alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), AlarmManager.INTERVAL_HALF_HOUR, pendingIntent);
    }

    /**
     * 取消一个每天启动一次的闹钟，所有的参数必须和设置闹钟时一样
     */
    public static void cancelAlarmPerHalfHour(Context ctx, Class<? extends BroadcastReceiver> receiverCls,
                                         String action) {
        Intent intent = new Intent(ctx, receiverCls);
        intent.setAction(action);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(ctx, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);

        AlarmManager alarmManager = (AlarmManager) ctx.getSystemService(Context.ALARM_SERVICE);
        alarmManager.cancel(pendingIntent);
    }

    /**
     * 设置一个一次性的闹钟
     */
    public static void setAlarmOnce(Context ctx, Class<? extends BroadcastReceiver> receiverCls,
                                    Calendar calendar, String action) {
        Intent intent = new Intent(ctx, receiverCls);
        intent.setAction(action);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(ctx, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);

        AlarmManager alarmManager = (AlarmManager) ctx.getSystemService(Context.ALARM_SERVICE);
        alarmManager.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), pendingIntent);
    }
}
