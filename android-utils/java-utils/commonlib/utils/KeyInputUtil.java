package com.auric.intell.commonlib.utils;

import android.app.Instrumentation;
import android.view.KeyEvent;

/**
 * @author white
 * @description：按键输入工具
 * @date 2016/10/26
 */

public class KeyInputUtil {

    private static final String TAG = "KeyInputUtil";

    /**
     * 模拟返回键
     * need permission{android.permission.INJECT_EVENTS}
     */
    public static void sendKeyBack(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                Instrumentation instrumentation = new Instrumentation();
                instrumentation.sendKeyDownUpSync(KeyEvent.KEYCODE_BACK);
                LogTool.d(TAG, "sendKeyBack");
            }
        }).start();
    }

    /**
     * 模拟Home键
     * need permission{android.permission.INJECT_EVENTS}
     */
    public static void sendKeyHome(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                Instrumentation instrumentation = new Instrumentation();
                instrumentation.sendKeyDownUpSync(KeyEvent.KEYCODE_HOME);
                LogTool.d(TAG, "sendKeyHome");
            }
        }).start();
    }

}
