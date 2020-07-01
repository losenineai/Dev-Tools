package com.auric.intell.commonlib.manager.media;

import android.content.Context;

import com.danikula.videocache.CacheListener;
import com.danikula.videocache.HttpProxyCacheServer;

import java.io.File;

/**
 * Created by zhangxiliang on 2017/1/3.
 */
public class MediaCacheManager {

    private static HttpProxyCacheServer mHttpProxyCacheServer;

    public  static void init(Context context, String cacheDir, long maxSize, int maxCacheCount){

        if(mHttpProxyCacheServer==null){
            HttpProxyCacheServer.Builder builder=new HttpProxyCacheServer.Builder(context);
            builder.cacheDirectory(new File(cacheDir));
            builder.maxCacheSize(maxSize);
            builder.maxCacheFilesCount(maxCacheCount);
            mHttpProxyCacheServer=builder.build();
        }

    }


    public static String getProxyUrl(String url){
        return mHttpProxyCacheServer.getProxyUrl(url);
    }

    public static void registerCacheListener(CacheListener cacheListener,String url){
        mHttpProxyCacheServer.registerCacheListener(cacheListener,url);
    }
    public static void unregisterCacheListener(CacheListener cacheListener){
        mHttpProxyCacheServer.unregisterCacheListener(cacheListener);
    }

    public static boolean isCached(String url){
        return mHttpProxyCacheServer.isCached(url);
    }


}
