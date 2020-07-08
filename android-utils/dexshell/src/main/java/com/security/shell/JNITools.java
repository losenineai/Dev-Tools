package com.security.shell;

import android.content.Context;

public class JNITools {

    private static Context sCtx = null;

    //this is for dex vmp
    public static boolean getIsShell(){
        return true;
    }

    //this is for dex vmp
    public static void setContext(Context ctx) {
        sCtx = ctx;
    }
    public static Context getContext() {
        return sCtx;
    }

    //this is for dex vmp
    public static byte[] getAddr(){
        return ShellSupporter.getSrcAddr();
    }


}
