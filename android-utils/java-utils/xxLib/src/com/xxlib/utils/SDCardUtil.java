package com.xxlib.utils;

import android.os.Build;
import android.os.Environment;
import android.os.StatFs;

import com.xxlib.utils.base.LogTool;

public class SDCardUtil {

	private static final String TAG = "SDCardUtil";

	public static final long SIZE_1_M = 1024;
	
	
	/**
	 * 是否大于容量，单位为K
	 * @param kSize 指定的大小
	 * @return 是否比指定大小大
	 */
	public static boolean isSdcardAvailableThan(long kSize){
		return getAvailableStore() >= kSize;
	}
	
	
	/**
	 * 获取sd卡的容量 
	 * @return 以KByte为单位的大小
	 */
	public static long getAvailableStore() {
		String path = Environment.getExternalStorageDirectory().getPath();
		// 取得sdcard文件路径
		StatFs statFs = new StatFs(path);
		// 获取block的SIZE
		long blockSize;
		long availableBlocks;
		if (Build.VERSION.SDK_INT >= 18) {
			blockSize = statFs.getBlockSizeLong();
			availableBlocks = statFs.getAvailableBlocksLong();
		} else {
			blockSize = statFs.getBlockSize();
			availableBlocks = statFs.getAvailableBlocks();
		}
		long availableSpaceInByte = availableBlocks * blockSize;
		LogTool.i(TAG, availableSpaceInByte + "B");
		LogTool.i(TAG, availableSpaceInByte / 1024 + "KB");
		LogTool.i(TAG, availableSpaceInByte / 1024 / 1024 + "MB");
		return availableSpaceInByte / 1024;
	}
	
	
	/**
	 * sd卡是否加载
	 * @return 是否有SD卡
	 */
	public static boolean isSdcardMounted(){
		return Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState());
	}
}
