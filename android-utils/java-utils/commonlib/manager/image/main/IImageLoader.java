package com.auric.intell.commonlib.manager.image.main;

import android.view.View;
import android.widget.ImageView;

import java.io.File;

/**
 * Created by white on 16/7/25.
 */
public interface IImageLoader {

    void loadImage(View view, String url);

    void loadImage(View view, String url, IImageLoaderCallback callback);

    void loadImage(View view, String url, int defaultResId);

    void loadImage(View view, String url, int defaultResId, IImageLoaderCallback callback);

    void loadImage(View view, int resId, int defaultResId, IImageLoaderCallback callback);

    void loadImage(String url, IImageLoaderCallback callback);

    void loadImage(int resId, IImageLoaderCallback callback);

    File loadImageSync(String url);

    void loadGif(ImageView imageView, String url, int defaultResId);

    void loadCircleImage(ImageView view,String url);

    void loadRoundImage(ImageView view,int round,int defaultImageRes,String url,final IImageLoaderCallback callback);

}
