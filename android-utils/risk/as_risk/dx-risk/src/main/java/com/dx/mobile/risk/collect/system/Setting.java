package com.dx.mobile.risk.collect.system;

import android.content.ContentResolver;
import android.content.Context;
import android.provider.Settings;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Setting {

    public static String getSettingInfo() {

        Context ctx = ContextFinder.getApplication();
        ContentResolver cr = ctx.getContentResolver();

        JSONObject action_info = new JSONObject();

        try {
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX14, JNIConst.VALUE_APP_KOCK);
            JSONUtils.jsonPut(action_info, "K1500", Settings.System.getString(cr, "bd_setting_i"));
            JSONUtils.jsonPut(action_info, "K1501", Settings.System.getString(cr, "mqBRboGZkQPcAkyk"));
            JSONUtils.jsonPut(action_info, "K1502", Settings.System.getString(cr, "dxCRMxhQkdGePGnp"));
            JSONUtils.jsonPut(action_info, "K1503", Settings.System.getString(cr, "dxCRMxhQDPkdGePG"));
            JSONUtils.jsonPut(action_info, "K1504", Settings.System.getString(cr, "com.baidu.deviceid"));

            JSONUtils.jsonPut(action_info, "K1510", Settings.System.getString(cr, "usb_mass_storage_enabled"));
            JSONUtils.jsonPut(action_info, "K1511", Settings.System.getString(cr, "development_settings_enabled"));
            JSONUtils.jsonPut(action_info, "K1512", Settings.System.getString(cr, "accelerometer_rotation"));
            JSONUtils.jsonPut(action_info, "K1513", Settings.System.getString(cr, "lock_pattern_autolock"));
            JSONUtils.jsonPut(action_info, "K1514", Settings.System.getString(cr, "lock_pattern_visible_pattern"));
            JSONUtils.jsonPut(action_info, "K1515", Settings.System.getString(cr, "screen_brightness_mode"));
            JSONUtils.jsonPut(action_info, "K1516", Settings.System.getString(cr, "sound_effects_enabled"));
            JSONUtils.jsonPut(action_info, "K1517", Settings.System.getString(cr, "screen_off_timeout"));



        } catch (Throwable t) { }

        try {
            int enable = Settings.Secure.getInt(ctx.getContentResolver(), Settings.Secure.ACCESSIBILITY_ENABLED);
            if (enable == 1) {
                JSONUtils.jsonPut(action_info, "K1500", Settings.System.getString(cr, "bd_setting_i"));
            }
        } catch (Throwable t) { }

        return action_info.length() > 0 ? action_info.toString() : "";
    }
}
