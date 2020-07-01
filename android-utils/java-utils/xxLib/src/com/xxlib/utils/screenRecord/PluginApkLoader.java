package com.xxlib.utils.screenRecord;

import android.content.Context;

import com.xxAssistant.DanMuKu.Tool.apk.IRecordCompose;
import com.xxlib.config.LibParams;
import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.MD5Helper;
import com.xxlib.utils.NativeFileInstaller;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.dyload.DexInjector;
import com.xxlib.utils.io.ZipUtil;

import java.io.File;
import java.lang.reflect.Constructor;

/**
 * Created by mushroom0417 on 7/4/16.
 */
public class PluginApkLoader {

    private static final String TAG = "PluginApkLoader";

    private static PluginApkLoader sInstance;
    private IRecordCompose mIRecordCompose;
    private PluginApkLoader() {
        // load apk的类进行实例化，再合并视频
        String apkDir = LibParams.LOAD_APK_DIR;
        File apkDirFile = new File(apkDir);
        if (!apkDirFile.exists()) {
            apkDirFile.mkdirs();
        }
        String apkName = "xx_apk_load.apk";
        String apkPath = apkDir + apkName;
        String apkMd5 = "1e9552672ecf8527b7b2fbcbfcf30ef5";
        String classPath = "com.xx.RecordCompose";

        Context context = ApplicationUtils.sSDKContext;
        if (context == null)
            context = ApplicationUtils.getApplication();

        // 文件是否存在，用md5做检验
        File apkFile = new File(apkPath);
        if (!apkFile.exists() || !MD5Helper.getFileMD5(apkFile).equals(apkMd5)) {
            LogTool.i(TAG, "copy xx_apk_load.apk to " + apkPath);
            NativeFileInstaller.extractAssetsFile(new File(apkDir), apkName, context);
        }



        String dexPath = "/data/data/" + context.getPackageName() + "/guopan/sdk/plugin/dex/classes_mp4.dex";
        String optPath = "/data/data/" + context.getPackageName() + "/guopan/sdk/plugin/dex/opt";
        String libPath = "/data/data/" + context.getPackageName() + "/guopan/sdk/plugin/dex/lib";
        String pluginDexPath = "/data/data/" + context.getPackageName() + "/guopan/sdk/plugin/dex";

        ZipUtil.unZip(apkPath, pluginDexPath);

        boolean result = DexInjector.inject(dexPath, optPath, libPath, classPath);

        if (result) {

            try {
                Class clazz = Class.forName(classPath);
                if (clazz != null) {
                    Constructor constructor = clazz.getConstructor(Context.class);
                    Object object = constructor.newInstance(context);
                    if (object instanceof IRecordCompose)
                        mIRecordCompose = (IRecordCompose) object;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static PluginApkLoader getInstance() {
        if (sInstance == null) {
            synchronized (PluginApkLoader.class) {
                if (sInstance == null)
                    sInstance = new PluginApkLoader();
            }
        }
        return sInstance;
    }

    public IRecordCompose getIRecordCompose() {
        return mIRecordCompose;
    }

}
