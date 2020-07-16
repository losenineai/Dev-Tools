package com.dx.mobile.risk.supplier.huawei;

import android.content.Context;
import android.content.Intent;

import com.uodis.opendevice.aidl.OpenDeviceIdentifierService;

public class AdvertisingIdClient {

    public static final class Info {

        private final String advertisingId;
        private final boolean limitAdTrackingEnabled;

        Info(String paramString, boolean paramBoolean) {
            this.advertisingId = paramString;
            this.limitAdTrackingEnabled = paramBoolean;
        }

        public final String getId()
        {
            return this.advertisingId;
        }

        public final boolean isLimitAdTrackingEnabled()
        {
            return this.limitAdTrackingEnabled;
        }
    }

    public static Info getAdvertisingIdInfo(Context context) {

//        if (Looper.myLooper() == Looper.getMainLooper()) {
//            String str = "Cannot be called from the main thread";
//            Log.w(getTag(), str);
//            throw new IllegalStateException(str);
//        }

        String errMsg = "";

        try {
            context.getPackageManager().getPackageInfo("com.huawei.hwid", 0);
            HuaWeiServiceConn service = new HuaWeiServiceConn();

            Intent intent = new Intent("com.uodis.opendevice.OPENIDS_SERVICE");
            intent.setPackage("com.huawei.hwid");
            if (context.bindService(intent, service, Context.BIND_AUTO_CREATE)) {
//                Log.i(getTag(), "bind ok");
                try {

                    if (service.state) {
                        return null;
                    }
                    service.state = true;

                    OpenDeviceIdentifierService hwService = OpenDeviceIdentifierService.Stub.asInterface(service.binderQueue.take());
                    Info info = new Info(hwService.readString(), hwService.readBoolean());
                    context.unbindService(service);
                    return info;

                } catch (Throwable th) {
                    context.unbindService(service);
                }
            }
        } catch (Exception e3) {
        }

        return null;
    }

    private static String getTag() {
        return "white";
    }


}
