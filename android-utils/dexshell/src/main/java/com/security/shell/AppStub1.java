package com.security.shell;


import android.app.Application;
import android.app.Instrumentation;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.ProviderInfo;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;

import com.security.mobile.annotation.JXCStub;
import com.security.mobile.utils.DXLog;
import com.keep.DebugUtils;
import com.security.mobile.utils.RefInvoke;

import org.json.JSONObject;

import java.io.*;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * DingXiang Proxy Application
 */

public class AppStub1 extends Application {

    private long mTimeInit = 0;
    private int mIsSupportVmp = 0;

    public AppStub1(){
        mTimeInit = System.currentTimeMillis();
    }

    @JXCStub
    private String getOriApplicationName(){
        //获取配置在清单文件的源Apk的Application路劲
        String appClassName = "android.app.Application";

        try {
            ApplicationInfo ai = this.getPackageManager().getApplicationInfo(this.getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;

            if (bundle != null && bundle.containsKey("APPLICATION_CLASS_NAME")) {
                appClassName = bundle.getString("APPLICATION_CLASS_NAME");//className 是配置在xml文件中的。
            }

        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

        return appClassName;
    }

    @JXCStub
    private static byte[] encrypt(byte[] data, int l) {
        byte xorByte = 0x51;

        for (int i = 0; i < l; ++i) {
            if (data[i] == 0 || data[i] == xorByte){
                data[i] = data[i];
            } else {
                data[i] ^= xorByte;
            }
        }

        return data;
    }

    @JXCStub
    private void _attachBaseContext(Context base) {

        String apkPath = getApplicationInfo().sourceDir;
        //这个用于8.0以上出问题找到base.apk的路径，请勿删除
        DXLog.i("apkPath:%s", apkPath);

        DebugUtils.init(base);
        DXLog.d("0 memory usage : %d", DebugUtils.getMemorySize());

        try {
            {
                //这个代码只是为了伪装360加固，sophix会根据这个文件是否存在判断某些特定的加固从而走响应的兼容逻辑
                File jiagu = new File(base.getFilesDir().getParentFile(), ".jiagu");
                boolean res1 = jiagu.mkdirs();
                File fake360 = new File(jiagu, "libjiagu.so");
                boolean res2 = fake360.createNewFile();
                DXLog.d("create fake files res1:%d, res2:%d", res1, res2);
            }

            DXLog.d("2.1 memory usage : %d", DebugUtils.getMemorySize());
            String cfgPath = "_dcfg_.data";

            int dexNum = 3;
            int isTestIn = 0;
            int payloadType = 1;
            int oatEncSize = 5000;
            int dexEncSize = 1024*1024;
            //memory load
            StringBuffer sb = new StringBuffer();
            DXLog.d("2.2 memory usage : %d", DebugUtils.getMemorySize());
            try {
                InputStream is = getAssets().open(cfgPath);
                ByteArrayOutputStream boas = new ByteArrayOutputStream();

                DXLog.d("2.3 memory usage : %d", DebugUtils.getMemorySize());
                byte[] buff = new byte[50];
                int n = 0;
                while ((n = is.read(buff)) != -1) {
                    boas.write(encrypt(buff, n), 0, n);
                }

                String json = new String(boas.toByteArray());

                DXLog.d("2.4 memory usage : %d", DebugUtils.getMemorySize());

                JSONObject js = new JSONObject(json);
                oatEncSize = js.getInt("oatEncSize");
                dexEncSize = js.getInt("dexEncSize");
                dexNum = js.getInt("size");
                payloadType = js.getInt("payloadType");
                isTestIn = js.getInt("isTestIn");
                mIsSupportVmp = js.getInt("supportVmp");

                DXLog.d("supportVmp:%d dexNumber:%d", mIsSupportVmp, dexNum);

                boas.close();
            } catch (IOException e) {
                DXLog.e(e.toString());
            }

            DXLog.d("2.5 memory usage : %d", DebugUtils.getMemorySize());

            File extractDir = this.getDir("csn0", MODE_PRIVATE);
            String extractDirStr = extractDir.getAbsolutePath();

            String zipFile = apkPath;

            DXLog.d("2.6 memory usage : %d", DebugUtils.getMemorySize());

            //dex path
            ShellBase  shell = new MemShell(this, zipFile, getPackageName(), extractDirStr, dexNum, oatEncSize, dexEncSize, isTestIn, payloadType);

            DXLog.d("2.7 memory usage : %d", DebugUtils.getMemorySize());

            DXLog.d("before shell init dexNumber:%d", dexNum);
            shell.init();

            DXLog.d("2.8 memory usage : %d", DebugUtils.getMemorySize());

            //配置动态加载环境
            //反射获取主线程对象，并从中获取所有已加载的package信息，并中找到当前的LoadApk对象的弱引用
            Object currentActivityThread = RefInvoke.invokeStaticMethod("android.app.ActivityThread", "currentActivityThread", new Class[] {}, new Object[] {});
            String packageName = this.getPackageName();
            Object mPackages = RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mPackages");

            DXLog.d("2.9 memory usage : %d", DebugUtils.getMemorySize());

            WeakReference<?> ref = ((Map<String, WeakReference<?>>)mPackages).get(packageName);
            Object loadedApk = ref.get();

            DXLog.d("start to new DexClassLoader");
            DXLog.d("2.10 memory usage : %d", DebugUtils.getMemorySize());

            ClassLoader oldLoader = (ClassLoader) RefInvoke.getFieldOjbect("android.app.LoadedApk", loadedApk, "mClassLoader");
            DXLog.i("before patch classloader "+oldLoader.toString());

            DXLog.d("before createClassLoader memory usage : %d", DebugUtils.getMemorySize());
            ClassLoader dLoader = shell.createClassLoader(oldLoader);
            DXLog.d("after createClassLoader memory usage : %d", DebugUtils.getMemorySize());
            DXLog.i("finish patch classloader "+dLoader.toString());

            //getClassLoader()等同于 (ClassLoader) RefInvoke.getFieldOjbect()
            //但是为了替换掉父节点我们需要通过反射来获取并修改其值
            //将父节点DexClassLoader替换
            RefInvoke.setFieldOjbect("android.app.LoadedApk", "mClassLoader", loadedApk, dLoader);

            DXLog.d("2.13 memory usage : %d", DebugUtils.getMemorySize());
            //restrictedBackupMode
            Object mBoundApplication = RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mBoundApplication");
            RefInvoke.setFieldOjbect("android.app.ActivityThread$AppBindData", "restrictedBackupMode", mBoundApplication, true);
            //根据源码，这个变量被设为true后，就不走installProvider流程了，也就是不走ContentProvider的onCreate
            DXLog.d("2.14 memory usage : %d", DebugUtils.getMemorySize());

            DXLog.d("shell _attachBaseContext used:%d", System.currentTimeMillis()-mTimeInit);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void attachBaseContext(Context base) {
        //该方法为了启动速度，不能打log，不能做dexvmp
        JNITools.setContext(base);
        super.attachBaseContext(base);
        _attachBaseContext(base);
    }

    @JXCStub
    private void _installContentProvider(Object activityThread, Object boundApplication, Application realApp, String appClassName) {

        Object providers = RefInvoke.getFieldOjbect("android.app.ActivityThread$AppBindData", boundApplication, "providers");
        DXLog.i("in _installContentProvider "+providers);
        if (providers == null) {
            DXLog.i("_installContentProvider skip because providers is null");
            return;
        }
        if (providers instanceof List) {
            List<ProviderInfo> infos = (List<ProviderInfo>)providers;
            DXLog.i("in _installContentProvider infos "+infos);
            // 乐变热更新在处理contentProvider时使用修改provider.applicationInfo.packageName的方式，让后续的代码认为
            // 该contentProvider不存在。此处处理，当packageName不同时，直接忽略
            ApplicationInfo ai = realApp.getApplicationInfo();
            if (ai != null) {
                for (ProviderInfo info : infos) {
                    if (info != null
                      && info.applicationInfo != ai
                      && info.applicationInfo != null
                      && info.applicationInfo.packageName != null
                      && info.applicationInfo.packageName.equals(ai.packageName)) {
                        fixupApplicationInfo(info.applicationInfo, appClassName);
                    }
                }
            }

            RefInvoke.invokeMethod(activityThread.getClass().getName(),"installContentProviders", activityThread,
                    new Class[] { Context.class, List.class }, new Object[] { realApp, providers });

            Object mH = RefInvoke.getFieldOjbect("android.app.ActivityThread", activityThread, "mH");
            DXLog.i("in _installContentProvider mH "+mH);
            if (mH instanceof Handler) {
                DXLog.i("in _installContentProvider handler");
                int sdk_version = Build.VERSION.SDK_INT;
                Integer what = 132;
                if (sdk_version <= Build.VERSION_CODES.O) {
                    //9.0以上不允许反射ENABLE_JIT
                    what = (Integer) RefInvoke.getFieldOjbect(mH.getClass().getName(), mH, "ENABLE_JIT");
                }
                Handler h = (Handler)mH;
                h.sendEmptyMessageDelayed(what, 10 * 1000);
                DXLog.i("in _installContentProvider after send");
            }
        }
        else{
            DXLog.i("_installContentProvider providers is %s not a list", providers.toString());
        }
        DXLog.i("out _installContentProvider");
    }

    private void fixupApplicationInfo(ApplicationInfo appInfo, String className){
        appInfo.className = className;
    }

    @JXCStub
    @Override
    public void onCreate() {
        super.onCreate();

        //获取配置在清单文件的源Apk的Application路劲
        String appClassName = getOriApplicationName();

        //获取当前壳Apk的ApplicationInfo
        Object currentActivityThread = RefInvoke.invokeStaticMethod("android.app.ActivityThread", "currentActivityThread", new Class[] {}, new Object[] {});
        Object mBoundApplication = RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mBoundApplication");

        //换回false,准备调用InstallProviders
        RefInvoke.setFieldOjbect("android.app.ActivityThread$AppBindData", "restrictedBackupMode", mBoundApplication, false);

        Object loadedApkInfo = RefInvoke.getFieldOjbect("android.app.ActivityThread$AppBindData", mBoundApplication, "info");
        //将LoadedApk中的mApplication, 后面LoadedApk才会重新创建app
        RefInvoke.setFieldOjbect("android.app.LoadedApk", "mApplication", loadedApkInfo, null);

        //获取currentActivityThread中注册的Application
        Object oldApplication = RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mInitialApplication");

        //获取ActivityThread中所有已注册的Application，并将当前壳Apk的Application从中移除
        ArrayList<Application> mAllApplications = (ArrayList<Application>) RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mAllApplications");
        mAllApplications.remove(oldApplication);
        ApplicationInfo appinfo_In_LoadedApk = (ApplicationInfo) RefInvoke.getFieldOjbect("android.app.LoadedApk", loadedApkInfo, "mApplicationInfo");
        ApplicationInfo appinfo_In_AppBindData = (ApplicationInfo) RefInvoke.getFieldOjbect("android.app.ActivityThread$AppBindData", mBoundApplication, "appInfo");

        //替换原来的Application
        fixupApplicationInfo(appinfo_In_AppBindData, appClassName);
        fixupApplicationInfo(appinfo_In_LoadedApk, appClassName);

        //注册Application
        Application app = (Application) RefInvoke.invokeMethod(loadedApkInfo.getClass().getName(),"makeApplication", loadedApkInfo, new Class[] { boolean.class, Instrumentation.class }, new Object[] { false, null });
        //调用客户application attachBaseContext
        if (app == null) {
            DXLog.e("AppStub1", "android.app.LoadedApk return null!!!");
            return;
        }

        //替换ActivityThread中的Application
        RefInvoke.setFieldOjbect("android.app.ActivityThread", "mInitialApplication", currentActivityThread, app);
        _installContentProvider(currentActivityThread, mBoundApplication, app, appClassName);

        Map<?, ?> mProviderMap = (Map<?, ?>) RefInvoke.getFieldOjbect("android.app.ActivityThread", currentActivityThread, "mProviderMap");
        DXLog.i("provider map "+mProviderMap);

        Iterator it = mProviderMap.values().iterator();
        while (it.hasNext()) {
            Object providerClientRecord = it.next();
            Object localProvider = RefInvoke.getFieldOjbect("android.app.ActivityThread$ProviderClientRecord", providerClientRecord, "mLocalProvider");

            //在魅族mx4 android5.0 + xposed机器下会出现localProvider为空的情况
            if (localProvider != null)
                RefInvoke.setFieldOjbect("android.content.ContentProvider", "mContext", localProvider, app);
        }

        //算出壳最终占用时间
        DXLog.i("shell init ok time used:%d", System.currentTimeMillis()-mTimeInit);
        //手动调用客户application onCreate
        app.onCreate();

        DXLog.d("shell call onCreate ok time used:%d", System.currentTimeMillis()-mTimeInit);
        //不支持vmp的话没必要保留预留给vmp的内存了，清除即可
        if (mIsSupportVmp == 0) {
            DXLog.i("before cleanup IsSupportVmp:%d", mIsSupportVmp);
            ShellSupporter.cleanup();
        }
    }
}
