package com.auric.intell.commonlib.network.wifi;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.auric.intell.commonlib.utils.LogTool;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;


public class WiFiUtil {
    private static final String TAG = "WiFiUtil";

    private WifiManager localWifiManager;//提供Wifi管理的各种主要API，主要包含wifi的扫描、建立连接、配置信息等
	private List<ScanResult> wifiScanList;//ScanResult用来描述已经检测出的接入点，包括接入的地址、名称、身份认证、频率、信号强度等
	private List<WifiConfiguration> wifiConfigList;//WIFIConfiguration描述WIFI的链接信息，包括SSID、SSID隐藏、password等的设置
	private WifiInfo wifiConnectedInfo;//已经建立好网络链接的信息
	private WifiLock wifiLock;//手机锁屏后，阻止WIFI也进入睡眠状态及WIFI的关闭
	private DhcpInfo dhcpInfo;

	public WiFiUtil(Context context) {
		localWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
		this.getConfiguration();
	}

	//检查WIFI状态
	public int WifiCheckState() {
		return localWifiManager.getWifiState();
	}

	//开启WIFI
	public void WifiOpen() {
		if (!localWifiManager.isWifiEnabled()) {
			localWifiManager.setWifiEnabled(true);
		}
	}

	//关闭WIFI
	public void WifiClose() {
		if (localWifiManager.isWifiEnabled()) {
			localWifiManager.setWifiEnabled(false);
		}

	}

	public int CreateWifiInfo(String SSID)
	{
		WifiConfiguration config = new WifiConfiguration();
		config.allowedAuthAlgorithms.clear();
		config.allowedGroupCiphers.clear();
		config.allowedKeyManagement.clear();
		config.allowedPairwiseCiphers.clear();
		config.allowedProtocols.clear();
		config.SSID = "\"" + SSID + "\"";

		WifiConfiguration tempConfig = this.IsExsits(SSID);
		if(tempConfig != null) {
			localWifiManager.removeNetwork(tempConfig.networkId);
		}
		//config.wepKeys[0] = "";
		config.hiddenSSID = true;
		config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
		//config.wepTxKeyIndex = 0;

		Log.d("DDDDDDDDDDDDD","config is null:"+(config==null));

		int id = localWifiManager.addNetwork(config);
		return id;
	}

	private WifiConfiguration IsExsits(String SSID)
	{
		List<WifiConfiguration> existingConfigs = localWifiManager.getConfiguredNetworks();
        if (existingConfigs != null) {
            for (WifiConfiguration existingConfig : existingConfigs){
                if (existingConfig.SSID.equals("\""+SSID+"\"")){
                    return existingConfig;
                }
            }
        }
		return null;
	}

	//扫描wifi
	public void WifiStartScan() {
		localWifiManager.startScan();
	}

	public boolean isWifiEnable(){

		return localWifiManager.isWifiEnabled();
	}

	//得到Scan结果
	public List<ScanResult> getScanResults() {
		return localWifiManager.getScanResults();//得到扫描结果
	}

	//Scan结果转为Sting
	public List<String> scanResultToString(List<ScanResult> list) {
		List<String> strReturnList = new ArrayList<String>();
		for (int i = 0; i < list.size(); i++) {
			ScanResult strScan = list.get(i);
			String str = strScan.toString();
			boolean bool = strReturnList.add(str);
			if (!bool) {
				Log.i("scanResultToSting", "Addfail");
			}
		}
		return strReturnList;
	}

	//得到Wifi配置好的信息
	public void getConfiguration() {
		wifiConfigList = localWifiManager.getConfiguredNetworks();//得到配置好的网络信息
		if (wifiConfigList!=null&&wifiConfigList.size()>0){
			for (int i = 0; i < wifiConfigList.size(); i++) {

			}
		}
	}

	//得到Wifi配置好的信息
	public WifiConfiguration getConfigurationBySSID(String ssid) {
		wifiConfigList = localWifiManager.getConfiguredNetworks();//得到配置好的网络信息
        if (wifiConfigList != null) {
            for(WifiConfiguration wifiConfiguration : wifiConfigList){
                    String thisSSID = wifiConfiguration.SSID;
                    if ((ssid.replaceAll("\"","")).equals(thisSSID.replaceAll("\"",""))) {
                        return wifiConfiguration;
                    }
            }
        }
		return null;
	}


	//判定指定WIFI是否已经配置好,依据WIFI的地址BSSID,返回NetId
	public int IsConfiguration(String SSID) {
		if (wifiConfigList!=null&&wifiConfigList.size()>0){
			for (int i = 0; i < wifiConfigList.size(); i++) {
				Log.i(wifiConfigList.get(i).SSID, String.valueOf(wifiConfigList.get(i).networkId));
				if (wifiConfigList.get(i).SSID.equals(SSID)) {//地址相同
					return wifiConfigList.get(i).networkId;
				}
			}
			return -1;
		}else {
			return  -1;
		}
	}

