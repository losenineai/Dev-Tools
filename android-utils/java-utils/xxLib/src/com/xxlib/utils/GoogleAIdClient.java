package com.xxlib.utils;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Looper;
import android.os.Parcel;
import android.os.RemoteException;

import java.io.IOException;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Detect google advertising id
 */
public final class GoogleAIdClient {

    public static final class AdInfo {

        private final String mAdvertisingId;
        private final boolean mLimitAdTrackingEnabled;

        AdInfo(String advertisingId, boolean limitAdTrackingEnabled) {
            mAdvertisingId = advertisingId;
            mLimitAdTrackingEnabled = limitAdTrackingEnabled;
        }

        public String getId() {
            return mAdvertisingId;
        }

        public boolean isLimitAdTrackingEnabled() {
            return mLimitAdTrackingEnabled;
        }
    }

    public static AdInfo getAdvertisingIdInfo(Context context) throws Exception {
        if (Looper.myLooper() == Looper.getMainLooper())
            throw new IllegalStateException("Cannot be called from the main thread");

        try {
            PackageManager pm = context.getPackageManager();
            pm.getPackageInfo("com.android.vending", 0);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }

        AdvertisingConnection connection = new AdvertisingConnection();
        Intent intent = new Intent("com.google.android.gms.ads.identifier.service.START");
        intent.setPackage("com.google.android.gms");

        if (context.bindService(intent, connection, Context.BIND_AUTO_CREATE)) {
            try {
                AdvertisingInterface adInterface = new AdvertisingInterface(connection.getBinder());
                AdInfo adInfo = new AdInfo(adInterface.getId(),
                        adInterface.isLimitAdTrackingEnabled(true));
                return adInfo;
            } catch (Exception exception) {
                throw exception;
            } finally {
                context.unbindService(connection);
            }
        }
        throw new IOException("Google Play connection failed");
    }

    private static final class AdvertisingConnection implements ServiceConnection {

        boolean mRetrieved = false;

        private final LinkedBlockingQueue<IBinder> mQueue =
                new LinkedBlockingQueue<IBinder>(1);

        public void onServiceConnected(ComponentName name, IBinder service) {
            try {
                mQueue.put(service);
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }

        public void onServiceDisconnected(ComponentName name) {
        }

        public IBinder getBinder() throws InterruptedException {
            if (mRetrieved) {
                throw new IllegalStateException();
            }
            mRetrieved = true;
            return mQueue.take();
        }
    }

    private static final class AdvertisingInterface implements IInterface {

        private IBinder mBinder;

        public AdvertisingInterface(IBinder binder) {
            mBinder = binder;
        }

        public IBinder asBinder() {
            return mBinder;
        }

        public String getId() throws RemoteException {
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            String id;
            try {
                data.writeInterfaceToken("com.google.android.gms.ads.identifier.internal.IAdvertisingIdService");
                mBinder.transact(1, data, reply, 0);
                reply.readException();
                id = reply.readString();
            } finally {
                reply.recycle();
                data.recycle();
            }
            return id;
        }

        public boolean isLimitAdTrackingEnabled(boolean paramBoolean)
                throws RemoteException {
            Parcel data = Parcel.obtain();
            Parcel reply = Parcel.obtain();
            boolean limitAdTracking;
            try {
                data.writeInterfaceToken("com.google.android.gms.ads.identifier.internal.IAdvertisingIdService");
                data.writeInt(paramBoolean ? 1 : 0);
                mBinder.transact(2, data, reply, 0);
                reply.readException();
                limitAdTracking = 0 != reply.readInt();
            } finally {
                reply.recycle();
                data.recycle();
            }
            return limitAdTracking;
        }
    }
}