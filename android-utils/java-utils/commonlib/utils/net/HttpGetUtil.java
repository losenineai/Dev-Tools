package com.auric.intell.commonlib.utils.net;


import android.util.Log;

import com.auric.intell.commonlib.utils.LogTool;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by white on 15/11/9.
 */
public class HttpGetUtil {

    private static final String TAG = "HttpGetUtil";


    public static String get(String urlString) {
        return get(urlString, 5000); // 默认5s超时
    }

    /**
     * 请求开关配置
     * @param urlString 请求的地址
     * @return 请求获得的数据
     */
    public static String get(String urlString, int timeout) {
        HttpURLConnection conn;
        BufferedInputStream bin;
        String result;

        try {
            URL url = new URL(urlString);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            conn.setConnectTimeout(timeout);
            int code = conn.getResponseCode();
            if (code != 200) {
                return null;
            }

            InputStream in = conn.getInputStream();
            bin = new BufferedInputStream(in);
            ByteArrayOutputStream bAos = new ByteArrayOutputStream();
            BufferedOutputStream buf = new BufferedOutputStream(bAos);
            // 数据缓冲区
            byte[] buffer = new byte[2048];
            int size;
            while ((size = bin.read(buffer)) != -1)
                buf.write(buffer, 0, size);

            buf.flush();
            buf.close();
            bin.close();
            in.close();
            bAos.flush();
            bAos.close();
            result = bAos.toString();

            LogTool.i(TAG, "switch result = " + result);
            return result;
        } catch (Exception e) {
            LogTool.w(TAG, Log.getStackTraceString(e));
            return null;
        }
    }


    public static boolean downloadFile(String urlString, String destPath) {
        HttpURLConnection conn = null;
        try {
            URL url = new URL(urlString);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            conn.setConnectTimeout(10000);

            InputStream is = conn.getInputStream();
            FileOutputStream fos = new FileOutputStream(destPath, false);
            byte[] buffer = new byte[1024];
            int len = 0;
            // 创建一个文件对象 ,记录当前某个文件的下载位置
            while ((len = is.read(buffer)) != -1) {
                fos.write(buffer, 0, len);
            }
            is.close();
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        } finally {
            try {
                conn.disconnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return true;
    }


    public static boolean downloadFile(String urlString, String destPath, HttpPostUtil.HttpPostListener listener) {
        HttpURLConnection conn = null;
        try {
            URL url = new URL(urlString);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            conn.setConnectTimeout(10000);

            InputStream is = conn.getInputStream();
            FileOutputStream fos = new FileOutputStream(destPath, false);
            byte[] buffer = new byte[1024];
            int len = 0;
            int cur = 0;
            // 协议长度
            int total = conn.getContentLength();
            String range = conn.getHeaderField("Content-Range");
            LogTool.d(TAG, "total:"+total);
            LogTool.d(TAG, "range:"+range);
            // 创建一个文件对象 ,记录当前某个文件的下载位置
            while ((len = is.read(buffer)) != -1) {
                fos.write(buffer, 0, len);
                cur += len;
                if (listener != null)
                    listener.onProgress(total, cur);
            }
            is.close();
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        } finally {
            try {
                conn.disconnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return true;
    }
}
