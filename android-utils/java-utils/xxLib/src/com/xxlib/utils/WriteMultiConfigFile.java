package com.xxlib.utils;

import android.content.Context;
import android.os.Handler;

import com.xxlib.utils.base.LogTool;

import java.io.File;

/**
 * Created by jammy on 15/12/17.
 */
public class WriteMultiConfigFile {
    public static final String TAG = "WriteMultiConfigFile";


    public static void writeConfigForMulti(final Context context, final String packageName, String mainDir) {
        try {
            String filepath = mainDir + packageName + ".plist";
            final File file = new File(filepath);
            if (file.createNewFile()) {
                new Handler(context.getMainLooper()).postDelayed(new Thread() {

                    @Override
                    public void run() {
                        if (null != file) {
                            file.delete();
                            LogTool.i(TAG, "multi plist file delete:" + packageName);
                        }
                    }
                }, 3000);
            }
        } catch (Exception e) {
            LogTool.i(TAG, "create multi plist file fail" + e.toString());
        }
    }
}
