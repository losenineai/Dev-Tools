package com.dx.mobile.risk.collect.hardware;

/**
 * Time: 2019-11-01
 * Author: blue
 * Description:
 */
public class USB {

    private static String usbState = "1";  // 1 未使用 2 使用中

    static {

        // 进程结束了广播还在监听，导致app被唤醒。此采集项暂时屏蔽 - blue 20190702
        /*if (!isRunUsbCheck) {

            isRunUsbCheck = true;

            usbBroadcastReceiver = new BroadcastReceiver() {

                @Override
                public void onReceive(Context context, Intent intent) {

                    String action = intent.getAction();

                    if (action.equals("android.hardware.usb.action.USB_STATE")) {

                        usbState = intent.getExtras().getBoolean("connected") ? "2" : "1";

                        try {
                            ContextFinder.getApplication().unregisterReceiver(usbBroadcastReceiver);
                            //LocalBroadcastManager.getInstance(ContextFinder.getApplication()).unregisterReceiver(usbBroadcastReceiver);
                        } catch (Exception e) {
                        }
                    }
                }
            };

            IntentFilter filter = new IntentFilter();
            filter.addAction("android.hardware.usb.action.USB_STATE");
            ContextFinder.getApplication().registerReceiver(usbBroadcastReceiver, filter);
            //LocalBroadcastManager.getInstance(ContextFinder.getApplication()).registerReceiver(usbBroadcastReceiver, filter);
        }*/

    }

    /**
     * 获得系统USB状态
     *
     * @return
     */
    public static String readUSBState() {
        return usbState;
    }

}
