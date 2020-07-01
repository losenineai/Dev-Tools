package com.auric.intell.commonlib.robot;


import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.text.TextUtils;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.FileUtil;
import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.PropertyUtil;

import java.util.Properties;

/**
 * @author white
 * @description：RBT设备获取信息
 * @date 2016/10/14
 */

public class RBTDeviceUtil {

    private static final String TAG = "RBTDeviceUtil";


    private static final String FILE_NAME_HOST_MODE = "/private/host.properties";
    private static final String KEY_HOST_MODE = "KEY_HOST_MODE";
    // 沙盒域名
    private final static String HOST_DEBUG = "http://rsdb.oduola.com";
    private final static String HOST_DEBUG_HTTPS = "https://rsdb.oduola.com";
    // 上线域名
    private final static String HOST_RELEASE = "http://rapi.oduola.com";
    private final static String HOST_RELEASE_HTTPS = "https://rapi.oduola.com";


    /**
     * 获取当前RBT OS版本号
     * @return
     */
    public static String getVersion(){
        String version = "0.0.1";
        try{
            if (!TextUtils.isEmpty(Build.ID) && Build.ID.lastIndexOf("_") != -1) {
                version = Build.ID.substring(Build.ID.lastIndexOf("_") + 1);
            }
        }catch(Exception e){
            e.printStackTrace();
        }
        LogTool.d(TAG, "getVersion:" + version);
        return version;
    }

    /**
     * 获取RBT通用host
     * @return
     */
    public static String getCommonHost(){
        String host = HOST_RELEASE;
        if (isDebugMode()) {
            host = HOST_DEBUG;
        }
        LogTool.d(TAG, "getCommonHost " + host);

        return host;
    }

    /**
     * 获取RBT通用host
     * @return
     */
    public static String getCommonHttpsHost(){
        String host = HOST_RELEASE_HTTPS;
        if (isDebugMode()) {
            host = HOST_DEBUG_HTTPS;
        }
        LogTool.d(TAG, "getCommonHttpsHost " + host);

        return host;

    }

    public static boolean isDebugMode() {
        boolean isDebugMode = false;
        Properties properties = PropertyUtil.loadConfig(FILE_NAME_HOST_MODE);
        String mode = properties.getProperty(KEY_HOST_MODE, "0");

        // 默认上线域名
        if (mode.equals("0")) {
            isDebugMode = false;
        }

        // 沙盒域名
        else if (mode.equals("1")) {
            isDebugMode = true;
        }

        LogTool.d(TAG, "isDebugMode " + isDebugMode);
        return isDebugMode;
    }


    /**
     * 获取wifi mac地址
     * need permission android.permission.ACCESS_WIFI_STATE
     * @return
     */
    public static String getWifiMacAddress(){
        String addr = "";
        try{
            Context app = ContextFinder.getApplication();
            WifiManager wm = (WifiManager) app.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            addr = wm.getConnectionInfo().getMacAddress();
            addr = addr == null ? "" : addr;
        }catch(Throwable t){
            t.printStackTrace();
        }
        LogTool.d(TAG, "getWifiMacAddress " + addr);
        return addr;
    }

    /**
     * 获取蓝牙mac 地址
     * need permission android.permission.BLUETOOTH
     * @return
     */
    public static String getBtMacAddress(){
        String addr = "";
        BluetoothAdapter ba = BluetoothAdapter.getDefaultAdapter();
        try {
            if (ba != null) {
                addr = ba.getAddress();
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        LogTool.i(TAG, "getBtMacAddress " + addr);
        return addr;
    }

    /**
     * 获取RBT设备号
     * @return
     */
    public static String getSn(){
        String sn = "";
        try {
            sn = FileUtil.readFileString("/private/serialno.txt");
            sn = sn == null ? "" : sn;
        } catch (Exception e) {
            e.printStackTrace();
        }

        LogTool.d(TAG, "getSn:" + sn);
        return sn.trim();
    }

    /**
     * 获取机器人全部存储空间
     * @return
     */
    public static long getStorageSizeForTotal(){
        String sdcardPath = Environment.getExternalStorageDirectory().getAbsolutePath();

        long rootSize =(long) FileUtil.getFileSizeAllTotal(Environment.getRootDirectory().getAbsolutePath());
        long sdcardSize = (long)FileUtil.getFileSizeAllTotal(sdcardPath);
        return rootSize + sdcardSize;
    }


}
