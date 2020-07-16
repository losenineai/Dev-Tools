package com.dx.mobile.risk.collect.hardware;

import android.Manifest;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.PermissionUtils;
import com.dx.mobile.risk.utils.SimUtils;

import org.json.JSONObject;

/**
 * Time: 2019-10-30
 * Author: blue
 * Description:
 */
public class Telephone {

    private static TelephonyManager telephoneMgr;

    static {
        if (telephoneMgr == null) {
            try {
                telephoneMgr = (TelephonyManager) ContextFinder.getApplication().getSystemService(Context.TELEPHONY_SERVICE);
                SimUtils.init(telephoneMgr);
            } catch (SecurityException e) {
            }

        }
    }

    public static String getDeviceIDs() {

        Context ctx = ContextFinder.getApplication();
        JSONObject action_info = new JSONObject();


        getDeviceID(ctx, action_info);
        getDeviceID_ext(action_info);

        return action_info.toString();
    }

    public static String getDeviceID() {

        Context ctx = ContextFinder.getApplication();
        JSONObject action_info = new JSONObject();

        return getDeviceID(ctx, action_info);
    }

    public static String getDeviceID(Context ctx, JSONObject action_info) {

        try {
            // v5.3.0
            // annotation for google grounding - by blue 2018.10.31
//            if (!StringUtils.isEmpty(str = SimUtils.getPhoneNumber()))
//                JSONUtils.jsonPut(action_info, "K44", str);
//
//            if (!StringUtils.isEmpty(str = SimUtils.getVoiceMailNumber()))
//                JSONUtils.jsonPut(action_info, "K45", str);
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX11, JNIConst.VALUE_APP_KOCK);
            JSONUtils.jsonPut(action_info, "K46", SimUtils.getCurrentNetworkType(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K48", SimUtils.getOperatorName(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K50", SimUtils.getDeviceId(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K51", SimUtils.getSubscriberid(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K52", SimUtils.getSimSerialNumber(SimUtils.SIM_CARD1));

            if (SimUtils.getSimCardCount() >= 2) {

                JSONUtils.jsonPut(action_info, "K47", SimUtils.getCurrentNetworkType(SimUtils.SIM_CARD2));
                JSONUtils.jsonPut(action_info, "K49", SimUtils.getOperatorName(SimUtils.SIM_CARD2));
                JSONUtils.jsonPut(action_info, "K53", SimUtils.getDeviceId(SimUtils.SIM_CARD2));
                JSONUtils.jsonPut(action_info, "K54", SimUtils.getSubscriberid(SimUtils.SIM_CARD2));
                JSONUtils.jsonPut(action_info, "K55", SimUtils.getSimSerialNumber(SimUtils.SIM_CARD2));
            }

            JSONUtils.jsonPut(action_info, "K56", Settings.System.getString(ctx.getContentResolver(), Settings.Secure.ANDROID_ID));
            JSONUtils.jsonPut(action_info, "K57", Build.SERIAL);

            return action_info.toString();
        } catch (Throwable t) {  }

        return "";
    }

    public static String getDeviceID_ext() {

        JSONObject action_info = new JSONObject();

        return getDeviceID_ext(action_info);
    }

    public static String getDeviceID_ext(JSONObject action_info) {

        try {

            JSONUtils.jsonPut(action_info, "K1710", SimUtils.getTele2DeviceId());

            JSONUtils.jsonPut(action_info, "K1711", SimUtils.getMsimDeviceId(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K1712", SimUtils.getMsimDeviceId(SimUtils.SIM_CARD2));

            JSONUtils.jsonPut(action_info, "K1713", SimUtils.getDeviceIdGemini(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K1714", SimUtils.getDeviceIdGemini(SimUtils.SIM_CARD2));

            JSONUtils.jsonPut(action_info, "K1715", SimUtils.getDefault(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K1716", SimUtils.getDefault(SimUtils.SIM_CARD2));

            JSONUtils.jsonPut(action_info, "K1720", SimUtils.getTele2SubscriberIdId());

            JSONUtils.jsonPut(action_info, "K1721", SimUtils.getMsimSubscriberId(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K1722", SimUtils.getMsimSubscriberId(SimUtils.SIM_CARD2));

            JSONUtils.jsonPut(action_info, "K1723", SimUtils.getSubscriberIdGemini(SimUtils.SIM_CARD1));
            JSONUtils.jsonPut(action_info, "K1724", SimUtils.getSubscriberIdGemini(SimUtils.SIM_CARD2));

            return action_info.toString();

        } catch (Throwable t) { }

        return "";
    }

    public static String getTelephoneInfo() {

        JSONObject action_json = new JSONObject();

        if (telephoneMgr != null) {
            try {
                JSONUtils.jsonPut(action_json, "K251", String.valueOf(telephoneMgr.getSimState()));

                JSONUtils.jsonPut(action_json, "K500", telephoneMgr.getNetworkCountryIso());
                JSONUtils.jsonPut(action_json, "K501", telephoneMgr.getNetworkOperator());
                JSONUtils.jsonPut(action_json, "K502", telephoneMgr.getNetworkOperatorName());
                JSONUtils.jsonPut(action_json, "K503", telephoneMgr.getSimCountryIso());
                JSONUtils.jsonPut(action_json, "K504", telephoneMgr.getSimOperator());
                JSONUtils.jsonPut(action_json, "K505", telephoneMgr.getSimOperatorName());
                JSONUtils.jsonPut(action_json, "K506", telephoneMgr.getNetworkType());

                if(PermissionUtils.checkPermissions(Manifest.permission.ACCESS_NETWORK_STATE)) {
                    final NetworkInfo network = ((ConnectivityManager) ContextFinder.getApplication().getApplicationContext()
                            .getSystemService(Context.CONNECTIVITY_SERVICE))
                            .getActiveNetworkInfo();

                    JSONUtils.jsonPut(action_json, "K520", network.getType() == 0 ? network.getSubtype(): network.getType());
                    JSONUtils.jsonPut(action_json, "K521", network.getTypeName());
                    JSONUtils.jsonPut(action_json, "K522", network.getSubtypeName());

                }

            } catch (Exception e) {
            }
        }

        return action_json.toString();
    }
}
