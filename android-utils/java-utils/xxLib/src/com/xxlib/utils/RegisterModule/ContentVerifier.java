package com.xxlib.utils.RegisterModule;


import com.xxlib.utils.RegisterContentVerifier;

/**
 * 检测用户输入的内容合法性
 * @author wxj
 * 2014
 */
public class ContentVerifier {

	/**
	 * 是否合法用户名
	 * @param text
	 * @return
	 */
	public static boolean isLegalUserName(String text){
        return RegisterContentVerifier.isLegalUserName(text);
	}
	
	/**
	 * 是否合法密码
	 * @param text
	 * @return
	 */
	public static boolean isLegalPassword(String text){
		return RegisterContentVerifier.isLegalPassword(text);
	}
	
	
	
	
	/**
	 * 是否合法的昵称
	 * @param text
	 * @return
	 */
    private static final int NICK_NAME_MAX_LENGTH = 24;
	public static boolean isLegalNickName(String text){
		return RegisterContentVerifier.isLegalNickName(text, NICK_NAME_MAX_LENGTH);
	}
}
