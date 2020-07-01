package com.xxlib.utils;

/**
 * Created by jammy on 15/11/21.
 */
public class ThreadUtil {

    public static void sleepForDetectGap(int gapTime) {
        try {
            Thread.sleep(gapTime);
        } catch (Exception e) {
            e.printStackTrace();
            try {
                Thread.sleep(gapTime);
            } catch (Exception er) {
                er.printStackTrace();
            }
        }
    }
}
