package com.xxlib.utils;

import android.text.TextUtils;

/**
 * 字符串过滤工具类
 * @author linbin
 * 2014
 */
public class StringFilter {

	/**
	 * weaiken
	 * @description  字符串截断显示处理
	 * @param filterString 原来的String
	 * @param filterNum 需要的长度
	 * @return 超过长度使用。。。代替
	 */
	public static String shortenString(String filterString , int filterNum){
		if(filterString != null&& filterString.length() > 0){
			if(filterString.length() <= filterNum){
				return filterString;
				
			}else{
				return filterString.subSequence(0, filterNum)+"...";
			}
		
			
		}else{
			
			return "no message ";
		}
		
	}
	
	
	/**
	 * 截取字符串
	 * @description  
	 * @param filterString 源字符串
	 * @param filterNum 截取前filterNum个字符
	 * @return
	 */
	public static String cutString(String filterString , int filterNum){
		if(filterString != null&& filterString.length() > 0){
			if(filterString.length() <= filterNum){
				return filterString;
				
			}else{
				return filterString.subSequence(0, filterNum).toString();
			}
		
			
		}else{
			
			return "";
		}
		
	}
	
	/**
	 * wxj, 移除末尾所有换行符号
	 * @param srcStr
	 * @return
	 */
	public static String cutEndEnterChar(String srcStr){
		if(TextUtils.isEmpty(srcStr)){
			return srcStr;
		}
		
		String enterChar = "\n";
		
		if(srcStr.lastIndexOf(enterChar) == (srcStr.length() - 1)){
			srcStr = cutEndEnterChar(srcStr.substring(0, srcStr.length() - 1));
		}
		
		return srcStr;
		
	}

}