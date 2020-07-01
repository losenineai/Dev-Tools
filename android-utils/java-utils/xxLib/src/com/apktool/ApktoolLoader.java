package com.apktool;

import com.xxlib.utils.base.LogTool;

import java.lang.reflect.Constructor;

import dalvik.system.DexClassLoader;

/**
 * Created by mushroom0417 on 12/16/15.
 */
public class ApktoolLoader {

    private static final String TAG = "ApktoolLoader";

    /**
     * 加载dex
     */
    public static Class<?> loadDexFromApk(String filepath, String dexOutDirPath, String driverClassPath) {
        return loadDexFromApk(filepath, dexOutDirPath, driverClassPath, null);
    }


    /**
     * 加载dex
     */
    public static Class<?> loadDexFromApk(String filepath, String dexOutDirPath, String driverClassPath, String nativePath) {
        try {
            DexClassLoader dexLoader = new DexClassLoader(filepath, dexOutDirPath, nativePath, ApktoolLoader.class.getClassLoader());
            return dexLoader.loadClass(driverClassPath);
        } catch (Exception e) {
            LogTool.e(TAG, e.toString());
        }
        return null;
    }


    public static Object createDexClassInstance(Class<?> driverClass) {
        Object obj = null;
        try {
            Constructor<?> driverConstructor = driverClass.getConstructor();
            if (null != driverConstructor) {
                obj = driverConstructor.newInstance();
            }
        } catch (Exception e) {
            LogTool.e(TAG, e.toString());
            obj = null;
        }
        return obj;
    }

}
