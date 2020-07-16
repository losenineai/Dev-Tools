package com.dx.mobile.risk.supplier.asus;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;

import com.asus.msa.SupplementaryDID.IDidAidlInterface;
import com.asus.msa.sdid.ISupplementaryDIDListener;

public class SupplementaryDIDManager {

    public static final String TAG = "SupplementaryDIDManager";
    public Context _ctx;
    public IDidAidlInterface mDidService;
    public ISupplementaryDIDListener mListener;

    public ServiceConnection mServiceConnection = new ServiceConnection() {

        public void onServiceConnected(ComponentName name, IBinder service) {

            mDidService = IDidAidlInterface.Stub.asInterface(service);
            notifyAllListeners(true);

//            try {
//                Log.d(SupplementaryDIDManager.TAG, "UDID=" + mDidService.getUDID() +
//                                ", OAID=" + mDidService.getOAID() +
//                                ", VAID=" + mDidService.getVAID() +
//                                ", AAID=" + mDidService.getAAID());
//            } catch (RemoteException e) {
//                Log.d(SupplementaryDIDManager.TAG, e.toString());
//            }
        }

        public void onServiceDisconnected(ComponentName name) {
            notifyAllListeners(false);
        }
    };

    public SupplementaryDIDManager(Context context) {
        _ctx = context;
    }

    private void notifyAllListeners(boolean flag) {

        if (flag) {
            try {
                mListener.handle(mDidService);
                return;
            } catch (RemoteException e) {
//                Log.e(TAG, "notify did bind status error :" + e.getMessage());
                return;
            }
        }

        mListener.handle();
    }

    public void deInit() {

        if (mServiceConnection != null) {

            if (_ctx != null) {
                _ctx.unbindService(mServiceConnection);
            }
        }
    }

    public void init(ISupplementaryDIDListener listener) {
        try {

            mListener = listener;

            Intent intent = new Intent("com.asus.msa.action.ACCESS_DID");
            ComponentName name = new ComponentName("com.asus.msa.SupplementaryDID", "com.asus.msa.SupplementaryDID.SupplementaryDIDService");
            intent.setComponent(name);

//            Log.e(TAG, "start to bind did service");

            _ctx.bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);

        } catch (Exception e) {
            notifyAllListeners(false);
        }
    }
}
