package com.dx.mobile.risk.collect.hardware;

import android.app.Service;
import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.os.storage.StorageManager;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.FileUtils;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.ProcessUtils;
import com.dx.mobile.risk.utils.StringUtils;

import org.json.JSONObject;

import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Time: 2019-11-01
 * Author: blue
 * Description:
 */
public class SDCard {

    public static long totalSize = 0;
    public static long availableSize = 0;

    public static String getSDCardInfo() {

        Context ctx = ContextFinder.getApplication();
        JSONObject action_info = new JSONObject();

        JSONUtils.jsonPut(action_info, "K2100", getMountedPointsList(ctx));

        initMemSize_1();
        JSONUtils.jsonPut(action_info, "K2101", totalSize);
        JSONUtils.jsonPut(action_info, "K2102", availableSize);

        try {
            initMemSize_2();
            JSONUtils.jsonPut(action_info, "K2103", totalSize);
            JSONUtils.jsonPut(action_info, "K2104", availableSize);
            JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX10, JNIConst.VALUE_APP_KOCK);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return action_info.length() > 0 ? action_info.toString() : "";

    }

    public static ArrayList<String> getMountedPointsList(Context context) {

        StorageManager storageManager = (StorageManager) context.getSystemService(Service.STORAGE_SERVICE);
        ArrayList<String> arrayList = new ArrayList();

        try {
            String[] strArr = (String[]) storageManager.getClass().getMethod("getVolumePaths",
                    new Class[0]).invoke(storageManager, new Object[0]);

            if (strArr != null && strArr.length > 0) {
                Method method = storageManager.getClass().getMethod("getVolumeState", new Class[]{String.class});
                for (String str : strArr) {
                    if ("mounted".equals(method.invoke(storageManager, new Object[]{str}))) {
                        arrayList.add(str);
                    }
                }
                return arrayList;
            }
        } catch (Exception e) { }

        return null;
    }

    private static void initMemSize_1() {

        File externalStorageDirectory = Environment.getExternalStorageDirectory();
        if (externalStorageDirectory != null && externalStorageDirectory.exists()) {
            String path = externalStorageDirectory.getPath();

            if(path == null)
                return;

            StatFs statFs;

            try {
                statFs = new StatFs(path);
            } catch (Exception e) {
                e.printStackTrace();
                statFs = null;
            }

            if (statFs == null) {
                return;
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
                try {
                    totalSize = statFs.getTotalBytes();
                } catch (Exception e2) {
                    totalSize = (long) (statFs.getBlockSize() * statFs.getBlockCount());
                }
                try {
                    availableSize = statFs.getAvailableBytes();
                    return;
                } catch (Exception e3) {
                    availableSize = (long) (statFs.getAvailableBlocks() * statFs.getBlockSize());
                    return;
                }
            }

            totalSize = (long) (statFs.getBlockCount() * statFs.getBlockSize());
            availableSize = (long) (statFs.getAvailableBlocks() * statFs.getBlockSize());
        }
    }

    // private void e()
    private static void initMemSize_2() {

        String[] split = ProcessUtils.exec_get_all(new String[]{"df", "/mnt/sdcard"}).split("\n");
        int length = split.length;

        for (int i = 0; i < length; i++) {
            String str = split[i];

            if (str.contains("Filesystem"))
                continue;

            Matcher matcher = Pattern.compile("\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+").matcher(str);
            if (matcher.find()) {

                long parseLong = Long.parseLong(matcher.group(1));
                Long.parseLong(matcher.group(2));
                long parseLong2 = Long.parseLong(matcher.group(3));

                totalSize = parseLong;
                availableSize = parseLong2;
            }
        }
    }

    public static String getSDCardDir() {

        String sdcardDir = FileUtils.existSDCard() ? Environment.getExternalStorageDirectory().getAbsolutePath() : "";

        // x分身SDCard路径
        if (sdcardDir.endsWith("/godinsec")) {
            sdcardDir = sdcardDir.replace("/godinsec", "");
            // x-phone SDCard路径
        } else if (sdcardDir.endsWith("/godinsec_xphone")) {
            sdcardDir = sdcardDir.replace("/godinsec_xphone", "");
        }

        if (StringUtils.isEmpty(sdcardDir))
            sdcardDir = "/sdcard";

        return sdcardDir;
    }

    //Android5.1以下可以使用
    public static String exgetSDCardDir() {
        return System.getenv("SECONDARY_STORAGE");
    }

    public static int getSDCardStatus() {
        return FileUtils.getSDCardStatus();
    }


}
