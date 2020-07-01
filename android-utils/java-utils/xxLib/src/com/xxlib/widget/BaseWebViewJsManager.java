package com.xxlib.widget;

import android.app.Activity;
import android.text.TextUtils;
import android.webkit.WebView;

import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.CopyToClipboard;
import com.xxlib.utils.base.LogTool;

import org.json.JSONObject;

/**
 * Created by jammy on 16/3/8.
 */
public class BaseWebViewJsManager {

    private static final String TAG = "BaseWebViewJsManager";

    /* 所有的cmd定义 开始 */
    protected static final String CMD_QUERY = "query";
    protected static final String CMD_JUMP_TO_ZONE = "jumpToZone";
    protected static final String CMD_JUMP_TO_GIFT_LIST = "jumpToGiftList";
    protected static final String CMD_JUMP_TO_LOGIN = "jumpToLogin";
    protected static final String CMD_JUMP_TO_BIND_PHONE = "jumpToBindPhone";
    protected static final String CMD_JUMP_TO_VIP_WELFARE = "jumpToVipWelfare";

    protected static final String CMD_SHOW_TOAST = "showToast";
    protected static final String CMD_GET_AUTHENTICATION = "getAuthentication";
    @Deprecated
    protected static final String CMD_COPY_TO_CLIPBOARD = "copyToClipboard";
    protected static final String CMD_SET_WEBVIEW_TITLE = "setWebViewTitle";
    protected static final String CMD_OPEN_WEBVIEW = "openWebView";
    protected static final String CMD_SHOW_CONFIRM = "showConfirm";
    protected static final String CMD_SAVE_IMAGE_TO_LOCAL = "saveImageToLocal";

    protected static final String CMD_AUTHORIZE = "authorize";
    protected static final String CMD_DOWNLOAD_PKG = "download_pkg";

    protected static final String CMD_OPEN_CHARGE = "openCharge";
    protected static final String CMD_DO_TASK = "buqianDoTask";
    protected static final String CMD_EXCHANGE_BUQIAN_CARD = "exchangeBuqianCard";

    protected static final String CMD_JUMP_TO_PAGE = "jumpToPage";
    /* 所有的cmd定义 结束 */

    // json的key
    protected static final String JSON_KEY_CMDS = "cmds";
    protected static final String JSON_KEY_RESULT = "result";
    protected static final String JSON_KEY_PARAMS = "params";
    protected static final String JSON_KEY_PAGE_NAME = "page_name";
    protected static final String JSON_KEY_PAGE_PARAMS = "page_params";


    /* js回调支持的功能，需要继承之后返回支持的cmd */
    protected String[] getSupportCmds() {
        return new String[]{
                CMD_QUERY,
                CMD_COPY_TO_CLIPBOARD
        };
    }

    /**
     * 有专区按钮和礼包按钮的h5页面
     * 测试链接: http://test.m.guopan.cn/dede/gpmobile/app_article.php?id=23361
     */
    public void forJsCallback(final WebView webview) {
        LogTool.i(TAG, "forJsCallback init");
        webview.addJavascriptInterface(new SDKJSBridge(new SDKJSBridge.SDKJSBridgeObserver() {
            @Override
            public void onJSCall(final String cmd, final String param, final String callback) {
                LogTool.i(TAG, "cmd==>" + cmd + ",param==>" + param + ",callback==>" + callback);
                webview.post(new Runnable() {
                    @Override
                    public void run() {
                        if (CMD_QUERY.equals(cmd)) {
                            // 查询接口
                            jsCallback_handleJsQuery(param, callback, webview);
                        } else if (CMD_JUMP_TO_ZONE.equals(cmd)) {
                            // 去游戏专区
                            jsCallback_jumpToZoneByWebview(param, callback);
                        } else if (CMD_JUMP_TO_GIFT_LIST.equals(cmd)) {
                            // 去礼包列表
                            jsCallback_jumpToGiftListByWebview(param, callback);
                        } else if (CMD_JUMP_TO_LOGIN.equals(cmd)) {
                            // 去登陆
                            jsCallback_jumpToLoginByWebview(param, callback, webview);
                        } else if (CMD_JUMP_TO_BIND_PHONE.equals(cmd)) {
                            // 去绑定手机
                            jsCallback_jumpToBindPhoneByWebview(param, callback, webview);
                        } else if (CMD_JUMP_TO_VIP_WELFARE.equals(cmd)) {
                            // 去VIP尊享页面
                            jsCallback_jumpToVIPWelfare(param, callback, webview);
                        } else if (CMD_SHOW_TOAST.equals(cmd)) {
                            // 弹出toast
                            jsCallback_showToastFromWebview(param);
                        } else if (CMD_COPY_TO_CLIPBOARD.equals(cmd)) {
                            // 拷贝字符串到剪切板
                            jsCallback_copyToClipboard(webview, callback, param);
                        } else if (CMD_GET_AUTHENTICATION.equals(cmd)) {
                            // 验证
                            jsCallback_getAuthentication(webview, callback);
                        } else if (CMD_SET_WEBVIEW_TITLE.equals(cmd)) {
                            // 设置页面title
                            jsCallback_setViewTitleFromWebview(param);
                        } else if (CMD_OPEN_WEBVIEW.equals(cmd)) {
                            // 新页面来浏览
                            jsCallback_openNewWebview(param);
                        } else if (CMD_SHOW_CONFIRM.equals(cmd)) {
                            // 打开对话框
                            jsCallback_showConfirm(param, callback, webview);
                        } else if (CMD_SAVE_IMAGE_TO_LOCAL.equals(cmd)) {
                            // 保存图片
                            jsCallback_saveImageToLocal(param);
                        } else if (CMD_AUTHORIZE.equals(cmd)) {
                            // 获取授权信息
                            jsCallback_authorize(webview, callback, param);
                        } else if (CMD_DOWNLOAD_PKG.equals(cmd)) {
                            // 下载游戏，通过包名
                            jsCallback_downloadGame(param, callback, webview);
                        } else if (CMD_OPEN_CHARGE.equals(cmd)) {
                            jsCallback_openCharge(param, callback, webview);
                        }
                        else if(CMD_DO_TASK.equals(cmd)) {
                            jsCallback_doTask(param, callback, webview);
                        }
                        else if(CMD_EXCHANGE_BUQIAN_CARD.equals(cmd)) {
                            jsCallback_exchangeBuqianCard(param, callback, webview);
                        } else if (CMD_JUMP_TO_PAGE.equals(cmd)) {
                            jsCallback_jumpToPage(param, callback, webview);
                        }
                    }


                });
            }


        }), SDKJSBridge.TAG);
    }

