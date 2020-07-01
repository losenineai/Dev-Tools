package com.auric.intell.commonlib.manager.retrofit;

import com.auric.intell.commonlib.utils.ContextFinder;

import java.io.IOException;

import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;

/**
 * Created by zhangxiliang on 2018/1/16.
 */

public class OffLineCacheInterceptor implements Interceptor {
    private static final int TIMEOUT_DISCONNECT = 60 * 60 * 24 * 7; //7天

    @Override
    public Response intercept(Chain chain) throws IOException {
        Request request = chain.request();
        //离线的时候为7天的缓存。
        if (!NetworkUtil.isNetworkAvailable(ContextFinder.getApplication())) {
            request = request.newBuilder()
                    .header("Cache-Control", "public, only-if-cached, max-stale="+TIMEOUT_DISCONNECT)
                    .build();
        }
        return chain.proceed(request);
    }
}
