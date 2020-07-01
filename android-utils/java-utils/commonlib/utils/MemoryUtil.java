package com.auric.intell.commonlib.utils;

import android.app.ActivityManager;
import android.content.Context;
import android.os.Build;
import android.util.Log;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;

/**
 * @author white
 * @description：内存管理
 * @date 2016/11/15
 */

public class MemoryUtil {

    private static final String TAG = "MemoryUtil";

    /**
     * 清空内存,杀死无用进程
     * need permission KILL_BACKGROUND_PROCESSES
     * @param context
     */
    public static void clear(Context context){
        if (context == null) {
            return;
        }
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> proInfoList = am.getRunningAppProcesses();
        List<ActivityManager.RunningServiceInfo> serviceInfos = am.getRunningServices(100);

        long beforeMem = getAvailMemory(context);
        LogTool.d(TAG, "cur men total:" + getTotalMemory(context));
        LogTool.d(TAG, "before clear mem: " + beforeMem);

        int count = 0;
        if (proInfoList != null) {
            for (int i = 0; i < proInfoList.size(); ++i) {
                ActivityManager.RunningAppProcessInfo appProcessInfo = proInfoList.get(i);
                Log.d(TAG, "[" + i + "]process name : " + appProcessInfo.processName);
                //importance 该进程的重要程度  分为几个级别，数值越低就越重要。
                Log.d(TAG, "---importance : " + appProcessInfo.importance);

                // 一般数值大于RunningAppProcessInfo.IMPORTANCE_SERVICE的进程都长时间没用或者空进程了
                // 一般数值大于RunningAppProcessInfo.IMPORTANCE_VISIBLE的进程都是非可见进程，也就是在后台运行着
                try {
                    if (appProcessInfo.importance > ActivityManager.RunningAppProcessInfo.IMPORTANCE_VISIBLE) {
                        String[] pkgList = appProcessInfo.pkgList;
                        for (int j = 0; j < pkgList.length; ++j) {//pkgList 得到该进程下运行的包名
                            Log.d(TAG, "It will be killed, package name : " + pkgList[j]);
                            am.killBackgroundProcesses(pkgList[j]);
                            count++;
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        long afterMem = getAvailMemory(context);
        Log.d(TAG, "after clear mem:" + afterMem);
        Log.d(TAG, "clear " + count + " process, "
                + (beforeMem - afterMem) + "M");
//        Toast.makeText(context, "clear " + count + " process, "
//                + (afterMem - beforeMem) + "M", Toast.LENGTH_LONG).show();
    }

    /**
     * 获取可用内存大小
     * @param context
     * @return
     */
    public static long getAvailMemory(Context context) {
        // 获取android当前可用内存大小
        ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        am.getMemoryInfo(mi);
        //mi.availMem; 当前系统的可用内存
        //return Formatter.formatFileSize(context, mi.availMem);// 将获取的内存大小规格化
        LogTool.d(TAG, "可用内存---->>>" + mi.availMem / (1024 * 1024));
        return mi.availMem / (1024 * 1024);
    }


    /**
     * 获取内存总值
     * @return
     */
    public static long getTotalMemory(Context context) {
        // 大于16
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
            ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
            am.getMemoryInfo(mi);
            return (mi.totalMem / (1024 * 1024));
        }
        else{
            String str1 = "/proc/meminfo";// 系统内存信息文件
            String str2;
            String[] arrayOfString;
            long initial_memory = 0;

            try {
                FileReader localFileReader = new FileReader(str1);
                BufferedReader localBufferedReader = new BufferedReader(localFileReader, 8192);
                str2 = localBufferedReader.readLine();// 读取meminfo第一行，系统总内存大小

                arrayOfString = str2.split("\\s+");
                for (String num : arrayOfString) {
                    // Log.i(str2, num + "\t");
                }

                initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte
                localBufferedReader.close();

            } catch (IOException e) {
            }
            // return Formatter.formatFileSize(context, initial_memory);//
            // Byte转换为KB或者MB，内存大小规格化
            return initial_memory / (1024 * 1024);
        }
    }
}
