package com.xxlib.widget;

import android.annotation.SuppressLint;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.View;
import android.webkit.DownloadListener;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebSettings.RenderPriority;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.xxlib.utils.StringUtils;
import com.xxlib.utils.ToastUtils;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.floatview.CheckAppRunning;

import java.io.File;

/**
 * 开启尽可能多的websettings
 */
public abstract class ExBaseWebView extends WebView {

    private static final String TAG = "ExBaseWebView";

    public static final String SCHEME_HTTP = "http";
    public static final String SCHEME_HTTPS = "https";
    public static final String WX_PAY_SCHEME = "weixin";
    public static final String WX_PKG_NAME = "com.tencent.mm";
    protected static final String WX_PAY_PREFIX = "http://api.ipaynow.cn/?notifyUrl=http";
    protected Context mContext;

    public ExBaseWebView(Context context) {
        super(context);
        this.mContext = context;
        initParams();
    }

    public ExBaseWebView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.mContext = context;
        initParams();
    }

    public ExBaseWebView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.mContext = context;
        initParams();
    }


    /**
     * 小于0表示可以回退。
     * @return
     */
    protected int exGoBackStep() {
        int step = 0;
        if (!canGoBack()) {
            LogTool.i(TAG, "canGoBack false");
            return step;
        }
        try {
            if (null == copyBackForwardList() || copyBackForwardList().getCurrentIndex() == 0) {
                step = 0;
            } else {
                int curIndex = copyBackForwardList().getCurrentIndex();
                String curUrl = copyBackForwardList().getCurrentItem().getUrl();
                LogTool.i(TAG, "index " + curIndex + ", curUrl " + curUrl);
                for (int i = curIndex - 1; i >= 0; i--) {
                    String tmpUrl = copyBackForwardList().getItemAtIndex(i).getUrl();
                    LogTool.i(TAG, "index " + i + ", tmpUrl " + tmpUrl);
                    if (!TextUtils.isEmpty(curUrl) && !TextUtils.isEmpty(tmpUrl) && !curUrl.equals(tmpUrl)
                            && !tmpUrl.startsWith(WX_PAY_PREFIX)) { // 微信支付的url，跳过。不然每次进入会导致进入微信支付页面，导致重复订单
                        step = i - curIndex;
                        break;
                    }
                }
            }
        } catch (Exception e) {
            LogTool.w(TAG, e);
        }
        LogTool.i(TAG, "step " + step);
        return step;
    }


    public void exGoBack() {
        int step = exGoBackStep();
        goBackOrForward(step);
    }


    public boolean exCanGoBack() {
        int step = exGoBackStep();
        return step < 0;
    }


    /**
     * 最好不要调用了，用getExtraAgentString来支持。咨询zemin
     */
    @Deprecated
    protected void setAgent() {
    }

    /**
     * 不用空格，外部继承这个方法，出入需要的参数即可
     * @return
     */
    protected String getExtraAgentString() {
        return "";
    }

    protected abstract int getProductId();

    protected abstract String getProductVersion();


    @SuppressLint("NewApi")
    protected void initParams() {
        if (isInEditMode()) {
            return;
        }
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {   // 3.0
                getSettings().setAllowContentAccess(true);
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {  // 4.1
                getSettings().setAllowFileAccessFromFileURLs(true);
                getSettings().setAllowUniversalAccessFromFileURLs(true);
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {  // 4.2
                getSettings().setMediaPlaybackRequiresUserGesture(false);
            }

            if (Build.VERSION.SDK_INT <= 18)
                getSettings().setSavePassword(false); // 4.3或者以下去掉自动保存密码功能,否则如果在非activity的环境下回导致crash

            getSettings().setDatabaseEnabled(true);
            getSettings().setLightTouchEnabled(true);
            getSettings().setSaveFormData(true);

            setAgent();         // 不同的产品，agent可能不一样，需要的话，重载setAgent的方法

            // 设置系统和app的一些信息
            // sysApi/19 sysVersion/4.4.4 productId/109 productVersion/1.1.0
            String sysVersion = (null == Build.VERSION.RELEASE) ? "" : Build.VERSION.RELEASE;
            String sysAndAppInfo = StringUtils.format("sysApi/%d sysVersion/%s productId/%d productVersion/%s",
                    Build.VERSION.SDK_INT, sysVersion, getProductId(), getProductVersion());
            getSettings().setUserAgentString(getSettings().getUserAgentString() + " " + sysAndAppInfo);

            // 额外的一些agent信息
            String extraAgent = getExtraAgentString();
            LogTool.i(TAG, "extraAgent is " + extraAgent);
            if (TextUtils.isEmpty(extraAgent) || TextUtils.isEmpty(extraAgent.trim())) {
                // do nothing
            } else {
                getSettings().setUserAgentString(getSettings().getUserAgentString() + " " + extraAgent);
            }
            LogTool.i(TAG, "getSettings().getUserAgentString() " + getSettings().getUserAgentString());

            //是否支持缩放 默认是打开的
            getSettings().setSupportZoom(true);
            //支持通过JS打开新窗口
            getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
            //是否支持javaScript
            getSettings().setJavaScriptEnabled(true);
            //webView 按任意比例缩放
            getSettings().setUseWideViewPort(true);
            // 缩放至屏幕的大小
            getSettings().setLoadWithOverviewMode(true);
            // 支持内容从新布局
            getSettings().setLayoutAlgorithm(LayoutAlgorithm.NORMAL);
            // 多窗口
            getSettings().supportMultipleWindows();
            // 开启mWebView中缓存策略
            getSettings().setCacheMode(WebSettings.LOAD_DEFAULT);
            // 设置可以访问文件
            getSettings().setAllowFileAccess(true);
            // 当mWebView调用requestFocus时为mWebView设置节点
            getSettings().setNeedInitialFocus(true);
            // 支持自动加载图片
            getSettings().setLoadsImagesAutomatically(true);
            // 支持缩放
            getSettings().setBuiltInZoomControls(false);
            // 设置缩放比例
            setInitialScale(10);
            // 设置滚动条隐藏
            setScrollBarStyle(View.SCROLLBARS_OUTSIDE_OVERLAY);
            // 启用地理定位
            getSettings().setGeolocationEnabled(true);
            // 设置渲染优先级
            getSettings().setRenderPriority(RenderPriority.NORMAL);
            // 设置定位的数据库路径
            String dir = "/sdcard/temp";
            getSettings().setGeolocationDatabasePath(dir);
            getSettings().setDatabasePath(dir);

            //webView 支持手动获取焦点（输入）
            requestFocusFromTouch();

            // 允许获得焦点
            setClickable(true);
            setFocusable(true);
            setFocusableInTouchMode(true);

            // 允许localStorage
            getSettings().setDomStorageEnabled(true);
            getSettings().setAppCacheMaxSize(1024 * 1024 * 5);
            String appCachePath = mContext.getCacheDir().getAbsolutePath();
            getSettings().setAppCachePath(appCachePath);
            getSettings().setAppCacheEnabled(true);

            // 打开插件，解决部分机型视频不能播放的问题
            getSettings().setPluginState(WebSettings.PluginState.ON);

            // 监听下载
            setDownloadListener(new DownloadListener() {
                @Override
                public void onDownloadStart(String url, String userAgent,
                                            String contentDisposition, String mimetype, long contentLength) {
                    try {
                        // 调用系统的下载
                        Uri uri = Uri.parse(url);
                        Intent intent = new Intent(Intent.ACTION_VIEW, uri);
                        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        getContext().startActivity(intent);
                    } catch (ActivityNotFoundException e) {
                        e.printStackTrace();
                    }
                }
            });

            // wxj, 用于视频播放。
            setWebViewClient(new WebViewClient());

            // 用于视频播放，文件上传
            setWebChromeClient(new ExWebChromeClient());

        } catch (Exception e) {
            LogTool.e(TAG, e.toString());
            e.printStackTrace();
        }
    }


    @Override
    public void destroy() {
        super.destroy();
        mUploadCallback = null;
        mUploadCallback21 = null;
        mUploadFileListener = null;
    }

    /**
     ******************************** 客户端文件上传 ********************************
     */
    private static ValueCallback<Uri> mUploadCallback;
    private static ValueCallback<Uri[]> mUploadCallback21;
    private static IUploadFileListener mUploadFileListener;

    public static class ExWebChromeClient extends WebChromeClient {
        // For Android 3.0+
        public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType) {
            LogTool.i(TAG, "openFileChooser--3.0+  uploadFile=" + uploadMsg.toString());
            showFileChooser(uploadMsg, null, acceptType);
        }

        // For Android < 3.0
        public void openFileChooser(ValueCallback<Uri> uploadMsg) {
            LogTool.i(TAG, "openFileChooser-- <3.0  uploadFile=" + uploadMsg.toString());
            showFileChooser(uploadMsg, null, "");
        }

        // For Android  > 4.1.1, 系统api不能重写
        public void openFileChooser(ValueCallback<Uri> uploadFile, String acceptType, String capture) {
            LogTool.i(TAG, "openFileChooser--4.1.1  uploadFile=" + uploadFile.toString());
            showFileChooser(uploadFile, null,  acceptType);

        }

        // For Android >= 5.0(21)
        @Override
        public boolean onShowFileChooser(WebView webView, ValueCallback<Uri[]> filePathCallback,
                                         FileChooserParams fileChooserParams) {
            LogTool.i(TAG, "onShowFileChooser-- >=5.0  uploadFile=" + filePathCallback.toString());
            showFileChooser(null, filePathCallback, "");
            return true;
        }

        /**
         * 显示文件选择器
         * @param filePathCallback
         * @param filePathCallback21
         * @param acceptType
         */
        private void showFileChooser(ValueCallback<Uri> filePathCallback, ValueCallback<Uri[]> filePathCallback21, String acceptType){
            LogTool.i(TAG, "showFileChooser");
            if(mUploadFileListener != null){
                mUploadCallback = filePathCallback;
                mUploadCallback21 = filePathCallback21;
                mUploadFileListener.onOpenFileChooser(acceptType);
            }
        }

    }


    /**
     * 外部主动调用，选择文件结果回调，只有通过webview回调选择图片的时候才能上传，不然无效
     *
     * @param filePath
     */
    public void onResultForOpenFileChooser(boolean hasResult, String filePath) {
        if (hasResult) {
            LogTool.i(TAG, "file path: " + filePath);

            File file = new File(filePath);
            if(mUploadCallback != null){
                mUploadCallback.onReceiveValue(Uri.fromFile(file));
                mUploadCallback = null;
            }
            if(mUploadCallback21 != null){
                mUploadCallback21.onReceiveValue(new Uri[]{Uri.fromFile(file)});
                mUploadCallback21 = null;
            }
            ToastUtils.show(mContext, "开始上传...", !CheckAppRunning.isXxRunningOnForeground(mContext, mContext.getPackageName()));
            LogTool.i(TAG, "file length: " + file.length());
        } else {
            // 返回null表示取消上传操作
            if(mUploadCallback != null){
                mUploadCallback.onReceiveValue(null);
                mUploadCallback = null;
            }
            if(mUploadCallback21 != null){
                mUploadCallback21.onReceiveValue(null);
                mUploadCallback21 = null;
            }
        }
    }

    public void setEnableUploadFile(boolean enable, IUploadFileListener listener){
        if(enable){
            mUploadFileListener = listener;
        }
    }

    /**
     * 打开客户端的文件选择器回调接口
     */
    public interface IUploadFileListener {
        void onOpenFileChooser(String acceptType);
    }


    /**
     * 通过判断url的scheme要不要交给系统处理。一般非http和https的，都可以发送intent出去给系统处理
     * @return
     */
    public boolean handleShouldOverrideUrlLoading(String url) {
        try {
            Uri uri = Uri.parse(url);
            String scheme = uri.getScheme();
            LogTool.i(TAG, "scheme " + scheme);
            if (!TextUtils.isEmpty(scheme) && !scheme.equals(SCHEME_HTTPS) && !scheme.equals(SCHEME_HTTP)) {
                // 不是http和https的，那么直接用start activity给其他app处理
                // 例如微信支付 weixin://.... ; 跳转市场 market://......
                return openActivityFromWebview_ShouldOverrideUrlLoading(url);
            } else {
                return false;
            }
        } catch (Exception e) {
            LogTool.i(TAG, e.toString());
        }
        return false;
    }


    protected boolean openActivityFromWebview_ShouldOverrideUrlLoading(String url) {
        Intent intent = new Intent();
        try {
            Uri uri = android.net.Uri.parse(new java.net.URI(url).toString());
            intent.setData(uri);
            getContext().startActivity(intent);
        } catch (Exception e) {
            LogTool.w(TAG, e);
            return false;
        }
        return true;
    }
}
