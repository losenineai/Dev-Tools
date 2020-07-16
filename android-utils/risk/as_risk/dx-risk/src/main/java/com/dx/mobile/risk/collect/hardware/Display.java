package com.dx.mobile.risk.collect.hardware;

import android.content.Context;
import android.os.Build;
import android.os.PowerManager;
import android.provider.Settings;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.DisplayCompat;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

import java.util.Locale;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Display {

    private static PowerManager powerManager;

    public static String getScreenInfo() {

        try {
            JSONObject action_info = new JSONObject();
            Context ctx = ContextFinder.getApplication();
            WindowManager wm = (WindowManager) ctx.getSystemService(Context.WINDOW_SERVICE);
            DisplayCompat dyc = new DisplayCompat(wm.getDefaultDisplay());

            DisplayMetrics metrics = new DisplayMetrics();
            dyc.getRealMetrics(metrics);

            int systemBrightness = 0;
            try {
                systemBrightness = Settings.System.getInt(ctx.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS);
            } catch (Throwable t) {
            }

            /* delete from 5.3.0
            JSONUtils.jsonPut(action_info, "K110", dyc.getName());
            JSONUtils.jsonPut(action_info, "K112", metrics.densityDpi);
            JSONUtils.jsonPut(action_info, "K113", metrics.density);
            */

            JSONUtils.jsonPut(action_info, "K111", String.format(Locale.US, "%dx%d", metrics.widthPixels, metrics.heightPixels));
            JSONUtils.jsonPut(action_info, "K114", systemBrightness);
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX7, JNIConst.VALUE_APP_KOCK);

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR_MR1) {

                try {
                    if (powerManager == null)
                        powerManager = (PowerManager) ContextFinder.getApplication().getSystemService(Context.POWER_SERVICE);
                    JSONUtils.jsonPut(action_info, "K115", powerManager.isScreenOn());
                } catch (SecurityException e) {
                }
            }

            return action_info.toString();
        } catch (Throwable t) {

        }

        return "";
    }
}
