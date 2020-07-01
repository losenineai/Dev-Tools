package com.auric.intell.commonlib.robot;

import android.os.Environment;

/**
 * @author white
 * @description：RBT 相关常量
 * @date 2017/1/10
 */

public class RBTConstant {

//    public static final String OTA_FILE_PATH = "/mnt/shell/emulated/0/OTA/update.zip";
    public static final String OTA_FILE_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + "/OTA/update.zip";
}
