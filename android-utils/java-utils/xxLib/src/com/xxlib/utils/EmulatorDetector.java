package com.xxlib.utils;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.os.Build;
import android.telephony.TelephonyManager;

import com.xxlib.utils.base.LogTool;

/**
 * 模拟器检车工具类
 * Created by white on 15-12-7.
 */
public class EmulatorDetector {
    private static final String TAG = "EmulatorDetector";
    private static boolean sBetteryCheck = false;

    static{
        BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver(){

            @Override
            public void onReceive(Context context, Intent intent) {
                try {
                    int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL,  -1);
                    int health = intent.getIntExtra(BatteryManager.EXTRA_HEALTH,  -1);
                    int temp = intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE,	 -1);

                    sBetteryCheck = level == 100 && health == 0 && temp == 0;
                    context.unregisterReceiver(this);
                } catch (Exception e) {
                    // IllegalArgumentException:Receiver not registered
                }
            }
        };

        IntentFilter filter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Context sContext = ContextFinder.getApplication();
        sContext.registerReceiver(mBroadcastReceiver, filter);
    }

    public static void init(){

    }

    private static boolean isIn(String name, String ... names){
        if(name != null){
            for(String n : names){
                if(n.equals(name)){
                    return true;
                }
            }
        }
        return false;
    }

    private static boolean contains(String s1, String s2){
        return s1 != null && s1.contains(s2);
    }

    private static boolean equals(String s1, String s2){
        return s1 != null && s1.equals(s2);
    }

    private static boolean telephonyManagerCheck(){
        try{
            Context context = ContextFinder.getApplication();
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            return equals(tm.getSubscriberId(), "310260000000000")
                    || contains(tm.getVoiceMailNumber(), "15552175049")
                    || equals(tm.getSimSerialNumber(), "89014103211118510720");
        }catch(Throwable e){
            LogTool.i(TAG, "telephonyManagerCheck e " + e.toString());
        }
       return false;
    }

    private static boolean buildCheck(){
        return equals(Build.HARDWARE, "vbox86")
                //TODO完善补充
                || isIn(Build.PRODUCT, "Droid4X")
                || isIn(Build.DEVICE, "gpdroidp");
    }


    public static boolean isEmulator(){
        return telephonyManagerCheck() || buildCheck() || sBetteryCheck;
    }

}
