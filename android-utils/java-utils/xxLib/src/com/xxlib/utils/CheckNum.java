package com.xxlib.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.zip.CRC32;

/**
 * 校验码生成
 * @author linbin
 * 2014
 */
public class CheckNum {

	public static long check(File file) throws IOException
	{
		//File file = new File("/Users/flamingo/Desktop/WorkTools/python/RM2.3.1.1_2017.apk");
		FileInputStream fis = new FileInputStream(file);
		byte[] bytes = new byte[1024*128];
		int n = 0;
		CRC32 crc32 = new CRC32();
//		System.out.println("文件大小："+fis.available());
		if(fis.available()<=307200)
		{
		while((n=(fis.read(bytes)))!=-1)
		{
		   crc32.update(bytes,0,n);
		}
//		System.out.println("文件小于300K  "+"CRC32值："+crc32.getValue());
		return crc32.getValue();
		}
		else
		{
			
			n = fis.read(bytes);
			long lastposition = fis.available() - 1024*128;
			crc32.update(bytes,0,n);
			fis.skip(lastposition);
//			System.out.println(crc32.getValue()+"   "+fis.available());
			byte[] bytes2 = new byte[1024*128];
			n = fis.read(bytes2);
			crc32.update(bytes2,0,n);
//			System.out.println("文件大于300K  "+"CRC32值："+crc32.getValue());
			return crc32.getValue();
		}
	}
}
