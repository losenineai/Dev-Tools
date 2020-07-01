package com.auric.intell.commonlib.utils;

import android.text.TextUtils;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by white on 16/7/19.
 */
public class CharsetUtil {


    public static String utf8ToUnicode(String inStr) {
        char[] myBuffer = inStr.toCharArray();
        // Log.e("!!"+inStr,"!!"+inStr);
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < inStr.length(); i++) {
            Character.UnicodeBlock ub = Character.UnicodeBlock.of(myBuffer[i]);
            if (ub == Character.UnicodeBlock.BASIC_LATIN) {
                // 英文及数字等
                sb.append(myBuffer[i]);
            } else if (ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {
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
        if(TextUtils.isEmpty(theString)){
            return theString;
        }
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
     * 把json格式内容转成utf-8编码
     * @param jsonStr
     * @return
     */
    public static String toUtf8ByJson(String jsonStr){
        if(TextUtils.isEmpty(jsonStr)){
            return jsonStr;
        }

        // unicode转utf-8
        try {
            org.json.JSONArray jsonArray = new org.json.JSONArray(jsonStr);
            jsonStr = jsonArray.toString();
        } catch (JSONException e) {
            JSONObject jsonObj = null;
            try {
                jsonObj = new JSONObject(jsonStr);
                jsonStr = jsonObj.toString();
            } catch (JSONException e1) {
            }
        }
        return jsonStr;
    }

}
