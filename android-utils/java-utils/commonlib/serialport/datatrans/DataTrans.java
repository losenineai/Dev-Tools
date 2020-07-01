package com.auric.intell.commonlib.serialport.datatrans;

/**
 * Created by suihao on 2017/4/21.
 */

public class DataTrans {
    public static String bytesToString(byte[] instruction) {
        String message = "";
        for (int i = 0; i < instruction.length; i++) {
            String hex = Integer.toHexString(instruction[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            message += hex + " ";
        }
        return message;
    }

    public static byte[] hexStringToBytes(String hexString) {
        if (hexString == null || hexString.equals("")) {
            return null;
        }
        hexString=hexString.replaceAll(" ", "");
        hexString = hexString.toUpperCase();
        int length = hexString.length() / 2;

        char[] hexChars = hexString.toCharArray();
        byte[] d = new byte[length];
        for (int i = 0; i < length; i++) {
            int pos = i * 2;
            d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));
        }
        return d;
    }
    private static byte charToByte(char c) {
        return (byte) "0123456789ABCDEF".indexOf(c);
    }
}
