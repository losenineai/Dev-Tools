package com.apktool;

import android.content.Context;

import com.apktool.access.IMultiApkHelper;
import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Created by mushroom0417 on 12/16/15.
 */
public class ApktoolManager {

    private static final String TAG = "ApktoolManager";

    private static ApktoolManager instance;

    public static ApktoolManager getInstance() {
        if (instance == null) {
            synchronized (ApktoolManager.class) {
                if (instance == null) {
                    instance = new ApktoolManager();
                }
            }
        }
        return instance;
    }

    private ApktoolManager() {
        try {
            Context context = ApplicationUtils.getApplication();
            if (context != null) {
                File apkFile = new File(ApktoolUpdate.getApkFilePath(context));
//                File dir = apkFile.getParentFile();
                if (!apkFile.exists()) {
//                    FileUtils.writeFile(apkFile.getAbsolutePath(), context.getAssets().open("xxApktool-release_1.0.0_100.apk"));
                }
                copyNecessaryResource(apkFile);
            } else
                throw new Exception("Context not found");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void copyNecessaryResource(File apkFile) {
        ZipInputStream zipInputStream;
        try {
            File rootDir = apkFile.getParentFile();
            zipInputStream = new ZipInputStream(new FileInputStream(apkFile));
            ZipEntry zipEntry;
            String entryName;
            while ((zipEntry = zipInputStream.getNextEntry()) != null) {
                entryName = zipEntry.getName();
                if (zipEntry.isDirectory()) {
                    // get the folder name of the widget
                    entryName = entryName.substring(0, entryName.length() - 1);
                    File folder = new File(rootDir.getAbsolutePath() + File.separator + entryName);
                    folder.mkdirs();
                } else {

                    File file = new File(rootDir.getAbsolutePath() + File.separator + entryName);
                    File parentDir = file.getParentFile();
                    parentDir.mkdirs();
                    file.createNewFile();
                    // get the output stream of the file
                    FileOutputStream out = new FileOutputStream(file);
                    int len;
                    byte[] buffer = new byte[1024];
                    // read (len) bytes into buffer
                    while ((len = zipInputStream.read(buffer)) != -1) {
                        // write (len) byte from buffer at the position 0
                        out.write(buffer, 0, len);
                        out.flush();
                    }
                    out.close();
                }
            }
            zipInputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public LoadApktoolResult getMultiApkHelper(String apkPath) {
        return getMultiApkHelper(apkPath, "com.apktool.MultiApkHelper");
    }

    /**
     * ??????xxplugin????????????
     */
    public LoadApktoolResult getMultiApkHelper(String apkPath, String classPath) {
        // ??????apk??????class
        Class<?> driverClass = loadClass(apkPath, classPath);
        if (null == driverClass) {
            LogTool.e(TAG, "null == driverClass");
            return new LoadApktoolResult(LoadApktoolResult.LOAD_ASSIST_ERR_LOAD_CLASS_FAIL);
        }

        // ??????class???????????????
        Object obj = ApktoolLoader.createDexClassInstance(driverClass);
        if (null == obj) {
            LogTool.e(TAG, "null == obj");
            return new LoadApktoolResult(LoadApktoolResult.LOAD_ASSIST_ERR_NEW_INSTANCE_FAIL);
        }
        if (!(obj instanceof IMultiApkHelper)) {
            LogTool.e(TAG, "!(obj instanceof IMultiApkHelper)");
            return new LoadApktoolResult(LoadApktoolResult.LOAD_ASSIST_ERR_WRONG_INSTANCE_FAIL);
        }

        // ????????????
        LoadApktoolResult loadApktoolResult = new LoadApktoolResult(LoadApktoolResult.LOAD_ASSIST_ERR_NONE);
        loadApktoolResult.iMultiApkHelper = (IMultiApkHelper) obj;
        return loadApktoolResult;
    }

    public Class<?> loadClass(String apkPath, String classPath) {
        // ???????????????*.dex?????????????????????????????????
        File apkFile = new File(apkPath);
        String outDexDirStr = apkFile.getParent() + File.separator + "dexout";
        String outDexFileName = "";
        int indexOfDot = apkFile.getName().indexOf(".");
        if (indexOfDot >= 0) {
            outDexFileName = apkFile.getName().substring(0, indexOfDot) + ".dex";
        } else {
            outDexFileName = apkFile.getName() + ".dex";
        }
        String outDexFilePath = outDexDirStr + File.separator + outDexFileName;
        File outDexFile = new File(outDexFilePath);
        if (outDexFile.exists()) {
            LogTool.i(TAG, "dex file exist, delete it");
            outDexFile.delete();
            LogTool.i(TAG, "dex file exist " + outDexFile.exists());
        }

        // ?????????outdex?????????
        File dexout = new File(outDexDirStr);
        LogTool.i(TAG, "apk path : " + apkPath);
        LogTool.i(TAG, "dex out dir " + outDexDirStr);
        if (dexout.exists()) {
            if (!dexout.isDirectory()) {
                dexout.delete();
                dexout.mkdir();
            }
        } else {
            dexout.mkdir();
        }

        // ??????apk??????class
        return ApktoolLoader.loadDexFromApk(apkPath, outDexDirStr, classPath);
    }

}
