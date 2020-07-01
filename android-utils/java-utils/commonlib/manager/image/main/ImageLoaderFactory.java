package com.auric.intell.commonlib.manager.image.main;

import com.auric.intell.commonlib.manager.image.glide.GlideLoader;

/**
 * Created by white on 16/7/25.
 */
public class ImageLoaderFactory {

    private static ImageLoaderFactory sInstance;
    private static GlideLoader mGlideLoader;

    public static synchronized IImageLoader getInstance() {
        if (sInstance == null) {
            sInstance = new ImageLoaderFactory();
        }
        return sInstance.getImageLoader();
    }


    private synchronized IImageLoader getImageLoader() {
        if (mGlideLoader == null) {
            mGlideLoader = new GlideLoader();
        }
        return mGlideLoader;
    }
}
