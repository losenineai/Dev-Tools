package com.auric.intell.commonlib.utils;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.os.Bundle;
import android.text.TextUtils;
import android.net.Uri;

import java.util.List;


/**
 * Created by white on 16/7/21.
 */
public class StartAppUtil {
    private static final String TAG = "StartAppUtil";

    /**
     *
     学知识：cn.alpha.intell.auric.knowledge
     视频聊天：cn.alpha.intell.auric.telephone
     听音乐：cn.alpha.intell.auric.music
     看动画：cn.alpha.intell.auric.video
     讲故事：cn.alpha.intell.auric.story
     设置：cn.alpha.intell.auric.setting
     机器人：cn.alpha.intell.auric.playground
     桌面：cn.alpha.intell.auric.launcher
     */
    public static final String PKG_NAME_VIDEO = "cn.alpha.intell.auric.video";
    public static final String PKG_NAME_KNOWLEDGE = "cn.alpha.intell.auric.knowledge";
    public static final String PKG_NAME_MUSIC = "cn.alpha.intell.auric.music";
    public static final String PKG_NAME_TELEPHONE = "cn.alpha.intell.auric.telephone";
    public static final String PKG_NAME_STORY = "cn.alpha.intell.auric.newstory";
    public static final String PKG_NAME_SETTING = "cn.alpha.intell.auric.setting";
    public static final String PKG_NAME_ROBOTX = "cn.alpha.intell.auric.robotx";
    public static final String PKG_NAME_LAUNCHER = "cn.alpha.intell.auric.launcher";
    public static final String PKG_NAME_CAMERA = "cn.alpha.intell.auric.camera";
    public static final String PKG_NAME_SECURITY = "com.cleanmaster.mguard";
    public static final String CLASS_NAME_LAUNCHER_ACTIVITY = "org.cocos2dx.javascript.AppActivity";
    public static final String CLASS_NAME_TELEPHONE_ACTIVITY = "cn.alpha.intell.auric.telephone.ui.avchat.AVCallActivity";

    /**
     * 通用 Bundle key
     */
    public static final String KEY_INT_SETTINGS_TAB = "KEY_INT_SETTINGS_TAB";
    public static final String KEY_BOOLEAN_OTA_FORCE = "KEY_BOOLEAN_OTA_FORCE";

    public static boolean startAppByPkgName(String packagename, Bundle extras) {
        Intent intent = null;
        Context context = ContextFinder.getApplication();
        PackageManager packageManager = context.getPackageManager();
        intent = packageManager.getLaunchIntentForPackage(packagename);
        if (intent == null){
            intent = new Intent();
        }
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        if(extras!=null){
            intent.putExtras(extras);
        }

        try{
            LogTool.d(TAG, "pkg:"+packagename + " extras:"+extras);
            context.startActivity(intent);
            return true;
        }catch (Exception e){
            e.printStackTrace();
            LogTool.d(TAG, LogTool.getStackTraceString(e));
            return false;
        }
    }

    public static boolean startAppByPkgName(String packagename, Uri uri) {
        Intent intent = null;
        Context context = ContextFinder.getApplication();
        PackageManager packageManager = context.getPackageManager();
        intent = packageManager.getLaunchIntentForPackage(packagename);
        if (intent == null){
            intent = new Intent();
        }
        if (uri != null) {
            intent.setData(uri);
        }
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        try{
            context.startActivity(intent);
            return true;
        }catch (Exception e){
            e.printStackTrace();
            LogTool.d(TAG, LogTool.getStackTraceString(e));
            return false;
        }
    }


    public static boolean startAppByClassName(String pkgName, String className, Bundle bundle){
        if(TextUtils.isEmpty(className)){
            return false;
        }
        try{
            Context context = ContextFinder.getApplication();
            Intent intent = new Intent();
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            intent.putExtras(bundle);
            ComponentName componentName = new ComponentName(pkgName, className);
            intent.setComponent(componentName);
            context.startActivity(intent);
            return true;
        }catch(Exception e){
            e.printStackTrace();
            return false;
        }
    }


    public static void startService(String action, String packageName){
        Intent testActivityIntent = new Intent();
        testActivityIntent.setAction(action) ;
        testActivityIntent.setPackage(packageName);
        ContextFinder.getApplication().stopService(testActivityIntent);
        ContextFinder.getApplication().startService(testActivityIntent);
    }

    public static void playSound(String content){
        Intent intent = new Intent();
        intent.setAction("cn.alpha.intent.action.SOUND");
        intent.putExtra("content", content);
        ContextFinder.getApplication().sendBroadcast(intent);
    }


    public static void setLoginResult(boolean result){
        Intent intent = new Intent();
        intent.setAction("cn.alpha.intent.action.LOGIN_RESULT");
        intent.putExtra("result", result);
        ContextFinder.getApplication().sendBroadcast(intent);
    }

    public static boolean startAppForTelephone(Bundle bundle){
        return startAppByClassName(PKG_NAME_TELEPHONE, CLASS_NAME_TELEPHONE_ACTIVITY, bundle);
    }



   public static Intent getAppOpenIntentByPackageName(Context context,String packageName){
        // MainActivity完整名
        String mainAct = null;
        // 根据包名寻找MainActivity
        PackageManager pkgMag = context.getPackageManager();
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.setFlags(Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED|Intent.FLAG_ACTIVITY_NEW_TASK);

        List<ResolveInfo> list = pkgMag.queryIntentActivities(intent,
                PackageManager.GET_ACTIVITIES);
        for (int i = 0; i < list.size(); i++) {
            ResolveInfo info = list.get(i);
            if (info.activityInfo.packageName.equals(packageName)) {
                mainAct = info.activityInfo.name;
                break;
            }
        }
        if (TextUtils.isEmpty(mainAct)) {
            return null;
        }
        intent.setComponent(new ComponentName(packageName, mainAct));
        return intent;
    }



    public static Context getPackageContext(Context context, String packageName) {
        Context pkgContext = null;
        if (context.getPackageName().equals(packageName)) {
            pkgContext = context;
        } else {
            // 创建第三方应用的上下文环境
            try {
                pkgContext = context.createPackageContext(packageName,
                        Context.CONTEXT_IGNORE_SECURITY
                                | Context.CONTEXT_INCLUDE_CODE);
            } catch (PackageManager.NameNotFoundException e) {
                e.printStackTrace();
            }
        }
        return pkgContext;
    }

    public static boolean openPackage(Context context, String packageName) {
        Context pkgContext = getPackageContext(context, packageName);
        Intent intent = getAppOpenIntentByPackageName(context, packageName);
        if (pkgContext != null && intent != null) {
            pkgContext.startActivity(intent);
            return true;
        }
        return false;
    }
}
