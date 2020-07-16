package com.dx.mobile.risk.supplier.oppo;

import android.content.Context;

public class OppoId {

    public static void init(Context context) {
//        OppoCfg.bSupported = OppoHelper._instance.isPkgValid(OppoCfg.getContext(context));
//        OppoCfg.bInit = true;
    }

    public static boolean isSupported() {
        return OppoCfg.bSupported;

    }

    // public static String b(Context context)
    public static String getOUID(Context context) {
        return OppoHelper._instance.getId(OppoCfg.getContext(context), "OUID");
    }

    // public static String c(Context context)
    public static String getDUID(Context context) {
        return OppoHelper._instance.getId(OppoCfg.getContext(context), "DUID");
    }

    // public static String d(Context context)
    public static String getAUID(Context context) {
        return OppoHelper._instance.getId(OppoCfg.getContext(context), "AUID");
    }
}
