package com.xxlib.utils;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;

import com.xxlib.utils.RegisterModule.FileUtil;
import com.xxlib.utils.base.Base64;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.XXTea;

import org.json.JSONObject;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Created by rosejames on 16/1/22.
 * 传入path返回一个已经赋值的DeviceInfo，里面的信息包括IMEI,DeviceID,ChannelID
 */
public class DeviceInfo {

    private static final String TAG = "DeviceInfo";
    public String mIMEI;
    public String mDeviceID;
    public String mChannelID;

    public static String sIMEI = "";
    public static String sDeviceID = "";
    public static String sChannelID = "";

    private String mDefaultChannelID;
    private static final String mDefaultIMEI = "Imei-xxAssistant";

    public static final String JSON_IMEI = "imei";
    public static final String JSON_DEVICE_ID = "deviceID";
    public static final String JSON_CHANNEL_ID = "channelID";


    /**
     * @param path 存储设备信息的路径
     */
    public DeviceInfo(String path, String defaultChannelID) {
        mDefaultChannelID = defaultChannelID;
        //从内存读取
        initFromMemory();
        //从文件读取
        initFromFile(path);
        //从系统读取
        initFromSystem();
        //保存信息
        saveDeviceInfoToFile(path);
    }

    /**
     * 从内存读取
     */
    private void initFromMemory() {
        mDeviceID = sDeviceID;
        mIMEI = sIMEI;
        mChannelID = sChannelID;

        LogTool.i(TAG, "initFromMemory:" + mIMEI + ", " + mDeviceID + ", " + mChannelID);
    }

    /**
     * 从系统读取
     *
     * @return
     */
    private void initFromSystem() {
        if (StringUtils.isEmpty(sIMEI) || sIMEI.equals(mDefaultIMEI)) {
            try {
                sIMEI = getIMEI(ApplicationUtils.getApplication());
            } catch (Throwable e) {
            }
        }

        if (StringUtils.isEmpty(sDeviceID)) {
            try {
                sDeviceID = getDeviceIDFromSys();
            } catch (Throwable e) {
            }
        }

        if (StringUtils.isEmpty(sChannelID)) {
            try {
                sChannelID = ApkCommentChannelIdUtils.getChannelId(ApplicationUtils.getApplication(), mDefaultChannelID);
            } catch (Throwable e) {
            }
        }

        mIMEI = sIMEI;
        mDeviceID = sDeviceID;
        mChannelID = sChannelID;

        LogTool.i(TAG, "initFromSystem:" + mIMEI + ", " + mDeviceID + ", " + mChannelID);
    }

    /**
     * 从文件读取
     *
     * @param path
     * @return
     */
    private void initFromFile(String path) {
        try {
            String jsonEncode = FileUtils.readFile(path, "utf-8").toString();
            byte[] baseJsonEncode = Base64.decode(jsonEncode);
            String jsonDecode = new String(XXTea.XXDoTeaDecrypt(baseJsonEncode, baseJsonEncode.length, LogTool.Key.getBytes()));
            JSONObject jsonObject = null;
            try {
                jsonObject = new JSONObject(jsonDecode);
            } catch (Throwable e) {
            }

            try {
                sIMEI = jsonObject.getString(JSON_IMEI);
            } catch (Throwable e) {
            }

            try {
                sDeviceID = jsonObject.getString(JSON_DEVICE_ID);
            } catch (Throwable e) {
            }

            try {
//                sChannelID = jsonObject.getString(JSON_CHANNEL_ID);
                //1.3.0更改渠道号从sp读取,直接注释掉  从 initFromSystem 读取
            } catch (Throwable e) {
            }

            mIMEI = sIMEI;
            mDeviceID = sDeviceID;
            mChannelID = sChannelID;

            LogTool.i(TAG, "initFromFile:" + mIMEI + ", " + mDeviceID + ", " + mChannelID);
        } catch (Exception e) {
        }
    }


    /**
     * 保存设备信息到文件
     *
     * @param path
     */
    private void saveDeviceInfoToFile(String path) {
        try {
            FileUtils.deleteFile(new File(path));

            JSONObject jsonObject = new JSONObject();
            jsonObject.put(JSON_IMEI, sIMEI);
            jsonObject.put(JSON_DEVICE_ID, sDeviceID);
            jsonObject.put(JSON_CHANNEL_ID, sChannelID);
            String jsonStr = jsonObject.toString();
            byte[] jsonEncodeByte = XXTea.XXTeaEncrypt(jsonStr.getBytes(), jsonStr.getBytes().length, LogTool.Key.getBytes());
            String jsonEncodeStr = Base64.encode(jsonEncodeByte);

            FileUtil.writeFile(jsonEncodeStr.getBytes(), path);
        } catch (Throwable e) {
        }
    }


    public static String getIMEI(Context context) {
        TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
        try {
            if (tm.getDeviceId() != null) {
                return tm.getDeviceId();
            } else {
                return mDefaultIMEI;
            }
        } catch (Exception e) {
            return mDefaultIMEI;
        }
    }

    public static String getDeviceIDFromSys() {
        LogTool.i(TAG, "getDeviceIDFromSys:" + sDeviceID);
        if (StringUtils.isEmpty(sDeviceID)) {
            synchronized (DeviceInfo.class) {
                if (StringUtils.isEmpty(sDeviceID)) {
                    String rawstr = getMacAddress() + getAndroidID() + getPseudoUniqueID();
                    LogTool.i(TAG, "rawStr :" + rawstr);
                    try {
                        MessageDigest e = MessageDigest.getInstance("SHA1");
                        byte[] rawbytes = e.digest(rawstr.getBytes());
                        sDeviceID = bytesToHexString(rawbytes);
                    } catch (NoSuchAlgorithmException var5) {
                        var5.printStackTrace();
                    }
                }
            }
        }
        LogTool.i(TAG, "getDeviceIDFromSys:" + sDeviceID);
        return sDeviceID;
    }

    private static String getPseudoUniqueID() {
        return Build.BOARD + Build.CPU_ABI + Build.CPU_ABI2 + Build.HARDWARE + Build.HOST + Build.MODEL + Build.DISPLAY + Build.DEVICE + Build.FINGERPRINT + Build.SERIAL;
    }

    private static String getMacAddress() {
        Context app = ContextFinder.getApplication();
        WifiManager wm = (WifiManager) app.getSystemService(Context.WIFI_SERVICE);
        BluetoothAdapter ba = BluetoothAdapter.getDefaultAdapter();
        return String.format("%s%s", wm.getConnectionInfo().getMacAddress(), ba != null ? ba.getAddress() : null);
    }

    private static String getAndroidID() {
        Context app = ContextFinder.getApplication();
        return Settings.Secure.getString(app.getContentResolver(), Settings.Secure.ANDROID_ID);
    }

    private static String bytesToHexString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }
        for (int i = 0; i < src.length; i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString();
    }


}
