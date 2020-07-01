package com.auric.intell.commonlib.network.wifi;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.ScanResult;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.CountDownTimer;
import android.util.Log;

import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.ReflectUtil;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.List;


import static android.net.wifi.WifiManager.EXTRA_NEW_STATE;
import static android.net.wifi.WifiManager.WIFI_MODE_SCAN_ONLY;
import static com.auric.intell.commonlib.utils.StringHasChinese.isChinese;

public class WifiConnector {
    private static final String TAG = "WifiConnector";
    private static final int WIFI_CONNECT_TIMEOUT = 10; //连接WIFI的超时时间
    private Context mContext;
    private WifiManager mWifiManager;
    private WiFiConncetReceiver mWifiConnectReceiver;
    private WifiConnectListener mWifiConnectListener;
    private WifiManager.WifiLock mWifiLock;
    //private boolean mIsConnnected = false;
    private int mNetworkID = -1;

    public WifiConnector(Context context, WifiConnectListener listener) {
        mContext = context;
        //mLock = new ReentrantLock();
        //mCondition = mLock.newCondition();
        mWifiManager = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
        mWifiConnectReceiver = new WiFiConncetReceiver();
        mWifiConnectListener = listener;
    }

    public void reConnect( String ssid, String password, WifiCipherType mode){
        mWifiManager.disconnect();
        connect(ssid,password,mode);
    }

