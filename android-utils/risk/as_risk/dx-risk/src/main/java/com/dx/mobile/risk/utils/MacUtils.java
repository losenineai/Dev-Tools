package com.dx.mobile.risk.utils;

import android.net.wifi.WifiInfo;
import android.util.Log;

import java.io.FileReader;
import java.io.Reader;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Created by blue on 2017/11/20.
 */

public class MacUtils {


    public static String getWlanMacAddr() {
        try {
            Enumeration<NetworkInterface> nif = NetworkInterface.getNetworkInterfaces();
            if (nif == null) {
                return "";
            }

            while (nif.hasMoreElements()) {
                NetworkInterface networkInterface = nif.nextElement();

                if (networkInterface != null && "wlan0".equals(networkInterface.getName())) {
                    byte[] hardwareAddress = null;

                    if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.GINGERBREAD) {
                        hardwareAddress = networkInterface.getHardwareAddress();
                        if (hardwareAddress == null || hardwareAddress.length == 0) {
                            continue;
                        }
                        StringBuilder buf = new StringBuilder();
                        for (byte b : hardwareAddress) {
                            buf.append(String.format("%02X:", b));
                        }
                        if (buf.length() > 0) {
                            buf.deleteCharAt(buf.length() - 1);
                        }
                        String mac = buf.toString().toLowerCase();
                        return mac;
                    }
                    else{
                        break;
                    }
                }
            }
        } catch (Throwable e) {
        }
        return "";
    }


    public static String getMacAddress(WifiInfo info) {

        Log.e("OpenGLRenderer", "**");
        String wifiMac = "";
        try {
            int wifiMacCount = 0;
            Map<String, Integer> macMap = new HashMap<String, Integer>();

            /* sdk before M */
            wifiMac = getMacAddressBeforeM(info);

            if (isValidMac(wifiMac)) {
                wifiMacCount = macMap.get(wifiMac) == null ? 0 : macMap.get(wifiMac);
                macMap.put(wifiMac.toLowerCase(), ++wifiMacCount);
            }

            /* sdk before N */
            wifiMac = getMacAddressFromMToN();

            if (isValidMac(wifiMac)) {
                wifiMacCount = macMap.get(wifiMac) == null ? 0 : macMap.get(wifiMac);
                macMap.put(wifiMac.toLowerCase(), ++wifiMacCount);
            }

            /* sdk after N */
            wifiMac = getMacAddressAfterN();

            if (isValidMac(wifiMac)) {
                wifiMacCount = macMap.get(wifiMac) == null ? 0 : macMap.get(wifiMac);
                macMap.put(wifiMac.toLowerCase(), ++wifiMacCount);
            }

            /* get mac from hardware map */
            Map hardwareMap = getMachineHardwareAddress();

            Object obj = hardwareMap.get("wlan0");
            if (obj != null) {

                wifiMac = obj.toString();

                if (isValidMac(wifiMac)) {
                    wifiMacCount = macMap.get(wifiMac) == null ? 0 : macMap.get(wifiMac);
                    macMap.put(wifiMac.toLowerCase(), ++wifiMacCount);
                }
            }

            /* get mac from busy box */
            wifiMac = getLocalMacAddressFromBusybox();

            if (isValidMac(wifiMac)) {
                wifiMacCount = macMap.get(wifiMac) == null ? 0 : macMap.get(wifiMac);
                macMap.put(wifiMac.toLowerCase(), ++wifiMacCount);
            }

            Set<String> macSet = macMap.keySet();
            List<String> macList = new ArrayList<String>();
            macList.addAll(macSet);

            if (macList.size() > 0) {

                wifiMac = macList.get(0);
                int maxMacCount = macMap.get(wifiMac);
                int macSize = macList.size();

                for (int i = 1; i < macSize; i++) {

                    int macCount = macMap.get(macList.get(i));

                    if (maxMacCount < macCount) {
                        maxMacCount = macCount;
                        wifiMac = macList.get(i);
                    }
                }

            } else {
                return "";
            }

        } catch (Throwable t) {

        }

        Log.e("OpenGLRenderer", "--");

        return wifiMac;
    }

    /**
     * get mac address before 6.0
     *
     * @return
     */
    public static String getMacAddressBeforeM(WifiInfo info) {

        String macAddress = null;

        if (info != null && (macAddress = info.getMacAddress()) != null && isValidMac(macAddress)) {
            return macAddress;
        }

        return null;
    }

    /**
     * get mac address from android 6.0 to 7.0
     *
     * @return
     */
    public static String getMacAddressFromMToN() {

        String wifiMac = ProcessUtils.exec("cat /sys/class/net/wlan0/address");

        if (StringUtils.isEmpty(wifiMac)) {
            try {
                wifiMac = loadFileAsString("/sys/class/net/eth0/address")
                        .toUpperCase().substring(0, 17);
            } catch (Exception e) {
                //e.printStackTrace();
            }
        }

        return wifiMac;
    }

    /**
     * get mac address after android 7.0
     *
     * @return
     */
    public static String getMacAddressAfterN() {
        String strMacAddr = null;
        try {
            //获得IpD地址
            InetAddress ip = getLocalInetAddress();
            byte[] b = NetworkInterface.getByInetAddress(ip).getHardwareAddress();
            StringBuffer buffer = new StringBuffer();

            for (int i = 0; i < b.length; i++) {
                if (i != 0) {
                    buffer.append(':');
                }

                String str = Integer.toHexString(b[i] & 0xFF);
                buffer.append(str.length() == 1 ? 0 + str : str);
            }
            strMacAddr = buffer.toString().toUpperCase();
        } catch (Exception e) {

        }

        return strMacAddr;
    }

    /**
     * 获取移动设备本地IP
     *
     * @return
     */
    private static InetAddress getLocalInetAddress() {
        InetAddress ip = null;
        try {
            //列举
            Enumeration<NetworkInterface> en_netInterface = NetworkInterface.getNetworkInterfaces();
            while (en_netInterface.hasMoreElements()) {//是否还有元素
                NetworkInterface ni = en_netInterface.nextElement();//得到下一个元素
                Enumeration<InetAddress> en_ip = ni.getInetAddresses();//得到一个ip地址的列举
                while (en_ip.hasMoreElements()) {
                    ip = en_ip.nextElement();
                    if (!ip.isLoopbackAddress() && ip.getHostAddress().indexOf(":") == -1)
                        break;
                    else
                        ip = null;
                }

                if (ip != null) {
                    break;
                }
            }
        } catch (Exception e) {
//            e.printStackTrace();
        }
        return ip;
    }

    public static Map<String, String> getMachineHardwareAddress() {

        Map hardwareMap = new HashMap<String, String>();

        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();

            while (interfaces.hasMoreElements()) {
                NetworkInterface nif = interfaces.nextElement();

                if ("lo".equals(nif.getName())) {
                    continue;
                }

                byte[] addr = nif.getHardwareAddress();
                if (addr == null || addr.length == 0) {
                    continue;
                }

                StringBuilder buf = new StringBuilder();
                for (byte b : addr) {
                    buf.append(String.format("%02x:", b));
                }
                if (buf.length() > 0) {
                    buf.deleteCharAt(buf.length() - 1);
                }

                hardwareMap.put(nif.getName(), buf.toString());
            }

        } catch (Exception e) {
//            e.printStackTrace();
        }

        return hardwareMap;
    }

    /**
     * 根据busybox获取本地Mac
     *
     * @return
     */
    public static String getLocalMacAddressFromBusybox() {

        String result = "";
        String mac = "";
        result = ProcessUtils.exec("busybox ifconfig", "HWaddr");

        //如果返回的result == null，则说明网络不可取
        //也可能是系统里没有busybox命令 -by blue
        if (result == null) {
            return null;
        }

        //对该行数据进行解析
        //例如：eth0      Link encap:Ethernet  HWaddr 00:16:E8:3E:DF:67
        if (result.length() > 0 && result.contains("HWaddr") == true) {
            mac = result.substring(result.indexOf("HWaddr") + 6, result.length() - 1);
            result = mac;
        }

        return result;
    }

    private static String loadReaderAsString(Reader reader) throws Exception {
        StringBuilder builder = new StringBuilder();
        char[] buffer = new char[4096];
        int readLength = reader.read(buffer);
        while (readLength >= 0) {
            builder.append(buffer, 0, readLength);
            readLength = reader.read(buffer);
        }
        return builder.toString();
    }

    private static String loadFileAsString(String fileName) throws Exception {
        FileReader reader = new FileReader(fileName);
        String text = loadReaderAsString(reader);
        reader.close();
        return text;
    }

    public static boolean isValidMac(String mac) {

        return !StringUtils.isEmpty(mac) &&
                !("11:22:33:44:55:66".equals(mac) ||
                        "00:00:00:00:00:00".equals(mac) ||
                        "00:90:4c:11:22:33".equals(mac) ||
                        "02:00:00:00:00:00".equals(mac) ||
                        "ff:ff:ff:ff:ff:ff".equals(mac) ||
                        "58:02:03:04:05:06".equals(mac) // 华为在4G下会获取到这个mac
                );

        //return !StringUtils.isEmpty(mac) && !"02:00:00:00:00:00".equals(mac) && !"00:00:00:00:00:00".equals(mac);
    }

    public static String intToIp(int i) {
        return (i & 255) + "." + ((i >> 8) & 255) + "." + ((i >> 16) & 255) + "." + ((i >> 24) & 255);
    }
}
