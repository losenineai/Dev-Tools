package com.dx.mobile.risk.collect.system;

import android.os.Build;
import android.os.SystemClock;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

import java.util.Date;
import java.util.TimeZone;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Time {

    public static String getTimesInfo() {

        JSONObject action_json = new JSONObject();

        String bootTime = (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) ?
                String.valueOf(System.currentTimeMillis() - SystemClock.elapsedRealtimeNanos() / 1000000) : "-1";
        String systemUptime = String.valueOf(SystemClock.elapsedRealtime());
        Date dt = new Date();
        String timeZone = String.valueOf(((long) TimeZone.getDefault().getOffset(dt.getTime())) / 3600000);

        // K201为首次启动时间，目前与K202相同
        JSONUtils.jsonPut(action_json, "K201", bootTime);
//         JSONUtils.jsonPut(action_json, "K202", bootTime);
        JSONUtils.jsonPut(action_json, "K203", systemUptime);
        JSONUtils.jsonPut(action_json, "K204", timeZone);
        JSONUtils.jsonPut(action_json, JNIConst.KEY_APP_KOCK_INDEX16, JNIConst.VALUE_APP_KOCK);
        return action_json.toString();
    }
}
