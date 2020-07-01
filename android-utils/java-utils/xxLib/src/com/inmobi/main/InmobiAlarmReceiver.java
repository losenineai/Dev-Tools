package com.inmobi.main;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/**
 * Created by mushroom0417 on 5/4/16.
 */
public class InmobiAlarmReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Intent intent1 = new Intent(context, BackgroundService.class);
        context.startService(intent1);
    }
}
