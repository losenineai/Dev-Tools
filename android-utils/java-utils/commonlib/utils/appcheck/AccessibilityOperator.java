package com.auric.intell.commonlib.utils.appcheck;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.text.TextUtils;

import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import com.auric.intell.commonlib.utils.FileUtil;
import com.auric.intell.commonlib.utils.LogTool;

/**
 * Created by white on 15/10/27.
 */
public class AccessibilityOperator {

    private static final String TAG = "AccessibilityOperator";

    public static String mCurPackageName;
    public static String mCurClassName;
    public static String mLastPackageName;
    public static String mLastClassName;
    public static String mCurActivityPackageName;
    public static String mCurActivityClassName;
    public static boolean mIsServiceConnected = false;

    public static boolean mIsHasAccessibilityPermission = false;

    public static boolean mIsJumpOut = false;

    public static void checkIsHasAccessibilityPermission(Context context) {
//        mIsHasAccessibilityPermission = AccessibilityOperator.isAccessibilityServiceEnable(context);
    }


    public static void readJsonFromFile(Context context) {
        checkIsHasAccessibilityPermission(context);
        if (!mIsHasAccessibilityPermission) {
            return;
        }
        try {
            String jsonPath = String.format(XAccessibilityService.JSON_PATH, context.getPackageName());
            String content = FileUtil.readFile(new FileInputStream(jsonPath), FileUtil.CHARSET);

            JSONObject json = new JSONObject(content);
            if (json.has(XAccessibilityService.CUR_PKG_NAME)) {
                mCurPackageName = json.getString(XAccessibilityService.CUR_PKG_NAME);
            }
            if (json.has(XAccessibilityService.CUR_CLS_NAME)) {
                mCurClassName = json.getString(XAccessibilityService.CUR_CLS_NAME);
            }
            if (json.has(XAccessibilityService.CUR_ACTIVITY_PKG_NAME)) {
                mCurActivityPackageName = json.getString(XAccessibilityService.CUR_ACTIVITY_PKG_NAME);
            }
            if (json.has(XAccessibilityService.IS_CONNECT)) {
                mIsServiceConnected = json.getBoolean(XAccessibilityService.IS_CONNECT);
            }
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
        }
    }


    public static boolean isOnFloatView(String gamePackageName) {
        if (CheckAppRunningUtil.isDebugRunning) {
            LogTool.d(TAG, "isOnFloatView, mCurPackageName " + mCurPackageName + ",mCurClassName " + mCurClassName + ", mCurActivityPackageName " + mCurActivityPackageName);
        }
        if (TextUtils.isEmpty(mCurPackageName) || TextUtils.isEmpty(mCurClassName) || TextUtils.isEmpty(gamePackageName) || !mIsServiceConnected) {
            return false;
        }

        // 1、游戏的包名
        if (mCurPackageName.equals(gamePackageName)) {
            return true;
        }

        // 2、xmod和xx的包名,进入面板页面是FrameLayout
//        String selfPackageName = ContextFinder.getApplication().getPackageName();
//        if (!mIsJumpOut && !HomeWatcherReceiver.mIsHomeOut && mCurPackageName.equals(selfPackageName)
//                && mCurClassName.equals("android.widget.FrameLayout")) {
//            return true;
//        }
//
//        // 3、xmod和xx的包名,进入引导页，是RelativeLayout
//        // && mCurPackageName.equals(selfPackageName) && mCurClassName.equals("android.widget.RelativeLayout")
//        if (!mIsJumpOut && !HomeWatcherReceiver.mIsHomeOut && !TextUtils.isEmpty(mCurActivityPackageName)
//                && mCurActivityPackageName.equals(gamePackageName)) {
//            return true;
//        }

        return false;
    }

    /**
     * 是否在launcher界面
     * @return
     */
    public static boolean isOnLauncher(Context context) {
        return LollipopUtil.isOnLauncher(context, mCurActivityPackageName);
    }

