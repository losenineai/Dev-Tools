package com.dx.mobile.risk.supplier.oppo;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.IBinder;
import android.os.RemoteException;
import android.text.TextUtils;

import com.heytap.openid.IOpenID;

import java.security.MessageDigest;

public class OppoHelper {

    public IOpenID _openId = null;      // a
    public String _pkgName = null;      // b
    public String _signature = null;    // c
    public final Object _lockObj = new Object();   // d

    public static final OppoHelper _instance = new OppoHelper();

    public ServiceConnection _service = new ServiceConnection() {

        public void onServiceConnected(ComponentName name, IBinder iBinder) {

            _openId = IOpenID.Stub.asInterface(iBinder);

            synchronized (_lockObj) {
                _lockObj.notify();
            }
        }

        public void onServiceDisconnected(ComponentName name) {
            _openId = null;
        }
    };


    public void init(Context context){

    }

    public synchronized String getId(Context context, String str) {

        String id;

        if (_openId == null) {

            Intent intent = new Intent();
            intent.setComponent(new ComponentName("com.heytap.openid", "com.heytap.openid.IdentifyService"));
            intent.setAction("action.com.heytap.openid.OPEN_ID_SERVICE");

            if (context.bindService(intent, _service, Service.BIND_AUTO_CREATE)) {
//                synchronized (_lockObj) {
//                    try {
//                        _lockObj.wait(3000);
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                    }
//                }
            }

            if (_openId == null) {
                id = "";
            } else {
                try {
                    id = getId_internal(context, str);
                } catch (RemoteException e) {
                    id = "";
                }
            }
        } else {
            try {
                id = getId_internal(context, str);
            } catch (RemoteException e) {
                id = "";
            }
        }

        return id;
    }

    // public boolean a(Context context)
    public boolean isPkgValid(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo("com.heytap.openid", 0);
            return VERSION.SDK_INT >= Build.VERSION_CODES.P ? packageInfo != null && packageInfo.getLongVersionCode() >= 1 : packageInfo != null && packageInfo.versionCode >= 1;
        } catch (NameNotFoundException e) {
            return false;
        }
    }

    // public final String b(Context context, String str)
    public final String getId_internal(Context context, String str) throws RemoteException {

        if (TextUtils.isEmpty(_pkgName)) {
            _pkgName = context.getPackageName();
        }

        if (TextUtils.isEmpty(_signature)) {

            String signature = null;

            try {
                PackageInfo info = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);

                byte[] cert = info.signatures[0].toByteArray();

                MessageDigest md = MessageDigest.getInstance("SHA1");
                byte[] digest = md.digest(cert);
                StringBuilder stringBuilder = new StringBuilder();

                for (byte b : digest) {
                    stringBuilder.append(Integer.toHexString((b & 255) | 256).substring(1, 3));
                }

                signature = stringBuilder.toString();

            } catch (Exception e) {
            }

            _signature = signature;

        }

        String id = _openId.getId(_pkgName, _signature, str);
        return TextUtils.isEmpty(id) ? "" : id;
    }


}