    protected void jsCallback_handleJsQuery(String param, String callback, WebView webview) {
        String[] supportCmds = getSupportCmds();
        if (null == supportCmds || supportCmds.length == 0) {
            return;
        }
        String jsSupportStr = "";
        for (String cmd : supportCmds) {
            jsSupportStr = jsSupportStr + "," + cmd;
        }
        jsSupportStr = jsSupportStr.substring(1);
        JSONObject json = new JSONObject();
        try {
            json.put(JSON_KEY_CMDS, jsSupportStr);
        } catch (Exception e) {
            e.printStackTrace();
        }
        LogTool.i(TAG, "query params " + json.toString());
        callJSCallback(webview, callback, json.toString());
    }

    protected void callJSCallback(WebView webView, String callback, String param) {
        if (param == null) {
            param = "";
        }
        if (!TextUtils.isEmpty(callback)) {
            String jsString = String.format("javascript:void((function(){%s(%s)})())", callback, param);
            LogTool.i(TAG, "jsString:" + jsString);
            runJSInvokeCallback(jsString, webView);
        }
    }

    protected void callJSCallback_forSDK(Activity activity, String callback, String content, boolean isString, WebView webview) {
        String jsString = "";
        if (TextUtils.isEmpty(content)) {
            jsString = String.format("javascript:void((function(){%s(200)})())", callback);
        } else if (isString)
            jsString = String.format("javascript:void((function(){%s(200,'%s')})())", callback, content);
        else
            jsString = String.format("javascript:void((function(){%s(200,%s)})())", callback, content);
        LogTool.i(TAG, "jsString:" + jsString);
        if (!TextUtils.isEmpty(callback)) {
            runJSInvokeCallback(jsString, webview);
        }
    }

    protected void runJSInvokeCallback(final String jsString, final WebView webview) {
        if (!TextUtils.isEmpty(jsString) && null != webview) {
            if (!TextUtils.isEmpty(jsString) && webview != null) {
                webview.loadUrl(jsString);
            }
        }
    }

    protected void jsCallback_copyToClipboard(WebView webview, String callback, String param) {
        CopyToClipboard.copy(ApplicationUtils.getApplication(), param, "");
        callJSCallback(webview, callback, param);
    }

    protected void jsCallback_jumpToZoneByWebview(String param, String callback) {

    }

    protected void jsCallback_jumpToGiftListByWebview(String param, String callback) {

    }

    protected void jsCallback_jumpToLoginByWebview(String param, final String callback, final WebView webView) {

    }

    protected void jsCallback_jumpToBindPhoneByWebview(String param, final String callback, final WebView webView) {

    }

    protected void jsCallback_jumpToVIPWelfare(String param, final String callback, final WebView webView) {

    }

    protected void jsCallback_showToastFromWebview(String param) {

    }

    protected void jsCallback_getAuthentication(WebView webview, String callback) {

    }

    protected void jsCallback_setViewTitleFromWebview(String param) {

    }

    protected void jsCallback_openNewWebview(String url) {

    }

    protected void jsCallback_showConfirm(String param, final String callback, final WebView webview) {

    }

    protected void jsCallback_saveImageToLocal(final String url) {

    }

    protected void jsCallback_authorize(WebView webview, String callback, String param) {

    }

    protected void jsCallback_downloadGame(final String param, final String callback, final WebView webview) {

    }

    protected void jsCallback_openCharge(String param, String callback, WebView webview) {
    }

    protected void jsCallback_doTask(String param, String callback, WebView webview) {
    }

    protected void jsCallback_exchangeBuqianCard(String param, String callback, WebView webview) {
    }

    protected void jsCallback_jumpToPage(String param, String callback, WebView webview) {
    }
}
