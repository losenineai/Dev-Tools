package com.auric.intell.commonlib.utils.net;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by white on 16/1/4.
 */
public class HttpPostUtil {

    private static final String TAG = "HttpPostUtil";
    private static final int TIME_OUT_CONNECTION = 60 * 1000;
    private static final int TIME_OUT_READ = 60 * 1000;


    public static final byte[] post(String urlStr, byte[] postData, HttpPostListener listener){
        HttpURLConnection conn = null;
        try {
            URL url = new URL(urlStr);
            conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("POST");
            conn.setConnectTimeout(TIME_OUT_CONNECTION);
            conn.setReadTimeout(TIME_OUT_READ);
            conn.setDoInput(true);
            conn.setDoOutput(true);

            OutputStream os = conn.getOutputStream();
            os.write(postData);
            os.flush();
            os.close();

            int code = conn.getResponseCode();
            if (code != 200) {
                return null;
            }

            InputStream is = conn.getInputStream();
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            // 协议长度
            int total = conn.getContentLength();
            int cur = 0;
            byte[] buffer = new byte[520];
            int len = 0;
            // 创建一个文件对象 ,记录当前某个文件的下载位置
            while ((len = is.read(buffer)) != -1) {
                out.write(buffer, 0, len);
                cur += len;
                if(listener != null)
                    listener.onProgress(total, cur);
            }
            is.close();
            out.flush();
            out.close();

            return out.toByteArray();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                conn.disconnect();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }


    public interface HttpPostListener{
        void onProgress(int total, int cur, Object... objs);
    }



}
