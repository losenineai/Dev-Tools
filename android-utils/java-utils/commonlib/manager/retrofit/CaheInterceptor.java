package com.auric.intell.commonlib.manager.retrofit;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import com.auric.intell.commonlib.utils.StringUtil;
import com.auric.intell.commonlib.utils.UIThread;

import java.io.IOException;

import okhttp3.CacheControl;
import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;

/**
 * caheInterceptor
 * Created by Tamic on 2016-08-09.
 */
public class CaheInterceptor implements Interceptor {



    public CaheInterceptor() {
    }

    @Override
    public Response intercept(Chain chain) throws IOException {
        Request request = chain.request();
        String cache=chain.request().header("cache");
        Response response = chain.proceed(request);

        if(!StringUtil.isEmpty(cache)){
            return response.newBuilder()
                    .removeHeader("Pragma")
                    .removeHeader("Cache-Control")
                    .header("Cache-Control", "public, max-age=" + cache)
                    .build();
        }
        return response;
      /*else {
            request = request.newBuilder()
                    .cacheControl(CacheControl.FORCE_CACHE)
                    .build();
            Response response = chain.proceed(request);
            //set cahe times is 3 days
            int maxStale = 60 * 60 * 24 * 3;
            return response.newBuilder()
                    .removeHeader("Pragma")
                    .removeHeader("Cache-Control")
                    .header("Cache-Control", "public, only-if-cached, max-stale=" + maxStale)
                    .build();
        }*/
    }
}
