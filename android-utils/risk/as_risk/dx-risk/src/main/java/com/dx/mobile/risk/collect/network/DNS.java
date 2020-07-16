package com.dx.mobile.risk.collect.network;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.Network;
import android.net.NetworkInfo;
import android.os.Build;
import android.util.Log;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.ReflectUtil;

import org.json.JSONObject;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.net.InetAddress;
import java.util.LinkedList;
import java.util.List;

/**
 * @author white
 * @description：
 * @date 2020-02-05
 */
public class DNS {

    private static final String TAG = "DXDNS";

    /**
     * 获取dns 地址
     * @return
     */
    public static String getDnsInfo() {
        Context context = ContextFinder.getApplication();

        JSONObject rootJson = new JSONObject();

        List<String> dnsApiList = getDnsByApi(context);
        StringBuilder sb = new StringBuilder();
        for (String dns : dnsApiList) {
            sb.append(dns);
            sb.append("|");
        }
        JSONUtils.jsonPut(rootJson, "K2300", sb.toString());

        List<String> dnsRList = getDnsByApi(context);
        StringBuilder sbR = new StringBuilder();
        for (String dns : dnsRList) {
            sbR.append(dns);
            sbR.append("|");
        }
        JSONUtils.jsonPut(rootJson, "K2301", sbR.toString());

        Log.i(TAG, "getDnsInfo:"+rootJson);
        return rootJson.toString();
    }


    //通过 getprop 命令获取
    private static String[] getDnsFromCommand() {
        LinkedList<String> dnsServers = new LinkedList<>();
        try {
            Process process = Runtime.getRuntime().exec("getprop");
            InputStream inputStream = process.getInputStream();
            LineNumberReader lnr = new LineNumberReader(new InputStreamReader(inputStream));
            String line = null;
            while ((line = lnr.readLine()) != null) {
                Log.i(TAG, "line:" + line);
                int split = line.indexOf("]: [");
                if (split == -1) continue;
                String property = line.substring(1, split);
                String value = line.substring(split + 4, line.length() - 1);
                if (property.endsWith(".dns")
                        || property.endsWith(".dns1")
                        || property.endsWith(".dns2")
                        || property.endsWith(".dns3")
                        || property.endsWith(".dns4")) {
                    InetAddress ip = InetAddress.getByName(value);
                    if (ip == null) continue;
                    value = ip.getHostAddress();
                    if (value == null) continue;
                    if (value.length() == 0) continue;
                    dnsServers.add(value);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return dnsServers.isEmpty() ? new String[0] : dnsServers.toArray(new String[dnsServers.size()]);
    }


    private static List<String> getDnsByApi(Context context) {
        List<String> dnsList = new LinkedList<>();
        try {
            if (Build.VERSION.SDK_INT >= 21 && context != null) {
                ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);
                if (connectivityManager != null) {
                    NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
                    if (activeNetworkInfo != null) {
                        for (Network network : connectivityManager.getAllNetworks()) {
                            NetworkInfo networkInfo = connectivityManager.getNetworkInfo(network);
                            if (networkInfo != null && networkInfo.getType() == activeNetworkInfo.getType()) {
                                LinkProperties lp = connectivityManager.getLinkProperties(network);
                                for (InetAddress addr : lp.getDnsServers()) {
                                    dnsList.add(addr.getHostAddress());
                                }
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {

        }

        return dnsList;

    }

    private static List<String> getDnsByReflect(Context context){
        List<String> dnsList = new LinkedList<>();

        try {
            if (Build.VERSION.SDK_INT >= 21 && context != null) {
                ConnectivityManager cmr = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);
                if (cmr != null) {
                    NetworkInfo activeNetworkInfo = cmr.getActiveNetworkInfo();
                    if (activeNetworkInfo != null) {
                        for (Network network : cmr.getAllNetworks()) {
                            NetworkInfo networkInfo = cmr.getNetworkInfo(network);
                            if (networkInfo != null && networkInfo.getType() == activeNetworkInfo.getType()) {
                                LinkProperties lp = (LinkProperties)ReflectUtil.invokeMethod(cmr, "getLinkProperties", network);
                                for (InetAddress addr : (List<InetAddress>)ReflectUtil.invokeMethod(lp, "getDnsServers")) {
                                    dnsList.add((String)ReflectUtil.invokeMethod(addr, "getHostAddress"));
                                }
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
        }


        return dnsList;
    }
}
