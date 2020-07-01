package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.os.Environment;
import android.os.StatFs;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by zhangxiliang on 2015/6/11.
 */
public class SdCardUtils {

    /**
     * 判断ＳＤ卡是否挂载
     */
    public static boolean isSDCardMounted() {
        String state = Environment.getExternalStorageState();
        return state.equals(Environment.MEDIA_MOUNTED);
    }

    /**
     * 返回ＳＤ卡根目录
     *
     * @return
     */
    public static String getSDCardRootDir() {
        return Environment.getExternalStorageDirectory().getPath();
    }

    /**
     * 获取ＳＤ卡总容量
     */
    public static long getSDCardTotalSize() {
        if (isSDCardMounted()) {
            //StatFs Statistic File System  簇
            StatFs sf = new StatFs(getSDCardRootDir());
            int blockSize = sf.getBlockSize();
            int blockCount = sf.getBlockCount();

            return blockSize * blockCount / 1024 / 1024;  //返回ＭＢ
        }
        return 0;
    }

    /**
     * 获取ＳＤ卡可用容量
     *
     * @return
     */
    public static long getSDCardAvailableSize() {
        if (isSDCardMounted()) {
            StatFs sf = new StatFs(getSDCardRootDir());
            int availableBlockCount = sf.getAvailableBlocks();
            int blockSize = sf.getBlockSize();

            return (long) availableBlockCount * blockSize / 1024 / 1024;
        }
        return 0;
    }


    /**
     * 向ＳＤ卡私有Cache目录写入数据
     *
     * @param context  　上下文，用来找到storage/sdcard0/Android/data/packageName/cache
     * @param data     需要保存的数据
     * @param type     　文件加类型
     * @param fileName 　另存为的文件名名称
     * @return
     */
    public static boolean saveFileToExternalCacheDir(Context context, byte[] data,
                                                     String fileName) {
        if (isSDCardMounted()) {
            BufferedOutputStream bos = null;
            File fileDir = context.getExternalCacheDir();
            File file = new File(fileDir, fileName);
            if (!file.exists()) {
                try {
                    file.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            try {
                bos = new BufferedOutputStream(new FileOutputStream(file));
                bos.write(data);
                bos.flush();
                return true;
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (bos != null) {
                    try {
                        bos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
        return false;
    }

    /**
     * 从ＳＤ卡中读取数据
     *
     * @param fileAbsolutePath 　　读取文件的绝对路径
     * @return
     */
    public static byte[] loadDataFromSDCard(String fileAbsolutePath) {
        if (isSDCardMounted()) {
            BufferedInputStream bis = null;
            ByteArrayOutputStream baos = new ByteArrayOutputStream();

            try {
                bis = new BufferedInputStream(
                        new FileInputStream(fileAbsolutePath));
                byte[] buffer = new byte[1024 * 8];
                int len = 0;
                while ((len = bis.read(buffer)) != -1) {
                    baos.write(buffer, 0, len);
                    baos.flush();
                }
                return baos.toByteArray();
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                try {
                    if (bis != null) {
                        bis.close();
                    }
                    if (baos != null) {
                        baos.close();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return null;
    }

    public static String getSDCardCacheDir(Context context) {
        return context.getExternalCacheDir().getPath();
    }
}
