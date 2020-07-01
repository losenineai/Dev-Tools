package com.auric.intell.commonlib.connectivity.ap;

import android.content.Context;
import android.content.IntentFilter;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.os.SystemClock;
import android.text.TextUtils;

import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.LogUtils;

import java.util.List;

import static android.net.wifi.WifiManager.WIFI_MODE_SCAN_ONLY;

/**
 * @author white
 * @description：
 * @date 2017/9/5
 */

public class WifiConnector extends WifiBase {

    private static final String TAG = "WifiConnector";
    private static final int TIME_OUT_ENABLE_WIFI = 8000;

    private WifiConnectReceiver mReceiver;

    public WifiConnector(Context context) {
        super(context);
    }

    /**
     * 连接wifi
     *
     * @param SSID
     * @param pwd
     * @param listener
     * @return
     */
    public void connectWifi(final String SSID, final String pwd, final WifiConnectReceiver.WifiConnectStateListener listener) {
        LogTool.d(TAG, "connectWifi SSID:"+SSID + " pwd:"+pwd);
        if (TextUtils.isEmpty(SSID)) {
            if (listener != null) {
                listener.onAuthenFail(SSID, -1);
            }
            return;
        }
        initReceiver();
        // 在扫描结果有的情况下，才去连接wifi
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    LogTool.d(TAG, "isWifiEnabled:" + mWifiManager.isWifiEnabled());

                    if (!mWifiManager.isWifiEnabled()) {
                        setWifiApEnable(false);
                        setWifiEnabled(true);
                    }
                    int enableWifiTime = 0;
                    while (!mWifiManager.isWifiEnabled()) {
                        LogTool.d(TAG, "connectWifi wifi not enable while ");
                        // 打开wifi超时
                        if (enableWifiTime > TIME_OUT_ENABLE_WIFI) {
                            listener.onAuthenFail(SSID, -2);
                            return;
                        }
                        enableWifiTime += 1000;
                        SystemClock.sleep(1000);
                    }

                    lockWifi(WIFI_MODE_SCAN_ONLY, TAG);
                    mWifiManager.startScan();
                    LogTool.d(TAG, "connectWifi wifi enable while end");

                    // 开始连接
                    WifiCipherType pwdType = WifiCipherType.WPA;
                    if (TextUtils.isEmpty(pwd)) {
                        pwdType = WifiCipherType.NONE;
                    }
                    doConnectWifi(SSID, pwd, pwdType, listener);

                } catch (Exception e) {
                    e.printStackTrace();
                    if (listener != null) {
                        listener.onAuthenFail(SSID, -1000);
                    }
                }

            }
        }).start();

    }


    private void doConnectWifi(String SSID, String pwd, WifiCipherType keyType, WifiConnectReceiver.WifiConnectStateListener listener) {
        LogTool.d(TAG, "doConnectWifi SSID:"+SSID+" pwd:"+pwd+" keyType:"+keyType);
        boolean addConfig = false;
        mWifiManager.setWifiEnabled(true);

        // 防止打开wifi之后，自动连接wifi
//        mWifiManager.disconnect();
//        removeNetwork(SSID);
        // 连接指定SSID，需要添加上引号才能连接
        WifiConfiguration config = createWifiConfig("\"" + SSID + "\"", pwd, keyType);
        int id = mWifiManager.addNetwork(config);
        LogTool.d(TAG, "doConnectWifi netId:" + id);
        // 连接
        if (id != -1) {
            addConfig = mWifiManager.enableNetwork(id, true);
            mWifiManager.saveConfiguration();
            if (addConfig) {
                initReceiver();
                mReceiver.setWifiConnectStateListener(listener);
                mReceiver.setSpecialSSID(SSID);
                return;
            }
        }
        LogTool.e(TAG, "doConnectWifi Wifi Test enableNetwork() fail ");
        listener.onAuthenFail(SSID, -100);

    }

    public void setWifiEnabled(boolean enable) {
        mWifiManager.setWifiEnabled(enable);
    }

    public void release() {
        releaseLockWifi();
        try {
            mContext.unregisterReceiver(mReceiver);
        } catch (Exception e) {
        }
        mReceiver = null;
    }

    private void removeNetwork(String SSID){
        if (TextUtils.isEmpty(SSID)) {
            return;
        }
        List<WifiConfiguration> configList = mWifiManager.getConfiguredNetworks();
        if (configList == null) {
            return;
        }

        for (WifiConfiguration config : configList) {
            String ssid = filterIllegalForSSID(config.SSID);
            if (SSID.equals(ssid)) {
                mWifiManager.removeNetwork(config.networkId);
            }
        }
        mWifiManager.saveConfiguration();
    }

    private void disableAllNetwork() {
        List<WifiConfiguration> configList = mWifiManager.getConfiguredNetworks();
        if (configList == null) {
            return;
        }

        for (WifiConfiguration config : configList) {
            boolean result = mWifiManager.disableNetwork(config.networkId);
            LogTool.d(TAG, "SSID:"+config.SSID+" disable:"+result);
        }
    }




    private void initReceiver() {
        if (mReceiver != null) {
            return;
        }
        mReceiver = new WifiConnectReceiver();
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        intentFilter.addAction(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION);

        try {
            mContext.registerReceiver(mReceiver, intentFilter);
        } catch (Exception e) {
            mContext.unregisterReceiver(mReceiver);
            mContext.registerReceiver(mReceiver, intentFilter);
            e.printStackTrace();
        }
    }

    /**
     * 打开wifi
     * @param context
     * @param enable
     */
    public static void setWifiEnable(Context context, boolean enable) {
        if (context == null) {
            return;
        }
        WifiConnector wifiConnector = new WifiConnector(context);
        wifiConnector.setWifiApEnable(!enable);
        wifiConnector.setWifiEnable(enable);
    }

}
