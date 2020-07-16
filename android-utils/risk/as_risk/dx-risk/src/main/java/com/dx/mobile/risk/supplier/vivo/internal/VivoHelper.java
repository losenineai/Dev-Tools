package com.dx.mobile.risk.supplier.vivo.internal;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.SystemClock;


public class VivoHelper {

    private static Context _ctx; // a
    private static boolean _bSupport = false;   // b
    private static VivoHelper _instance = null;  // c
    private static VivoContentResolver _resolver = null; // d
    private static VivoContentObserver _oaidObserver = null;    // e
    private static VivoContentObserver _vaidObserver = null;    // f
    private static VivoContentObserver _aaidObserver = null;    // g
    private static Object _lockObj = new Object(); // h
    private static HandlerThread _handlerThread = null; // i
    private static Handler _handler = null;             // j
    private static String _tmp_id = null;               // k
    private static String _str_oaid = null;             // l
    private static String _str_vaid = null;             // m
    private static String _str_aaid = null;             // n

    private VivoHelper() {
    }

    // public static b a(Context context)
    public static VivoHelper getInstance(Context context) {
        if (_instance == null) {
            _instance = new VivoHelper();
            _ctx = context;
            startHandlerThread();
            _resolver = new VivoContentResolver(_ctx);
            init_support();
        }
        return _instance;
    }

    // public static String a(String key, String def)
    public static String sys_get(String key, String def) {
        try {
            Class cls = Class.forName("android.os.SystemProperties");
            return (String) cls.getMethod("get", new Class[]{String.class, String.class}).invoke(cls, new Object[]{key, "unknown"});
        } catch (Throwable th) {
            return def;
        }
    }

    // private static void a(Context context, int type, String appId)
    private static void registerContentObserver(Context context, int type, String appId) {
        switch (type) {
            case 0:
                _oaidObserver = new VivoContentObserver(_instance, 0, null);
                context.getContentResolver().registerContentObserver(Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/OAID"), true, _oaidObserver);
                return;
            case 1:
                _vaidObserver = new VivoContentObserver(_instance, 1, appId);
                context.getContentResolver().registerContentObserver(Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/VAID_" + appId), false, _vaidObserver);
                return;
            case 2:
                _aaidObserver = new VivoContentObserver(_instance, 2, appId);
                context.getContentResolver().registerContentObserver(Uri.parse("content://com.vivo.vms.IdProvider/IdentifierId/AAID_" + appId), false, _aaidObserver);
                return;
            default:
                return;
        }
    }

    // private void b(int i, String appId)
    private void setAppid(int type, String appId) {

        Message obtainMessage = _handler.obtainMessage();
        obtainMessage.what = 11;

        Bundle bundle = new Bundle();
        bundle.putInt("type", type);

        if (type == 1 || type == 2) {
            bundle.putString("appid", appId);
        }

        obtainMessage.setData(bundle);
        _handler.sendMessage(obtainMessage);
    }

    // public static void c()
    public static void init_support() {
        _bSupport = "1".equals(sys_get("persist.sys.identifierid.supported", "0"));
    }

    // private static void f()
    private static void startHandlerThread() {

        _handlerThread = new HandlerThread("SqlWorkThread");
        _handlerThread.start();

        _handler = new Handler(_handlerThread.getLooper()) {

            public void handleMessage(Message message) {
                if (message.what == 11) {
                    _tmp_id = _resolver.getValue(message.getData().getInt("type"), message.getData().getString("appid"));
                    synchronized (_lockObj) {
                        _lockObj.notify();
                    }
                    return;
                }

            }
        };
    }

    // public String a(String appId)
    public String getVAID(String appId) { // vaid

        if (!isSupport()) {
            return null;
        }
        if (_str_vaid != null) {
            return _str_vaid;
        }
        getId(1, appId);
        if (_vaidObserver == null && _str_vaid != null) {
            registerContentObserver(_ctx, 1, appId);
        }

        return _str_vaid;
    }

    // public void a(int type, String appId)
    public void getId(int type, String appId) {

        synchronized (_lockObj) {

            setAppid(type, appId);

            long uptimeMillis = SystemClock.uptimeMillis();
            try {
                _lockObj.wait(2000);
            } catch (InterruptedException e) {
            }

            if (SystemClock.uptimeMillis() - uptimeMillis < 2000) {
                switch (type) {
                    case 0:
                        _str_oaid = _tmp_id;
                        _tmp_id = null;
                        break;
                    case 1:
                        if (_tmp_id == null) {
                            break;
                        }
                        _str_vaid = _tmp_id;
                        _tmp_id = null;
                        break;
                    case 2:
                        if (_tmp_id == null) {
                            break;
                        }
                        _str_aaid = _tmp_id;
                        _tmp_id = null;
                        break;
                }
            }

        }
    }

    // public boolean a()
    public boolean isSupport() {
        return _bSupport;
    }

    // public String b()
    public String getOAID() {

        if (!isSupport()) {
            return null;
        }

        if (_str_oaid != null) {
            return _str_oaid;
        }

        getId(0, null);

        if (_oaidObserver == null) {
            registerContentObserver(_ctx, 0, null);
        }

        return _str_oaid;
    }

    // public String b(String appId)
    public String getAAID(String appId) {

        if (!isSupport()) {
            return null;
        }

        if (_str_aaid != null) {
            return _str_aaid;
        }

        getId(2, appId);

        if (_aaidObserver == null && _str_aaid != null) {
            registerContentObserver(_ctx, 2, appId);
        }

        return _str_aaid;
    }
}
