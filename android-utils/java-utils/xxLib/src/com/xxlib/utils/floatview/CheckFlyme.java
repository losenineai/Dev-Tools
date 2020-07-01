package com.xxlib.utils.floatview;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

import com.xxlib.utils.base.LogTool;

public class CheckFlyme extends CheckEMUI {
	private static final String TAG = "CheckEMUI";
	
	/**
	 * 直接跳转到设置“弹框消息”权限那个界面
	 */
	public static boolean openFlymeFloatPermissionDirectByFlyme3(Context c) {
		try {
			LogTool.i(TAG, "openFlymeFloatPermissionDirect");
			Intent intent = new Intent();
			intent.setClassName("com.android.settings", "com.android.settings.Settings$AppControlSettingsActivity");
			intent.putExtra("packageName", c.getPackageName());
			c.startActivity(intent);
			
			return true;
		} catch (Exception e) {
			LogTool.w(TAG, "openFlymeFloatPermissionDirect ex " + e.toString());
		}
		return false;
	}
	
	/**
	 * 直接跳转到设置“弹框消息”权限那个界面
	 */
	public static boolean openFlymeFloatPermissionDirectByFlyme4(Context c) {
		try {
			LogTool.i(TAG, "openFlymeFloatPermissionDirect");
			Intent intent = new Intent();
			intent.setClassName("com.meizu.safe", "com.meizu.safe.security.AppSecActivity");
			intent.putExtra("packageName", c.getPackageName());
			c.startActivity(intent);
			
			return true;
		} catch (Exception e) {
			LogTool.w(TAG, "openFlymeFloatPermissionDirect ex " + e.toString());
		}
		return false;
	}
	

	/**
	 * 跳转到应用程序管理的xx入口那里，需要再进入一级才是悬浮窗权限设置
	 */
	public static boolean openFlymeFloatPermission(Context c) {
		try {
			LogTool.i(TAG, "openFlymeFloatPermission");
			Intent intent = new Intent(android.provider.Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
			Uri uri = Uri.fromParts("package", "com.xxAssistant", null);
			intent.setData(uri);
			c.startActivity(intent);
			return true;
		} catch (Exception e) {
			LogTool.w(TAG, "openFlymeFloatPermission ex " + e.toString());
		}
		return false;
	}
	
	private static final String KEY_Flyme_VERSION = "ro.build.display.id";

	public static boolean isFlymeUI() {
		try {
			//Flyme OS 4.2.2.2U
			final BuildProperties prop = BuildProperties.newInstance();
			return prop.getProperty(KEY_Flyme_VERSION, null).contains("Flyme") || prop.getProperty(KEY_Flyme_VERSION, null).contains("flyme");
		} catch (final Exception e) {
			return false;
		}
	}
	
	
	public static int getFlymeVersion(){
		try {
			//Flyme OS 4.2.2.2U
			final BuildProperties prop = BuildProperties.newInstance();
			if(isFlymeUI()){
				String versionString = prop.getProperty(KEY_Flyme_VERSION, null);
				int version = 	Integer.valueOf(versionString.substring(9,10));
				Log.e("versionString", version + "");
				return version;
			}else{
				return -1;
			}
		} catch (Exception e) {
			return -1;
		}
		
	}
	
}
