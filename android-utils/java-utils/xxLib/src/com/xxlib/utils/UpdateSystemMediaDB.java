package com.xxlib.utils;

import android.content.Context;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;

import java.io.File;


/**
 * 更新系统媒体数据库
 * @Description 
 *
 * 版本号    作者    日期    相关修改
 *  1.0   white   2014-11-5  创建
 *
 * @author white
 * 2014-11-5   --  下午3:26:35
 */
public class UpdateSystemMediaDB {
	
	/**
	 * 如果有新添文件，更新该文件到系统数据库中
	 * @description  
	 * @param context
	 * @param file
	 */
	public static void updateFile(Context context, File file){
		new SingleMediaScanner(context, file);
	}
	
	/**
	 * 指定一个文件，更新媒体数据库
	 * @author wxj
	 * 2014-10-27
	 */
	public static class SingleMediaScanner implements MediaScannerConnectionClient {

		private MediaScannerConnection mMs;
		private File mFile;

		public SingleMediaScanner(Context context, File f) {
		    mFile = f;
		    mMs = new MediaScannerConnection(context, this);
		    mMs.connect();
		}

		@Override
		public void onMediaScannerConnected() {
		    mMs.scanFile(mFile.getAbsolutePath(), null);
		}

		@Override
		public void onScanCompleted(String path, Uri uri) {
		    mMs.disconnect();
		}

		}
	
}
