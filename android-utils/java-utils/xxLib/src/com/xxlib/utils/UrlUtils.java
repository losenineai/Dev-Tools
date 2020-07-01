package com.xxlib.utils;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.text.TextUtils;

import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.XXTea;

import java.net.URLEncoder;

public class UrlUtils {
	
	private static final String TAG = "UrlUtils";
    public static final String KEY = "#%$*)&*M<><vance";

	/**
	 * 浏览器打开url
	 * @description  
	 * @param context
	 * @param url
	 */
	public static boolean openUrl(Context context, String url){
		LogTool.i(TAG, "" + url);
		if(TextUtils.isEmpty(url)){
			return false;
		}
		
		try {
			Intent intent = new Intent();
			intent.setAction("android.intent.action.VIEW");
			Uri CONTENT_URI_BROWSERS = Uri.parse(url);
			intent.setData(CONTENT_URI_BROWSERS);
			intent.setClassName("com.android.browser", "com.android.browser.BrowserActivity");
			context.startActivity(intent);
		} catch (Exception er) {
			try {
				Intent it = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
				context.startActivity(it);
			} catch (Exception err) {
                return false;
			}
		}

        return true;
	}

    // 更好的实现方法，是解析?&到一个map中移除，再组装
    public static String removeParamFromUrl(String url, String key) {
        if (TextUtils.isEmpty(url) || TextUtils.isEmpty(key)) {
            return url;
        }
        String key1 = "?"+key+"=";
        String key2 = "&"+key+"=";
        int keyStartPos1 = url.indexOf(key1);
        int keyStartPos2 = url.indexOf(key2);
        LogTool.i(TAG, "keyStartPos1 " + keyStartPos1 + ", keyStartPos2 " + keyStartPos2);
        if (keyStartPos1 < 0 && keyStartPos2 < 0) {
            return url;
        }
        String newUrl = "";
        if (keyStartPos1 >=0 ) {
            int nextAndToken = url.substring(keyStartPos1+1).indexOf("&");
            if (nextAndToken >= 0) {
                // http://guopan.cn?key=b&c=d
                newUrl = url.substring(0, keyStartPos1+1) + url.substring(nextAndToken+1);
            } else {
                // http://guopan.cn?key=b
                newUrl = url.substring(0, keyStartPos1);
            }
        } else if (keyStartPos2 >= 0) {
            int nextAndToken = url.substring(keyStartPos2+1).indexOf("&");
            if (nextAndToken >=0 ) {
                // http://guopan.cn?a=b&key=d&e=d
                newUrl = url.substring(0, keyStartPos2+1) + url.substring(nextAndToken+1);
            } else {
                // http://guopan.cn?a=b&key=d
                newUrl = url.substring(0, keyStartPos2);
            }
        }

        LogTool.i(TAG, "newUrl " + newUrl);
        return newUrl;
    }

    public static String filterByLoginState(String url, String uin, String loginkey, String imei, int productId) {
        return filterByLoginState(url, uin, loginkey, imei, productId, "");
    }

    public static String filterByLoginState(String url, String uin, String loginkey, String imei, int productId, String deviceId){
        if(url == null){
            return null;
        }
        LogTool.i(TAG, "filterByLoginState " + url);

        if(url.contains("guopan.cn")){
            url = addLoginStateParams(url, uin, loginkey, imei, productId, deviceId);
        }
        else if(url.contains("xxzhushou.cn")){
            url = addLoginStateParams(url, uin, loginkey, imei, productId, deviceId);
        }

        return url;
    }

    public static String addLoginStateParams(String baseUrl, String uin, String loginkey, String imei, int productId) {
        return addLoginStateParams(baseUrl, uin, loginkey, imei, productId, "");
    }

    public static String addLoginStateParams(String baseUrl, String uin, String loginkey, String imei, int productId, String deviceid){
        if(baseUrl == null){
            return null;
        }

        if(baseUrl.contains("params=")){
            return baseUrl;
        }

        if(baseUrl.contains("?")){
            baseUrl = baseUrl + "&params=" + getLoginStateByEncrypt(uin, loginkey, imei, productId, deviceid);
        }
        else{
            baseUrl = baseUrl + "?params=" + getLoginStateByEncrypt(uin, loginkey, imei, productId, deviceid);
        }

        LogTool.i(TAG, "addLoginStateParams " + baseUrl);

        return baseUrl;
    }

    public static String getLoginStateByEncrypt(String uin, String loginkey, String imei, int productId) {
        return getLoginStateByEncrypt(uin, loginkey, imei, productId, "");
    }

    public static String getLoginStateByEncrypt(String uin, String loginkey, String imei, int productId, String deviceId) {
        // 默认要url encode，不encode的场景比较少，现在传递js的param，encode会有换行
        return getLoginStateByEncrypt(uin, loginkey, imei, productId, deviceId, true);
    }

    public static String getLoginStateByEncrypt(String uin, String loginkey, String imei, int productId, String deviceId, boolean isUrlEncode){
        String content = "";
        if (TextUtils.isEmpty(deviceId)) {
            // device id如果是空的话，那么久不用追加到参数列表中，保持原来的不变 zemin
            content = "product_id=" + productId
                    + "&uin=" + uin
                    + "&uuid=" + imei
                    + "&login_key=" + loginkey;
        } else {
            content = "product_id=" + productId
                    + "&uin=" + uin
                    + "&uuid=" + imei
                    + "&login_key=" + loginkey
                    + "&device_id=" + deviceId;
        }

        byte[] data = content.getBytes();

        // XXTea加密
        byte[] xxDst = XXTea.XXTeaEncrypt(data, data.length, KEY.getBytes());
        if (xxDst == null) {
            return null;
        }
        // Base64加密
        byte[] b64Dst = android.util.Base64.encode(xxDst, android.util.Base64.DEFAULT);

        // url encode
        if (isUrlEncode) {
            String encodeStr = URLEncoder.encode(new String(b64Dst));
            LogTool.i(TAG, "getLoginStateByEncrypt " + encodeStr);
            return new String(encodeStr);
        } else {
            String strWithoutUrlEncode = new String(b64Dst);
            LogTool.i(TAG, "getLoginStateByEncrypt strWithoutUrlEncode " + strWithoutUrlEncode);
            return strWithoutUrlEncode;
        }
    }
}
