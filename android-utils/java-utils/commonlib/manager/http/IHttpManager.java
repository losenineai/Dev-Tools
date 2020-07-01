package com.auric.intell.commonlib.manager.http;

import android.annotation.TargetApi;
import android.os.Build;
import android.text.TextUtils;

import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.auric.intell.commonlib.manager.http.okhttp.OkHttpManagerImp;

/**
 * Http请求入口
 * Created by white on 16/7/19.
 */
public abstract class IHttpManager {

    /**
     * Http请求类型
     */
    public static final int HTTP_TYPE_GET = 101;
    public static final int HTTP_TYPE_POST = 102;
    public static final int HTTP_TYPE_DELETE = 103;
    public static final int HTTP_TYPE_PATCH = 104;
    public static final int HTTP_TYPE_HEAD = 105;
    public static final int HTTP_TYPE_PUT = 106;
    public static final int HTTP_GET_OK = 200;
    public static final int HTTP_POST_OK = 202;
    public static final int BAD_REQUEST = 400;
    public static final int NO_CONTENT = 204;

    private static final String TAG = "IHttpManager";
    public static String DATA_TYPE_JSON = "application/json; charset=utf-8";
    public static String DATA_TYPE_FORM = "application/x-www-form-urlencode";
    private static IHttpManager sInstance;
    protected ExecutorService mThreadPool;
    protected IHttpManager() {
        mThreadPool = Executors.newFixedThreadPool(3);
    }

    // 连接超时时间
    protected static final int TIME_OUT_CONNECT_MS = 10 * 1000;
    // 读超时时间
    protected static final int TIME_OUT_READ_MS = 10 * 1000;
    // 写超时时间
    protected static final int TIME_OUT_WRITE_MS = 30 * 1000;


    public static IHttpManager getIntance() {
        if (sInstance == null) {
            synchronized (IHttpManager.class) {
                if (sInstance == null) {
                    sInstance = new OkHttpManagerImp();
                }
            }
        }
        return sInstance;
    }

    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public void doPostFormParam(final String url, final Map<String, String> formParam, final IHttpListener listener) {
        HttpHeader headerMap = HttpHeader.getRbtCommonHeader();
        HttpParams params = new HttpParams();
        params.url = url;
        params.reqContentType = DATA_TYPE_FORM;
        params.headerMap = headerMap;
        params.fromData = formParam;
        postAsync(params, listener);

    }

    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public void doPutFormParam(final String url, final String token, final Map<String, String> formParam, final IHttpListener listener) {

        HttpHeader headerMap = HttpHeader.getRbtCommonHeader();

        if (!TextUtils.isEmpty(token)) {
            headerMap.putToken(token);
        }

        HttpParams params = new HttpParams();
        params.url = url;
        params.reqContentType = DATA_TYPE_FORM;
        params.headerMap = headerMap;
        params.fromData = formParam;
        putAsync(params, listener);
    }


    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public void doPostFormParam(final String url, final Map<String,String> header, final Map<String, String> formParam, final IHttpListener listener) {

        HttpHeader headerMap = HttpHeader.getRbtCommonHeader();
        for (Map.Entry<String,String> entry : header.entrySet()){
            headerMap.put(entry.getKey(),entry.getValue());
        }
        HttpParams params = new HttpParams();
        params.url = url;
        params.reqContentType = DATA_TYPE_FORM;
        params.headerMap = headerMap;
        params.fromData = formParam;
        postAsync(params, listener);
    }


    @TargetApi(Build.VERSION_CODES.GINGERBREAD)
    public void doDelFormParam(final String url, final String token, final Map<String, String> formParam, final IHttpListener listener) {

        HttpHeader headerMap = HttpHeader.getRbtCommonHeader();
        if (!TextUtils.isEmpty(token)) {
            headerMap.putToken(token);
        }

        HttpParams params = new HttpParams();
        params.url = url;
        params.reqContentType = DATA_TYPE_FORM;
        params.headerMap = headerMap;
        params.fromData = formParam;

        deleteAsync(params, listener);
    }


//    public void test(final String url, final String json, final IHttpListener listener) {
//
//        Map<String, String> headerMap = new HashMap<>();
//        headerMap.put("Accept", "application/vnd.robot.v1+json");
//        headerMap.put("Authorization", "Bearer " + TokenUtil.getToken());
//        HttpParams params = new HttpParams();
//        params.url = url;
//        params.reqContentType = DATA_TYPE_JSON;
//        params.reqContentByte = json.getBytes(Charset.defaultCharset());
//        params.headerMap = headerMap;
//        postAsync(params, listener);

//        mThreadPool.execute(new Runnable() {
//            @Override
//            public void run() {
//
//
//
//
//                byte[] resBytes = ;
//
//                HttpResult result = new HttpResult();
//                try {
//                    if(resBytes != null){
//                        result.resultCode = HttpResult.RESULT_CODE_SUCCESS;
//                        result.dataObj = new String(resBytes, "utf-8");
////                        String test = CharsetUtil.unicodeToUtf8(new String(resBytes, "utf-8"));
////                        LogTool.d(TAG, "test unicodeToUtf8:" + test);
//                    }
//                    else{
//                        result.resultCode = HttpResult.RESULT_CODE_ERR_NOT_200;
//
//                    }
//                } catch (Exception e) {
//                    e.printStackTrace();
//                    result.resultCode = HttpResult.RESULT_CODE_ERR_IO;
//                }
//
//                if(listener != null){
//                    if(result.resultCode == HttpResult.RESULT_CODE_SUCCESS){
//                        listener.onSuccess(result);
//                    }
//                    else{
//                        listener.onFailed(result);
//                    }
//                }
//            }
//        });

//    }


    /**
     * ************************ 抽象接口 ************************
     */
    abstract public String getForStr(String url);

    abstract public String postForStr(String url);

    abstract public void getAsync(String url, final IHttpListener listener);

    abstract public void getAsync(HttpParams params, final IHttpListener listener);

    abstract public void postAsync(HttpParams params, final IHttpListener listener);

    abstract public void patchAsync(HttpParams params, final IHttpListener listener);

    abstract public void deleteAsync(HttpParams params, final IHttpListener listener);

    abstract public void putAsync(HttpParams params, final IHttpListener listener);


}
