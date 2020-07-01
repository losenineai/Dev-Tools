package com.auric.intell.commonlib.utils;

import android.text.TextUtils;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

/**
 * @author white
 * @description：
 * @date 2017/11/10
 */

public class ZipUtil {


    /**
     * Zip加压数据
     * @param unZipStr
     * @return
     */
    public static String compress(String unZipStr) {

        if (TextUtils.isEmpty(unZipStr)) {
            return null;
        }
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            ZipOutputStream zip = new ZipOutputStream(baos);
            zip.putNextEntry(new ZipEntry("0"));
            zip.write(unZipStr.getBytes());
            zip.closeEntry();
            zip.close();
            byte[] encode = baos.toByteArray();
            baos.flush();

            return new String(encode, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                baos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return null;
    }

    /**
     * Zip解压数据
     * @param zipStr
     * @return
     */
    public static String decompress(String zipStr) {

        if (TextUtils.isEmpty(zipStr)) {
            return null;
        }
        try {
            byte[] t = zipStr.getBytes("UTF-8");
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            ByteArrayInputStream in = new ByteArrayInputStream(t);
            ZipInputStream zip = new ZipInputStream(in);
            zip.getNextEntry();
            byte[] buffer = new byte[1024];
            int n = 0;
            while ((n = zip.read(buffer, 0, buffer.length)) > 0) {
                out.write(buffer, 0, n);
            }
            zip.close();
            in.close();
            out.close();
            return out.toString("UTF-8");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
