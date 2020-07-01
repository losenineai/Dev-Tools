package com.xxlib.utils.xxPlugin;

import android.content.Context;

import com.xxAssistant.DanMuKu.plugin.apk.IXXPlugin;
import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.ClearMemory;
import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.base.LogTool;

import java.io.File;

/**
 * 单例，保留辅助 ipcController那些实例
 * 
 * @author jammy
 * 2015-2-5   --  下午3:00:31
 */
public class XXPluginManager {

	private static final String TAG = "XXPluginManager";
	private static XXPluginManager mSelf;
	private int mGamePid = -1;
	
	// 辅助
	private static final String ASSIST_APK_CLASS_PATH = "com.xx.XXPlugin"; 	        // APK中辅助的类路劲
	private static final String SPEED_APK_CLASS_PATH = "com.xx.speed.XXPlugin"; 	// APK中加速器的类路劲
	private IXXPlugin mAssistPlugin;
	private IXXPlugin mSpeedPlugin;
	private String mAssistApkPath = "";
	public static boolean mIsShowUpdate = false;
	// 测试新游戏的辅助
	public static boolean isTestPluginForNewGame = false;
	public static int testNewGamePluginUid = 999;
	
	private XXPluginManager() {
	}
	
	
	public static XXPluginManager getInstance() {
		synchronized (XXPluginManager.class) {
			if (null == mSelf) {
				mSelf = new XXPluginManager();
			}
			return mSelf;
		}
	}
	
	
	public void clearSpeedAndAssist() {
		// 清除游戏进程id
		mGamePid = -1;

		// 清除辅助
		mAssistApkPath = "";
		if (null != mAssistPlugin) {
			mAssistPlugin.destroy();
			mAssistPlugin = null;
		}
		mIsShowUpdate = false;
		
		mSelf = null;
	}
	

	public String getGameAssistUiApkPath(int uid) {
        String packageName = ContextFinder.getApplication().getPackageName();
        LogTool.i(TAG, "getGameAssistUiApkPath package name " + packageName);
		return "/data/data/"+packageName+"/app_plugin/" + uid + "/xxp_plugin_" + uid + ".apk";
	}


    public String getGameSpeedUiApkPath() {
        String packageName = ContextFinder.getApplication().getPackageName();
        LogTool.i(TAG, "getGameAssistUiApkPath package name " + packageName);
        return "/data/data/"+packageName+"/app_speed/xxp_plugin_speed.apk";
    }

	/**
     *
	 * @param gamePkgName 游戏包名
	 * @return GamePid
	 */
	public int getGamePid(String gamePkgName) {
		if (-1 == mGamePid || 0 == mGamePid) {
			mGamePid = ClearMemory.getGamePid(ContextFinder.getApplication(), gamePkgName);
		}
		LogTool.i(TAG, "gamePkgName " + gamePkgName);
		LogTool.i(TAG, "gamePid " + mGamePid);
		return mGamePid;
	}
	
	
	public void setAssistApkPath(String apkPath) {
		this.mAssistApkPath = apkPath;
	}


    public LoadXXPluginResult getAssistPlugin(int gameAssistUid) {
        LoadXXPluginResult xxPluginResult = null;
        if (null == mAssistPlugin) {
            xxPluginResult = getXXPlugin(getGameAssistUiApkPath(gameAssistUid), ASSIST_APK_CLASS_PATH);
            mAssistPlugin = xxPluginResult.mXXPlugin;
        } else {
            xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_NONE);
            xxPluginResult.mXXPlugin = mAssistPlugin;
        }
        return xxPluginResult;
    }

    public LoadXXPluginResult getSpeedPlugin(int speedPluginId) {
        LoadXXPluginResult xxPluginResult = null;
        if (null == mSpeedPlugin) {
            xxPluginResult = getXXPlugin(getGameAssistUiApkPath(speedPluginId), SPEED_APK_CLASS_PATH);
            mSpeedPlugin = xxPluginResult.mXXPlugin;
        } else {
            xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_NONE);
            xxPluginResult.mXXPlugin = mSpeedPlugin;
        }
        return xxPluginResult;
    }

    public LoadXXPluginResult getSpeedPlugin() {
        LoadXXPluginResult xxPluginResult = null;
        if (null == mSpeedPlugin) {
            xxPluginResult = getXXPlugin(getGameSpeedUiApkPath(), SPEED_APK_CLASS_PATH);
            mSpeedPlugin = xxPluginResult.mXXPlugin;
        } else {
            xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_NONE);
            xxPluginResult.mXXPlugin = mSpeedPlugin;
        }
        return xxPluginResult;
    }

	/**
	 * 获取xxplugin对象实例
	 */
	public LoadXXPluginResult getXXPlugin(String apkPath, String classPath) {
        // 获取apk中的class
        Class<?> driverClass = loadClass(apkPath, classPath);
        if (null == driverClass) {
            LogTool.e(TAG, "null == driverClass");
            LoadXXPluginResult xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_LOAD_CLASS_FAIL);
            return xxPluginResult;
        }

        // 获取class对应的实例
        Context context = ContextFinder.getApplication();
        Object obj = XXPluginLoader.createDexClassInstance(driverClass, context);
        if (null == obj) {
            LogTool.e(TAG, "null == obj");
            LoadXXPluginResult xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_NEW_INSTANCE_FAIL);
            return xxPluginResult;
        }
        if (!(obj instanceof IXXPlugin)) {
            LogTool.e(TAG, "!(obj instanceof IXXPlugin)");
            LoadXXPluginResult xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_WRONG_INSTANCE_FAIL);
            return xxPluginResult;
        }

        // 返回结果
        LoadXXPluginResult xxPluginResult = new LoadXXPluginResult(LoadXXPluginResult.LOAD_ASSIST_ERR_NONE);
        xxPluginResult.mXXPlugin = (IXXPlugin)obj;;
		return xxPluginResult;
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
		Context context = ApplicationUtils.getApplication();
		Class<?> driverClass = XXPluginLoader.loadDexFromApk(context, apkPath, outDexDirStr, classPath);
		return driverClass;
	}
}
