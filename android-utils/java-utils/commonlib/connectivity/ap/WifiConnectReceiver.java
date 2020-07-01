package com.auric.intell.commonlib.connectivity.ap;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.text.TextUtils;

import com.auric.intell.commonlib.utils.LogUtils;


/**
 * Created by white on 2017/10/10.
 */

public class WifiConnectReceiver extends BroadcastReceiver {
    protected static final String TAG = "WifiConnectReceiver";

    private WifiConnectStateListener mListener;

    private NetworkInfo.State mPreNetworkState;
    private int mCurWifiState;  // 当前wifi状态
    private String mSpecialSSID;

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        LogUtils.d("action:"+action);
        WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        String curSSID = null;
        if (wifiInfo != null) {
            curSSID = WifiBase.filterIllegalForSSID(wifiInfo.getSSID());
            LogUtils.d("SSID:"+curSSID);
        }

        // 网络变化广播
        if (action.equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
            NetworkInfo networkInfo = intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
            doHandleNetwordChange(networkInfo, curSSID);
        }
        // Wifi 开启状态广播
        else if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
            mCurWifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);
            doHandleWifiChange();
        }
        // TODO: 2017/10/11 目前不使用supplicant广播来判断是否连上网
        // 请求连接wifi状态广播
        else if (action.equals(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION)) {
            SupplicantState state = intent.getParcelableExtra(WifiManager.EXTRA_NEW_STATE);
            LogUtils.d("SupplicantState：" + state);
            int error = intent.getIntExtra(WifiManager.EXTRA_SUPPLICANT_ERROR, 0);
            switch (error) {
                case WifiManager.ERROR_AUTHENTICATING:
                    LogUtils.d("密码认证错误Code为："+error);
                    if(mListener != null){
                        mListener.onAuthenFail(curSSID, error);
                    }
                    break;
                default:
                    break;
            }

        }
    }

    /**
     * 处理wifi开关发生变化
     */
    private void doHandleWifiChange() {
        switch (mCurWifiState) {
            case WifiManager.WIFI_STATE_ENABLING:
                LogUtils.d("wifi正在启用");
                if (mListener != null) {
                    mListener.onEnabling();
                }
                break;
            case WifiManager.WIFI_STATE_ENABLED:
                LogUtils.d("Wi-Fi已启用。");
                if (mListener != null) {
                    mListener.onEnabled();
                }
                break;
            case WifiManager.WIFI_STATE_DISABLING:
                LogUtils.d("Wi-Fi正在关闭。");
                if (mListener != null) {
                    mListener.onDisabed();
                }
                break;
            case WifiManager.WIFI_STATE_DISABLED:
                LogUtils.d("Wi-Fi已关闭。");
                if (mListener != null) {
                    mListener.onDisabed();
                }
                break;
        }
    }

    /**
     * 处理网络发生变化
     * @param networkInfo
     * @param curSSID
     */
    private void doHandleNetwordChange(NetworkInfo networkInfo, String curSSID) {
        LogUtils.d(""+networkInfo.getDetailedState());
        if (networkInfo == null || networkInfo.getState() == mPreNetworkState) {
            return;
        }
        LogUtils.e("Wifi Test doHandleNetwordChange() start ");
        LogUtils.e("Wifi Test networkInfo.getDetailedState() =  " + networkInfo.getDetailedState());
        // 是否指定SSID
        boolean hasSpecialSSID = !TextUtils.isEmpty(mSpecialSSID);
        // 是否匹配到指定SSID
        boolean isMatchSpecialSSID = hasSpecialSSID && TextUtils.equals(mSpecialSSID, curSSID);
        LogUtils.d("hasSpecialSSID "+hasSpecialSSID);
        LogUtils.d("isMatchSpecialSSID "+isMatchSpecialSSID);
        switch (networkInfo.getState()) {
            case DISCONNECTED:
                if (mListener != null) {
                    if (!hasSpecialSSID || isMatchSpecialSSID) {
                        // wifi已经启动之后才算有效
                        mListener.onDisconnect(curSSID);
//                        if (mCurWifiState == WifiManager.WIFI_STATE_ENABLED) {
//                            mListener.onDisconnect(curSSID);
//                        }
                    }
                }
                break;
            case CONNECTING:
                if (mListener != null) {
                    if (!hasSpecialSSID || isMatchSpecialSSID) {
                        mListener.onConnecting(curSSID);
                    } else {
                        mListener.onConnecting(curSSID);
                    }
                }
                break;
            case CONNECTED:
                if(mListener != null){
                    if (!hasSpecialSSID || isMatchSpecialSSID) {
                        mListener.onConnected(curSSID);
                    } else {
                        mListener.onConnected(curSSID);
                    }
                }
                break;
        }

        mPreNetworkState = networkInfo.getState();
    }



    public  interface WifiConnectStateListener {
        void onDisabed();
        void onEnabling();
        void onEnabled();
        void onAuthenFail(String SSID, int error);
        void onConnecting(String SSID);
        void onConnected(String SSID);
        void onDisconnect(String SSID);

    }

    /**
     * 设置监听器，系统广播转化回调
     * @param listener
     */
    public void setWifiConnectStateListener(WifiConnectStateListener listener){
        mListener = listener;
    }

    /**
     * 设置指定连接的SSID，只会回调指定的SSID，广播器会过滤无关SSID
     * @param SSID
     */
    public void setSpecialSSID(String SSID) {
        mSpecialSSID = SSID;
    }



}
