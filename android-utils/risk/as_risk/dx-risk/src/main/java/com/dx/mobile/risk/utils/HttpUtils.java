package com.dx.mobile.risk.utils;

import android.Manifest;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.Proxy;
import java.net.URL;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.concurrent.Executors;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.X509TrustManager;
import javax.security.auth.x500.X500Principal;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class HttpUtils {

    public static final String TAG = "DXRisk";

    public static final int HTTP_POST = 0;
    public static final int HTTP_GET = 1;

    public static class DXTrustManager implements X509TrustManager {

        public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
        }

        public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {

            boolean isDX = false;

            for (X509Certificate certificate : chain) {
                X500Principal subjectX500Principal = certificate
                        .getSubjectX500Principal();
                String name = subjectX500Principal.getName();

                if (!StringUtils.isEmpty(name) && name.contains("*.dingxiang-inc.com")) {
                    isDX = true;
                    break;
                }
            }

            if (!isDX)
                throw new CertificateException();
        }

        public X509Certificate[] getAcceptedIssuers() {
            return new X509Certificate[]{};
        }
    }

    public static class NormalTrustManager implements X509TrustManager {

        public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
        }

        public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
        }

        public X509Certificate[] getAcceptedIssuers() {
            return new X509Certificate[]{};
        }
    }

    public static void asyncNetRequest(final int type, final String url, final byte[] data,
                                       final boolean bSaas, final String contentType) {

        if (url.startsWith("https://")){
            Executors.newSingleThreadExecutor().execute(new Runnable() {

                @Override
                public void run() {
                    httpsRequest(type, url, data, bSaas, contentType);
                }
            });
        } else {
            Executors.newSingleThreadExecutor().execute(new Runnable() {

                @Override
                public void run() {
                    httpRequest(type, url, data, bSaas, contentType);
                }
            });
        }
    }

    /**
     * bDX 是否是顶象的后台
     */
    public static byte[] netRequest(int type, String url, byte[] data, boolean bSaas, String contentType) {

        if (url.startsWith("https://"))
            return httpsRequest(type, url, data, bSaas, contentType);
        else
            return httpRequest(type, url, data, bSaas, contentType);
    }

    public static byte[] httpRequest(int type, String url, byte[] data, boolean bSaas, String contentType) {

        Log.d(TAG, "httpRequest url:" + url);
        if (!PermissionUtils.checkPermissions(Manifest.permission.INTERNET)) {
            return null;
        }

        HttpURLConnection conn = null;
        OutputStream out = null;
        InputStream in = null;

        byte[] result = null;

        try {
            URL readl_url = new URL(url);

            conn = (HttpURLConnection) readl_url.openConnection(Proxy.NO_PROXY);
            conn.setConnectTimeout(15 * 1000);
            conn.setReadTimeout(15 * 1000);

            conn.setRequestProperty("accept", "*/*");
            conn.setRequestProperty("connection", "keep-Alive");
            conn.setRequestProperty("User-Agent", "STEE-SDK");
            if ("json".equals(contentType))
                conn.setRequestProperty("Content-type", "application/json");
            else
                conn.setRequestProperty("Content-type", "application/octet-stream");

            if (type == HTTP_GET) {
                conn.setUseCaches(false);
                conn.setRequestMethod("GET");
                conn.connect();
            } else {
                conn.setRequestMethod("POST");
                conn.setDoOutput(true);
                conn.setDoInput(true);
                conn.setUseCaches(false);

                out = new DataOutputStream(conn.getOutputStream());
                out.write(data);
                out.flush();
            }

            if (conn.getResponseCode() == HttpURLConnection.HTTP_OK) {

                in = new BufferedInputStream(conn.getInputStream());
                //OutputStream oos = (outfile != null && outfile.length() > 0) ? new FileOutputStream(outfile) : new ByteArrayOutputStream();
                OutputStream oos = new ByteArrayOutputStream();

                byte buf[] = new byte[256];
                int len;

                while ((len = in.read(buf)) > 0) {
                    oos.write(buf, 0, len);
                }

                if (oos instanceof ByteArrayOutputStream) {
                    result = ((ByteArrayOutputStream) oos).toByteArray();
                } else {
                    result = null;
                }
            }
            else{
                Log.e(TAG, "fail on fetching " + url + " res code:" + conn.getResponseCode());
            }

        } catch (Throwable e) {

            // 后台可能不支持m2请求
            if (!url.contains("/udid/m2")) {
                Log.e(TAG, "fail on fetching " + url, e);
            }
        } finally {
            try {

                if (in != null) {
                    in.close();
                }

                if (out != null) {
                    out.close();
                }

                if (conn != null) {
                    conn.disconnect();
                }
            } catch (Exception e) {
//                e.printStackTrace();
            }
        }

//        Log.d(TAG, "httpRequest result:" + new String(result));
        return result;
    }

    public static byte[] httpsRequest(int type, String url, byte[] data, boolean bSaas, String contentType) {

//        Log.d(TAG, "httpsRequest url:" + url + " bSaas:" + bSaas);

        if (!PermissionUtils.checkPermissions(Manifest.permission.INTERNET)) {
            return null;
        }

        HttpsURLConnection conn = null;
        OutputStream out = null;
        InputStream in = null;

        byte[] result = null;

        try {
            URL readl_url = new URL(url);
            SSLContext sc = SSLContext.getInstance("TLS");

            if (bSaas)
                sc.init(null, new X509TrustManager[]{new DXTrustManager()}, new java.security.SecureRandom());
            else
                sc.init(null, new X509TrustManager[]{new NormalTrustManager()}, new java.security.SecureRandom());

            // 不使用代理
            conn = (HttpsURLConnection) readl_url.openConnection(Proxy.NO_PROXY);
            conn.setSSLSocketFactory(new TLSOnlySocketFactory(sc.getSocketFactory()));
//            conn.setSSLSocketFactory(sc.getSocketFactory());
            conn.setConnectTimeout(15 * 1000);
            conn.setReadTimeout(15 * 1000);

            conn.setRequestProperty("accept", "*/*");
            conn.setRequestProperty("connection", "keep-Alive");
            conn.setRequestProperty("User-Agent", "STEE-SDK");
            if ("json".equals(contentType))
                conn.setRequestProperty("Content-type", "application/json");
            else
                conn.setRequestProperty("Content-type", "application/octet-stream");

            if (type == HTTP_GET) {
                conn.setUseCaches(false);
                conn.setRequestMethod("GET");
                conn.connect();
            } else {
                conn.setRequestMethod("POST");
                conn.setDoOutput(true);
                conn.setDoInput(true);
                conn.setUseCaches(false);

                out = new DataOutputStream(conn.getOutputStream());
                out.write(data);
                out.flush();
            }

            if (conn.getResponseCode() == HttpURLConnection.HTTP_OK) {

                in = new BufferedInputStream(conn.getInputStream());
                //OutputStream oos = (outfile != null && outfile.length() > 0) ? new FileOutputStream(outfile) : new ByteArrayOutputStream();
                OutputStream oos = new ByteArrayOutputStream();

                byte buf[] = new byte[256];
                int len;

                while ((len = in.read(buf)) > 0) {
                    oos.write(buf, 0, len);
                }

                if (oos instanceof ByteArrayOutputStream) {
                    result = ((ByteArrayOutputStream) oos).toByteArray();
                } else {
                    result = null;
                }
            }
            else{
                Log.e(TAG, "fail on fetching " + url + " res code:" + conn.getResponseCode());
            }

        } catch (Exception e) {
            // 后台可能不支持m2请求
            if (!url.contains("/udid/m2")) {
                Log.e(TAG, "fail on fetching " + url, e);
            }
        } finally {
            try {
                if (in != null) {
                    in.close();
                }

                if (out != null) {
                    out.close();
                }

                if (conn != null) {
                    conn.disconnect();
                }
            } catch (Exception e) {
//                e.printStackTrace();
            }
        }

        return result;
    }
}
