package com.xxlib.utils;

/**
 * ClassName:  GoogleAIdHelper
 * Desc: google advertise id helper
 * Created by xf on 16/2/19.
 */
public class GoogleAIdHelper {

    private static String mId = "";

    private static volatile boolean mHasLoad = false;

    public static String getID() {
        if (!mHasLoad) {
            mHasLoad = true;

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        GoogleAIdClient.AdInfo adInfo =
                                GoogleAIdClient.getAdvertisingIdInfo(ContextFinder.getApplication());
                        mId = adInfo.getId();
                    } catch (Exception e) {
                    }
                }
            }).start();
        }

        return mId;
    }
}
