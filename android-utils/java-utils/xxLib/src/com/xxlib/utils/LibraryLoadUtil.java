package com.xxlib.utils;

import android.content.Context;

import java.io.File;

public class LibraryLoadUtil {
	private static final String TAG = "LibraryLoadUtil";

	/**
	 * 对java.lang.System.loadLibrary进行封装，优先从/data/data/xxx/lib目录下寻找
	 * @param libName .so库文件名，可以不加lib前缀及.so后缀
	 * @return 成功返回true，失败返回false
	 */
	public static boolean loadLibrary(String libName, Context context)
	{
		// 找到/data/data/xxx/lib目录，没有直接取到的方法，先取cache目录
		// 没有什么好办法，总不能写死/data/data/xxx这样的路径，取cache目录会创建cache目录
		String originName = libName;
		String libPath = null;
		try
		{
			// 下面这句在初次启动的时候有可能会失败
			libPath = context.getCacheDir().toString();
			if(libPath.endsWith("/") == true)
			{
				// 本身已经是以"/"结尾
				libPath = libPath.substring(0, libPath.length() - 2);
			}
			int pos = libPath.lastIndexOf('/');
			if(pos != -1)
			{
				libPath = libPath.substring(0, pos);
				libPath += "/lib/";
			}
			else
			{
				libPath = "";
			}
		}
		catch(Exception e)
		{
			// 如果失败，只有写死了
			libPath = "/data/data/" + context.getPackageName() + "/lib/";
			e.printStackTrace();
		}

		if(libPath.length() != 0)
		{
			// 使用System.load方法
			boolean bLoaded = false;
			File soFile = new File(libPath + libName);
			if(soFile.exists())
			{
				try
				{
					System.load(libPath + libName);
					bLoaded = true;
				}
				catch(UnsatisfiedLinkError e)
				{
					e.printStackTrace();
				}
			}
			if(bLoaded == false)
			{
				// 在后面尝试加.so后缀
				if(libName.endsWith(".so") == false)
				{
					libName += ".so";
					soFile = new File(libPath + libName);
					if(soFile.exists())
					{
						try
						{
							System.load(libPath + libName);
							bLoaded = true;
						}
						catch(UnsatisfiedLinkError e)
						{
							e.printStackTrace();
						}
					}
				}
			}
			if(bLoaded == false)
			{
				// 尝试加lib前缀
				if(libName.startsWith("lib") == false)
				{
					libName = "lib" + libName;
					soFile = new File(libPath + libName);
					if(soFile.exists())
					{
						try
						{
							System.load(libPath + libName);
							bLoaded = true;
						}
						catch(UnsatisfiedLinkError e)
						{
							e.printStackTrace();
						}
					}
				}
			}
			if(bLoaded == false)
			{
				// 最后尝试使用System.loadLibrary方法
				try
				{
					System.loadLibrary(originName);
					bLoaded = true;
				}
				catch(UnsatisfiedLinkError e)
				{
					e.printStackTrace();
					return false;
				}
			}
		}
		else
		{
			// 使用System.loadLibrary方法
			try
			{
				System.loadLibrary(libName);
			}
			catch(UnsatisfiedLinkError e)
			{
				e.printStackTrace();
				return false;
			}
		}

		return true;
	}
}