	//添加指定WIFI的配置信息,原列表存在此SSID
	public int AddWifiConfig(List<ScanResult> wifiList, String ssid, String pwd) {
		String ssid2 = ssid.replaceAll("\"","");
		int wifiId = -1;
		for (int i = 0; i < wifiList.size(); i++) {
			ScanResult wifi = wifiList.get(i);
			if (wifi.SSID.equals(ssid2)) {
				Log.i("AddWifiConfig", "equals");
				WifiConfiguration wifiCong = new WifiConfiguration();
				wifiCong.SSID = "\"" + wifi.SSID + "\"";//\"转义字符，代表"
				wifiCong.preSharedKey = "\"" + pwd + "\"";//WPA-PSK密码
				wifiCong.hiddenSSID = false;
				wifiCong.status = WifiConfiguration.Status.ENABLED;
				wifiId = localWifiManager.addNetwork(wifiCong);//将配置好的特定WIFI密码信息添加,添加完成后默认是不激活状态，成功返回ID，否则为-1
				if (wifiId != -1) {
					return wifiId;
				}
			}
		}
		return wifiId;
	}


	public void setConfiguration(WifiConfiguration configuration){
		if (localWifiManager!=null){
			localWifiManager.addNetwork(configuration);
		}
	}


	//连接指定Id的WIFI
	public boolean ConnectWifi(int wifiId) {
		if(wifiConfigList!=null && wifiConfigList.size() > 0){
			for (int i = 0; i < wifiConfigList.size(); i++) {
				WifiConfiguration wifi = wifiConfigList.get(i);
				if (wifi.networkId == wifiId) {
					boolean result = localWifiManager.enableNetwork(wifiId,true);
					if (!result) {//激活该Id，建立连接
						Log.i("ConnectWifi", String.valueOf(wifiConfigList.get(wifiId).status));//status:0--已经连接，1--不可连接，2--可以连接
					}
					return true;
				}
			}
		}
		return false;
	}

	public void ConnectWifiNoPass(int wifiId){
		Log.i("ConnectWifi", "no wifi id is:"+wifiId);
		localWifiManager.enableNetwork(wifiId,true);
	}

	//断开WiFi连接
	public void disConnectWifi() {
		localWifiManager.disconnect();
	}

	//创建一个WIFILock
	public void createWifiLock(String lockName) {
		wifiLock = localWifiManager.createWifiLock(lockName);
	}

	//锁定wifilock
	public void acquireWifiLock() {
		wifiLock.acquire();
	}

	//解锁WIFI
	public void releaseWifiLock() {
		if (wifiLock.isHeld()) {//判定是否锁定
			wifiLock.release();
		}
	}

	//得到建立连接的信息
	public WifiInfo getConnectedInfo() {
			wifiConnectedInfo = localWifiManager.getConnectionInfo();
			return wifiConnectedInfo;
	}

	public void getConnectedDhcpInfo() {
		dhcpInfo = localWifiManager.getDhcpInfo();
	}


	//得到连接的MAC地址
	public String getConnectedMacAddr() {
		return (wifiConnectedInfo == null) ? "NULL" : wifiConnectedInfo.getMacAddress();
	}

	//得到连接的名称SSID
	public String getConnectedSSID() {
		wifiConnectedInfo = getConnectedInfo();
		if(wifiConnectedInfo == null){
			return null;
		}else {
			String ssid=wifiConnectedInfo.getSSID();
			ssid=ssid.substring(1,ssid.length()-1);
			return ssid;
		}
	}

	//得到连接的ID
	public int getConnectedID() {
		return (wifiConnectedInfo == null) ? 0 : wifiConnectedInfo.getNetworkId();
	}

	//得到连接WiFi的信号强度
	public String getConnectedLevel() {
		int strength = WifiManager.calculateSignalLevel(wifiConnectedInfo.getRssi(), 5);
		if (strength == 5) {
			return "强";
		} else if (strength == 4) {
			return "较强";
		} else if (strength == 3) {
			return "中等";
		} else if (strength == 2) {
			return "较弱";
		} else if (strength == 1) {
			return "弱";
		}
		return null;
	}

	//得到连接WiFi的连接速度
	public String getConnectedLinkSpeed() {
		int linkSpeed = wifiConnectedInfo.getLinkSpeed();
		return linkSpeed + "Mbps";
	}

	//得到连接WiFi的频率
	public String getConnectedFrequency(String SSID) {
		float frequency;
		wifiScanList = localWifiManager.getScanResults();
		for (int i = 0; i < wifiScanList.size(); i++) {
			if (wifiScanList.get(i).SSID.equals(SSID)) {
				frequency = wifiScanList.get(i).frequency;
				frequency=frequency-frequency%100;
				return frequency / 1000 + "GHz";
			}
		}
		return null;
	}

