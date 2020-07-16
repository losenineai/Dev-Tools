package com.dx.mobile.risk.dx;

import android.content.Context;

import com.dx.mobile.risk.collect.app.App;
import com.dx.mobile.risk.collect.hardware.*;
import com.dx.mobile.risk.collect.hook.Xposed;
import com.dx.mobile.risk.collect.location.BaseStation;
import com.dx.mobile.risk.collect.location.GPS;
import com.dx.mobile.risk.collect.network.DNS;
import com.dx.mobile.risk.collect.network.Network;
import com.dx.mobile.risk.collect.system.OS;
import com.dx.mobile.risk.collect.system.Setting;
import com.dx.mobile.risk.collect.system.Supplier;
import com.dx.mobile.risk.collect.system.Time;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.FileUtils;
import com.dx.mobile.risk.utils.HttpUtils;

import static com.dx.mobile.risk.data.JNIConst.*;

/**
 * @author white
 * @description：JNIHelper 实现类
 * @date 2018/12/13
 */
public class a {

    public static final String TAG = "DXRisk";

    /*
    采集 GET STR
     */
    public static String a(int id){
//        Log.e(TAG, "a id:"+id);
        switch (id) {
            case GET_BATTERY_INFO:
                return Battery.getInstance().getBatteryJson();
            case GET_APP_LIST:
                return App.getAppList();
            case GET_GPS_INFO:
                return GPS.getGPSInfo();
            case GET_DEVICE_IDS:
                return Telephone.getDeviceIDs();
            case GET_TELEPHONE_INFO:
                return Telephone.getTelephoneInfo();
            case GET_WIFI_INFO:
                return Network.getWifiInfo();
            case GET_BASESTATION_INFO:
                return BaseStation.getBaseStation();
            case GET_SETTING_INFO:
                return Setting.getSettingInfo();
            case GET_AUDIO_INFO:
                return Audio.getAudioInfo();
            case GET_APP_INFO:
                return App.getAppInfo();
            case GET_BUILD_STRS:
                return Builds.getBuildStrings();
            case GET_GPU_INFO:
                return GPU.getGPUInfo();
            case GET_MEM_INFO:
                return Memory.getMemInfo();
            case GET_PROXY_INFO:
                return Network.getProxyInfo();
            case GET_MAC_LIST:
                return Network.getMacList();
            case GET_BLUETOOTH_INFO:
                return BlueTooth.getBTInfo();
            case GET_USER_AGENT:
                return Network.getUserAgent();
            case GET_SENSOR_INFO:
                return Sensor.getSensorInfo();
            case GET_SCREEN_INFO:
               return Display.getScreenInfo();
            case GET_TIME_INFO:
                return Time.getTimesInfo();
            case GET_SUPPLIER_IDS:
                return Supplier.getSupplierIds();
            case GET_USB_STATE:
                return USB.readUSBState();
            case GET_CAMERA_INFO:
                return Cameras.getCamerasInfo();
            case GET_SDCARD_INFO:
                return SDCard.getSDCardInfo();
            case GET_XPOSED_INFO:
                return Xposed.getXposedInfo();

            case GET_APP_VERSION:
                return App.getAppVersion();
            case GET_OS_VERSION:
                return OS.getOSVersion();
            case GET_PKG_NAME:
                return App.getPkgName();
            case GET_SDK_VERSION:
                return App.getSDKVersion();
            case GET_WORK_DIR:
                return App.getWorkDir();
            case GET_SDCARD_DIR:
                return SDCard.getSDCardDir();
            case GET_EXSDCARD_DIR:
                return SDCard.exgetSDCardDir();
            case GET_DNS_INFO:
                return DNS.getDnsInfo();

            default:
                return "";
        }
    }


    /*
    工具 GET INT
     */
    /**
     * 1 getAppVersionCode
     * 2 getSdcardStatus
     * @return
     */
    public static int b(int id){
        switch (id){
            case GET_APP_VERSION_CODE:
                return App.getAppVersionCode();
            case GET_SDCARD_STATUS:
                return SDCard.getSDCardStatus();
        }

        return -1;
    }

    /**
     * netRequest
     * @param type
     * @param url
     * @param data
     * @param contentType
     * @param bSaas
     */
    public static byte[] b(int type, String url, byte[] data, boolean bSaas, String contentType){
//        Log.e(TAG, "b");
        return HttpUtils.netRequest(type, url, data, bSaas, contentType);

    }

    /**
     * asyncNetRequest
     * @param type
     * @param url
     * @param data
     * @param contentType
     * @param bSaas
     * @return
     */
    public static byte[] c(int syncType, int type, String url, byte[] data, boolean bSaas, String contentType) {
//        Log.e(TAG, "c");

        if(syncType == NETWORK_SYNC) {
            return HttpUtils.netRequest(type, url, data, bSaas, contentType);
        } else if (syncType == NETWORK_ASYNC){
            HttpUtils.asyncNetRequest(type, url, data, bSaas, contentType);
        }

        return null;
    }

    /**
     * getContext
     * @return
     */
    public static Context e(){
        return ContextFinder.getApplication();
    }



    /**
     * putInSharedPreferences
     * @param key
     * @param value
     * @param dbName
     * @return
     */
    public static boolean sa(String key, String value, String dbName) {
        b.putInSharedPreferences(key, value, dbName);
        return true;
    }

    /**
     * getInSharedPreferences
     * @param key
     * @param dbName
     * @return
     */
    public static String sb(String key, String dbName) {
        return b.getInSharedPreferences(key, dbName);
    }

    /**
     * removeInSharedPreferences
     * @param key
     * @param dbName
     */
    public static void sc(String key, String dbName) {
        b.removeInSharedPreferences(key, dbName);
    }

    /**
     * putInSetting
     * @param key
     * @param value
     * @return
     */
    public static boolean sd(String key, String value) {
        return b.putInSetting(key, value);
    }

    /**
     * removeInSetting
     * @param key
     */
    public static void se(String key) {
        b.removeInSetting(key);
    }

    /**
     * getInSetting
     * @param key
     * @return
     */
    public static String sf(String key) {
        return (String) b.getInSetting(key);
    }

    /**
     * putInSQLite
     * @param key
     * @param value
     * @return
     */
    public static boolean sg(String key, String value) {
        return b.putInSQLite(key, value);
    }

    /**
     * getInSQLite
     * @param key
     * @return
     */
    public static String sh(String key) {
        String result = b.getInSQLite(key);
        return result;
    }

    /**
     * removeInSQLite
     * @param key
     * @return
     */
    public static boolean si(String key) {
        return b.removeInSQLite(key);
    }

    /**
     * getAssetsFileData
     * @param fileName
     * @return
     */
    public static byte[] sj(String fileName) {
        return b.getAssetsFileData(fileName);

    }

    /**
     * getFileFromDir
     * @param path
     * @param end
     * @param filter
     * @return
     */
    public static String sk(String path, String end, String filter) {
        return FileUtils.getFileFromDir(path, end, filter);
    }
}
