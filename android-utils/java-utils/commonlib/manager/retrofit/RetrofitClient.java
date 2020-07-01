package com.auric.intell.commonlib.manager.retrofit;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import com.auric.intell.commonlib.manager.retrofit.rxconvert.CustomRxJavaCallAdapterFactory;
import com.auric.intell.commonlib.manager.retrofit.rxconvert.DefaultIntercept;
import com.auric.intell.commonlib.uikit.SimpleCallBack;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.facebook.stetho.okhttp3.StethoInterceptor;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import javax.net.SocketFactory;

import okhttp3.Cache;
import okhttp3.Call;
import okhttp3.ConnectionPool;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import rx.Observable;
import rx.Subscriber;


/**
 * RetrofitClient
 * Created by Tamic on 2016-06-15.
 * {@link # https://github.com/NeglectedByBoss/RetrofitClient}
 */
public class  RetrofitClient {

    private static final int DEFAULT_TIMEOUT = 1000 * 20;
    private static OkHttpClient okHttpClient;
    private static Context mContext;
    private static final String TAG = "RetrofitClient";


    private Retrofit retrofit;
    private Cache cache = null;
    private File httpCacheDirectory;





    public static RetrofitClient getInstance(Context context, String url) {
        if (context != null) {
            mContext = context;
        }

        return new RetrofitClient(context, url);
    }


    public static RetrofitClient getInstance(Context context, String url, Interceptor interceptor) {
        if (context != null) {
            mContext = context;
        }
        return new RetrofitClient(context, url, interceptor,false);
    }


    public static RetrofitClient getInstance(Context context, String url, Interceptor interceptor,boolean needCache) {
        if (context != null) {
            mContext = context;
        }
        return new RetrofitClient(context, url, interceptor,needCache);
    }



    private RetrofitClient(Context context, String url) {

        this(context, url, null,false);
    }



    private RetrofitClient(Context context, String url, Interceptor interceptor,boolean needCache) {

        mContext=context;
        if (httpCacheDirectory == null) {
            httpCacheDirectory = new File(mContext.getCacheDir(), "tamic_cache");
        }

        try {
            if (cache == null) {
                cache = new Cache(httpCacheDirectory, 50 * 1024 * 1024);
            }
        } catch (Exception e) {
            Log.e("OKHttp", "Could not create http cache", e);
        }


        HttpLoggingInterceptor loggingInterceptor = new HttpLoggingInterceptor(new HttpLoggingInterceptor.Logger() {
            @Override
            public void log(String message) {
                Log.e(TAG, message);
            }
        });

        loggingInterceptor.setLevel(HttpLoggingInterceptor.Level.BODY);

        OkHttpClient.Builder builder = new OkHttpClient.Builder();
        builder.socketFactory(SocketFactory.getDefault());
        builder.addInterceptor(loggingInterceptor);
        builder.addNetworkInterceptor(new StethoInterceptor());
        builder.addInterceptor(interceptor==null ?new DefaultIntercept():interceptor);
        if(needCache){
            builder.addInterceptor(new OffLineCacheInterceptor());
            builder.addNetworkInterceptor(new CaheInterceptor());
        }
        builder.cache(cache);
        builder.connectTimeout(DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
        builder.writeTimeout(DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
        builder.connectionPool(new ConnectionPool(8, 15, TimeUnit.SECONDS));
        // 这里你可以根据自己的机型设置同时连接的个数和时间，我这里8个，和每个保持时间为10s
        okHttpClient = builder.build();
        retrofit = new Retrofit.Builder()
                .client(okHttpClient)
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(CustomRxJavaCallAdapterFactory.create())
                .baseUrl(url)
                .build();

    }



    public OkHttpClient getOkHttpClient(){
        return okHttpClient;
    }






    public Call request(Method method, String url,RequestBody body, HashMap<String,String> headsMap){
        Request.Builder builder  =new Request.Builder().url(url).method(method.name(),body);
        if(headsMap!=null) {
            for (Map.Entry<String, String> entry : headsMap.entrySet()) {
                builder.addHeader(entry.getKey(), entry.getValue());
            }
        }
        Request request=builder.build();
        return okHttpClient.newCall(request);
    }


    public Call request(Method method,String url, RequestBody body){
        return request(method,url, body,null);
    }



    /**
     * create you ApiService
     * Create an implementation of the API endpoints defined by the {@code service} interface.
     */
    public <T> T create(final Class<T> service) {
        if (service == null) {
            throw new RuntimeException("Api service is null!");
        }

        return  retrofit.create(service);
    }







    public static void request(Observable observable, Subscriber subscriber){
        observable.compose(RxSchedulers.io_main()).subscribe(subscriber);
    }






    public static class Builder{
        private  String baseUrl;
        private  boolean needCache;
        private  boolean needAuth;
        private Class tClass;
        private  Interceptor interceptor;

        public Builder baseUrl(String baseUrl){
            this.baseUrl=baseUrl;
            return this;
        }

        public Builder needAuth(boolean need){
            this.needAuth=need;
            return this;
        }

        public Builder openCache(boolean open){
            this.needCache=open;
            return this;
        }

        public Builder addIntercepet(Interceptor interceptor){
          this.interceptor=interceptor;
            return this;
        }

        public static <T> T checkNotNull(T reference, String errorMessage) {
            if (reference == null) {
                throw new NullPointerException(errorMessage);
            }
            return reference;
        }

        public  <T> T build(Class<T> service){
            checkNotNull(baseUrl,"baseUrl is null!!");
            RetrofitClient retrofitClient= new RetrofitClient(ContextFinder.getApplication(),baseUrl,interceptor,needCache);
            return retrofitClient.create(service);
        }

    }



}
