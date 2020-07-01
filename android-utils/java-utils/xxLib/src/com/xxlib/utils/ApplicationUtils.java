package com.xxlib.utils;

import android.app.Application;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * Created by rosejames on 15/8/25.
 */
public class ApplicationUtils {

    public static Application sApplication;
    private static Application sYourApplication; // 当前app所处的application
    public static Context sSDKContext;
    public static int test;

    /**
     * 保存当前应用的application，在getApplication为null时使用
     * @param application your Application
     */
    public static void setYourApplication(Application application) {
        if (application == null) {
            return;
        }
        sYourApplication = application;
    }

    public static Application getApplication() {
        if (sApplication != null) {
            return sApplication;
        }
        //通过反射拿到application
        try {
            Class<?> clsRuntimeInit = Class.forName("com.android.internal.os.RuntimeInit");
            Field fieldRuntimeInit = clsRuntimeInit.getDeclaredField("mApplicationObject");
            fieldRuntimeInit.setAccessible(true);
            Object objApplicationThread = fieldRuntimeInit.get(clsRuntimeInit);

            Class<?> clsApplicationThread = Class.forName("android.app.ActivityThread$ApplicationThread");
            Field fieldApplicationThread = clsApplicationThread.getDeclaredField("this$0");
            fieldApplicationThread.setAccessible(true);
            Object objActivityThread = fieldApplicationThread.get(objApplicationThread);

            Class<?> clsActivityThread = Class.forName("android.app.ActivityThread");
            Method methodActivityThread = clsActivityThread.getMethod("getApplication", new Class[]{});
            methodActivityThread.setAccessible(true);
            sApplication = (Application) methodActivityThread.invoke(objActivityThread, new Object[]{});
            if (sApplication == null && sYourApplication != null) {
                return sYourApplication;
            }
            return sApplication;
        } catch (Exception e) {
            e.printStackTrace();
            return sYourApplication;
        }
    }

    public static String getAppVersionName(Context context) {
        String versionName = "";
        try {
            // 获取软件版本名称
            versionName = context.getPackageManager().getPackageInfo(context.getPackageName(), 1).versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return versionName;
    }

    public static int getAppVersionCode(Context context) {
        try {
            return context.getPackageManager().getPackageInfo(context.getPackageName(), 0).versionCode;
        }catch(Exception e){
            return 0;
        }
    }

    public static String getApplicationName() {
        String applicationName = "";
        try {
            PackageManager packageManager = null;
            ApplicationInfo applicationInfo;
            packageManager = ApplicationUtils.getApplication().getPackageManager();
            applicationInfo = packageManager.getApplicationInfo(ApplicationUtils.getApplication().getPackageName(), 0);
            applicationName =
                    (String) packageManager.getApplicationLabel(applicationInfo);
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return applicationName;
    }
}
