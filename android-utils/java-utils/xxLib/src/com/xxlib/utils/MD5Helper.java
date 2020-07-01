package com.xxlib.utils;

import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.math.BigInteger;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MD5Helper {

	private static final String TAG = "MD5Helper";

    /**
     * 比较md5，由于以前的md5工具，前缀是0会去掉，所以比较的时候，会去掉MD5开头的0（如果开头有0的话）
     */
    public static boolean compareMD5(String a, String b) {
        if (TextUtils.isEmpty(a) || TextUtils.isEmpty(b))
            return false;
        String stra = deletePrefixZero(a);
        String strb = deletePrefixZero(b);
        return stra.equals(strb);
    }

    private static String deletePrefixZero(String s) {
        if (TextUtils.isEmpty(s))
            return s;
        String ret = s;
        while (ret.startsWith("0")) {
            ret = ret.substring(1);
        }
        return ret;
    }

	public static String encode(String str) {
	    MessageDigest digester;
		try {
			digester = MessageDigest.getInstance("MD5");
			
		    digester.update(str.getBytes());
		    byte[] hash = digester.digest();
		    StringBuffer hexString = new StringBuffer();
		    for (int i = 0; i < hash.length; i++) {
		        if ((0xff & hash[i]) < 0x10) {
		            hexString.append("0" + Integer.toHexString((0xFF & hash[i])));
		        } else {
		            hexString.append(Integer.toHexString(0xFF & hash[i]));
		        }
		    }
		    return hexString.toString();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	
	}
	
	
	public static String getFileMD5(File file){
		if (null == file || !file.exists()) {
			return "";
		}
		
		try {
			return getFileMD5(new FileInputStream(file));
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		
		return "";
	}
	
	
	/**
	 * @description 内部会调用inputStream close  
	 * @param inputStream
	 * @return
	 */
	public static String getFileMD5(InputStream inputStream){
		if (null == inputStream) {
			return "";
		}
		int buffersize = 512 * 1024;
		DigestInputStream digestIn = null;
		try {
			MessageDigest messageDigest = MessageDigest.getInstance("MD5");
			// 该流的特点是在每次给该流传输数据都回调用update方法，受on（）方法的控制；
			digestIn = new DigestInputStream(inputStream, messageDigest);
			digestIn.on(true);
			byte[] buffer = new byte[buffersize];

			while(digestIn.read(buffer) > 0)
				; //do nothing

			// 获得最终的结果
			messageDigest = digestIn.getMessageDigest();
			
			BigInteger bigInt = new BigInteger(1, messageDigest.digest());
			digestIn.close();
			inputStream.close();
			String md5 = bigInt.toString(16);
            while (md5.length() < 32) {
                md5 = "0" + md5;
            }
			return md5;
		} catch (Exception e) {
			e.printStackTrace();
		} 
		return "";
	}
}