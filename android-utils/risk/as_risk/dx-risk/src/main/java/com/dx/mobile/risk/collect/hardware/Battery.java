package com.dx.mobile.risk.collect.hardware;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

import java.lang.reflect.Method;
import com.dx.mobile.risk.data.JNIConst;

/**
 * Time: 2019-10-30
 * Author: blue
 * Description: 电池信息
 */

public class Battery {

    private static Battery _instance;

    public static Battery getInstance() {

        if (_instance == null) {
            synchronized (Battery.class) {
                if (_instance == null) {
                    _instance = new Battery();
                }
            }
        }
        return _instance;
    }

    private String getBatteryStatus(Intent intent) {

        String bs = "unknown";

        switch (intent.getIntExtra(BatteryManager.EXTRA_STATUS, BatteryManager.BATTERY_STATUS_UNKNOWN)) {
            case BatteryManager.BATTERY_STATUS_UNKNOWN:
                bs = "unknown";
                break;
            case BatteryManager.BATTERY_STATUS_CHARGING:
                bs = "charging";
                break;
            case BatteryManager.BATTERY_STATUS_DISCHARGING:
                bs = "discharging";
                break;
            case BatteryManager.BATTERY_STATUS_NOT_CHARGING:
                bs = "not charging";
                break;
            case BatteryManager.BATTERY_STATUS_FULL:
                bs = "full";
                break;
        }

        return bs;
    }

    private String getBatteryPlugged(Intent intent) {

        String bp = "unknown";

        int ibp = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, 0);

        if (ibp == 1) {
            bp = "ac";
        } else if (ibp == 2) {
            bp = "usb";
        } else if (ibp == 4) {
            bp = "wireless";
        }

        return bp;
    }

    public String getCapaCity(Context context) {

        double batteryCapacity = 0;
        String POWER_PROFILE_CLASS = "com.android.internal.os.PowerProfile";

        try {

            Class cls = Class.forName(POWER_PROFILE_CLASS);

            Object newInstance = cls.getConstructor(Context.class).newInstance(context);

            Method method = cls.getMethod("getBatteryCapacity");
            method.setAccessible(true);

            batteryCapacity = (double) method.invoke(newInstance);

        } catch (Throwable th) {

        }

        return "" + batteryCapacity;
    }

    public String getBatteryJson() {

        try {
            JSONObject action_info = new JSONObject();
            Context ctx = ContextFinder.getApplication();

            Intent intent = ctx.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));

            float level = (float)intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
            float scale = (float)intent.getIntExtra(BatteryManager.EXTRA_SCALE, 0);
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX1, JNIConst.VALUE_APP_KOCK);
            JSONUtils.jsonPut(action_info, "K89", getCapaCity(ctx));
            JSONUtils.jsonPut(action_info, "K90", getBatteryStatus(intent));
            JSONUtils.jsonPut(action_info, "K91", intent.getIntExtra(BatteryManager.EXTRA_HEALTH, BatteryManager.BATTERY_HEALTH_UNKNOWN));
            JSONUtils.jsonPut(action_info, "K92", intent.getBooleanExtra(BatteryManager.EXTRA_PRESENT, false));
            JSONUtils.jsonPut(action_info, "K93", level);
            JSONUtils.jsonPut(action_info, "K94", scale);
            JSONUtils.jsonPut(action_info, "K95", getBatteryPlugged(intent));
            JSONUtils.jsonPut(action_info, "K96", intent.getIntExtra(BatteryManager.EXTRA_VOLTAGE, 0));
            JSONUtils.jsonPut(action_info, "K97", intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE, 0));
            JSONUtils.jsonPut(action_info, "K98", intent.getStringExtra(BatteryManager.EXTRA_TECHNOLOGY));
            JSONUtils.jsonPut(action_info, "K99", level / scale * 100);

            return action_info.toString();

        } catch (Throwable th) {

        }

        return "";
    }
}

