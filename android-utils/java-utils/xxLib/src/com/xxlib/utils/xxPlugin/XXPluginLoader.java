package com.xxlib.utils.xxPlugin;

import android.content.Context;

import com.xxlib.utils.base.LogTool;

import java.lang.reflect.Constructor;

import dalvik.system.DexClassLoader;

public class XXPluginLoader {
	private static final String TAG = "XXPluginLoader";

	/**
	 *  加载dex
	 */
	public static Class<?> loadDexFromApk(Context c, String filepath, String dexOutDirPath, String driverClassPath){
		return loadDexFromApk(c, filepath, dexOutDirPath, driverClassPath, null);
	}


    /**
     *  加载dex
     */
    public static Class<?> loadDexFromApk(Context c, String filepath, String dexOutDirPath, String driverClassPath, String nativePath){
        try {
            DexClassLoader dexLoader = new DexClassLoader(filepath, dexOutDirPath, nativePath, XXPluginLoader.class.getClassLoader());
            Class<?> dirverCalss = dexLoader.loadClass(driverClassPath);	//要加载的类的路径
            return dirverCalss;
        } catch (Exception e) {
            LogTool.e(TAG, e.toString());
        }
        return null;
    }
	
	
	public static Object createDexClassInstance(Class<?> driverClass, Context context) {
		Object obj = null;
		try {
			Constructor<?> driverConstructor = driverClass.getConstructor(new Class[]{Context.class});
			if (null != driverConstructor){
				obj = driverConstructor.newInstance(new Object[]{context});
			}
		} catch (Exception e) {
			LogTool.e(TAG, e.toString());
			obj = null;
		}
		return obj;
	}
}
