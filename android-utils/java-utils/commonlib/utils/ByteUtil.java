package com.auric.intell.commonlib.utils;

/**
 * 字节处理工具类
 * 
 * @author sun.jun
 * @serial
 * @since 2012-8-15 上午10:31:24
 */
public class ByteUtil {

    /**
     * 字节数组转16进制字符串
     * 
     * @param b
     *            字节数组
     * @return 16进制字符串
     */
    public static String byte2hex(byte[] b) {
        StringBuffer hs = new StringBuffer();
        String stmp;
        for (int i = 0; i < b.length; i++) {
            stmp = Integer.toHexString(b[i] & 0xFF);
            if (stmp.length() == 1) {
                hs.append("0").append(stmp);
            } else {
                hs.append(stmp);
            }
        }
        return hs.toString();
    }

    /**
     * 16进制字符串转字节数组
     * 
     * @param hex
     *            16进制字符串
     * @return 字节数组
     */
    public static byte[] hex2byte(String hex) {
        if (!isHexString(hex)) {
            return null;
        }
        char[] arr = hex.toCharArray();
        byte[] b = new byte[hex.length() / 2];
        for (int i = 0, j = 0, l = hex.length(); i < l; i++, j++) {
            String swap = "" + arr[i++] + arr[i];
            int byteint = Integer.parseInt(swap, 16) & 0xFF;
            b[j] = new Integer(byteint).byteValue();
        }
        return b;
    }

    /**
     * 校验是否是16进制字符串
     * 
     * @param hex
     * @return
     */
    public static boolean isHexString(String hex) {
        if (hex == null || hex.length() % 2 != 0) {
            return false;
        }
        for (int i = 0; i < hex.length(); i++) {
            char c = hex.charAt(i);
            if (!isHexChar(c)) {
                return false;
            }
        }
        return true;
    }

    /**
     * 校验是否是16进制字符
     * 
     * @param c
     * @return
     */
    private static boolean isHexChar(char c) {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    }

}