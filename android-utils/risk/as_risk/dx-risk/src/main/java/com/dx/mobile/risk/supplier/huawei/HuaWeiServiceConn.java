package com.dx.mobile.risk.supplier.huawei;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;

import java.util.concurrent.LinkedBlockingQueue;

public final class HuaWeiServiceConn implements ServiceConnection {

    public boolean state = false;
    public final LinkedBlockingQueue<IBinder> binderQueue = new LinkedBlockingQueue(1);

    public final void onServiceConnected(ComponentName componentName, IBinder iBinder) {
        try {
            binderQueue.put(iBinder);
        } catch (InterruptedException e) {
        }
    }

    public final void onServiceDisconnected(ComponentName componentName) {
    }
}