package com.dingxiang.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * 获取字符串或文件的哈希值
 */
public class HashTool {

    /**
     * 默认的密码字符串组合，用来将字节转换成 16 进制表示的字符, apache校验下载的文件的正确性用的就是默认的这个组合
     */
    protected final static char hexDigits[] = {'0', '1', '2', '3', '4', '5',
            '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    /**
     * 计算文件 MD5
     *
     * @param file 文件
     * @throws IOException
     */
    public static String getMD5(File file) throws IOException {
        MessageDigest messageDigest = null;
        try {
            messageDigest = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        InputStream inputStream = new FileInputStream(file);
        byte[] buffer = new byte[4096];
        int numRead;
        while ((numRead = inputStream.read(buffer)) > 0) {
            if (messageDigest != null) {
                messageDigest.update(buffer, 0, numRead);
            } else {
                throw new IOException("MessageDigest is NULL!");
            }
        }

        inputStream.close();

        if (messageDigest != null) {
            byte[] bytes = messageDigest.digest();
            return bytesToHex(bytes);

        } else {
            throw new IOException("MessageDigest is NULL!");
        }

    }


    /**
     * 计算文件 SHA256
     *
     * @param file 文件
     * @throws IOException
     */
    public static String getSHA256(File file) throws IOException {
        MessageDigest messageDigest = null;
        try {
            messageDigest = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        InputStream inputStream = new FileInputStream(file);
        byte[] buffer = new byte[4096];
        int numRead;
        while ((numRead = inputStream.read(buffer)) > 0) {
            if (messageDigest != null) {
                messageDigest.update(buffer, 0, numRead);
            } else {
                throw new IOException("MessageDigest is NULL!");
            }
        }

        inputStream.close();

        if (messageDigest != null) {
            byte[] bytes = messageDigest.digest();
            return bytesToHex(bytes);
        } else {
            throw new IOException("MessageDigest is NULL!");
        }

    }

    /**
     * 计算字节数组的 md5
     *
     * @param aData 字节数组
     */

    public static String getMD5(byte[] aData) throws IOException {
        MessageDigest messagedigest = null;
        try {
            messagedigest = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        if (messagedigest != null) {
            messagedigest.update(aData);
            return bufferToHex(messagedigest.digest());
        } else {
            throw new IOException("MessageDigest is NULL!");
        }
    }

    /**
     * 计算字节数组的 md5
     *
     * @param aData 字节数组
     */

    public static String getSHA256(byte[] aData) throws IOException {
        MessageDigest messagedigest = null;
        try {
            messagedigest = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        if (messagedigest != null) {
            messagedigest.update(aData);
            return bytesToHex(messagedigest.digest());
        } else {
            throw new IOException("MessageDigest is NULL!");
        }
    }

    private static String bytesToHex(byte[] bytes) {
        String str = "";
        String tmp;
        for (byte aByte : bytes) {
            tmp = Integer.toHexString(aByte & 0xFF);
            if (tmp.length() == 1) {
                str += "0";
            }
            str += tmp;
        }

        return str;

    }

    /**
     * 16进制转字符串
     *
     * @param data 字节数组
     * @return 字符串
     */
    static public String hex2Str(byte[] data) {
        final String HEX = "0123456789ABCDEF";
        final StringBuilder sb = new StringBuilder();
        for (final byte b : data) {
            sb.append(HEX.charAt((b >> 4) & 0x0F));
            sb.append(HEX.charAt(b & 0x0F));
        }
        return sb.toString();
    }



    private static String bufferToHex(byte bytes[]) {
        return bytes2Hex(bytes, 0, bytes.length);
    }

    private static String bytes2Hex(byte bytes[], int m, int n) {
        StringBuffer stringbuffer = new StringBuffer(2 * n);
        int k = m + n;
        for (int l = m; l < k; l++) {
            appendHexPair(bytes[l], stringbuffer);
        }
        return stringbuffer.toString();
    }

    private static void appendHexPair(byte bt, StringBuffer stringbuffer) {
        char c0 = hexDigits[(bt & 0xf0) >> 4];// 取字节中高 4 位的数字转换, >>>
        // 为逻辑右移，将符号位一起右移,此处未发现两种符号有何不同
        char c1 = hexDigits[bt & 0xf];// 取字节中低 4 位的数字转换
        stringbuffer.append(c0);
        stringbuffer.append(c1);
    }

}
