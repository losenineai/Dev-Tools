package com.xxlib.utils;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.util.List;

/**
 * Created by jammy on 16/5/27.
 */
public class ApkTool {

    private static final String TAG = "ApkTool";

    public static String getInstalledApkMd5(Context context, String pkgname) {
        long cur = System.currentTimeMillis();
        if (null == context || TextUtils.isEmpty(pkgname)) {
            return "";
        }
        String md5 = "";
        PackageManager pkgMgr = context.getPackageManager();
        List<ApplicationInfo> pkgs = pkgMgr.getInstalledApplications(PackageManager.GET_META_DATA);
        for (ApplicationInfo appInfo : pkgs) {
            if (appInfo.packageName.equals(pkgname)) {
                String apkPath = appInfo.sourceDir;
                LogTool.i(TAG, "pkgname " + pkgname + ", apkPath " + apkPath);
                String apkMd5 = MD5Helper.getFileMD5(new File(apkPath));
                LogTool.i(TAG, "apkMd5 " + apkMd5);
                md5 = apkMd5;
            }
        }
        LogTool.i(TAG, "getInstalledApkMd5 cost " + (System.currentTimeMillis() - cur));
        return md5;
    }


    public static String getRawApkPackageName(String apkPath) {
        try {
            long cur = System.currentTimeMillis();
            Context context = ApplicationUtils.getApplication();
            PackageManager pm = context.getPackageManager();
            PackageInfo info = pm.getPackageArchiveInfo(apkPath, PackageManager.GET_ACTIVITIES);
            LogTool.i(TAG, "getRawApkPackageName COST " + info.packageName + " " + (System.currentTimeMillis() - cur));
            return info.packageName;
        } catch (Exception e) {

        }
        return "";
    }
}
