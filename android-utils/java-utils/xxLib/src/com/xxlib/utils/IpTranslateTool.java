package com.xxlib.utils;

import java.nio.ByteBuffer;

/**
 * 
 * @Description 整型的ip格式转为字符串工具
 *
 * 版本号    作者    日期    相关修改
 *  1.0   linbin  2014-6-6  创建
 *
 * @author linbin
 * 2014-6-6
 */
public class IpTranslateTool {

	public static String intToString(int ip)
	{
		 
//		int part1, part2, part3, part4;
//
//	    part1 = ip/16777216;
//	    ip = ip%16777216;
//	    part2 = ip/65536;
//	    ip = ip%65536;
//	    part3 = ip/256;
//	    ip = ip%256;
//	    part4 = ip;
//
//	    String ipString = part1+"."+part2+"."+part3+"."+part4;
//	    
//	    Log.e(ipString, ipString);
 
		byte[] bytes = ByteBuffer.allocate(4).putInt(ip).array();
		int[] parts = new int[4];
		for (int i=0; i<4; ++i) {
			parts[i] = bytes[i] & 0xFF;
		}
		
	    String ipString2 = parts[0]+"."+parts[1]+"."+parts[2]+"."+parts[3];

		return ipString2;
	}
}
