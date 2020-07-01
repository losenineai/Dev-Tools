package com.auric.intell.commonlib.utils;

import android.text.Html;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;

import java.io.UnsupportedEncodingException;
import java.lang.Character.UnicodeBlock;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StringUtil {
    private static final String TAG = "StringUtil";


	/**
	 * is null or its length is 0 or it is made by space
	 * 
	 * <pre>
	 * isBlank(null) = true;
	 * isBlank(&quot;&quot;) = true;
	 * isBlank(&quot;  &quot;) = true;
	 * isBlank(&quot;a&quot;) = false;
	 * isBlank(&quot;a &quot;) = false;
	 * isBlank(&quot; a&quot;) = false;
	 * isBlank(&quot;a b&quot;) = false;
	 * </pre>
	 * 
	 * @param str
	 * @return if string is null or its size is 0 or it is made by space, return
	 *         true, else return false.
	 */
	public static boolean isBlank(String str) {
		return (str == null || str.trim().length() == 0);
	}

	/**
	 * is null or its length is 0
	 * 
	 * <pre>
	 * isEmpty(null) = true;
	 * isEmpty(&quot;&quot;) = true;
	 * isEmpty(&quot;  &quot;) = false;
	 * </pre>
	 * 
	 * @param str
	 * @return if string is null or its size is 0, return true, else return
	 *         false.
	 */
	public static boolean isEmpty(String str) {
		return (str == null || str.length() == 0);
	}

	public static String toUTF8String(String input) {
		try {
			// Convert from Unicode to UTF-8
			String string = input;
			byte[] utf8 = string.getBytes("UTF-8");
			// Convert from UTF-8 to Unicode
			string = new String(utf8, "UTF-8");
			return string;
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return "";
	}

	public static boolean isPasswordContent(String str)
	{
		Pattern pattern = Pattern.compile("[a-zA-Z0-9]+");
		Matcher matcher = pattern.matcher(str);

		if (matcher.matches())
		{
			return true;
		}
		return false;
	}


	public static String utf8ToUnicode(String inStr) {
		char[] myBuffer = inStr.toCharArray();
		// Log.e("!!"+inStr,"!!"+inStr);
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < inStr.length(); i++) {
			UnicodeBlock ub = UnicodeBlock.of(myBuffer[i]);
			if (ub == UnicodeBlock.BASIC_LATIN) {
				// 英文及数字等
				sb.append(myBuffer[i]);
			} else if (ub == UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
				// 全角半角字符
				int j = (int) myBuffer[i] - 65248;
				sb.append((char) j);
			} else {
				// 汉字
				short s = (short) myBuffer[i];
				String hexS = Integer.toHexString(s);
				String unicode = "\\u" + hexS;
				sb.append(unicode.toLowerCase());
			}
		}
		return sb.toString();
	}

	/**
	 * unicode 转换成 utf-8
	 * 
	 * @author fanhui 2007-3-15
	 * @param theString
	 * @return
	 */
	public static String unicodeToUtf8(String theString) {
		char aChar;
		int len = theString.length();
		StringBuffer outBuffer = new StringBuffer(len);
		for (int x = 0; x < len;) {
			aChar = theString.charAt(x++);
			if (aChar == '\\') {
				aChar = theString.charAt(x++);
				if (aChar == 'u') {
					// Read the xxxx
					int value = 0;
					for (int i = 0; i < 4; i++) {
						aChar = theString.charAt(x++);
						switch (aChar) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							value = (value << 4) + aChar - '0';
							break;
						case 'a':
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
							value = (value << 4) + 10 + aChar - 'a';
							break;
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
							value = (value << 4) + 10 + aChar - 'A';
							break;
						default:
							throw new IllegalArgumentException("Malformed   \\uxxxx   encoding.");
						}
					}
					outBuffer.append((char) value);
				} else {
					if (aChar == 't')
						aChar = '\t';
					else if (aChar == 'r')
						aChar = '\r';
					else if (aChar == 'n')
						aChar = '\n';
					else if (aChar == 'f')
						aChar = '\f';
					outBuffer.append(aChar);
				}
			} else
				outBuffer.append(aChar);
		}
		return outBuffer.toString();
	}



	/**
	 * 判定输入汉字
	 * 
	 * @param c
	 * @return
	 */
	public static boolean isChinese(char c) {
		UnicodeBlock ub = UnicodeBlock.of(c);
		if (ub == UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS
				|| ub == UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS
				|| ub == UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A
				|| ub == UnicodeBlock.GENERAL_PUNCTUATION
				|| ub == UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION
				|| ub == UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
			return true;
		}
		return false;
	}

	public static int getChineseCount(String s) {
		int num = 0;
		for(int i = 0;i < s.length(); i++)
		{
			char c = s.charAt(i);
			UnicodeBlock ub = UnicodeBlock.of(c);
			if (ub == UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS
					|| ub == UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS
					|| ub == UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A
					|| ub == UnicodeBlock.GENERAL_PUNCTUATION
					|| ub == UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION
					|| ub == UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
				num ++;
			}
		}
		return num;
	}


	public static String replaceReturn(String str) {
		String dest = "";
		if (str != null) {
			Pattern p = Pattern.compile("\r|\n");
			Matcher m = p.matcher(str);
			dest = m.replaceAll("");
		}
		return dest;
	}
	
	
	/**
	 * String解析成html
	 * @description  
	 * @param str
	 * @return
	 */
	public static Spanned parseToHtml(String str){
		if(str == null){
			return null;
		}
		
//		Log.e("wxj", "before "+str);
		// 换行替换成<br/>
//		Pattern p = Pattern.compile("\r|\n");
		try{
			Pattern p = Pattern.compile("\r\n");
			Matcher m = p.matcher(str);
			str = m.replaceAll("<br/>");
			
			p = Pattern.compile("\n");
			m = p.matcher(str);
			str = m.replaceAll("<br/>");
			
			p = Pattern.compile("\r");
			m = p.matcher(str);
			str = m.replaceAll("<br/>");
//			str = str.replace("&", "&amp;");
//		Log.e("wxj", "after "+str);
			return Html.fromHtml(str);
		}catch(Exception er){
			
		}
		
		return new SpannableString(str);
	}
	
	public static String phonenumChangeUnsee(String phoneNum){
		
		String num = null;
		try {
			 num = phoneNum.substring(0, 5) + "****" + phoneNum.substring(9, 11);
		} catch (Exception e) {
			
		}
		return num;
	}

	/**
	 * 是否合法的email格式
	 * @description  
	 * @param email
	 * @return
	 */
	public static boolean isLegalEmail(String email){
		if(TextUtils.isEmpty(email)){
			return false;
		}
		
		return email.matches("^[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
	}
	
	/**
	 * 是否合法的QQ号
	 * @description  
	 * @param qq
	 * @return
	 */
	public static boolean isLegalQQ(String qq){
		if(TextUtils.isEmpty(qq)){
			return false;
		}
		
		return qq.matches("[0-9]{5,11}");
	}
	
	/**
	 * 是否合法的手机号
	 * @description  
	 * @param phoneNum
	 * @return
	 */
	public static boolean isLegalPhoneNum(String phoneNum){
		if(TextUtils.isEmpty(phoneNum)){
			return false;
		}
		
		return phoneNum.matches("^[1][3|4|5|7|8]\\d{9}$");
	}





}
