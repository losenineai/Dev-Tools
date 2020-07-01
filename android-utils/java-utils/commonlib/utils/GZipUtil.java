package com.auric.intell.commonlib.utils;

import android.text.TextUtils;
import android.util.Base64;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

/**
 * @author white
 * @description：GZIP 工具类
 * @date 2017/11/10
 */

public class GZipUtil {

    /**
     * Gzip 压缩数据
     * @param unGzipStr
     * @return
     */
    public static String compress(String unGzipStr) {
        if (TextUtils.isEmpty(unGzipStr)) {
            return null;
        }

        ByteArrayOutputStream baos = null;
        GZIPOutputStream gzipOut = null;
        try {
            baos = new ByteArrayOutputStream();
            gzipOut = new GZIPOutputStream(baos);
            gzipOut.write(unGzipStr.getBytes("UTF-8"));
            gzipOut.close();
            byte[] result = baos.toByteArray();
            return new String(Base64.encode(result, Base64.NO_WRAP), "US-ASCII");
            // 不可使用utf-8，否则解码会失败
//            return baos.toString("UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            try {
                gzipOut.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                baos.flush();
                baos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return null;
    }


    /**
     * Gzip解压数据
     * @param gzipStr
     * @return
     */
    public static String decompress(String gzipStr) {
        if (TextUtils.isEmpty(gzipStr)) {
            return null;
        }

        ByteArrayOutputStream out = null;
        GZIPInputStream gzipInput = null;
        try {
            byte[] t = Base64.decode(gzipStr.getBytes("US-ASCII"), Base64.NO_WRAP);
//            byte[] t = gzipStr.getBytes("UTF-8");
            out = new ByteArrayOutputStream();
            gzipInput = new GZIPInputStream(new ByteArrayInputStream(t));
            byte[] buffer = new byte[1024];
            int n = 0;
            while ((n = gzipInput.read(buffer, 0, buffer.length)) > 0) {
                out.write(buffer, 0, n);
            }

            return out.toString("UTF-8");
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            try {
                gzipInput.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                out.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return null;
    }
}
