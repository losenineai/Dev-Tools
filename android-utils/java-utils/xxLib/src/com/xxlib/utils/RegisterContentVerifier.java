package com.xxlib.utils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;


/**
 * 检测用户输入的内容合法性
 * @author wxj
 * 2014
 */
public class RegisterContentVerifier {

	/**
	 * 是否合法用户名
	 * @param text
	 * @return
	 */
	public static boolean isLegalUserName(String text){
		if(text == null || text.equals("")){
			return false;
		}
		
		// 不能是纯数字
//		if(text.matches("^[0-9]{0,16}$")){
//			return false;
//		}
		
		/**
		 * 6-16,
		 */
		return text.matches("^[a-zA-Z][a-zA-Z0-9_]{5,19}$");
	}
	
	/**
	 * 是否合法密码
	 * @param text
	 * @return
	 */
	public static boolean isLegalPassword(String text){
		if(text == null || text.equals("")){
			return false;
		}
		return text.matches("^[a-zA-Z0-9]{6,16}$");
	}
	
	
	/**
	 * 是否合法的手机号码
	 * @param text
	 * @return
	 */
	public static boolean isLegalPhoneNum(String text){
		if(text == null || text.equals("") || !text.trim().equals(text)){
			return false;
		}
		
		
		return text.matches("^[1]\\d{10}$");
	}
	
	
	/**
	 * 是否合法的昵称
	 * @param text
	 * @return
	 */
	public static boolean isLegalNickName(String text, int maxSize){
		if(text == null || text.equals("")){
			return false;
		}
		return text.matches("^[a-zA-Z0-9\\u4e00-\\u9fa5_]{1,"+maxSize+"}$") && StringUtils.calculateLength(text) <= maxSize;
	}
	
	/**
	 * 是否合法签名
	 * @param text
	 * @return
	 */
	public static boolean isLegalSignature(String text)
	{
//		if(text == null || text.equals("")){
//			return false;
//		}
//		return text.matches("^[a-zA-Z0-9\\u4e00-\\u9fa5_]{1,8}$");
		return true;
	}
	
	/**
     * 是否合法Email地址
     * 
     * @param email
     * @return boolean true for valid false for invalid
     */
    public static boolean isEmailValid(String email) {
        boolean isValid = false;

        String expression = "^[\\w\\.-]+@([\\w\\-]+\\.)+[A-Z]{2,4}$";
        CharSequence inputStr = email;

        Pattern pattern = Pattern.compile(expression, Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(inputStr);
        if (matcher.matches()) {
            isValid = true;
        }
        return isValid;
    }
	
}
