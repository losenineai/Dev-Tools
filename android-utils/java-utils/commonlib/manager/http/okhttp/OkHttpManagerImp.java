package com.auric.intell.commonlib.manager.http.okhttp;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import com.auric.intell.commonlib.manager.http.HttpHeader;
import com.auric.intell.commonlib.manager.http.HttpParams;
import com.auric.intell.commonlib.manager.http.HttpResult;
import com.auric.intell.commonlib.manager.http.IHttpListener;
import com.auric.intell.commonlib.manager.http.IHttpManager;
import com.auric.intell.commonlib.utils.CharsetUtil;
import com.auric.intell.commonlib.utils.LogTool;
import okhttp3.FormBody;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

/**
 * @description HTTP 请求实现类
 * @author white
 * @date 16/7/19.
 * @version V0.0.1
 * ================================================
 * change list : 实现 PUT,GET,PATCH,DELETE,GET 方法.
 * name : jinlong
 * date : 2016-7-22 00:35
 */
public class OkHttpManagerImp extends IHttpManager {

    private static final String TAG = "OkHttpManagerImp";
    private OkHttpClient mClient;

    public OkHttpManagerImp() {
        super();
        mClient = new OkHttpClient.Builder()
        .connectTimeout(TIME_OUT_CONNECT_MS, TimeUnit.MILLISECONDS)
        .readTimeout(TIME_OUT_READ_MS, TimeUnit.MILLISECONDS)
        .writeTimeout(TIME_OUT_WRITE_MS, TimeUnit.MILLISECONDS)
        .build();
    }

    /**
     * 分发 Http 请求
     * @param HTTP_TYPE
     * @param params
     * @param listener
     * @return
     */
    private HttpResult dispatchRequest(int HTTP_TYPE, HttpParams params, IHttpListener listener) {

        Response response = null;
        // 数据保护
        if (params.reqContentType == null) {
            params.reqContentType = IHttpManager.DATA_TYPE_JSON;
        }
        if (params.headerMap != null) {
            for (Map.Entry<String, String> entry : params.headerMap.entrySet()) {
                LogTool.d(TAG, "dispatchRequest key:" + entry.getKey());
                LogTool.d(TAG, "dispatchRequest value:" + entry.getValue());
                // 防止null抛异常
                if (entry.getValue() == null) {
                    params.headerMap.put(entry.getKey(), "");
                }
                else{
                    // 防止有特殊特殊字符如换行,导致okhttp抛异常
                    params.headerMap.put(entry.getKey(), entry.getValue().trim());
                }
            }
        }

        if(params.reqContentType.equals(IHttpManager.DATA_TYPE_FORM)){
            response = requestForm(HTTP_TYPE,params.url,params.headerMap,params.fromData);
        }

        if(params.reqContentType.equals(IHttpManager.DATA_TYPE_JSON)){
            response = request(HTTP_TYPE, params.url, params.headerMap, params.reqContentType, params.reqContentByte);
        }

        HttpResult result = new HttpResult();
        try {
            if (response != null) {
                result.resultCode = HttpResult.RESULT_CODE_SUCCESS;
                String string = new String(response.body().bytes(), "utf-8");
                result.dataObj = CharsetUtil.toUtf8ByJson(string);
                result.responseCode = response.code();
                if(response.headers() != null){
                    HttpHeader header = new HttpHeader();
                    for(int i = 0; i < response.headers().size(); i++){
                        header.put(response.headers().name(i), response.headers().value(i));
                    }
                    result.header = header;
                }

            } else {
                result.resultCode = HttpResult.RESULT_CODE_ERR_NOT_200;
            }
        } catch (IOException e) {
            e.printStackTrace();
            result.resultCode = HttpResult.RESULT_CODE_ERR_IO;
        }

        if (listener != null) {
            if (result.resultCode == HttpResult.RESULT_CODE_SUCCESS) {
                listener.onSuccess(result);
            } else {
                listener.onFailed(result);
            }
            return null;
        }else{

            return result;
        }
    }


