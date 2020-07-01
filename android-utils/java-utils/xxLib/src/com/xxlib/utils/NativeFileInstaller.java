package com.xxlib.utils;

import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/*
 * 先把so 和 inject 都写到 cache中，再把so复制到/data/local/tmp/mtplugin
 */

public class NativeFileInstaller
{
	static String cacheDir = "injecttest/";
	static String[] filenamesExe = {"injectso", "oom_adj_mdf", "injectso_21", "xxmagic"};
	static String[] filenamesSO = {"libxxghost.so"};
	
//	static String soFileDir = "/data/local/tmp/injecttest/";
//	static String[] libFile = new String[]{"/data/data/com.flamingo.injecttest/lib/libsubstrate.so"};
	
	static File installDirCache = null;
	static final String TAG = "NativeFileInstaller";
	
	public static boolean test(){
		int i, j;
		File file = null;
		
		
//		Log.d(TAG, "########  NativeFileInstaller test start!!!!\n");
		
		// 检测exe文件是否安装完成
		j = filenamesExe.length;
		for(i = 0; i < j; i++){
			if( (file =  new File(installDirCache.getAbsoluteFile(), filenamesExe[i])).exists() == false){
//				Log.d(TAG, file.getAbsolutePath() + " install failed!---2\n");
				return false;
			}
//			Log.d(TAG, file.getAbsolutePath() + " install succeed!\n");
		}
			
		// 检测so文件是否安装到了cache
		j = filenamesSO.length;
		for(i = 0; i < j; i++){
			if( (file = new File(installDirCache.getAbsoluteFile(), filenamesSO[i])).exists() == false){
//				Log.d(TAG, file.getAbsolutePath() + " install failed!---3\n");
				return false;
			}
//			Log.d(TAG, file.getAbsolutePath() + " install succeed!\n");
		}
		
	    return true;
	}
	// 无root权限，在自己的目录或者sd卡上等有权限的地方操作才可以
	static File ensureEmptyDir(File dir, Context context)
	{

		File installDir = dir;

//		Log.d("NativeFileInstaller", installDir.getAbsolutePath());

		if (installDir.exists())
			installDir.delete();

		// 未对创建结果进行判断，为何之前要进行删除？
		installDir.mkdirs();
		installDir.setReadable(true);
		installDir.setWritable(true);
		installDir.setExecutable(true);

		return installDir;
	}
	
	
	public static Boolean extractAssetsFile(File dir, String filename, Context context)
	{
		int buffsize = 4096;
		byte[] buffer = new byte[buffsize];

		File file = new File(dir.getAbsolutePath(), filename);
		if (file.exists())
			file.delete();

		try
		{
			InputStream inStream = context.getResources().getAssets()
					.open(filename);
			FileOutputStream outStream = new FileOutputStream(file);

			int chunkSize = -1;
			while ((chunkSize = inStream.read(buffer)) != -1)
				outStream.write(buffer, 0, chunkSize);

			outStream.flush();
			outStream.close();

			file.setReadable(true);
			file.setWritable(true);
			file.setExecutable(true);
		} catch (Exception e)
		{
//			e.printStackTrace();
			return false;
		}

		return true;
	}

	public static void install(Context context)
	{
//		Log.d("NativeFileInstaller", "### install begin!");

		// 确保文件路径将一定存在
		installDirCache = ensureEmptyDir(getCacheDirFile(context), context);

		// 将可执行文件文件写到cache目录下
		for (int i = 0; i < filenamesExe.length; i++)
		{
			Boolean suc = extractAssetsFile(installDirCache, filenamesExe[i],context);
//			if (!suc)Log.e("NativeFileInstaller", "Orz exe: " + filenamesExe[i]);
		}

		// 将filenamesSO写到cache目录下 
		for(int i=0; i<filenamesSO.length; i++)
		{
			Boolean suc = extractAssetsFile(installDirCache, filenamesSO[i], context);
//			if(!suc) Log.e("NativeFileInstaller", "Orz so: " + filenamesSO[i]);
		}
		
	}
	public static File getCacheDirFile(Context context)
	{
		File dirFile = new File(context.getCacheDir(), cacheDir);
		return dirFile;
	}

	public static String getCacheDir(Context context)
	{
		File dirFile = getCacheDirFile(context);
		return dirFile.getAbsolutePath();
	}
	
	public static boolean isRoot(){
		if ( (new File("/system/bin/su").exists()) || (new File("/system/xbin/su").exists()) ){
			return true;
		} else {
			return false;
		}
	}

}