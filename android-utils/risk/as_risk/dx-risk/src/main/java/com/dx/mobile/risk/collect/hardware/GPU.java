package com.dx.mobile.risk.collect.hardware;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Build;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class GPU {

    public static String getGPUInfo() {

        try {
            JSONObject action_info = new JSONObject();
            Context ctx = ContextFinder.getApplication();

            ActivityManager am = (ActivityManager) ctx.getSystemService(Context.ACTIVITY_SERVICE);

            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.CUPCAKE) {

                ConfigurationInfo configInfo = am.getDeviceConfigurationInfo();
                JSONUtils.jsonPut(action_info, "K61", configInfo.getGlEsVersion());
                JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX8, JNIConst.VALUE_APP_KOCK);
            }

            return action_info.toString();
        } catch (Throwable t) {

        }

        return "";
    }
}
