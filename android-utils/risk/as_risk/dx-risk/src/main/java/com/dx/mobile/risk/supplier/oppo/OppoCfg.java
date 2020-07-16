package com.dx.mobile.risk.supplier.oppo;

import android.content.Context;

public class OppoCfg {

    public static boolean bInit;        // a
    public static boolean bSupported;   // b

    public static Context getContext(Context context) {
        return (context == null || context.getApplicationContext() == null) ? context : context.getApplicationContext();
    }

}