    public void connect(final String ssid, final String password, final WifiCipherType mode) {
        lockWifi(WIFI_MODE_SCAN_ONLY,TAG);
        removeNetwork(ssid);
        new Thread(new Runnable() {
            @Override
            public void run() {
                //如果WIFI没有打开，则打开WIFI
                if (!mWifiManager.isWifiEnabled()) {
                    mWifiManager.setWifiEnabled(true);
                }

                //连接指定SSID
                try {
                    Thread.sleep(800);
                    List<ScanResult> scanResults = (List<ScanResult>) mWifiManager.getScanResults();
                    for (int i = 0; i < scanResults.size(); i++) {
                        ScanResult scanResult = scanResults.get(i);
                        String _ssid = scanResult.SSID;
                        String bssid = scanResult.BSSID;
                        String capabilities = scanResult.capabilities;
                        WifiCipherType mode;
                        if (capabilities.contains("WPA") || capabilities.contains("wpa")) {
                            mode = WifiCipherType.WIFICIPHER_WPA;
                        } else if (capabilities.contains("WEP") || capabilities.contains("wep"))
                            mode = WifiCipherType.WIFICIPHER_WEP;
                        else
                            mode = WifiCipherType.WIFICIPHER_NOPASS;
                        LogUtils.d("wifiConnector1" + _ssid);
                        LogUtils.d("wifiConnector2" + scanResult.BSSID);
                        LogUtils.d("wifiConnector4" + scanResult.capabilities);

                        if (_ssid.equals(ssid)) {
                            if (!onConnectbyBssid(_ssid, bssid, password, mode)) {
                                mWifiConnectListener.OnWifiConnectCompleted(false);
                            } else {
                                mWifiConnectListener.OnWifiConnectCompleted(true);
                            }
                            releaseLockWifi();
                            return;
                        }
                    }
                    mWifiConnectListener.OnWifiConnectCompleted(false);
                    releaseLockWifi();

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    public void connect(final int networkID) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                //如果WIFI没有打开，则打开WIFI
                if (!mWifiManager.isWifiEnabled()) {
                    mWifiManager.setWifiEnabled(true);
                }
                //注册连接结果监听对象
                //mContext.registerReceiver(mWifiConnectReceiver, new IntentFilter(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION));
                //连接指定SSID
                if (!onConnect(networkID)) {
                    mWifiConnectListener.OnWifiConnectCompleted(false);
                } else {
                    mWifiConnectListener.OnWifiConnectCompleted(true);
                }
                //删除注册的监听类对象
                //if(mWifiConnectReceiver!=null)
                //    mContext.unregisterReceiver(mWifiConnectReceiver);
            }
        }).start();
    }

    protected boolean onConnect(String ssid, String password, WifiCipherType mode) {
        boolean isoperSuccess = false;
        //添加新的网络配置
        WifiConfiguration cfg = createWifiConfig(ssid, password, mode);

        if (cfg != null) {
            //添加网络配置
            mNetworkID = mWifiManager.addNetwork(cfg);
            //连接该网络
            if (mNetworkID != -1) {
                if (mWifiManager.enableNetwork(mNetworkID, true)) {
                    isoperSuccess = true;
                } else {
                    isoperSuccess = false;
                }
            }
        }
        return isoperSuccess;
    }
    /**
    *wifi模式上锁
    */
    public void lockWifi(int lockType,String name) {
        if(mWifiLock == null){
            mWifiLock = mWifiManager.createWifiLock(lockType,name);
        }
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

    private void removeNetwork(String SSID){
        WifiManager wifiManager = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
        List<WifiConfiguration> config = (List<WifiConfiguration>) wifiManager.getConfiguredNetworks();
        for(int i = 0; i < config.size(); i++){
            String ssid=config.get(i).SSID;
            if(ssid!=null&&ssid.startsWith("\"")&&ssid.endsWith("\""))
            {
                ssid=ssid.substring(ssid.indexOf("\"")+1);
                ssid=ssid.substring(0,ssid.lastIndexOf("\""));
            }
            if(SSID.equals(ssid))
                wifiManager.removeNetwork(config.get(i).networkId);
            LogUtils.d("wifiU6:"+config.get(i).SSID);
        }
        wifiManager.saveConfiguration();
    }

    protected boolean onConnectbyBssid(String ssid, String bssid, String password, WifiCipherType mode) {
        LogUtils.d("wifiConnector3" + bssid);
        boolean isoperSuccess = false;
        //添加新的网络配置
        WifiConfiguration cfg = createWifiConfig_BSSID(ssid, bssid, password, mode);

        if (cfg != null) {
            //添加网络配置
            mNetworkID = mWifiManager.addNetwork(cfg);
            //连接该网络
            if (mNetworkID != -1) {
                if (mWifiManager.enableNetwork(mNetworkID, true)) {
                    isoperSuccess = true;
                } else {
                    isoperSuccess = false;
                }
            }
        }
        return isoperSuccess;
    }

    protected boolean onConnect(int networkID) {
        mNetworkID = networkID;
        boolean isOperSuccess;
        //mLock.lock();
        //mIsConnnected = false;
        //连接该网络
        if (mWifiManager.enableNetwork(mNetworkID, true)) {
            // mLock.unlock();
            isOperSuccess = true;
        } else {
            isOperSuccess = false;
        }
        return isOperSuccess;
    }

    public void removeWIFINetWorkID() {
        if (mNetworkID != -1) {
            mWifiManager.removeNetwork(mNetworkID);
        }
    }

    public void removeWIFINetWorkID(int networkID) {
        if (mNetworkID != -1) {
            mWifiManager.removeNetwork(networkID);
        }
    }

    public void disableWIFINetWorkID() {
        if (mNetworkID != -1) {
            mWifiManager.disableNetwork(mNetworkID);
        }

    }

    public void disableWIFINetWorkID(int networkID) {
        if (mNetworkID != -1) {
            mWifiManager.disableNetwork(mNetworkID);
        }
    }

    public WifiInfo getConnectNetworkInfo() {

        return mWifiManager.getConnectionInfo();

    }

    public void removeWiFiConfig() {
        List<WifiConfiguration> wifiConfigurations = mWifiManager.getConfiguredNetworks();
        if (wifiConfigurations != null) {
            for (WifiConfiguration wifiConfiguration : wifiConfigurations) {
                Log.d(TAG, wifiConfiguration.SSID);
                mWifiManager.removeNetwork(wifiConfiguration.networkId);
                mWifiManager.saveConfiguration();
            }
        }
    }

    public void removeWiFiConfigBySSID(String ssid) {
        List<WifiConfiguration> wifiConfigurations = mWifiManager.getConfiguredNetworks();
        if (wifiConfigurations != null) {
            for (WifiConfiguration wifiConfiguration : wifiConfigurations) {
                Log.d(TAG, wifiConfiguration.SSID);
                if (wifiConfiguration.SSID.replace("\"", "").equals(ssid.replace("\"", ""))) {
                    boolean operResult = mWifiManager.removeNetwork(wifiConfiguration.networkId);
                    Log.d(TAG, "operResult ==" + operResult);
                    mWifiManager.saveConfiguration();
                }
            }
        }

    }

    private WifiConfiguration createWifiConfig(String SSID, String Password,
                                               WifiCipherType Type) {
        WifiConfiguration config = new WifiConfiguration();

        config.allowedAuthAlgorithms.clear();
        config.allowedGroupCiphers.clear();
        config.allowedKeyManagement.clear();
        config.allowedPairwiseCiphers.clear();
        config.allowedProtocols.clear();
        config.priority = 1;

        config.SSID = "\"" + SSID + "\"";

        if (Type == WifiCipherType.WIFICIPHER_NOPASS) {
            // config.wepKeys[0] = "";
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
            config.hiddenSSID = true;
        } else if (Type == WifiCipherType.WIFICIPHER_WEP) {
            config.preSharedKey = "\"" + Password + "\"";
            config.hiddenSSID = true;
            config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
        } else if (Type == WifiCipherType.WIFICIPHER_WPA) {
            config.preSharedKey = "\"" + Password + "\"";
            config.hiddenSSID = true;
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

    private WifiConfiguration createWifiConfig_BSSID(String ssid, String BSSID, String Password,
                                                     WifiCipherType Type) {
        WifiConfiguration config = new WifiConfiguration();

        config.allowedAuthAlgorithms.clear();
        config.allowedGroupCiphers.clear();
        config.allowedKeyManagement.clear();
        config.allowedPairwiseCiphers.clear();
        config.allowedProtocols.clear();
        config.priority = 1;
        config.SSID = "\"" + ssid + "\"";
        config.BSSID = BSSID;

        if (Type == WifiCipherType.WIFICIPHER_NOPASS) {
            // config.wepKeys[0] = "";
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
            config.hiddenSSID = true;
        } else if (Type == WifiCipherType.WIFICIPHER_WEP) {
            config.preSharedKey = "\"" + Password + "\"";
            config.hiddenSSID = true;
            config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            config.wepTxKeyIndex = 0;
        } else if (Type == WifiCipherType.WIFICIPHER_WPA) {
            config.preSharedKey = "\"" + Password + "\"";
            config.hiddenSSID = true;
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

    public List<WifiConfiguration> getWifiConfiguration() {
        return mWifiManager.getConfiguredNetworks();
    }

    /**
     * 此wifi热点是否framework自动添加
     * 参考源码: WifiSettings
     *
     * @param configuration
     * @return
     */
    public static boolean isSysAutoAdd(WifiConfiguration configuration) {
        if (configuration == null) {
            return false;
        }
        boolean selfAdded = (boolean) ReflectUtil.getFieldValue(configuration, "selfAdded");
        int numAssociation = (int) ReflectUtil.getFieldValue(configuration, "numAssociation");
        LogTool.d(TAG, "isSysAutoAdd selfAdded:" + selfAdded + " numAssociation:" + numAssociation);
        if (selfAdded && numAssociation == 0) {
            return true;
        }
        return false;
    }

    //网络加密模式
    public enum WifiCipherType {
        WIFICIPHER_WEP, WIFICIPHER_WPA, WIFICIPHER_NOPASS, WIFICIPHER_INVALID
    }

    //通知连接结果的监听接口
    public interface WifiConnectListener {
        void OnWifiConnectCompleted(boolean isConnected);

        void OnAPToWifiCompleted(boolean wifiEnabled);
    }

    //监听系统的WIFI连接消息
    protected class WiFiConncetReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (!WifiManager.SUPPLICANT_STATE_CHANGED_ACTION.equals(intent.getAction())) {
                return;
            }
            //mLock.lock();
            SupplicantState supplicantState = intent.getParcelableExtra(EXTRA_NEW_STATE);

            WifiInfo info = mWifiManager.getConnectionInfo();

            if (info.getNetworkId() == mNetworkID && supplicantState == SupplicantState.COMPLETED) {
                //mIsConnnected = true;
                // mCondition.signalAll();
            }
            //mLock.unlock();
        }
    }

    public void apToWifi() {
        WifiConfiguration apConfig = null;
        final Method method;
        try {
            method = mWifiManager.getClass().getMethod("setWifiApEnabled", WifiConfiguration.class, Boolean.TYPE);
            method.invoke(mWifiManager, apConfig, false);
            mWifiManager.setWifiEnabled(true);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
//        new Thread(new Runnable() {
//            int maxtimes=100,times=0;
//            @Override
//            public void run() {
//                try {
//                    while(times<=maxtimes) {
//                        times++;
//                        Thread.sleep(100);
//                        if (mWifiManager.isWifiEnabled()) {
//                            mWifiConnectListener.OnAPToWifiCompleted(true);
//                            return;
//                        }
//                    }
//                    mWifiConnectListener.OnAPToWifiCompleted(false);
//
//                } catch (InterruptedException e) {
//                    e.printStackTrace();
//                }
//
//            }
//        }).start();
    }
    public void wifiToAP(String ssid){
        Method method1 = null;
        Log.d(TAG,"wifiToAP");

        if (mWifiManager.isWifiEnabled()) {
            mWifiManager.setWifiEnabled(false);
        }

        try {
            method1 = mWifiManager.getClass().getMethod("setWifiApEnabled",
                    WifiConfiguration.class, boolean.class);
            WifiConfiguration netConfig = new WifiConfiguration();
            netConfig.allowedAuthAlgorithms.clear();
            netConfig.allowedGroupCiphers.clear();
            netConfig.allowedKeyManagement.clear();
            netConfig.allowedPairwiseCiphers.clear();
            netConfig.allowedProtocols.clear();
            netConfig.SSID = ssid;
            netConfig.wepTxKeyIndex = 0;
            netConfig.allowedKeyManagement
                    .set(WifiConfiguration.KeyMgmt.NONE);

            method1.invoke(mWifiManager, netConfig, true);

        } catch (IllegalArgumentException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (SecurityException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void wifiToAP(String ssid, String password){
        Method method1 = null;
        Log.d(TAG,"wifiToAP");

        if (mWifiManager.isWifiEnabled()) {
            mWifiManager.setWifiEnabled(false);
        }

        try {
            method1 = mWifiManager.getClass().getMethod("setWifiApEnabled",
                    WifiConfiguration.class, boolean.class);
            WifiConfiguration netConfig = new WifiConfiguration();
            netConfig.allowedAuthAlgorithms.clear();
            netConfig.allowedGroupCiphers.clear();
            netConfig.allowedKeyManagement.clear();
            netConfig.allowedPairwiseCiphers.clear();
            netConfig.allowedProtocols.clear();
            netConfig.SSID = ssid;
            netConfig.preSharedKey = password;

            netConfig.allowedAuthAlgorithms
                    .set(WifiConfiguration.AuthAlgorithm.OPEN);
            netConfig.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
            netConfig.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
            netConfig.allowedKeyManagement
                    .set(WifiConfiguration.KeyMgmt.WPA_PSK);
            netConfig.allowedPairwiseCiphers
                    .set(WifiConfiguration.PairwiseCipher.CCMP);
            netConfig.allowedPairwiseCiphers
                    .set(WifiConfiguration.PairwiseCipher.TKIP);
            netConfig.allowedGroupCiphers
                    .set(WifiConfiguration.GroupCipher.CCMP);
            netConfig.allowedGroupCiphers
                    .set(WifiConfiguration.GroupCipher.TKIP);

            method1.invoke(mWifiManager, netConfig, true);

        } catch (IllegalArgumentException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (SecurityException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public List<ScanResult> getAvaliableWifi() {
        List<ScanResult> scanResults = mWifiManager.getScanResults();

        return scanResults;
    }

}