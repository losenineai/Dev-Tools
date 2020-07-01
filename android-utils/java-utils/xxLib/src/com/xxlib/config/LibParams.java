package com.xxlib.config;

import android.os.Environment;
import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.FileUtils;

public class LibParams {

    public static final String XX_PKG_NAME = "com.xxAssistant";
    public static final String XMOD_PKG_NAME = "com.xmodgame";

    /**
     * zzd 存放log文件的路径
      */
	public static String NATIVE_LOG_PATH = Environment.getExternalStorageDirectory().toString() + "/" + ContextFinder.getApplication().getPackageName() + "/log/";

	/**
	 * 动态加载的apk的存放文件夹
	 */
	public static final String LOAD_APK_DIR = String.format("/data/data/%s/apk/", ContextFinder.getApplication().getPackageName());

    static {
        FileUtils.makeDirs(NATIVE_LOG_PATH);
    }

    // 异常检测文件
    public final static String EXCEPTION_PATH = Environment.getExternalStorageDirectory().toString()
            + "/"+ContextFinder.getApplication().getPackageName()+"/" + "exception_check";
}
