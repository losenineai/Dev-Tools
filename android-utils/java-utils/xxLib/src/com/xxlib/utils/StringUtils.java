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
				// ??????????????????
				sb.append(myBuffer[i]);
			} else if (ub == UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
				// ??????????????????
				int j = (int) myBuffer[i] - 65248;
				sb.append((char) j);
			} else {
				// ??????
				short s = (short) myBuffer[i];
				String hexS = Integer.toHexString(s);
				String unicode = "\\u" + hexS;
				sb.append(unicode.toLowerCase());
			}
		}
		return sb.toString();
	}

	/**
	 * unicode ????????? utf-8
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
	 * ?????????????????????
	 */
	public static String getDownloadTime(int count) {
		if (count < 10000) {
			return count + "?????????";
		} else if (count % 10000 == 0) {
			return (count / 10000) + "????????????";
		} else if (count > 10000) {
			double temp = count / 10000.0;
			NumberFormat ddf = NumberFormat.getNumberInstance();
			ddf.setMaximumFractionDigits(1);
			ddf.format(temp);
			return ((int) temp) + "????????????";
		}
		return count + "";
	}

	/**
	 * ??????????????????????????????2??????????????????1?????????
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
	 * ??????????????????
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
		String regEx = "[`~!@#$%^&*()+=|{}':;',\\[\\].<>/?~???@#???%??????&*????????????+|{}????????????????????????????????????]";
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
	 * String?????????html
	 * @description  
	 * @param str
	 * @return
	 */
	public static Spanned parseToHtml(String str){
		if(str == null){
			return null;
		}
		
//		Log.e("wxj", "before "+str);
		// ???????????????<br/>
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
	 * ???????????????email??????
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
	 * ???????????????QQ???
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
	 * ????????????????????????
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
     * ??????????????????????????????????????????????????????????????????????????? <image1>
     * <p> ??????4???????????????2???????????????1??????
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
        int emojiCount = 0;           // ???????????????
        int emojiLength = 0;       // ?????????EditText???????????????????????????????????????[aaa]???????????????5???????????????????????????????????????
        while (matcher.find()) {
            emojiLength += matcher.group(0).length();
            emojiCount += 1;
        }

        Pattern pattern1 = Pattern.compile(DanMuChattConfig.EXPRESSION_COMMON_PATTERN, Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);
        Matcher matcher1 = pattern1.matcher(s);
        int systemEmojiCount = 0;           // ???????????????
        int systemEmojiLength = 0;       // ?????????EditText???????????????????????????????????????[aaa]???????????????5???????????????????????????????????????
        while (matcher1.find()) {
            systemEmojiLength += matcher1.group(0).length();
            systemEmojiCount += 1;
        }

        int expCount = emojiCount + systemEmojiCount;
        int expLength = emojiLength + systemEmojiLength;

        int chineseCount = getChineseCount(s);
        // remainCount?????????????????????????????????????????????
        int remainCount = (int) StringUtils.calculateLength(s) - chineseCount * 2 - expLength;
        LogTool.i(TAG, "remainCount " + remainCount + ", chineseCount " + chineseCount + ", expCount " + expCount + ", expStrLength " + expLength);
        return remainCount + chineseCount * 2 + expCount * 4;
    }

    /**
     * ????????????????????????????????????Emoji????????????
     * @param s
     * @return
     */
    public static int getSystemEmojiCount(String s){
        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_COMMON_PATTERN, Pattern.UNICODE_CASE | Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(s);
        int expCount = 0;           // ???????????????
        int expStrLength = 0;       // ?????????EditText???????????????????????????????????????[aaa]???????????????5???????????????????????????????????????
        while (matcher.find()) {
            expStrLength += matcher.group(0).length();
            expCount += 1;
        }
        return expCount;
    }

    /**
     * ????????????????????????XMG??????Emoji????????????
     * @param s
     * @return
     */
    public static int getEmojiCount(String s){
        Pattern pattern = Pattern.compile(DanMuChattConfig.EXPRESSION_PATTERN);
        Matcher matcher = pattern.matcher(s);
        int expCount = 0;           // ???????????????
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
     * @description  ???????????????????????????
     * @param filterString ?????????String
     * @param filterNum ???????????????
     * @return ?????????????????????????????????
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
     * ?????????????????????
     * @param text
     * @return
     */
    public static boolean isLegalUserName(String text){
        if(text == null || text.equals("")){
            return false;
        }

        // ??????????????????
//		if(text.matches("^[0-9]{0,16}$")){
//			return false;
//		}

        /**
         * 6-16,
         */
        return text.matches("^[a-zA-Z][a-zA-Z0-9_]{5,19}$");
    }

    /**
     * ??????????????????
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
     * ?????????????????????
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
     * ?????????????????????view????????????????????????????????????????????????
     * @param text ???????????????
     * @param viewWidth ????????????
     * @param textSize ?????????????????????px???
     * @return ????????????????????????
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
     * ?????????????????????view??????????????????????????????????????????????????????????????????\n???
     * @param text ???????????????
     * @param viewWidth ????????????
     * @param textSize ?????????????????????px???
     * @return ????????????????????????
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
     * ???????????????????????????, ??????"%"
     * @param targetNum ????????????
     * @param total ??????
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
	 * ??????????????????
	 * @param format ???????????????
	 * @param objects objects
     * @return ??????????????????????????????
     */
	public static String format(String format, Object... objects) {
		return format(null, format, objects);
	}

	/**
	 * ??????????????????
	 * @param locale ????????????(??????Locale.ENGLISH)
	 * @param format ???????????????
	 * @param objects objects
     * @return ??????????????????????????????
     */
	public static String format(Locale locale, String format, Object... objects) {
		if (locale == null) {
			locale = Locale.ENGLISH;
		}
		return String.format(locale, format, objects);
	}

	public static String changeTimeToPay(long time) {
		// ????????????
		Calendar now = Calendar.getInstance();
		// ????????????
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(time);

		if (calendar.get(Calendar.YEAR) != now.get(Calendar.YEAR)) {
			return new SimpleDateFormat("yyyy-MM-dd HH:mm").format(new Date(time));
		}

		if (calendar.get(Calendar.MONTH) != now.get(Calendar.MONTH)) {
			return new SimpleDateFormat("MM-dd HH:mm").format(new Date(time));
		}

		if (now.get(Calendar.DAY_OF_MONTH) - calendar.get(Calendar.DAY_OF_MONTH) == 1) {
			return "?????? " + new SimpleDateFormat("HH:mm").format(new Date(time));
		}

		if (now.get(Calendar.DAY_OF_MONTH) == calendar.get(Calendar.DAY_OF_MONTH)) {
			return "?????? " + new SimpleDateFormat("HH:mm").format(new Date(time));
		}

		return new SimpleDateFormat("MM-dd HH:mm").format(new Date(time));
	}

	/**
	 * ?????????????????????
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
	 * JUNNET ??????????????? SNDACARD ????????? SZX ????????? ZHENGTU ????????? QQCARD Q?????? UNICOM ?????????
	 * JIUYOU ????????? YPCARD ??????e?????? NETEASE ????????? WANMEI ????????? SOHU ????????? TELECOM ?????????
	 * ZONGYOU ??????????????? TIANXIA ??????????????? TIANHONG ???????????????
	 */
	public static int getAmtByCardName(String name) {
		name = replaceBlank(name);
		if (name.equals(changeStringToFrp("?????????"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("Q??????"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("?????????"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("??????e??????"))) {
			return 0;
		} else if (name.equals(changeStringToFrp("???????????????"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("???????????????"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("???????????????"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("??????"))) {
			return 10;
		} else if (name.equals(changeStringToFrp("??????"))) {
			return 20;
		} else if (name.equals(changeStringToFrp("??????"))) {
			return 50;
		} else if (name.equals(changeStringToFrp("???????????????"))) {
			return 1;
		} else if (name.equals(changeStringToFrp("?????????"))) {
			return 5;
		} else if (name.equals(changeStringToFrp("?????????"))) {
			return 15;
		} else if (name.equals(changeStringToFrp("?????????"))) {
			return 15;
		} else if (name.equals(changeStringToFrp("?????????"))) {
			return 0;
		}

		return 0;
	}

	/**
	 * JUNNET ??????????????? SNDACARD ????????? SZX ????????? ZHENGTU ????????? QQCARD Q?????? UNICOM ?????????
	 * JIUYOU ????????? YPCARD ??????e?????? NETEASE ????????? WANMEI ????????? SOHU ????????? TELECOM ?????????
	 * ZONGYOU ??????????????? TIANXIA ??????????????? TIANHONG ???????????????
	 */
	public static String changeStringToFrp(String name) {
		name = replaceBlank(name);
		if (name.equals("?????????")) {
			return "ZHENGTU";
		} else if (name.equals("Q??????")) {
			return "QQCARD";
		} else if (name.equals("?????????")) {
			return "JIUYOU";
		} else if (name.equals("??????e??????")) {
			return "YPCARD";
		} else if (name.equals("???????????????")) {
			return "ZONGYOU";
		} else if (name.equals("???????????????")) {
			return "TIANXIA";
		} else if (name.equals("???????????????")) {
			return "TIANHONG";
		} else if (name.equals("??????")) {
			return "SZX";
		} else if (name.equals("??????")) {
			return "UNICOM";
		} else if (name.equals("??????")) {
			return "TELECOM";
		} else if (name.equals("???????????????")) {
			return "JUNNET";
		} else if (name.equals("?????????")) {
			return "SNDACARD";
		} else if (name.equals("?????????")) {
			return "NETEASE";
		} else if (name.equals("?????????")) {
			return "WANMEI";
		} else if (name.equals("?????????")) {
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
			return "?????????";
		} else if (frp.equals("QQCARD")) {
			return "Q??????";
		} else if (frp.equals("JIUYOU")) {
			return "?????????";
		} else if (frp.equals("YPCARD")) {
			return "??????\ne??????";
		} else if (frp.equals("ZONGYOU")) {
			return "??????\n?????????";
		} else if (frp.equals("TIANXIA")) {
			return "??????\n?????????";
		} else if (frp.equals("TIANHONG")) {
			return "??????\n?????????";
		} else if (frp.equals("SZX")) {
			return "??????";
		} else if (frp.equals("UNICOM")) {
			return "??????";
		} else if (frp.equals("TELECOM")) {
			return "??????";
		} else if (frp.equals("JUNNET")) {
			return "??????\n?????????";
		} else if (frp.equals("SNDACARD")) {
			return "?????????";
		} else if (frp.equals("NETEASE")) {
			return "?????????";
		} else if (frp.equals("WANMEI")) {
			return "?????????";
		} else if (frp.equals("SOHU")) {
			return "?????????";
		}

		return "";
	}


	/**
	 * ??????String?????????????????????????????????????????????
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