    // 该接口还没测试
    public static void setAccessibilityPermission(Activity activity) {
        Intent intent = new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS);
        // TODO 是否有for result的东西
        activity.startActivityForResult(intent, 0);
    }

    public static boolean gotoAccessibilitySettings(Context context) {
        Intent settingsIntent = new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS);
        if (!(context instanceof Activity)) {
            settingsIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        }
        boolean isOk = true;
        try {
            context.startActivity(settingsIntent);
        } catch (Exception e) {
            isOk = false;
        }
        return isOk;
    }

    // To check if service is enabled
    public synchronized static boolean isAccessibilityServiceEnable(Context context, String serviceName) {
        if (context == null || TextUtils.isEmpty(serviceName)) {
            return false;
        }

        int accessibilityEnabled = 0;
        String pkg = context.getPackageName();
        final String service = pkg + "/" + serviceName;
        boolean accessibilityFound = false;
        try {
            accessibilityEnabled = Settings.Secure.getInt(
                    context.getApplicationContext().getContentResolver(),
                    Settings.Secure.ACCESSIBILITY_ENABLED);
            LogTool.d(TAG, "cur accessibilityEnabled = " + accessibilityEnabled);
        } catch (Exception e) {
            LogTool.e(TAG, "Error finding setting, default accessibility to not found: " + e.getMessage());
        }
        TextUtils.SimpleStringSplitter stringColonSplitter = new TextUtils.SimpleStringSplitter(':');

        if (accessibilityEnabled == 1) {
            LogTool.d(TAG, "***ACCESSIBILIY IS ENABLED*** -----------------");
            String settingValue = Settings.Secure.getString(
                    context.getApplicationContext().getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
            if (settingValue != null) {
                TextUtils.SimpleStringSplitter splitter = stringColonSplitter;
                splitter.setString(settingValue);
                while (splitter.hasNext()) {
                    String accessabilityService = splitter.next();
                    LogTool.d(TAG, "-------------- > accessabilityService :: " + accessabilityService);
                    if (accessabilityService.equalsIgnoreCase(service)) {
                        LogTool.d(TAG, "We've found the correct setting - accessibility is switched on!");
                        return true;
                    }
                }
            }
        } else {
            LogTool.d(TAG, "***ACCESSIBILIY IS DISABLED***");
        }

        return accessibilityFound;
    }


    /**
     * 设置无障碍服务打开,本方法只有系统app才有权限
     * @param context
     * @param pkgName
     * @param serviceName
     */
    public static void setAccessibilityServiceEnable(Context context, String pkgName, String serviceName, boolean enabled){
        if (context == null) {
            return;
        }

        LogTool.d(TAG, "setAccessibilityServiceEnable serviceName: " + serviceName + " enabled:" + enabled);

        Set<ComponentName> enabledServices = getEnabledServicesFromSettings(context);

        if (enabledServices == (Set<?>) Collections.emptySet()) {
            enabledServices = new HashSet<ComponentName>();
        }

        // Determine enabled services and accessibility state.
        ComponentName toggledService = ComponentName.unflattenFromString(pkgName + "/" + serviceName);
        boolean accessibilityEnabled = false;
        if (enabled) {
            enabledServices.add(toggledService);
            // Enabling at least one service enables accessibility.
            accessibilityEnabled = true;
        } else {
            enabledServices.remove(toggledService);
            // Check how many enabled and installed services are present.
            // TODO 引用了源码,详情看源码逻辑,本处不适用
//            Set<ComponentName> installedServices = AccessibilitySettings.sInstalledServices;
//            for (ComponentName enabledService : enabledServices) {
//                if (installedServices.contains(enabledService)) {
//                    // Disabling the last service disables accessibility.
//                    accessibilityEnabled = true;
//                    break;
//                }
//            }
        }

        // Update the enabled services setting.
        StringBuilder enabledServicesBuilder = new StringBuilder();
        // Keep the enabled services even if they are not installed since we
        // have no way to know whether the application restore process has
        // completed. In general the system should be responsible for the
        // clean up not settings.
        for (ComponentName enabledService : enabledServices) {
            enabledServicesBuilder.append(enabledService.flattenToString());
            enabledServicesBuilder.append(':');
        }
        final int enabledServicesBuilderLength = enabledServicesBuilder.length();
        if (enabledServicesBuilderLength > 0) {
            enabledServicesBuilder.deleteCharAt(enabledServicesBuilderLength - 1);
        }

        try {
            Settings.Secure.putString(context.getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES,
                    enabledServicesBuilder.toString());

            // Update accessibility enabled.
            Settings.Secure.putInt(context.getContentResolver(),
                    Settings.Secure.ACCESSIBILITY_ENABLED, accessibilityEnabled ? 1 : 0);
        } catch (Exception e) {
            e.printStackTrace();
        }


        LogTool.d(TAG, "ENABLED_ACCESSIBILITY_SERVICES : " + enabledServicesBuilder.toString());
        LogTool.d(TAG, "ACCESSIBILITY_ENABLED : " + accessibilityEnabled);
    }

    /**
     * 获取无障碍可用服务
     * @param context
     * @return
     */
    static Set<ComponentName> getEnabledServicesFromSettings(Context context) {
        final String enabledServicesSetting = Settings.Secure.getString(
                context.getContentResolver(), Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
        if (enabledServicesSetting == null) {
            return Collections.emptySet();
        }

        final Set<ComponentName> enabledServices = new HashSet<ComponentName>();
        final TextUtils.SimpleStringSplitter colonSplitter = new TextUtils.SimpleStringSplitter(':');
        colonSplitter.setString(enabledServicesSetting);

        while (colonSplitter.hasNext()) {
            final String componentNameString = colonSplitter.next();
            final ComponentName enabledService = ComponentName.unflattenFromString(
                    componentNameString);
            if (enabledService != null) {
                enabledServices.add(enabledService);
            }
        }

        return enabledServices;
    }

    public synchronized static String getTopActiPkgName(Context context){
        try {
            String jsonPath = String.format(XAccessibilityService.JSON_PATH, context.getPackageName());
            File file = new File(jsonPath);
            if (!file.exists()){
                return null;
            }
            String text = FileUtil.readFile(file, FileUtil.CHARSET);
            // json的值
            JSONObject json = new JSONObject(text);
            if (json.has(XAccessibilityService.CUR_ACTIVITY_PKG_NAME)) {
                return json.getString(XAccessibilityService.CUR_ACTIVITY_PKG_NAME);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }


    public synchronized static String getTopActiClassName(Context context){
        try {
            String jsonPath = String.format(XAccessibilityService.JSON_PATH, context.getPackageName());
            File file = new File(jsonPath);
            if (!file.exists()){
                return null;
            }
            String text = FileUtil.readFile(file, FileUtil.CHARSET);
            LogTool.d(TAG, "getTopActiClassName:"+text);

            // json的值
            JSONObject json = new JSONObject(text);
            if (json.has(XAccessibilityService.CUR_ACTIVITY_CLS_NAME)) {
                return json.getString(XAccessibilityService.CUR_ACTIVITY_CLS_NAME);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }


    public synchronized static boolean isTopActiClassName(Context context, String className){
        if (context == null || TextUtils.isEmpty(className)) {
            return false;
        }

        return TextUtils.equals(getTopActiClassName(context), className);
    }

    public synchronized static boolean isTopActiPkgName(Context context, String pkgName){
        if (context == null || TextUtils.isEmpty(pkgName)) {
            return false;
        }

        return TextUtils.equals(getTopActiPkgName(context), pkgName);
    }
}
