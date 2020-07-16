package com.dx.mobile.risk.supplier.xiaomi;

import android.content.Context;

import java.lang.reflect.Method;

public class XiaomiHelper {

    private static Object obj_idProviderImpl;     // a
    private static Class<?> clz_idProviderImpl;     // b
    private static Method meth_getDefaultUDID;    // c
    private static Method meth_getOAID;           // d
    private static Method meth_getVAID;           // e
    private static Method meth_getAAID;           // f

    static {
        meth_getDefaultUDID = null;
        meth_getOAID = null;
        meth_getVAID = null;
        meth_getAAID = null;

        try {
            clz_idProviderImpl = Class.forName("com.android.id.impl.IdProviderImpl");
            obj_idProviderImpl = clz_idProviderImpl.newInstance();
        } catch (Throwable e) {
        }
        try {
            meth_getDefaultUDID = clz_idProviderImpl.getMethod("getDefaultUDID", new Class[]{Context.class});
        } catch (Throwable e) {
        }
        try {
            meth_getOAID = clz_idProviderImpl.getMethod("getOAID", new Class[]{Context.class});
        } catch (Throwable e) {
        }
        try {
            meth_getVAID = clz_idProviderImpl.getMethod("getVAID", new Class[]{Context.class});
        } catch (Throwable e) {
        }
        try {
            meth_getAAID = clz_idProviderImpl.getMethod("getAAID", new Class[]{Context.class});
        } catch (Throwable e) {
        }
    }

    // public static boolean a() {
    public static boolean isSupported() {
        return (clz_idProviderImpl == null || obj_idProviderImpl == null) ? false : true;
    }

    // public static String a(Context context)
    public static String getOAID(Context context) {
        return invokeMethod(context, meth_getOAID);
    }

    // public static String b(Context context)
    public static String getVAID(Context context) {
        return invokeMethod(context, meth_getVAID);
    }

    // public static String c(Context context)
    public static String getAAID(Context context) {
        return invokeMethod(context, meth_getAAID);
    }

    // private static String a(Context context, Method method)
    private static String invokeMethod(Context context, Method method) {

        if (!(obj_idProviderImpl == null || method == null)) {
            try {
                Object invoke = method.invoke(obj_idProviderImpl, new Object[]{context});
                if (invoke != null) {
                    return (String) invoke;
                }
            } catch (Throwable e) {
            }
        }
        return null;
    }



}
