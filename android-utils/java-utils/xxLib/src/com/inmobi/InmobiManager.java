package com.inmobi;

import android.content.Context;

import com.inmobi.access.IADHelper;
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
public class InmobiManager {

    private static final String TAG = "InmobiManager";

    private static InmobiManager instance;

    public static InmobiManager getInstance() {
        if (instance == null) {
            synchronized (InmobiManager.class) {
                if (instance == null) {
                    instance = new InmobiManager();
                }
            }
        }
        return instance;
    }

    private InmobiManager() {
        try {
            Context context = ApplicationUtils.getApplication();
            if (context != null) {
                File apkFile = new File(InmobiUpdater.getApkFilePath(context));
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

    public LoadInmobiResult getADHelper(String apkPath) {
        return getADHelper(apkPath, "com.inmobi.main.ADHelper");
    }

    /**
     * 获取xxplugin对象实例
     */
    public LoadInmobiResult getADHelper(String apkPath, String classPath) {
        // 获取apk中的class
        Class<?> driverClass = loadClass(apkPath, classPath);
        if (null == driverClass) {
            LogTool.e(TAG, "null == driverClass");
            return new LoadInmobiResult(LoadInmobiResult.LOAD_ASSIST_ERR_LOAD_CLASS_FAIL);
        }

        // 获取class对应的实例
        Object obj = InmobiLoader.createDexClassInstance(driverClass);
        if (null == obj) {
            LogTool.e(TAG, "null == obj");
            return new LoadInmobiResult(LoadInmobiResult.LOAD_ASSIST_ERR_NEW_INSTANCE_FAIL);
        }
        if (!(obj instanceof IADHelper)) {
            LogTool.e(TAG, "!(obj instanceof IADHelper)");
            return new LoadInmobiResult(LoadInmobiResult.LOAD_ASSIST_ERR_WRONG_INSTANCE_FAIL);
        }

        // 返回结果
        LoadInmobiResult loadApktoolResult = new LoadInmobiResult(LoadInmobiResult.LOAD_ASSIST_ERR_NONE);
        loadApktoolResult.mADHelper = (IADHelper) obj;
        return loadApktoolResult;
    }

    public Class<?> loadClass(String apkPath, String classPath) {
        // 把上一次的*.dex文件删除，如果存在的话
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

        // 再创建outdex文件夹
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

        // 获取apk中的class
        return InmobiLoader.loadDexFromApk(apkPath, outDexDirStr, classPath);
    }

}
