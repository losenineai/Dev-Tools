package com.dx.mobile.risk.supplier.lenovo;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.zui.deviceidservice.IDeviceidInterface;

public class OpenDeviceId {

    private static String _tag = "OpenDeviceId library";    // c
    private static boolean _bLog = false;       // d
    private Context _ctx = null;                // a
    private IDeviceidInterface _deviceID;       // b
    private ServiceConnection _service;         // e
    private ILSupplier _supplier = null;        // f

    public interface ILSupplier {
        void handle(OpenDeviceId deviceId);
    }

    public OpenDeviceId(Context context, ILSupplier lenovoSupplier) {

        if (context == null) {
            throw new NullPointerException("Context can not be null.");
        }

        _ctx = context;
        _supplier = lenovoSupplier;
        _service = new ServiceConnection() {

            public synchronized void onServiceConnected(ComponentName name, IBinder iBinder) {

                _deviceID = IDeviceidInterface.Stub.asInterface(iBinder);

                if (_supplier != null) {
                    _supplier.handle(OpenDeviceId.this);
                }

                logi("Service onServiceConnected");
            }

            public void onServiceDisconnected(ComponentName name) {

                _deviceID = null;
                logi("Service onServiceDisconnected");
            }
        };

        Intent intent = new Intent();
        intent.setClassName("com.zui.deviceidservice", "com.zui.deviceidservice.DeviceidService");

        if (_ctx.bindService(intent, _service, Service.BIND_AUTO_CREATE)) {
            logi("bindService Successful!");
        } else {
            logi("bindService Failed!");
        }
    }

    // private void a(String str)
    private void logi(String str) {
        if (_bLog) {
            Log.i(_tag, str);
        }
    }

    // private void b(String str)
    private void loge(String str) {
//        if (_bLog) {
//            Log.e(_tag, str);
//        }
    }

    // public String a()
    public String getOAID() {

        if (_ctx == null) {
            loge("Context is null.");
            throw new IllegalArgumentException("Context is null, must be new OpenDeviceId first");
        }

        try {
            if (_deviceID != null) {
                return _deviceID.getOAID();
            }
        } catch (RemoteException e) {
            loge("getOAID error, RemoteException!");
        }

        return null;
    }

    // public String b()
    public String getUDID() {

        if (_ctx == null) {
            loge("Context is null.");
            throw new IllegalArgumentException("Context is null, must be new OpenDeviceId first");
        }

        try {
            if (_deviceID != null) {
                return _deviceID.getUDID();
            }
        } catch (RemoteException e) {
            loge("getUDID error, RemoteException!");
        }

        return null;
    }

    // public boolean c()
    public boolean isSupported() {

        boolean support = false;

        try {
            if (_deviceID != null) {
                logi("Device support opendeviceid");
                support = _deviceID.isSupported();
            }
        } catch (RemoteException e) {
            loge("isSupport error, RemoteException!");
        }

        return support;
    }

    // public String getVAID()
    public String getVAID() {

        if (_ctx == null) {
            loge("Context is null.");
            throw new IllegalArgumentException("Context is null, must be new OpenDeviceId first");
        }

        String packageName = _ctx.getPackageName();
        logi("liufeng, getVAID package\uff1a" + packageName);

        if (packageName == null || packageName.equals("")) {
            logi("input package is null!");

        } else {
            try {
                if (_deviceID != null) {
                    return _deviceID.getVAID(packageName);
                }
            } catch (RemoteException e) {
                loge("getVAID error, RemoteException!");
            }
        }
        return null;
    }

    // public String e()
    public String getAAID() {

        if (_ctx == null) {
            loge("Context is null.");
            throw new IllegalArgumentException("Context is null, must be new OpenDeviceId first");
        }

        String packageName = _ctx.getPackageName();
        logi("liufeng, getAAID package\uff1a" + packageName);

        if (packageName == null || packageName.equals("")) {
            logi("input package is null!");
        } else {
            try {
                if (_deviceID != null) {
                    return _deviceID.getAAID(packageName);
                }
            } catch (RemoteException e) {
                loge("getAAID error, RemoteException!");
            }
        }

        return null;
    }

    // public void f()
    public void unbind() {

        try {
            _ctx.unbindService(_service);
            logi("unBind Service successful");
        } catch (IllegalArgumentException e) {
            loge("unBind Service exception");
        }

        _deviceID = null;
    }

}
