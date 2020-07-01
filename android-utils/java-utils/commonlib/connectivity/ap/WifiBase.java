package com.auric.intell.commonlib.connectivity.ap;

import android.content.Context;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.text.TextUtils;

import com.auric.intell.commonlib.utils.LogUtils;

import java.lang.reflect.Method;

/**
 * @author white
 * @description：wifi基础类
 * @date 2017/9/7
 */

public class WifiBase {


    /**
     * wifi 密码加密方式
     */
    public enum WifiCipherType {
        NONE, WEP, WPA, OTHERS
    }

    protected Context mContext;
    protected WifiManager mWifiManager;
    protected WifiManager.WifiLock mWifiLock; // keep wifi wake when system screen locked

    public WifiBase(Context context) {
        mContext = context;
        mWifiManager = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
        mWifiLock = mWifiManager.createWifiLock(WifiManager.WIFI_MODE_SCAN_ONLY, mContext.getPackageName());
    }

    /**
     *wifi模式上锁
     */
    public void lockWifi(int lockType, String name) {
        mWifiLock = mWifiManager.createWifiLock(lockType,name);
        if(!mWifiLock.isHeld()){
            mWifiLock.acquire();
        }
    }

    /**
     *wifi模式解锁
     */
    public void releaseLockWifi() {
        if(mWifiLock != null && mWifiLock.isHeld()){
            mWifiLock.release();
        }
    }


    /**
     * 创建wifi配置类
     * @param SSID
     * @param password
     * @param chiperType
     * @return
     */
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

        if (chiperType == WifiCipherType.NONE) {
//            config.BSSID = null;                // 兼容连接无密码开放的wifi
            config.wepKeys[0] = "\"" + "\"";    // 兼容连接无密码开放的wifi
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
            LogUtils.e("config = NONE" + WifiCipherType.NONE);
//            config.hiddenSSID = false;
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


    /**
     * 过滤非法字符
     * @param SSID
     * @return
     */
    public static String filterIllegalForSSID(String SSID) {
        LogUtils.d("SSID:"+SSID);
        if (TextUtils.isEmpty(SSID)) {
            return SSID;
        }
        try {
            if (SSID != null && SSID.startsWith("\"") && SSID.endsWith("\"")) {
                SSID = SSID.substring(SSID.indexOf("\"") +1);
                SSID = SSID.substring(0, SSID.lastIndexOf("\""));
                LogUtils.d("after：" + SSID);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return SSID;

    }



    /**
     * ap是否已经开启
     * @return
     */
    public boolean isWifiAPEnabled() {
        try{
            Method method = mWifiManager.getClass().getDeclaredMethod("isWifiApEnabled");
            method.setAccessible(true);
            return (Boolean)method.invoke(mWifiManager);
        }catch(Exception err){
            err.printStackTrace();
        }
        return false;
    }


    /**
     * 开/关wifi ap
     * @param enable
     */
    public void setWifiApEnable(boolean enable) {
        if (isWifiAPEnabled() == enable) {
            return;
        }
        try{
            Method getConfigMethod = mWifiManager.getClass().getDeclaredMethod("getWifiApConfiguration");
            getConfigMethod.setAccessible(true);
            WifiConfiguration config = (WifiConfiguration)getConfigMethod.invoke(mWifiManager);

            Method setEnabledMethod = mWifiManager.getClass().getDeclaredMethod("setWifiApEnabled", WifiConfiguration.class, Boolean.TYPE);
            boolean result = (Boolean)setEnabledMethod.invoke(mWifiManager, config, enable);
            LogUtils.d("result:"+result);
            releaseLockWifi();
        }catch(Exception err){
            err.printStackTrace();
        }
    }

    /**
     * 开/关 wifi
     * @param enable
     */
    public void setWifiEnable(boolean enable) {
        if (mWifiManager.isWifiEnabled() == enable) {
            return;
        }

        boolean result = mWifiManager.setWifiEnabled(enable);
        LogUtils.d("result:"+result);
    }
}
