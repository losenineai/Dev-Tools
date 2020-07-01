package com.auric.intell.commonlib.connectivity.ap;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;

import com.auric.intell.commonlib.utils.LogUtils;

import java.lang.reflect.Method;

/**
 * @author white
 * @description：Wifi AP连接类
 * @date 2017/9/5
 */

public class WifiApConnector extends WifiBase {

    public static final String WIFI_AP_STATE_CHANGED_ACTION = "android.net.wifi.WIFI_AP_STATE_CHANGED";
    public static final String EXTRA_WIFI_AP_STATE = "wifi_state";
    public static int WIFI_AP_STATE_DISABLING;
    public static int WIFI_AP_STATE_DISABLED;
    public static int WIFI_AP_STATE_ENABLING;
    public static int WIFI_AP_STATE_ENABLED;
    public static int WIFI_AP_STATE_FAILED;

    /**
     * 利用反射初始化AP状态值
     */
    static {
        try {
            WIFI_AP_STATE_DISABLING = WifiManager.class.getDeclaredField("WIFI_AP_STATE_DISABLING").getInt(null);
            WIFI_AP_STATE_DISABLED = WifiManager.class.getDeclaredField("WIFI_AP_STATE_DISABLED").getInt(null);
            WIFI_AP_STATE_ENABLING = WifiManager.class.getDeclaredField("WIFI_AP_STATE_ENABLING").getInt(null);
            WIFI_AP_STATE_ENABLED = WifiManager.class.getDeclaredField("WIFI_AP_STATE_ENABLED").getInt(null);
            WIFI_AP_STATE_FAILED = WifiManager.class.getDeclaredField("WIFI_AP_STATE_FAILED").getInt(null);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private WifiApStateListener mListener;



    public WifiApConnector(Context context) {
        super(context);
    }

    /**
     * 配置AP并且打开
     *
     * @param SSID
     * @param password
     */
    public void openWifiAp(String SSID, String password, WifiBase.WifiCipherType cipherType, WifiApStateListener listener) {
        LogUtils.d("SSID:"+SSID);
        initReceiver();
        mListener = listener;

        // 配置AP信息
        WifiConfiguration config = createWifiConfig(SSID, password, cipherType);
//        mWifiManager.addNetwork(config);
        boolean result = setWifiAPConfiguration(config);
        LogUtils.d("setAPConfig result:" + result);

        setWifiEnable(false);
        setWifiApEnable(true);
        // 保持wifi ap活跃
        lockWifi(WifiManager.WIFI_MODE_SCAN_ONLY, SSID);
    }

    /**
     * 创建wifi配置类
     * 注意：重写BaseWifi类的方法，为了解决创建开放wifi，如果没有设置BSSID会导致其他设备搜索不到创建的AP
     * @param SSID
     * @param password
     * @param chiperType
     * @return
     */
    @Override
    protected WifiConfiguration createWifiConfig(String SSID, String password, WifiCipherType chiperType) {
        WifiConfiguration config = new WifiConfiguration();

        config.allowedAuthAlgorithms.clear();
        config.allowedGroupCiphers.clear();
        config.allowedKeyManagement.clear();
        config.allowedPairwiseCiphers.clear();
        config.allowedProtocols.clear();
        config.priority = 1;

//        config.SSID = "\"" + SSID + "\"";
        config.SSID = SSID;
        config.BSSID = SSID;

        if (chiperType == WifiCipherType.NONE) {
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
            config.hiddenSSID = false;
        } else if (chiperType == WifiCipherType.WEP) {
            config.preSharedKey = "\"" + password + "\"";
            config.hiddenSSID = false;
            config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
        } else if (chiperType == WifiCipherType.WPA) {
            config.preSharedKey = "\"" + password + "\"";
            config.hiddenSSID = false;
            config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);

            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);

            config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
            config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
            config.status = WifiConfiguration.Status.ENABLED;
        } else {
            return null;
        }

        return config;
    }


    public void release() {
        releaseLockWifi();
        try {
            mContext.unregisterReceiver(mApStateReceiver);
        } catch (Exception e) {
        }
    }


    private void initReceiver() {
        try {
            IntentFilter intentFilter = new IntentFilter(WIFI_AP_STATE_CHANGED_ACTION);
            mContext.registerReceiver(mApStateReceiver, intentFilter);
        } catch (Exception e) {
            mContext.unregisterReceiver(mApStateReceiver);
            IntentFilter intentFilter = new IntentFilter(WIFI_AP_STATE_CHANGED_ACTION);
            mContext.registerReceiver(mApStateReceiver, intentFilter);
        }
    }


    /**
     * 设置wifi ap配置表
     * @param config
     * @return
     */
    private boolean setWifiAPConfiguration(WifiConfiguration config){
        if(config == null){
            return false;
        }
        try{
            Class<?> wifiClass = mWifiManager.getClass();
            Method setMethod = wifiClass.getDeclaredMethod("setWifiApConfiguration", config.getClass());
            return (Boolean)setMethod.invoke(mWifiManager, config);
        }catch(Exception err){
            err.printStackTrace();
            return false;
        }
    }


    /**
     * ap状态广播接收器
     */
    private BroadcastReceiver mApStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            int state = intent.getIntExtra(EXTRA_WIFI_AP_STATE, 0);
            LogUtils.d("action:" + intent.getAction() + " state:" + state);
            if (mListener != null) {
                mListener.onStateChange(state);
            }
        }
    };


    /**
     * ap状态回调接口
     */
    public interface WifiApStateListener{
        void onStateChange(int state);
    }

}
