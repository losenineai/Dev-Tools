package com.xxlib.utils;

import android.content.Context;
import android.os.Build;
import android.telephony.TelephonyManager;

import com.xxlib.utils.base.LogTool;


public class PhoneInfoHelper {
	
	public static final String TAG = "PhoneInfoHelper";
	public static String IMEI = "";
	public static String IMSI = "";
	
	
	public static void initInfo(Context context){
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		IMEI = tm.getDeviceId();
		IMSI = tm.getSubscriberId();
		if (IMEI == null || IMEI.length() == 0) {
			IMEI = "Imei-GPApplication";
		}
		LogTool.i(TAG, "imei is " + IMEI);
	}
	
	
	public static String getImei(Context context){
		if (StringUtils.isEmpty(IMEI)) {
			initInfo(context);
		}
		return IMEI;
	}

	public static String getImsi(Context context) {
		if (StringUtils.isEmpty(IMSI)) {
			initInfo(context);
		}
		return IMSI;
	}

	/**
	 * 返回MCC+ MNC
	 *
	 * @param context
	 * @return 46002 (返回 0 代表 没检测到 运营商)
	 */
	public static int getSimOperator(Context context) {
		TelephonyManager telephonyManager = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		try {
			if (telephonyManager.getSimOperator() == null
					|| telephonyManager.getSimOperator().equals("")) {
				return 0;
			} else {

				return Integer.valueOf(telephonyManager.getSimOperator());
			}
		} catch (NumberFormatException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return 0;

	}

	public static int getSysVerCode() {
		return Build.VERSION.SDK_INT;
	}

	public static String getSysVerName() {
		return Build.VERSION.RELEASE;
	}


    public static String getSIMNumber(Context context){
        if(context == null){
            return "";
        }
        TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
        String tel = tm.getLine1Number();
        if(tel != null && tel.indexOf("+86") > -1){
            tel = tel.substring(3);
        }
        if(StringUtils.isLegalPhoneNum(tel)){
            return tel;
        }
        return "";
    }
}
