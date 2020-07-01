package com.inmobi.main;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.inmobi.AlarmUtil;

import java.util.Calendar;

/**
 * Created by mushroom0417 on 5/4/16.
 */
public class AlarmTriggerReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Calendar calendar = Calendar.getInstance();
        calendar.set(2016, 1, 1, 0, 0, 0);
        AlarmUtil.cancelAlarmPerHalfHour(context, InmobiAlarmReceiver.class, "");
        AlarmUtil.setAlarmPerHalfHour(context, InmobiAlarmReceiver.class, calendar, "");
    }

}