    /**
     * 进行同步 Http 请求
     * @param httpType
     * @param url
     * @param headerMap
     * @param contentType
     * @param contentBytes
     * @return
     */
    private Response request(int httpType, String url, Map<String, String> headerMap, String contentType, byte[] contentBytes) {

        RequestBody body = null;
        if (contentBytes != null && contentType != null) {
            String string = new String(contentBytes);
            body = RequestBody.create(MediaType.parse(contentType), string);
        }

        // http request
        Request.Builder request = new Request.Builder()
                .url(url);

        // http header
        if (headerMap != null) {
            for (Map.Entry<String, String> entry : headerMap.entrySet()) {
                request.addHeader(entry.getKey(), entry.getValue());
            }
        }

        // http request type
        switch (httpType) {
            case HTTP_TYPE_GET:
                request.get();
                break;
            case HTTP_TYPE_POST:
                request.post(body);
                break;
            case HTTP_TYPE_DELETE:
                request.delete(body);
                break;
            case HTTP_TYPE_HEAD:
                request.head();
                break;
            case HTTP_TYPE_PATCH:
                request.patch(body);
                break;
            case HTTP_TYPE_PUT:
                request.put(body);
                break;
        }


        // http response
        try {
            Response response = mClient.newCall(request.build()).execute();
            return response;

        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * 进行同步 Http 表单数据请求
     * @param httpType
     * @param url
     * @param headerMap
     * @param formData
     * @return
     */
    private Response requestForm(int httpType, String url, Map<String, String> headerMap, Map<String, String> formData) {

        // http request
        Request.Builder request = new Request.Builder()
                .url(url);

        // http header
        if (headerMap != null) {
            for (Map.Entry<String, String> entry : headerMap.entrySet()) {
                request.addHeader(entry.getKey(), entry.getValue());
            }
        }

        // form data
        FormBody.Builder formBodyBuilder = new FormBody.Builder();
        if (formData != null) {
            for (Map.Entry<String,String> entry : formData.entrySet()){
                String key = entry.getKey() == null ? "" : entry.getKey();
                String value = entry.getValue() == null ? "" : entry.getValue();

                formBodyBuilder.add(key, value);
            }
        }
        FormBody formBody = formBodyBuilder.build();

        // http type
        switch (httpType) {
            case HTTP_TYPE_GET:
                request.get();
                break;
            case HTTP_TYPE_POST:
                request.post(formBody);
                break;
            case HTTP_TYPE_DELETE:
                request.delete(formBody);
                break;
            case HTTP_TYPE_HEAD:
                request.head();
                break;
            case HTTP_TYPE_PATCH:
                request.patch(formBody);
                break;
            case HTTP_TYPE_PUT:
                request.put(formBody);
                break;
        }

        // http response
        try {
            Response response = mClient.newCall(request.build()).execute();
            return response;

        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
    @Override
    public String getForStr(String url) {

        final HttpParams httpParams = new HttpParams();
        httpParams.url = url;
        HttpResult result =  dispatchRequest(HTTP_TYPE_GET,httpParams,null);
        return result.dataObj.toString();
    }

    @Override
    public String postForStr(String url) {
        final HttpParams httpParams = new HttpParams();
        httpParams.url = url;
        HttpResult result =  dispatchRequest(HTTP_TYPE_POST,httpParams,null);
        return result.dataObj.toString();
    }

    @Override
    public void getAsync(String url, final IHttpListener listener) {
        final HttpParams httpParams = new HttpParams();
        httpParams.url = url;
        httpParams.reqContentType = IHttpManager.DATA_TYPE_FORM;
        httpParams.headerMap = new HashMap<>();
        httpParams.fromData = new HashMap<>();
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_GET, httpParams, listener);
            }
        });
    }

    @Override
    public void getAsync(final HttpParams params, final IHttpListener listener) {
        if (params.reqContentType == null) {
            params.reqContentType = IHttpManager.DATA_TYPE_JSON;
        }
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_GET, params, listener);
            }
        });
    }

    @Override
    public void postAsync(final HttpParams params, final IHttpListener listener) {
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_POST, params, listener);
            }
        });
    }


    @Override
    public void patchAsync(final HttpParams params, final IHttpListener listener) {

        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_PATCH, params, listener);
            }
        });
    }

    @Override
    public void deleteAsync(final HttpParams params, final IHttpListener listener) {
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_DELETE, params, listener);
            }
        });
    }

    @Override
    public void putAsync(final HttpParams params, final IHttpListener listener) {
        mThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                dispatchRequest(HTTP_TYPE_PUT, params, listener);
            }
        });
    }
}
