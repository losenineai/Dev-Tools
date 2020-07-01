package com.xxlib.utils;

import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ProviderInfo;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;

import com.xxlib.utils.base.LogTool;

import java.util.List;

public class ShortCutTool {

	protected static final String TAG = "ShortCutTool";

	/**
	 * 
	 * @description 添加桌面启动项
	 * @param context
	 * @param tName
	 *            启动项名称
	 */
//	public static boolean add(Context context, String tName) {
//		if (hasShortcut(context))
//			return true;
//		try {
//			doAddShortCut(context, tName);
//		} catch (Exception e) {
//			LogTool.e(TAG, e.toString());
//		}
//
//		return true;
//	}

//	private static void doAddShortCut(Context context, String tName) {
//		// 安装的Intent
//		Intent shortcut = new Intent(
//				"com.android.launcher.action.INSTALL_SHORTCUT");
//
//		// 快捷名称
//		shortcut.putExtra(Intent.EXTRA_SHORTCUT_NAME, tName);
//		// 快捷图标是允许重复
//		shortcut.putExtra("duplicate", false);
//
//		Intent shortcutIntent = new Intent(
//				"android.intent.action.AppsLaunchActivity");
//		shortcutIntent
//				.addCategory("android.intent.category.AppsLaunchActivity");
//		shortcutIntent.setClassName("com.xxAssistant",
//				"com.xxAssistant.View.AppsLaunchActivity");
//		shortcutIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
//		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
//
//		// 快捷图标
//		ShortcutIconResource iconRes = Intent.ShortcutIconResource.fromContext(
//				context, R.drawable.icon_launcher);
//		shortcut.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, iconRes);
//
//		// 发送广播
//		context.sendBroadcast(shortcut);
//	}

	public static boolean hasShortcut(Context context) {

		String url = "";
		String authority = null;
		authority = getAuthorityFromPermission(context);

		boolean ret = false;
		try {
			if (authority == null) {
				if (getSystemVersion() < 8) {
					url = "content://com.android.launcher.settings/favorites?notify=true";
				} else {
					url = "content://com.android.launcher2.settings/favorites?notify=true";
				}
			} else
				url = "content://" + authority + "/favorites?notify=true";

			ContentResolver resolver = context.getContentResolver();
			Cursor cursor = resolver.query(Uri.parse(url), null,
					"iconResource=?",
					new String[] { "com.xxAssistant:drawable/icon_launcher" },
					null);
			if (cursor != null) {
				if (cursor.moveToNext() && cursor.getCount() >= 1) {
					ret = true;
				}
				cursor.close();
			}
		} catch (Exception e) {
			LogTool.e(TAG, e.toString());
		}

		return ret;
	}

	/**
	 * 获取系统的SDK版本号
	 * 
	 * @return
	 */
	private static int getSystemVersion() {
		return Build.VERSION.SDK_INT;
	}

	/**
	 * 通过访问权限获取provider
	 * 
	 * @param context
	 * @param permission
	 * @return 通过这个方法遍历，会有1-2S的延时，循环可能有100多次，所以应该把返回的数据保存到文件、数据库中
	 */
	public static String getAuthorityFromPermission(Context context) {

		String permission = "android.launcher.permission.READ_SETTINGS";

		try {
			List<PackageInfo> packs = context.getPackageManager()
					.getInstalledPackages(PackageManager.GET_PROVIDERS);

			if (packs != null) {
				for (PackageInfo pack : packs) {
					ProviderInfo[] providers = pack.providers;
					if (providers != null) {
						for (ProviderInfo provider : providers) {
							if (provider.readPermission == null)
								continue;
							if (provider.readPermission.contains(permission)) {
								if (hasPermission(context,
										provider.readPermission))
									return provider.authority;
							}
							if (provider.writePermission == null)
								continue;
							if (provider.writePermission.contains(permission)) {
								if (hasPermission(context,
										provider.writePermission))
									return provider.authority;
							}
						}
					}
				}
			}
		} catch (Exception e) {

		}
		return null;
	}

	public static void delShortcut(Context context, String name) {
		Intent shortcut = new Intent(
				"com.android.launcher.action.UNINSTALL_SHORTCUT");

//		// 快捷方式的名称
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_NAME, name);

		
		Intent shortcutIntent = new Intent(
				"android.intent.action.AppsLaunchActivity");
		shortcutIntent
				.addCategory("android.intent.category.AppsLaunchActivity");
		shortcutIntent.setClassName("com.xxAssistant",
				"com.xxAssistant.View.AppsLaunchActivity");
		shortcutIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		shortcut.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);

		context.sendBroadcast(shortcut);

	}

	/**
	 *
	 * @description 判断APP是否拥有该权限
	 * @param context
	 * @param permissionName
	 * @return
	 */
	private static boolean hasPermission(Context context, String permissionName) {
		try {
			PackageManager pm = context.getPackageManager();
			PackageInfo pi = pm.getPackageInfo(context.getPackageName(), 0);
			// 得到自己的包名
			String pkgName = pi.packageName;

			PackageInfo pkgInfo = pm.getPackageInfo(pkgName, PackageManager.GET_PERMISSIONS);// 通过包名，返回包信息
			String sharedPkgList[] = pkgInfo.requestedPermissions;// 得到权限列表

			for (int i = 0; i < sharedPkgList.length; i++) {
				String permName = sharedPkgList[i];
				if (permissionName.equals(permName))
					return true;
			}
			return false;
		} catch (PackageManager.NameNotFoundException e) {
			return false;
		}
	}
}
