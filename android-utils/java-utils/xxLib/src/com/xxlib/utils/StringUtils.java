package com.xxlib.utils;

import android.content.Context;
import android.text.Html;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.util.TypedValue;
import android.widget.TextView;

import com.xxlib.config.DanMuChattConfig;
import com.xxlib.utils.base.LogTool;

import java.io.UnsupportedEncodingException;
import java.lang.Character.UnicodeBlock;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class StringUtils {
    private static final String TAG = "StringUtils";

	public static Spanned parseToWebUrl(String str, int color) {
		SpannableString spannableString = new SpannableString(str);
		ExLinkify.addLinks(spannableString, ExLinkify.WEB_URLS, color);
		return spannableString;
	}


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
	 * 格式化下载次数
	 */
	public static String getDownloadTime(int count) {
		if (count < 10000) {
			return count + "次下载";
		} else if (count % 10000 == 0) {
			return (count / 10000) + "万次下载";
		} else if (count > 10000) {
			double temp = count / 10000.0;
			NumberFormat ddf = NumberFormat.getNumberInstance();
			ddf.setMaximumFractionDigits(1);
			ddf.format(temp);
			return ((int) temp) + "万次下载";
		}
		return count + "";
	}

	/**
	 * 计算字符串长度，中文2个长度，英文1个长度
	 * 
	 * @param c
	 * @return
	 */
	public static long calculateLength(CharSequence c) {
		double len = 0;
		for (int i = 0; i < c.length(); i++) {
			if (StringUtils.isChinese(c.charAt(i))) {
				len += 2;
			} else {
				len++;
			}
		}
		return Math.round(len);
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

	public static boolean hasSpecialCommon(String str) {
		String regEx = "[`~!@#$%^&*()+=|{}':;',\\[\\].<>/?~！@#￥%……&*（）——+|{}【】‘；：”“’。，、？]";
		Pattern p = Pattern.compile(regEx);
		Matcher m = p.matcher(str);
		return m.find();
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

    /**========================xmod method start===========================**/
    public static int getChineseSymbolCount(String str){
        String regEX = "[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]";
        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN);
        Matcher matcher = pattern.matcher(str);
        int symbolCount = 0;
        while (matcher.find()) {
            symbolCount += 1;
        }
        return symbolCount;
    }

    /**
     * 获取产品形态上的字符长度，图片会被当做普通字符处理 <image1>
     * <p> 表情4字符，中文2字符，英文1字符
     * @param s
     * @return
     */
    public static int getWordingLength(String s) {
        if (null == s) {
            return 0;
        }
        LogTool.i(TAG, "wording is " + s);

        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN);
        Matcher matcher = pattern.matcher(s);
        int emojiCount = 0;           // 表情的个数
        int emojiLength = 0;       // 表情在EditText中的实际长度，例如一个表情[aaa]，这样子是5字符长度，代码实现上的长度
        while (matcher.find()) {
            emojiLength += matcher.group(0).length();
            emojiCount += 1;
        }

        Pattern pattern1 = Pattern.compile(DanMuChattConfig.EXPRESSION_COMMON_PATTERN, Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);
        Matcher matcher1 = pattern1.matcher(s);
        int systemEmojiCount = 0;           // 表情的个数
        int systemEmojiLength = 0;       // 表情在EditText中的实际长度，例如一个表情[aaa]，这样子是5字符长度，代码实现上的长度
        while (matcher1.find()) {
            systemEmojiLength += matcher1.group(0).length();
            systemEmojiCount += 1;
        }

        int expCount = emojiCount + systemEmojiCount;
        int expLength = emojiLength + systemEmojiLength;

        int chineseCount = getChineseCount(s);
        // remainCount是除了表情、中文之后的字符个数
        int remainCount = (int) StringUtils.calculateLength(s) - chineseCount * 2 - expLength;
        LogTool.i(TAG, "remainCount " + remainCount + ", chineseCount " + chineseCount + ", expCount " + expCount + ", expStrLength " + expLength);
        return remainCount + chineseCount * 2 + expCount * 4;
    }

    /**
     * 获取字符串中含有系统自带Emoji表情个数
     * @param s
     * @return
     */
    public static int getSystemEmojiCount(String s){
        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_COMMON_PATTERN, Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(s);
        int expCount = 0;           // 表情的个数
        int expStrLength = 0;       // 表情在EditText中的实际长度，例如一个表情[aaa]，这样子是5字符长度，代码实现上的长度
        while (matcher.find()) {
            expStrLength += matcher.group(0).length();
            expCount += 1;
        }
        return expCount;
    }

    /**
     * 获取字符串中含有XMG自带Emoji表情个数
     * @param s
     * @return
     */
    public static int getEmojiCount(String s){
        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN);
        Matcher matcher = pattern.matcher(s);
        int expCount = 0;           // 表情的个数
        while (matcher.find()) {
            expCount += 1;
        }
        return expCount;
    }

    public static int getEmptyCount(String s) {
        Pattern pattern = Pattern.compile("\\s");
        Matcher matcher = pattern.matcher(s);
        int count = 0;
        while (matcher.find()) {
            ++count;
        }
        return count;
    }

    /**
     * weaiken
     * @description  字符串截断显示处理
     * @param filterString 原来的String
     * @param filterNum 需要的长度
     * @return 超过长度使用。。。代替
     */
    public static String shortenString(String filterString , int filterNum){
        if(filterString != null&& !filterString.isEmpty()) {
            if(filterString.length() <= filterNum){
                return filterString;

            }else{
                return filterString.subSequence(0, filterNum)+"...";
            }


        }else{
            return "";
        }

    }

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
        return text.matches("^[a-zA-Z0-9]{6,20}$");
    }


    /**
     * 是否合法的昵称
     * @param text
     * @return
     */
    public static boolean isLegalNickname(String text){
        if(text == null || text.equals("")){
            return false;
        }
//		return text.matches("^[a-zA-Z0-9\\u4e00-\\u9fa5_]{1,30}$");
        return text.matches("^[a-zA-Z0-9_.]{1,30}$");
    }
    /**========================xmod method end===========================**/

    /**
     * 根据固定显示的view宽度和字体大小，对字符串进行截断
     * @param text 原始字符串
     * @param viewWidth 显示宽度
     * @param textSize 字体大小（单位px）
     * @return 截断的字符串数组
     */
    public static ArrayList<String> subStringByWidth(String text, int viewWidth, int textSize) {
        if (TextUtils.isEmpty(text) || viewWidth <= 0 || textSize <= 0) {
            return null;
        }
        ArrayList<String> subStrings = new ArrayList<>();
        Context context = ContextFinder.getApplication();
        TextView measureText = new TextView(context);
        measureText.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
        int index = 0;
        StringBuilder stringBuilder = new StringBuilder();
        int curWidth;
        char c;
        while (index < text.length()) {
            c = text.charAt(index);
            stringBuilder.append(c);
            if (index == text.length() - 1) {
                subStrings.add(stringBuilder.toString());
                break;
            }
            curWidth = (int) measureText.getPaint().measureText(stringBuilder.toString());
            if (curWidth > viewWidth) {
                stringBuilder.deleteCharAt(stringBuilder.length() - 1);
                subStrings.add(stringBuilder.toString());
                index--;
                stringBuilder.delete(0, stringBuilder.length());
            }
            index++;
        }
        return subStrings;
    }

    /**
     * 根据固定显示的view宽度和字体大小，对字符串进行截断，带换行符（\n）
     * @param text 原始字符串
     * @param viewWidth 显示宽度
     * @param textSize 字体大小（单位px）
     * @return 截断的字符串数组
     */
    public static ArrayList<String> subStringByWidthIncludeEnterChar(String text, int viewWidth, int textSize) {
        if (TextUtils.isEmpty(text) || viewWidth <= 0 || textSize <= 0) {
            return null;
        }
        ArrayList<String> subStrings = new ArrayList<>();
        Context context = ContextFinder.getApplication();
        TextView measureText = new TextView(context);
        measureText.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
        int index = 0;
        StringBuilder stringBuilder = new StringBuilder();
        int curWidth;
        char c;
        while (index < text.length()) {
            c = text.charAt(index);
            stringBuilder.append(c);
            if (index == text.length() - 1) {
                subStrings.add(stringBuilder.toString());
                break;
            }
            if (c == '\n') {
                subStrings.add(stringBuilder.toString());
                stringBuilder.delete(0, stringBuilder.length());
                index++;
                continue;
            }
            curWidth = (int) measureText.getPaint().measureText(stringBuilder.toString());
            if (curWidth > viewWidth) {
                stringBuilder.deleteCharAt(stringBuilder.length() - 1);
                subStrings.add(stringBuilder.toString());
                index--;
                stringBuilder.delete(0, stringBuilder.length());
            }
            index++;
        }
        return subStrings;
    }

    /**
     * 计算礼包剩余百分比, 不含"%"
     * @param targetNum 目标数量
     * @param total 总数
     * @return
     */
    public static int parseToPercent(int targetNum, int total) {
		if(total<=0)
			return 0;

        int result;
        result = targetNum * 100 / total;
        if (result == 0 && targetNum > 0) {
            result = 1;
        } else if (result == 100 && targetNum < total) {
            result = 99;
        }
        return result;
    }

	/**
	 * 字符串格式化
	 * @param format 格式字符串
	 * @param objects objects
     * @return 返回格式化后的字符串
     */
	public static String format(String format, Object... objects) {
		return format(null, format, objects);
	}

	/**
	 * 字符串格式化
	 * @param locale 语言环境(默认Locale.ENGLISH)
	 * @param format 格式字符串
	 * @param objects objects
     * @return 返回格式化后的字符串
     */
	public static String format(Locale locale, String format, Object... objects) {
		if (locale == null) {
			locale = Locale.ENGLISH;
		}
		return String.format(locale, format, objects);
	}

	public static String changeTimeToPay(long time) {
		// 现在时间
		Calendar now = Calendar.getInstance();
		// 指定时间
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(time);

		if (calendar.get(Calendar.YEAR) != now.get(Calendar.YEAR)) {
			return new SimpleDateFormat("yyyy-MM-dd HH:mm").format(new Date(time));
		}

		if (calendar.get(Calendar.MONTH) != now.get(Calendar.MONTH)) {
			return new SimpleDateFormat("MM-dd HH:mm").format(new Date(time));
		}

		if (now.get(Calendar.DAY_OF_MONTH) - calendar.get(Calendar.DAY_OF_MONTH) == 1) {
			return "昨天 " + new SimpleDateFormat("HH:mm").format(new Date(time));
		}

		if (now.get(Calendar.DAY_OF_MONTH) == calendar.get(Calendar.DAY_OF_MONTH)) {
			return "今天 " + new SimpleDateFormat("HH:mm").format(new Date(time));
		}

		return new SimpleDateFormat("MM-dd HH:mm").format(new Date(time));
	}

	/**
	 * 是否合法的昵称
	 *
	 * @param text
	 * @return
	 */
	public static boolean isLegalNickName(String text) {
		if (text == null || text.equals("")) {
			return false;
		}
		return calculateLength(text) <= 24 && text.matches("^[a-zA-Z0-9\\u4e00-\\u9fa5_]{1,24}$");
	}

	public static boolean isEquals(String actual, String expected) {
		return ObjectUtils.isEquals(actual, expected);
	}

	/**
	 * JUNNET 骏网一卡通 SNDACARD 盛大卡 SZX 神州行 ZHENGTU 征途卡 QQCARD Q币卡 UNICOM 联通卡
	 * JIUYOU 久游卡 YPCARD 易宝e卡通 NETEASE 网易卡 WANMEI 完美卡 SOHU 搜狐卡 TELECOM 电信卡
	 * ZONGYOU 纵游一卡通 TIANXIA 天下一卡通 TIANHONG 天宏一卡通
	 */
	public static int getAmtByCardName(String name) {
		name = replaceBlank(name);
		if (name.equals(changeStringToFrp("征途卡"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("Q币卡"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("久游卡"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("易宝e卡通"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("纵游一卡通"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("天下一卡通"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("天宏一卡通"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("移动"))) {
			return 10;
		} else if (name.equals(changeStringToFrp("联通"))) {
			return 20;
		} else if (name.equals(changeStringToFrp("电信"))) {
			return 50;
		} else if (name.equals(changeStringToFrp("骏网一卡通"))) {
			return 1;
		} else if (name.equals(changeStringToFrp("盛大卡"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("网易卡"))) {
			return 15;
		} else if (name.equals(changeStringToFrp("完美卡"))) {
			return 15;
		} else if (name.equals(changeStringToFrp("搜狐卡"))) {
			return 0;
		}

		return 0;
	}

	/**
	 * JUNNET 骏网一卡通 SNDACARD 盛大卡 SZX 神州行 ZHENGTU 征途卡 QQCARD Q币卡 UNICOM 联通卡
	 * JIUYOU 久游卡 YPCARD 易宝e卡通 NETEASE 网易卡 WANMEI 完美卡 SOHU 搜狐卡 TELECOM 电信卡
	 * ZONGYOU 纵游一卡通 TIANXIA 天下一卡通 TIANHONG 天宏一卡通
	 */
	public static String changeStringToFrp(String name) {
		name = replaceBlank(name);
		if (name.equals("征途卡")) {
			return "ZHENGTU";
		} else if (name.equals("Q币卡")) {
			return "QQCARD";
		} else if (name.equals("久游卡")) {
			return "JIUYOU";
		} else if (name.equals("易宝e卡通")) {
			return "YPCARD";
		} else if (name.equals("纵游一卡通")) {
			return "ZONGYOU";
		} else if (name.equals("天下一卡通")) {
			return "TIANXIA";
		} else if (name.equals("天宏一卡通")) {
			return "TIANHONG";
		} else if (name.equals("移动")) {
			return "SZX";
		} else if (name.equals("联通")) {
			return "UNICOM";
		} else if (name.equals("电信")) {
			return "TELECOM";
		} else if (name.equals("骏网一卡通")) {
			return "JUNNET";
		} else if (name.equals("盛大卡")) {
			return "SNDACARD";
		} else if (name.equals("网易卡")) {
			return "NETEASE";
		} else if (name.equals("完美卡")) {
			return "WANMEI";
		} else if (name.equals("搜狐卡")) {
			return "SOHU";
		}

		return "";
	}

	public static String getNumberForString(String str) {
		int count = str.length();
		String result = "";
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < 10; j++) {
				if (String.valueOf(str.charAt(i)).equals(String.valueOf(j))) {
					result += str.charAt(i);
					break;
				}
			}
		}
		return result;
	}


	public static String changeFrpToString(String frp) {
		frp = replaceBlank(frp);
		if (frp.equals("ZHENGTU")) {
			return "征途卡";
		} else if (frp.equals("QQCARD")) {
			return "Q币卡";
		} else if (frp.equals("JIUYOU")) {
			return "久游卡";
		} else if (frp.equals("YPCARD")) {
			return "易宝\ne卡通";
		} else if (frp.equals("ZONGYOU")) {
			return "纵游\n一卡通";
		} else if (frp.equals("TIANXIA")) {
			return "天下\n一卡通";
		} else if (frp.equals("TIANHONG")) {
			return "天宏\n一卡通";
		} else if (frp.equals("SZX")) {
			return "移动";
		} else if (frp.equals("UNICOM")) {
			return "联通";
		} else if (frp.equals("TELECOM")) {
			return "电信";
		} else if (frp.equals("JUNNET")) {
			return "骏网\n一卡通";
		} else if (frp.equals("SNDACARD")) {
			return "盛大卡";
		} else if (frp.equals("NETEASE")) {
			return "网易卡";
		} else if (frp.equals("WANMEI")) {
			return "完美卡";
		} else if (frp.equals("SOHU")) {
			return "搜狐卡";
		}

		return "";
	}


	/**
	 * 去除String中的空格、回车、换行符、制表符
	 */
	public static String replaceBlank(String str) {
		String dest = "";
		if (str != null) {
			Pattern p = Pattern.compile("\\s*|\t|\r|\n");
			Matcher m = p.matcher(str);
			dest = m.replaceAll("");
		}
		return dest;
	}



}
