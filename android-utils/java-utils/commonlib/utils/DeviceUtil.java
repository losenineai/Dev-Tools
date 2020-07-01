package com.auric.intell.commonlib.utils;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.telephony.TelephonyManager;
import android.text.TextUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

/**
 * Created by white on 15/7/14.
 * <p/>
 * 用来获得设备的各种信息
 * 1、设备名字
 * 2、设备签名
 * 3、设备别名
 */
public class DeviceUtil {

    private static final String KEY_DEVICE = "ro.product.device";
    private static final String KEY_MODEL = "ro.product.model";
    private static final String KEY_FINGERPRINT = "ro.build.fingerprint";
    private static final String TAG = "DeviceUtil";

    /**
     * @description 获取设备名
     */
    public static String getDevice() {
        try {
            BuildProperties prop = BuildProperties.newInstance();
            return prop.getProperty(KEY_DEVICE, "");
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * @description 获取设备别名
     */
    public static String getModel() {
        try {
            BuildProperties prop = BuildProperties.newInstance();
            return prop.getProperty(KEY_MODEL, "");
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * @description 获取设备签名
     */
    public static String getFingerprint() {
        try {
            BuildProperties prop = BuildProperties.newInstance();
            return prop.getProperty(KEY_FINGERPRINT, "");
        } catch (IOException e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * 获取imei
     * @param context
     * @return
     */
    public static String getIMEI(Context context) {
        TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
        return tm.getDeviceId();
    }


    public static class BuildProperties {

        private final Properties properties;

        public BuildProperties() throws IOException {
            properties = new Properties();
            properties.load(new FileInputStream(new File(Environment.getRootDirectory(), "build.prop")));
        }

        public boolean containsKey(final Object key) {
            return properties.containsKey(key);
        }

        public boolean containsValue(final Object value) {
            return properties.containsValue(value);
        }

        public Set<Map.Entry<Object, Object>> entrySet() {
            return properties.entrySet();
        }

        public String getProperty(final String name) {
            return properties.getProperty(name);
        }

        public String getProperty(final String name, final String defaultValue) {
            return properties.getProperty(name, defaultValue);
        }

        public boolean isEmpty() {
            return properties.isEmpty();
        }

        public Enumeration<Object> keys() {
            return properties.keys();
        }

        public Set<Object> keySet() {
            return properties.keySet();
        }

        public int size() {
            return properties.size();
        }

        public Collection<Object> values() {
            return properties.values();
        }

        public static BuildProperties newInstance() throws IOException {
            return new BuildProperties();
        }

    }

    public static String getWifiMac(){
        String defaultValue = "";
        try{
            Context app = ContextFinder.getApplication();
            WifiManager wm = (WifiManager) app.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            String addr = wm.getConnectionInfo().getMacAddress();
            return addr == null ? "" : addr;
        }catch(Throwable t){
            return defaultValue;
        }
    }

    /***
     * android 6.0下获取wifimac地址
     * @return
     */
    public static String getAndroidMWifiMac(){
        NetworkInterface networkInterface = null;
        String wifimac="";
        try {
            networkInterface = NetworkInterface.getByName("wlan0");
            byte[] mac = networkInterface.getHardwareAddress();

            wifimac=ByteUtil.byte2hex(mac);

        } catch (SocketException e) {
            e.printStackTrace();
        }

        return wifimac;
    }

    public static String getBluetoothMac(){
        BluetoothAdapter ba = BluetoothAdapter.getDefaultAdapter();
        String bluetoothAdapterAddress = "";
        try {
            if (ba != null) {
                bluetoothAdapterAddress = ba.getAddress();
            }
        } catch (Throwable e) {
        }
        LogTool.d(TAG, "bluetooth Adapter Address " + bluetoothAdapterAddress);
        return bluetoothAdapterAddress;
    }

    public static boolean isX86Cpu(){
        return Build.DEVICE.equals("gpdroidp") || (getCpuType() != null && getCpuType().contains("x86"));
    }

    public static boolean isARMCpu(){
        return getCpuType() != null && getCpuType().contains("arm");
    }

    private static String getCpuAbi() {
        String abi = TextUtils.isEmpty(Build.CPU_ABI) ? Build.CPU_ABI2 : Build.CPU_ABI;
        if(!TextUtils.isEmpty(abi)){
            return abi;
        }

        String strInfo = "";
        try {
            byte[] bs = new byte[1024];
            RandomAccessFile reader = new RandomAccessFile("/proc/cpuinfo", "r");
            reader.read(bs);
            String ret = new String(bs);
            int index = ret.indexOf(0);
            if (index != -1) {
                strInfo = ret.substring(0, index);
            } else {
                strInfo = ret;
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        return strInfo;
    }

    public static String getCpuType() {
        String cpuAbi = getCpuAbi();

        if (cpuAbi.contains("neon")) {
            cpuAbi += "_neon";
        } else if (cpuAbi.contains("vfpv3")) {
            cpuAbi += "_vfpv3";
        } else if (cpuAbi.contains(" vfp")) {
            cpuAbi += "_vfp";
        } else {
        }

        return cpuAbi.toLowerCase();
    }
}
