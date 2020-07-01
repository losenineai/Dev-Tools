 package com.xxlib.utils;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Base64;

import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * 获取设备唯一码
 * 
 * @author boyliang
 * 
 */
public final class DeviceIDHelper {

    private static final String TAG = "DeviceIDHelper";

	private static volatile String sDeviceID = null;

    private static final String KEY = "%device(";
    private static final String FILE_NAME = ".device_info";

	private DeviceIDHelper() {

	}

    private static byte[] iv = { 's','t','u','v','w','x','y','z' };
    public static String encryptDES(String encryptString, String encryptKey)
            throws Exception {
        IvParameterSpec zeroIv = new IvParameterSpec(iv);
        SecretKeySpec key = new SecretKeySpec(encryptKey.getBytes(), "DES");
        Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, key, zeroIv);
        byte[] encryptedData = cipher.doFinal(encryptString.getBytes());
        return new String(Base64.encode(encryptedData, Base64.DEFAULT), "UTF-8");
    }

    public static String decryptDES(String decryptString, String decryptKey)
            throws Exception {
        byte[] byteMi = Base64.decode(decryptString.getBytes("UTF-8"), Base64.DEFAULT);
        IvParameterSpec zeroIv = new IvParameterSpec(iv);
        SecretKeySpec key = new SecretKeySpec(decryptKey.getBytes(), "DES");
        Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
        cipher.init(Cipher.DECRYPT_MODE, key, zeroIv);
        byte decryptedData[] = cipher.doFinal(byteMi);

        return new String(decryptedData);
    }

	private static String getIMEI() {
		Context app = ContextFinder.getApplication();
		TelephonyManager tm = (TelephonyManager) app.getSystemService(Context.TELEPHONY_SERVICE);
		String imei = tm.getDeviceId();
		imei = imei != null && !"".equals(imei) ? imei : "";

		// 循环读10次，如果发现imei有所变化，证明imei是不稳定的，则返回""
		for (int i = 0; i < 10; i++) {
			String other_imei = tm.getDeviceId();
			if (other_imei != null && !other_imei.equals(imei)) {
				imei = "";
				break;
			}
		}

		return imei;
	}

	private static String getPseudoUniqueID() {
		return Build.BOARD + Build.CPU_ABI + Build.CPU_ABI2 + Build.HARDWARE + Build.HOST + Build.MODEL + Build.DISPLAY + Build.DEVICE + Build.FINGERPRINT + Build.SERIAL;
	}

	private static String getMacAddress() {
		Context app = ContextFinder.getApplication();
		WifiManager wm = (WifiManager) app.getSystemService(Context.WIFI_SERVICE);
		BluetoothAdapter ba = BluetoothAdapter.getDefaultAdapter();
        String bluetoothAdapterAddress = null;
        try {
            if (ba != null) {
                bluetoothAdapterAddress = ba.getAddress();
            }
        } catch (Exception e) {
            LogTool.w(TAG, e);
        }
        LogTool.i(TAG, "bluetooth Adapter Address " + bluetoothAdapterAddress);
		return String.format("%s%s", wm.getConnectionInfo().getMacAddress(), bluetoothAdapterAddress);
	}

	private static String getAndroidID() {
		Context app = ContextFinder.getApplication();
		return Secure.getString(app.getContentResolver(), Secure.ANDROID_ID);
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


    private static String getDeviceIDFromSys(){
        LogTool.i(TAG, "getDeviceIDFromSys" + sDeviceID);
        if (sDeviceID == null) {
            synchronized (DeviceIDHelper.class) {
                if (sDeviceID == null) {
                    String rawstr = getMacAddress() + getAndroidID() + getPseudoUniqueID();

                    try {
                        MessageDigest digest = MessageDigest.getInstance("SHA1");
                        byte[] rawbytes = digest.digest(rawstr.getBytes());
                        sDeviceID = bytesToHexString(rawbytes);
                    } catch (NoSuchAlgorithmException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        LogTool.i(TAG, "getDeviceIDFromSys" + sDeviceID);

        return sDeviceID;
    }


    /**
     * 加密方式存贮，需要解密
     * @param file
     * @return
     */
    private static String getDeviceIDFromFile(File file){
        if(file == null || !file.exists()){
            return null;
        }

        try{
            String fileStr = FileUtils.readFile(file, FileUtils.CHARSET);
            String decryptStr = decryptDES(fileStr, KEY);
            LogTool.i(TAG, "getDeviceIDFromFile " + decryptStr);
            return decryptStr;
        }catch(Exception er){
            er.printStackTrace();
            LogTool.e(TAG, LogTool.getStackTraceString(er));
        }
        return null;
    }

    private static void saveDeviceID(String did, File file){
        try{
            FileUtils.createDir(file.getParent());
            String encryptStr = encryptDES(did, KEY);
            FileUtils.saveContent(encryptStr, file, FileUtils.CHARSET);
            LogTool.i(TAG, "saveDeviceID " + encryptStr);
        }catch(Exception er){
            er.printStackTrace();
            LogTool.e(TAG, LogTool.getStackTraceString(er));
        }
    }

    private static String loadDeviceID(boolean isFromSdk){
        File deviceFile = null;
        // sdcard
        if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){
            deviceFile = new File(Environment.getExternalStorageDirectory() + (isFromSdk ? "/guopan/sdk/" : "/") + ApplicationUtils.getApplication().getPackageName(), FILE_NAME);
        }
        // /data/data
        else{
            deviceFile = new File(ApplicationUtils.getApplication().getFilesDir(), FILE_NAME);
        }

        if(!deviceFile.exists() || !deviceFile.isFile()){
            saveDeviceID(getDeviceIDFromSys(), deviceFile);
        }

        LogTool.i(TAG, "loadDeviceID " + deviceFile.getAbsolutePath());
        return getDeviceIDFromFile(deviceFile);
    }

    private static String loadDeviceID(){
        return loadDeviceID(false);
    }


	public static String getDeviceID() {
        return getDeviceID(false);
	}

    public static String getDeviceID(boolean isFromSdk) {
        String did = loadDeviceID(isFromSdk);
        if(TextUtils.isEmpty(did)){
            did = getDeviceIDFromSys();
        }
        LogTool.i(TAG, "getDeviceID " + did);
        return did;
    }

}
