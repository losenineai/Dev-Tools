package com.auric.intell.commonlib.connectivity.ap;

import android.content.Context;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.text.TextUtils;
import android.util.Log;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.StringUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhangxiliang on 2017/6/21.
 */

public class WifiHelper {

    public static final int WIFI_REQUEST = 111;

    WifiManager mWifiManager;
    public static WifiInfo getCurrentWifiInfo(Context context){
        WifiManager wifiManager= (WifiManager) context.getSystemService(context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        return wifiInfo;
    }


    private WifiHelper(){
        mWifiManager=(WifiManager) ContextFinder.getApplication().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
    }

    public static WifiHelper getInstance(){
        return IntanceHolder.wifiHelper;
    }

    static class IntanceHolder{
        private static WifiHelper wifiHelper=new WifiHelper();
    }

   public List<ScanResult> getScanResultList(){
       mWifiManager.startScan();
       return getFilterScanResult(mWifiManager.getScanResults());

   }


    private List<ScanResult> getFilterScanResult(List<ScanResult> list)
    {
        LogUtils.d("list.size:"+list.size());
        List<ScanResult> newlist = new ArrayList<ScanResult>();
        for (ScanResult result : list)
        {
            if(!StringUtil.isEmpty(result.SSID) && !isExistSameSsid(newlist,result.SSID)){
                newlist.add(result);
            }
        }
        return newlist;
    }


    private boolean isExistSameSsid(List<ScanResult> list,String ssid){

        for(ScanResult  scanResult:list){
            if(ssid.equals(scanResult.SSID)){
                return true;
            }
        }
        return false;
    }




    public String getEncryptionType(ScanResult scanResult) {
        if (!TextUtils.isEmpty(scanResult.SSID)) {
            String capabilities = scanResult.capabilities;
            LogUtils.e( "[" + scanResult.SSID + "]" + capabilities);
            if (!TextUtils.isEmpty(capabilities)) {
                if (capabilities.contains("WPA") || capabilities.contains("wpa")) {
                    return "wpa";
                } else if (capabilities.contains("WEP")
                        || capabilities.contains("wep")) {
                    return "wep";
                } else {
                    return "no password";
                }
            }
        }

        return scanResult.capabilities.toString();
    }

    public  String getCurrentWifi() {
        int wifiState = mWifiManager.getWifiState();
        WifiInfo info = mWifiManager.getConnectionInfo();
        String ssid = info != null ? info.getSSID() : "null";
        Log.i("WIFI_LOG", "ssid = " + ssid);
        String state = "";
        switch (wifiState) {
            case 0:
                state = "WIFI_STATE_DISABLING";
                break;
            case 1:
                state = "WIFI_STATE_DISABLED";
                break;
            case 2:
                state = "WIFI_STATE_ENABLING";
                break;
            case 3:
                state = "WIFI_STATE_ENABLED";
                break;
            case 4:
                state = "WIFI_STATE_UNKNOWN";
                break;
            default:
                break;
        }
        return state;
    }

    public boolean getWiFiState() {
        if (mWifiManager.isWifiEnabled() & getCurrentWifi().contains("ENABL")) {
            return true;
        }
        if (!mWifiManager.isWifiEnabled() && getCurrentWifi().contains("DISABL")) {
            return false;
        }
        return true;
    }

/*    public static AlertDialog openWiFiSetting(final Context context) {
        AlertDialog alertDialog = DialogUtil.showDialog(context, "提示", "请先打开WiFi开关", "确定", "取消", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Intent intent = new Intent(Settings.ACTION_WIFI_SETTINGS);
                ((Activity)context).startActivityForResult(intent, WIFI_REQUEST);
            }
        }, null);
        return alertDialog;
    }*/


    public  WifiConfiguration setWifiParamsPassword(String SSID, String Password) {
        WifiConfiguration apConfig = new WifiConfiguration();
        apConfig.SSID = "\"" + SSID + "\"";
        apConfig.preSharedKey = "\"" + Password + "\"";
        apConfig.hiddenSSID = true;
        apConfig.status = WifiConfiguration.Status.ENABLED;
        apConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        apConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        apConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        apConfig.allowedPairwiseCiphers
                .set(WifiConfiguration.PairwiseCipher.TKIP);
        apConfig.allowedPairwiseCiphers
                .set(WifiConfiguration.PairwiseCipher.CCMP);
        apConfig.allowedProtocols.set(WifiConfiguration.Protocol.RSN);

        mWifiManager.enableNetwork(mWifiManager.addNetwork(apConfig), true);

        return apConfig;
    }

    public static  enum WifiEncType {
        WEP, WPA, OPEN
    }


    public boolean connectWifi(String targetSsid, String targetPsd, WifiEncType enc) {
        if (TextUtils.isEmpty(targetPsd)) {
            enc = WifiEncType.OPEN;
        }
        // 1、注意热点和密码均包含引号，此处需要需要转义引号
        String ssid = "\"" + targetSsid + "\"";
        String psd = "\"" + targetPsd + "\"";

        //2、配置wifi信息
        WifiConfiguration conf = new WifiConfiguration();
        conf.SSID = ssid;
        switch (enc) {
            case WEP:
                // 加密类型为WEP
                conf.wepKeys[0] = psd;
                conf.wepTxKeyIndex = 0;
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
                break;
            case WPA:
                // 加密类型为WPA
                conf.preSharedKey = psd;
                break;
            case OPEN:
                //开放网络
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.wepKeys[0] = "\"" + "\"";
                conf.wepTxKeyIndex = 0;
        }


        //3、链接wifi
        WifiManager wifiManager = (WifiManager) ContextFinder.getApplication().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        wifiManager.disconnect();
        int id = wifiManager.addNetwork(conf);
        LogUtils.e("netId:"  + id);
        if (id == -1) {
            return false;
        }
        List<WifiConfiguration> list = wifiManager.getConfiguredNetworks();
        if (list == null) {
            return false;
        }
        boolean result = false;
//        setNetWorkEnabled(false);  //先将所有网络设置为不可连接
        for (WifiConfiguration i : list) {
            if (i.SSID != null && i.SSID.equals(ssid)) {
//                wifiManager.disconnect();
                //避免由于Android6.0 版本上，当 wifi 与蜂窝数据同时存在时，当wifi无法使用时，系统会自动切换到蜂窝数据。这里强制使用WiFi连接
                /*if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    ConnectivityManager mConnectivityManager = (ConnectivityManager)ContextFinder.getApplication().getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
                    final NetworkRequest networkRequest = new NetworkRequest.Builder()
                            .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                            .addTransportType(NetworkCapabilities.TRANSPORT_WIFI)
                            .build();
                    mConnectivityManager.requestNetwork(networkRequest, new ConnectivityManager.NetworkCallback(){
                        @Override
                        public void onAvailable(Network network) {
                            String url = "";
                            try {
                                if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                                    HttpURLConnection conn = (HttpURLConnection) network.openConnection(new URL(url));
                                }
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    });
                }*/
                result = wifiManager.enableNetwork(i.networkId, true);
                wifiManager.saveConfiguration();
                LogUtils.e("wifi list size + " + list.size());
                LogUtils.e("ssid" + i.SSID + "我进到这里来了");
                LogUtils.e("networkId" + i.networkId + "我进到这里来了");
//                boolean reconnect = wifiManager.reconnect();
                LogUtils.e("enablenetwork = " + result);
//                LogUtils.e("reconnect = " + reconnect);
                break;
            }
        }

        return result;
    }

    public void setNetWorkEnabled(boolean enabled){
        List<WifiConfiguration> list = mWifiManager.getConfiguredNetworks();
        if (list == null) {
            return;
        }
        for (WifiConfiguration i: list) {
            if (!enabled) {
                mWifiManager.disableNetwork(i.networkId);
            } else {
//                if (!mWifiManager.isWifiEnabled()) {
                    mWifiManager.enableNetwork(i.networkId, true);
                    mWifiManager.setWifiEnabled(enabled);
//                }
            }
        }
    }

}
