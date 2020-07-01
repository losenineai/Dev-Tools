package com.auric.intell.commonlib.utils;

/**
 * 防止连续点击
 */
public class NoDoubleClickUtils {
    private static long lastClickTime;
    private final static int SPACE_TIME = 1000;

    public static void initLastClickTime() {
        lastClickTime = 0;
    }

    /**
     * 是否连续点击
     * @return ture ---- 连续点击
     */
    public synchronized static boolean isDoubleClick() {
        long currentTime = System.currentTimeMillis();
        boolean isClick2;
        // 2 次点击时间间隔小于 SPACE_TIME
        if (currentTime - lastClickTime >
                SPACE_TIME) {
            isClick2 = false;
        } else {
            isClick2 = true;
        }
        lastClickTime = currentTime;
        return isClick2;
    }
}