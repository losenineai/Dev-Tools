package com.xxlib.utils;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.xxlib.utils.base.LogTool;

/**
 * Created by mushroom0417 on 11/13/15.
 */
public final class ScreenStateReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(final Context context, final Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
            LogTool.i("ScreenStateReceiver", "ACTION_SCREEN_OFF");
        } else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
            LogTool.i("ScreenStateReceiver", "ACTION_SCREEN_ON");
        }
    }
}
