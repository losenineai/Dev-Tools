package com.auric.intell.commonlib.manager.retrofit;
import android.util.ArrayMap;
import android.util.Log;

import com.auric.intell.commonlib.robot.Robot;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.DeviceUtils;
import com.auric.intell.commonlib.utils.JSONUtils;
import com.auric.intell.commonlib.utils.LogUtils;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.ResponseBody;
import retrofit2.http.HTTP;

/**
 * BaseInterceptor
 * Created by LIUYONGKUI726 on 2016-06-30.
 * {@link # https://github.com/NeglectedByBoss/RetrofitClient}
 */
public abstract class BaseInterceptor implements Interceptor{

    public BaseInterceptor() {

    }

    @Override
    public Response intercept(Chain chain) throws IOException {

        Request.Builder builder = chain.request()
                .newBuilder();


        Map<String,String> defaultHeaders=getHeaders();

        if (defaultHeaders != null && defaultHeaders.size() > 0) {
            Set<String> keys = defaultHeaders.keySet();
            for (String headerKey : keys) {
                builder.addHeader(headerKey, defaultHeaders.get(headerKey)).build();
            }
        }

        Response originReponse = chain.proceed(builder.build());

        return originReponse;



    }

    public abstract Map<String,String> getHeaders();





}