package com.xxlib.utils;

import android.os.Environment;
import android.os.StatFs;

import java.io.File;

/**
 * Created by rosejames on 16/6/23.
 * <p>
 * This class all about some method of android file system
 */
public class FileSystemUtils {

    /**
     * Read remain space of MB.
     *
     * @return MB of remain space.
     */
    public static long readRemainSpaceMB() {
        return readRemainSpaceKB() / 1024;
    }

    /**
     * Read remain space of KB.
     *
     * @return KB of remain space.
     */
    public static long readRemainSpaceKB() {
        return readRemainSpaceB() / 1024;
    }

    /**
     * Read remain space of Byte.
     *
     * @return Byte of remain space.
     */
    public static long readRemainSpaceB() {
        File path = Environment.getExternalStorageDirectory(); //取得sdcard文件路径
        StatFs stat = new StatFs(path.getPath());
        long blockSize = stat.getBlockSize();
        long availableBlocks = stat.getAvailableBlocks();
        return (availableBlocks * blockSize);
    }


}
