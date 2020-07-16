package com.dx.mobile.risk.collect.network;

import android.content.Context;
import android.net.TrafficStats;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.*;

import org.json.JSONArray;
import org.json.JSONObject;

import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Network {

    private static WifiManager wifiManager;

    static {
        if (wifiManager == null) {
            try {
                wifiManager = (WifiManager) ContextFinder.getApplication().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            } catch (SecurityException e) {
            }
        }
    }

    /**
     * 获取周围wifi信息
     *
     * @return
     */
//    @JXC_RISK
//    public static String getWifiNearList() {
//        Context ctx = ContextFinder.getApplication();
//        final WifiManager wm = (WifiManager) ctx.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
//        IntentFilter filter = new IntentFilter();
//        filter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
//        ctx.registerReceiver(new BroadcastReceiver() {
//            @Override
//            public void onReceive(Context context, Intent intent) {
//                if (WifiManager.SCAN_RESULTS_AVAILABLE_ACTION.equals(intent.getAction())) {
//                    List<ScanResult> list = wm.getScanResults();
//                }
//            }
//        }, filter);
//
//        wm.startScan();
//        List<ScanResult> list = wm.getScanResults();
//        return Arrays.toString(list.toArray());
//    }

    public static String getWifiInfo() {

        JSONObject action_json = new JSONObject();

        if (wifiManager != null) {
            try {

                WifiInfo info = wifiManager.getConnectionInfo();

                /* 通付盾-版本
                String wifi_ip_address = intToIp(info.getIpAddress());
                if (!StringUtils.isEmpty(wifi_ip_address)) {
                    JSONUtils.jsonPut(action_json, "K79", wifi_ip_address);
                }*/
                String wifi_ip_address = getLocalIpAddress();

                String wifiSsid = info.getSSID();
                if (!StringUtils.isEmpty(wifiSsid) && wifiSsid.startsWith("\""))
                    wifiSsid = wifiSsid.substring(1, wifiSsid.length() - 1);

                JSONUtils.jsonPut(action_json, JNIConst.KEY_APP_KOCK_INDEX13, JNIConst.VALUE_APP_KOCK);
                JSONUtils.jsonPut(action_json, "K74", info.getBSSID());
                JSONUtils.jsonPut(action_json, "K75", MacUtils.getMacAddress(info));

                JSONUtils.jsonPut(action_json, "K78", wifiSsid);
                JSONUtils.jsonPut(action_json, "K79", wifi_ip_address);

                if (wifiManager.isWifiEnabled()) {
                    int ipAdd = wifiManager.getDhcpInfo().gateway;
                    JSONUtils.jsonPut(action_json, "K255", MacUtils.intToIp(ipAdd));
                }

                //
                getLocalIpAddressV4_V6(action_json);

                //
                JSONUtils.jsonPut(action_json, "K600", info.getLinkSpeed());
                JSONUtils.jsonPut(action_json, "K601", String.valueOf(info.getRssi()));
                JSONUtils.jsonPut(action_json, "K602", String.valueOf(info.getNetworkId()));
                JSONUtils.jsonPut(action_json, "K603", getWifiScanList());
                JSONUtils.jsonPut(action_json, "K604", getWifiConfigurationList());
                JSONUtils.jsonPut(action_json, "K605", getWifiScanListByReflect());

                // 流量
                JSONUtils.jsonPut(action_json, "K640", TrafficStats.getMobileRxBytes());
                JSONUtils.jsonPut(action_json, "K641", TrafficStats.getMobileTxBytes());
                JSONUtils.jsonPut(action_json, "K642", TrafficStats.getTotalTxBytes());
                JSONUtils.jsonPut(action_json, "K643", TrafficStats.getTotalRxBytes());

                //
                JSONUtils.jsonPut(action_json, "K1805", MacUtils.getWlanMacAddr());
                JSONUtils.jsonPut(action_json, "K1806", MacUtils.getLocalMacAddressFromBusybox());
                JSONUtils.jsonPut(action_json, "K1807", MacUtils.getMacAddressAfterN());


            } catch (Exception e) {
            }
        }

        return action_json.toString();
    }

    /**
     * 获取本地IP
     *
     * @return
     */
    public static String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface
                    .getNetworkInterfaces(); en.hasMoreElements(); ) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf
                        .getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress() && !inetAddress.isLinkLocalAddress()) {
                        return inetAddress.getHostAddress().toString();
                    }
                }
            }
        } catch (Throwable ex) {
        }
        return null;
    }

    public static void getLocalIpAddressV4_V6(JSONObject action_json) {

        try {

            Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();

            boolean ipv4Flag = false;
            boolean ipv6Flag = false;

            if (en != null) {
                outer:
                while (en.hasMoreElements()) {
                    NetworkInterface intf = en.nextElement();

                    if (intf == null)
                        continue;

                    Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses();

                    if (enumIpAddr == null)
                        continue;

                    while (enumIpAddr.hasMoreElements()) {
                        InetAddress inetAddress = enumIpAddr.nextElement();

                        if (inetAddress == null || inetAddress.isLoopbackAddress())
                            continue;

                        if (!ipv4Flag && (inetAddress instanceof Inet4Address)) {
                            JSONUtils.jsonPut(action_json, "K400", inetAddress.getHostAddress());
                            ipv4Flag = true;
                        } else if (!ipv6Flag && (inetAddress instanceof Inet6Address)) {
                            JSONUtils.jsonPut(action_json, "K401", inetAddress.getHostAddress());
                            if (!inetAddress.isLinkLocalAddress()) {
                                ipv6Flag = true;
                            }
                        }
                        if (ipv4Flag && ipv6Flag) {
                            break outer;
                        }
                    }
                }
            }
        } catch (Throwable ex) {  }
    }

    public static String getWifiScanList() {
        if (!PermissionUtils.checkLocation())
            return "";

        JSONArray jsonArr = new JSONArray();
        try {
            List<ScanResult> wifi_list = wifiManager.getScanResults();
            Map<String, Integer> wifi_map = new HashMap();

            for (ScanResult wifi : wifi_list) {
                if (!wifi_map.containsKey(wifi.SSID)) {
                    wifi_map.put(wifi.SSID, Integer.valueOf(wifi.level));

                    JSONObject jobj = new JSONObject();
                    jobj.put("ssid", wifi.SSID);
                    jobj.put("bssid", wifi.BSSID);
                    jobj.put("level", wifi.level);
                    jobj.put("frequency", wifi.frequency);
                    jsonArr.put(jobj);
                }
            }
        } catch (Exception e) {
        }

        return jsonArr.length() > 0 ? jsonArr.toString() : "";
    }

    public static String getWifiScanListByReflect() {
        if (!PermissionUtils.checkLocation())
            return "";

        JSONArray jsonArr = new JSONArray();
        try {
            Object wifiServiceObj = ReflectUtil.getFieldValue(wifiManager, "mService");
            List<ScanResult> wifi_list = (List<ScanResult>)ReflectUtil.invokeMethod(wifiServiceObj, "getScanResults", ContextFinder.getApplication().getPackageName());
            Map<String, Integer> wifi_map = new HashMap();

            if (wifi_list != null) {
                for (ScanResult wifi : wifi_list) {
                    if (!wifi_map.containsKey(wifi.SSID)) {
                        wifi_map.put(wifi.SSID, Integer.valueOf(wifi.level));

                        JSONObject jobj = new JSONObject();
                        jobj.put("ssid", wifi.SSID);
                        jobj.put("bssid", wifi.BSSID);
                        jobj.put("level", wifi.level);
                        jobj.put("frequency", wifi.frequency);
                        jsonArr.put(jobj);
                    }
                }
            }
        } catch (Exception e) {
        }

        return jsonArr.length() > 0 ? jsonArr.toString() : "";
    }

    public static String getWifiConfigurationList() {

        JSONArray jsonArr = new JSONArray();
        try {
            List<WifiConfiguration> wifi_list = wifiManager.getConfiguredNetworks();
            Map<String, Integer> wifi_map = new HashMap();

            for (WifiConfiguration conf : wifi_list) {

                if (!wifi_map.containsKey(conf.SSID)) {
                    wifi_map.put(conf.SSID, Integer.valueOf(conf.BSSID));

                    JSONObject jobj = new JSONObject();
                    jobj.put("ssid", conf.SSID);
                    jobj.put("bssid", conf.BSSID);
                    jsonArr.put(jobj);
                }
            }
        } catch (Exception e) {
        }

        return jsonArr.length() > 0 ? jsonArr.toString() : "";
    }

    public static String getProxyInfo() {

        String proxyAddress;
        String portStr;
        int proxyPort;

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
                proxyAddress = System.getProperty("http.proxyHost");
                portStr = System.getProperty("http.proxyPort");
                proxyPort = Integer.parseInt((portStr != null ? portStr : "-1"));
            } else {
                Context ctx = ContextFinder.getApplication();
                proxyAddress = android.net.Proxy.getHost(ctx);
                proxyPort = android.net.Proxy.getPort(ctx);
            }

            JSONObject json_obj = new JSONObject();

            if (!StringUtils.isEmpty(proxyAddress)) {
                JSONUtils.jsonPut(json_obj, "K70", proxyAddress);
                JSONUtils.jsonPut(json_obj, "K71", proxyPort);
            }

            if (isVpnUsed())
                JSONUtils.jsonPut(json_obj, "K72", true);

            return json_obj.length() > 0 ? json_obj.toString() : "";
        } catch (Throwable t) {

        }

        return "";
    }

    /**
     * 是否正在使用VPN
     */
    public static boolean isVpnUsed() {

        try {
            Enumeration<NetworkInterface> niList = NetworkInterface.getNetworkInterfaces();
            if (niList != null) {
                for (NetworkInterface intf : Collections.list(niList)) {

                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD &&
                            (!intf.isUp() || intf.getInterfaceAddresses().size() == 0)) {
                        continue;
                    }

                    if ("tun0".equals(intf.getName()) || "ppp0".equals(intf.getName())) {
                        return true; // The VPN is up
                    }
                }
            }
        } catch (Throwable e) {
//            e.printStackTrace();
        }

        return false;
    }

    public static String getMacList() {

        try {
            JSONArray jsonArr = new JSONArray();

            Map<String, String> hardwareMap = MacUtils.getMachineHardwareAddress();

            for (Map.Entry<String, String> entry : hardwareMap.entrySet()) {

                String strInfo = new StringBuilder(entry.getKey())
                        .append("-").append(entry.getValue()).toString();

                jsonArr.put(strInfo);
            }

            return jsonArr.length() > 0 ? jsonArr.toString() : "";
        } catch (Throwable t) {

        }

        return "";

    }

    public static String getUserAgent() {
        return System.getProperty("http.agent");
    }


}
