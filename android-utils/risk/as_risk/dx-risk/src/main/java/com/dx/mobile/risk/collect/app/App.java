package com.dx.mobile.risk.collect.app;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.*;
import android.os.Build;

import com.dx.mobile.risk.BuildConfig;
import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.StringUtils;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class App {

    public static final String PackageInfoLock = "lock";

    public static String getAppVersion() {
        try {
            Context ctx = ContextFinder.getApplication();
            PackageInfo pi = null;
            // add synchroized for "Package manager has died"
            synchronized (PackageInfoLock) {
                pi = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
            }
            return pi != null ? pi.versionName : null;
        } catch (Throwable e) {
//            e.printStackTrace();
        }

        return "";
    }


    public static int getAppVersionCode() {
        Context ctx = ContextFinder.getApplication();
        PackageInfo pi = null;

        try {
            // add synchroized for "Package manager has died"
            synchronized (PackageInfoLock) {
                pi = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
            }
        } catch (Throwable e) {
//            e.printStackTrace();
        }

        return pi != null ? pi.versionCode : -1;
    }

    public static String getPkgName() {
        try {
            Context ctx = ContextFinder.getApplication();
            return ctx.getPackageName();
        } catch (Throwable t) {

        }
        return "";
    }

    public static String getSDKVersion() {
        return BuildConfig.VERSION_NAME;
    }

    public static String getWorkDir() {
        try {
            Context ctx = ContextFinder.getApplication();
            File file_dir = ctx.getFilesDir();
            File work_dir = new File(file_dir, "stee");

            if (!work_dir.exists()) {
                work_dir.mkdirs();
            }

            return work_dir.getAbsolutePath();
        } catch (Throwable t) {

        }

        return "";
    }

    public synchronized static String getAppList() {
        final int limit = 2000;
        String result = "";
        long start = System.currentTimeMillis();
        try {
            JSONArray appArr = new JSONArray();
            Context ctx = ContextFinder.getApplication();
            PackageManager pm = ctx.getPackageManager();

            List<PackageInfo> pkgList = pm.getInstalledPackages(PackageManager.GET_SIGNATURES);

            Collections.sort(pkgList, new Comparator<PackageInfo>() {
                @Override
                public int compare(PackageInfo o1, PackageInfo o2) {
                    if (o1 == null || o2 == null) {
                        return 0;
                    }

                    // 从大到小排序
//                    Log.d("white", "getAppList sort：" + (int)(o2.lastUpdateTime - o1.lastUpdateTime));
                    return (int)(o2.lastUpdateTime - o1.lastUpdateTime);
                }
            });

            int index = 0;
            // 数量限制
            final int max = 40;

            for (PackageInfo info : pkgList) {
                int appType = (info.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) > 0 ? 1 : 2;
                JSONObject json = new JSONObject();
                json.put("name", info.applicationInfo.loadLabel(pm).toString());
                json.put("pkg_name", info.packageName);
                json.put("category", appType);
                json.put("install_time", info.lastUpdateTime);
//                Log.d("white", "getAppList json：" + json);
                appArr.put(json);
                index++;

                if (index >= max) {
                    break;
                }
            }

            result = appArr.toString();
        } catch (Throwable throwable) {
        }


//        Log.d(TAG, "getAppList result：" + result);

        return result;
    }

    public static String getAppInfo() {

        Context ctx = ContextFinder.getApplication();
        JSONObject app_info = new JSONObject();

        // add synchroized for "Package manager has died"
        synchronized (PackageInfoLock) {

            PackageManager pm = ctx.getPackageManager();

            String packname = ctx.getPackageName();
            JSONUtils.jsonPut(app_info, "K2", packname);

            try {

                ApplicationInfo info = pm.getApplicationInfo(packname, 0);
                JSONUtils.jsonPut(app_info, JNIConst.KEY_APP_KOCK_INDEX2, JNIConst.VALUE_APP_KOCK);
                JSONUtils.jsonPut(app_info, "K3", info.loadLabel(pm).toString());

                PackageInfo packinfo = pm.getPackageInfo(packname, 0);
                JSONUtils.jsonPut(app_info, "K4", packinfo.versionName);

                JSONUtils.jsonPut(app_info, "K6", getProcessName());
                JSONUtils.jsonPut(app_info, "K7", getSignMd5Str());

                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) {
                    JSONUtils.jsonPut(app_info, "K850", packinfo.versionCode);
                    JSONUtils.jsonPut(app_info, "K851", packinfo.firstInstallTime);
                    JSONUtils.jsonPut(app_info, "K852", packinfo.lastUpdateTime);
                }

                JSONUtils.jsonPut(app_info, "K853", info.flags);

                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.FROYO) {
                    JSONUtils.jsonPut(app_info, "K854", ActivityManager.isUserAMonkey() ? "1" : 0);
                }

                { // launcher
                    Intent intent = new Intent(Intent.ACTION_MAIN);
                    intent.addCategory(Intent.CATEGORY_LAUNCHER);

                    ResolveInfo resolveInfo = pm.resolveActivity(intent, 0);
                    ActivityInfo activityInfo = resolveInfo.activityInfo;

                    JSONUtils.jsonPut(app_info, "K855", activityInfo.packageName);
                }

            } catch (Throwable e) {
//                e.printStackTrace();
            }
        }

        return app_info.toString();
    }

    public static String getProcessName() {
        BufferedReader cmdlineReader = null;
        try {
            cmdlineReader = new BufferedReader(new InputStreamReader(new FileInputStream("/proc/" + android.os.Process.myPid() + "/cmdline"), "iso-8859-1"));
            int c;
            StringBuilder processName = new StringBuilder();
            while ((c = cmdlineReader.read()) > 0) {
                processName.append((char) c);
            }
            return processName.toString();
        } catch (Throwable ignore) {
        } finally {
            try {
                if (cmdlineReader != null) {
                    cmdlineReader.close();
                }
            } catch (Throwable ignore) {
            }
        }
        return "";
    }

    /**
     * 获取app签名md5值,与“keytool -list -keystore D:\Desktop\app_key”‘keytool -printcert     *file D:\Desktop\CERT.RSA’获取的md5值一样
     */
    public static String getSignMd5Str() {
        try {
            Context ctx = ContextFinder.getApplication();
            PackageInfo packageInfo = ctx.getPackageManager().getPackageInfo(
                    ctx.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signs = packageInfo.signatures;
            Signature sign = signs[0];
            return StringUtils.md5(sign.toByteArray());
        } catch (Throwable e) {
//            e.printStackTrace();
        }
        return "";
    }

    public static String getPackageResourcePath() {
        return Build.VERSION.SDK_INT > Build.VERSION_CODES.FROYO ?
                ContextFinder.getApplication().getPackageResourcePath() : "";
    }

}
