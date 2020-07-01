package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

/**
 * @author white
 * @description：电量工具
 * @date 2016/10/26
 */

public class BatteryUtil {

    private static final String TAG = "BatteryUtil";

    public static int getRemainPercent(Context context) {
        if (context == null) {
            return 0;
        }
        Intent intent = context.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        // 获取当前电量
        int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
        // 电量的总刻度
        int scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, 100);
        //把它转成百分比
        int percent = (level * 100) / scale;

        LogTool.d(TAG, "getRemainPercent percent:" + percent);
        return percent;
    }

    /**
     * 返回当前电量信息
     * @param context
     * @return BatteryInfo
     */
    public static BatteryInfo getCurrBatteryInfo(Context context) {

        BatteryInfo batteryInfo = new BatteryInfo();

        if (context == null) {
            return batteryInfo;
        }
        Intent intent = context.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        // 获取当前电量
        int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
        // 电量的总刻度
        int scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, 100);
        // 充电状态
        int status = intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1);

        //把它转成百分比
        int percent = (level * 100) / scale;

        boolean isCharging = status == BatteryManager.BATTERY_STATUS_CHARGING ||
                status == BatteryManager.BATTERY_STATUS_FULL;

        batteryInfo.setLevel(percent);
        batteryInfo.setCharging(isCharging);
        LogTool.d(TAG, "getRemainPercent percent:" + batteryInfo.toString());
        return batteryInfo;
    }


    /**
     * 当前电量信息
     */
    public static  class BatteryInfo{
        private boolean isCharging;

        public boolean isisCharging() {
            return isCharging;
        }
        public void setCharging(boolean charging) {
            this.isCharging = charging;
        }

        public int getLevel() {
            return level;
        }

        public void setLevel(int level) {
            this.level = level;
        }

        private int level;

        @Override
        public String toString() {
            return "level === " + level +
                    "isCharging === " + isCharging;

        }
    }
}
