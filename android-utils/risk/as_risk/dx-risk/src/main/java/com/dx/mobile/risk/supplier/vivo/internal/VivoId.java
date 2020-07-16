package com.dx.mobile.risk.supplier.vivo.internal;

import android.content.Context;

public class VivoId {

    public static String getVAID(Context context, String appId) {
        return VivoHelper.getInstance(context).getVAID(appId);
    }

    public static boolean isSupport(Context context) {
        return VivoHelper.getInstance(context).isSupport();
    }

    public static String getOAID(Context context) {
        return VivoHelper.getInstance(context).getOAID();
    }

    public static String getAAID(Context context, String appId) {
        return VivoHelper.getInstance(context).getAAID(appId);
    }
}
