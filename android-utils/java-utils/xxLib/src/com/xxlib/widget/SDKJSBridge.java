package com.xxlib.widget;

import android.webkit.JavascriptInterface;
import android.webkit.JsPromptResult;
import android.webkit.WebView;
import org.json.JSONException;

import java.util.HashMap;

/**
 * Created by mushroom0417 on 8/15/15.
 */
public class SDKJSBridge {

    public final static String TAG = "SDK";
    public final static String OnJsPromptFlag="MyWebViewBridge://";
    private SDKJSBridgeObserver mSDKJSBridgeObserver;
    private HashMap<String, HandlerDo> mHandlerListeners=new HashMap<String, HandlerDo>();

    public SDKJSBridge(SDKJSBridgeObserver observer) {
        mSDKJSBridgeObserver = observer;
    }

    @JavascriptInterface
    public void callHandler(final String cmd, final String param, final String callback) {
        if (null != mSDKJSBridgeObserver) {
            mSDKJSBridgeObserver.onJSCall(cmd, param, callback);
        }
    }

    public void registerHandler(String handlerName, HandlerDo listener) {
        if(!handlerName.isEmpty() && listener!=null)
            mHandlerListeners.put(handlerName, listener);
    }

    public interface SDKJSBridgeObserver {
        void onJSCall(final String cmd, final String param, final String callback);
    }

    public interface HandlerDo {
        String handle(String data) throws JSONException;
    }

    // 外部一定要调用这个事件
    public boolean onJsPrompt(WebView view, String url, String message, String defaultValue, JsPromptResult result) {
        if (message.startsWith(OnJsPromptFlag)) {
            String handlerName = message.replace(OnJsPromptFlag, "");
            HandlerDo listener = mHandlerListeners.get(handlerName);

            if (listener != null) {
                try {
                    result.confirm(listener.handle(defaultValue));
                } catch (JSONException e) {
                    result.confirm(""); // webview没有confirm会奔溃
                    e.printStackTrace();
                }
            } else {
                result.confirm("");
            }
            return true;
        } else {
            return false;
        }
    }

}