	//获取连接WiFi的安全性
	public String getConnectedSecurity(String SSID) {
		String capabilities = null;
		wifiScanList = localWifiManager.getScanResults();
		for (int i = 0; i < wifiScanList.size(); i++) {
			if (wifiScanList.get(i).SSID.equals(SSID)) {
				capabilities = wifiScanList.get(i).capabilities;
			}
		}
		if (capabilities == null){
			return null;
		}else {
			if (capabilities.equals("[WPA-PSK-CCMP+TKIP][ESS]") || capabilities.equals("[WPA-PSK-CCMP][ESS]")) {
				return "WPA PSK";
			} else if (capabilities.equals("[WPA2-PSK-CCMP+TKIP][ESS]") || capabilities.equals("[WPA2-PSK-CCMP][ESS]")) {
				return "WPA2 PSK";
			} else if (capabilities.equals("[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]") ||
					capabilities.equals("[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][WPS][ESS]") ||
					capabilities.equals("[WPA-PSK-CCMP][WPA2-PSK-CCMP][WPS][ESS]") ||
					capabilities.equals("[WPA-PSK-CCMP][WPA2-PSK-CCMP][ESS]")) {
				return "WPA/WPA2 PSK";
			}
		}
		return null;
	}

	//得到连接的IP地址
	public String getConnectedIPAddr() {
		int ip = dhcpInfo.ipAddress;
		return (ip & 0xFF) + "." + ((ip >> 8) & 0xFF) + "." + ((ip >> 16) & 0xFF) + "." + ((ip >> 24) & 0xFF);
	}

	//得到连接的网关
	public String getConnectedGateway() {
		int gateway = dhcpInfo.gateway;
		return (gateway & 0xFF) + "." + ((gateway >> 8) & 0xFF) + "." + ((gateway >> 16) & 0xFF) + "." + ((gateway >> 24) & 0xFF);
	}

	//得到连接的网络前缀长度
	public int getConnectedLength() {
		int netmask = dhcpInfo.netmask;
		String netmaskStr = (netmask & 0xFF) + "." + ((netmask >> 8) & 0xFF) + "." + ((netmask >> 16) & 0xFF) + "." + ((netmask >> 24) & 0xFF);
		if (netmaskStr.equals("128.0.0.0")) {
			return 1;
		} else if (netmaskStr.equals("192.0.0.0")) {
			return 2;
		} else if (netmaskStr.equals("224.0.0.0")) {
			return 3;
		} else if (netmaskStr.equals("240.0.0.0")) {
			return 4;
		} else if (netmaskStr.equals("248.0.0.0")) {
			return 5;
		} else if (netmaskStr.equals("252.0.0.0")) {
			return 6;
		} else if (netmaskStr.equals("254.0.0.0")) {
			return 7;
		} else if (netmaskStr.equals("255.0.0.0")) {
			return 8;
		} else if (netmaskStr.equals("255.128.0.0")) {
			return 9;
		} else if (netmaskStr.equals("255.192.0.0")) {
			return 10;
		} else if (netmaskStr.equals("255.224.0.0")) {
			return 11;
		} else if (netmaskStr.equals("255.240.0.0")) {
			return 12;
		} else if (netmaskStr.equals("255.248.0.0")) {
			return 13;
		} else if (netmaskStr.equals("255.252.0.0")) {
			return 14;
		} else if (netmaskStr.equals("255.254.0.0")) {
			return 15;
		} else if (netmaskStr.equals("255.255.0.0")) {
			return 16;
		} else if (netmaskStr.equals("255.255.128.0")) {
			return 17;
		} else if (netmaskStr.equals("255.255.192.0")) {
			return 18;
		} else if (netmaskStr.equals("255.255.224.0")) {
			return 19;
		} else if (netmaskStr.equals("255.255.240.0")) {
			return 20;
		} else if (netmaskStr.equals("255.255.248.0")) {
			return 21;
		} else if (netmaskStr.equals("255.255.252.0")) {
			return 22;
		} else if (netmaskStr.equals("255.255.254.0")) {
			return 23;
		} else if (netmaskStr.equals("255.255.255.0")) {
			return 24;
		} else if (netmaskStr.equals("255.255.255.128")) {
			return 25;
		} else if (netmaskStr.equals("255.255.255.192")) {
			return 26;
		} else if (netmaskStr.equals("255.255.255.224")) {
			return 27;
		} else if (netmaskStr.equals("255.255.255.240")) {
			return 28;
		} else if (netmaskStr.equals("255.255.255.248")) {
			return 29;
		} else if (netmaskStr.equals("255.255.255.252")) {
			return 30;
		} else if (netmaskStr.equals("255.255.255.254")) {
			return 31;
		} else if (netmaskStr.equals("255.255.255.255")) {
			return 32;
		}
		return 0;
	}

