package com.xxlib.utils;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;

/**
 * 检测指定包名的app是否安装的工具类
 * 
 * @author linbin 2014
 */
public class CheckAppInstalled {

	/**
	 * 检测指定包名和版本的APP是否安装
	 * 
	 * @param context
	 * @param packageName
	 * @param version
	 * @return
	 */
	public static boolean check(Context context, String packageName,
			String version) {
		PackageInfo packageInfo;
		try {

			packageInfo = context.getPackageManager().getPackageInfo(

			packageName, 0);

		} catch (NameNotFoundException e) {

			packageInfo = null;


		}
		if (packageInfo == null) {

			return false;

		} else {
			if (packageInfo.versionName.equals(version))
				return true;
			else
				return false;
		}
	}

	// public static boolean check(Context context,String packageName,String
	// version ,XXSoftDataV2 soft)
	// {
	// PackageInfo packageInfo;
	// try {
	//
	// packageInfo = context.getPackageManager().getPackageInfo(
	//
	// packageName, 0);
	//
	// } catch (NameNotFoundException e) {
	//
	// packageInfo = null;
	//
	// e.printStackTrace();
	//
	// }
	//
	// if(packageInfo ==null){
	//
	// return false;
	//
	// }else{
	// if(packageInfo.versionName.compareTo(soft.getGameObject().getSbInfo().getVersion())>=0)
	// {
	// return true;
	// }
	// else
	// {
	// ToolActivity.mUpdateToolMap.put(packageName, soft);
	// return true;
	// }
	// }
	// }

	/**
	 * 检测指定包名的APP是否需要安装
	 * 
	 * @param context
	 * @param packageName
	 * @return
	 */
	public static boolean check(Context context, String packageName) {
		PackageInfo packageInfo;
		try {

			packageInfo = context.getPackageManager().getPackageInfo(

			packageName, 0);

		} catch (NameNotFoundException e) {

			packageInfo = null;

		}

		if (packageInfo == null) {

			return false;

		} else {
			return true;
		}
	}

	/**
	 * 检测指定包名和版本的APP是否需要安装（新版才需要安装）
	 * 
	 * @param context
	 * @param packageName
	 * @param version
	 * @return
	 */
	public static boolean checkGameNeedDownload(Context context,
			String packageName, String version) {
		PackageInfo packageInfo;
		try {

			packageInfo = context.getPackageManager().getPackageInfo(

			packageName, 0);

		} catch (NameNotFoundException e) {

			packageInfo = null;


		}
		if (packageInfo == null) {

			return false;

		} else {
			if (packageInfo.versionName.compareTo(version) >= 0)
				return true;
			else
				return false;
		}
	}

    /**
     * 检查是否有更新
     *
     * @return true表示有更新
     */
    public static boolean checkIsHasUpdate(Context context, String packageName, String version, String softVersion) {
        PackageInfo packageInfo;
        try {
            packageInfo = context.getPackageManager().getPackageInfo(packageName, 0);

            if (packageInfo == null) {
                return false;
            } else {
                String[] oldNumbers = packageInfo.versionName.split("\\.");
                String[] newNumbers = softVersion.split("\\.");

                // 取最短长度
                int compareLength = Math.min(oldNumbers.length, newNumbers.length);
                boolean hasUpdate = false;

                for (int i = 0; i < compareLength; ++i) {
                    int oldVersion = Integer.valueOf(oldNumbers[i]);
                    int newVersion = Integer.valueOf(newNumbers[i]);

                    if (oldVersion != newVersion) {
                        hasUpdate = newVersion > oldVersion;
                        break;
                    }
                }
                if (!hasUpdate && oldNumbers.length != newNumbers.length) {
                    hasUpdate = newNumbers.length > oldNumbers.length;
                }

                return hasUpdate;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return false;
    }

}