	//得到连接的DNS 1
	public String getConnectedDNS_1() {
		int dns1 = dhcpInfo.dns1;
		return (dns1 & 0xFF) + "." + ((dns1 >> 8) & 0xFF) + "." + ((dns1 >> 16) & 0xFF) + "." + ((dns1 >> 24) & 0xFF);
	}

	//得到连接的DNS 2
	public String getConnectedDNS_2() {
		int dns2 = dhcpInfo.dns2;
		return (dns2 & 0xFF) + "." + ((dns2 >> 8) & 0xFF) + "." + ((dns2 >> 16) & 0xFF) + "." + ((dns2 >> 24) & 0xFF);
	}

    /**
     * 是否需要portal验证的wifi,一般用于公共wifi
     * @return
     */
    public static void isPortalWifiAsync(final Handler.Callback callback){
        if (callback == null) {
            return;
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                String testUrl = "http://clients3.google.com/generate_204";
                final int TIME_OUT_MS_PORTAL_WIFI_VERTIFY = 10000;

                boolean result = false;
                HttpURLConnection urlConnection = null;
                try {
                    URL url = new URL(testUrl);
                    urlConnection = (HttpURLConnection) url.openConnection();
                    urlConnection.setInstanceFollowRedirects(false);
                    urlConnection.setConnectTimeout(TIME_OUT_MS_PORTAL_WIFI_VERTIFY);
                    urlConnection.setReadTimeout(TIME_OUT_MS_PORTAL_WIFI_VERTIFY);
                    urlConnection.setUseCaches(false);
                    urlConnection.getInputStream();
                    result = urlConnection.getResponseCode() != 204;
                } catch (IOException e) {
                    e.printStackTrace();
                    result = false;
                } finally {
                    if (urlConnection != null) {
                        urlConnection.disconnect();
                    }
                }
                LogTool.d(TAG, "isPortalWifi " + result);
                Message msg = Message.obtain();
                msg.obj = result;
                callback.handleMessage(msg);
            }
        }).start();

    }


    /**
     * 获取当前连接wifi SSID
     * @return
     */
    public static String getConnectedWifiSSID(Context context){
        WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();

        if (wifiInfo == null) {
            return null;
        }

        LogTool.d(TAG, "getConnectedWifiSSID "+wifiInfo.getSSID());
        return wifiInfo.getSSID();
//        StringBuffer sb = new StringBuffer();
//        sb.append("\n获取BSSID属性（所连接的WIFI设备的MAC地址）：" + wifiInfo.getBSSID());
////      sb.append("getDetailedStateOf()  获取客户端的连通性：");
//        sb.append("\n\n获取SSID 是否被隐藏："+ wifiInfo.getHiddenSSID());
//        sb.append("\n\n获取IP 地址：" + wifiInfo.getIpAddress());
//        sb.append("\n\n获取连接的速度：" + wifiInfo.getLinkSpeed());
//        sb.append("\n\n获取Mac 地址（手机本身网卡的MAC地址）：" + WifiMac);
//        sb.append("\n\n获取802.11n 网络的信号：" + wifiInfo.getRssi());
//        sb.append("\n\n获取SSID（所连接的WIFI的网络名称）：" + wifiInfo.getSSID());
//        sb.append("\n\n获取具体客户端状态的信息：" + wifiInfo.getSupplicantState());
    }

    /**
     * 获取当前连接wifi 速度
     * @return
     */
    public static String getConnectedWifiSpeed(Context context){
        WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();

        if (wifiInfo == null) {
            return null;
        }

        LogTool.d(TAG, "getConnectedWifiSpeed "+wifiInfo.getLinkSpeed());

        return wifiInfo.getLinkSpeed() + "Mbps";
    }

    public static void removeNetwork(Context mContext,String SSID){
		WifiManager wifiManager=(WifiManager)mContext.getSystemService(Context.WIFI_SERVICE);
		List<WifiConfiguration>config=(List<WifiConfiguration>)wifiManager.getConfiguredNetworks();
		for(int i=0;i<config.size();i++){
			String ssid=config.get(i).SSID;
			if(ssid!=null&&ssid.startsWith("\"")&&ssid.endsWith("\"")){
				ssid=ssid.substring(ssid.indexOf("\"")+1);
				ssid=ssid.substring(0,ssid.lastIndexOf("\""));
			}
			if(SSID.equals(ssid))
				wifiManager.removeNetwork(config.get(i).networkId);
		}
		wifiManager.saveConfiguration();
	}

}
