package com.auric.intell.commonlib.manager.image.glide;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.view.View;
import android.widget.ImageView;

import com.auric.intell.commonlib.manager.image.main.IImageLoader;
import com.auric.intell.commonlib.manager.image.main.IImageLoaderCallback;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogTool;
import com.auric.intell.commonlib.utils.LogUtils;
import com.bumptech.glide.BitmapRequestBuilder;
import com.bumptech.glide.GifRequestBuilder;
import com.bumptech.glide.Glide;
import com.bumptech.glide.RequestManager;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.bumptech.glide.request.FutureTarget;
import com.bumptech.glide.request.RequestListener;
import com.bumptech.glide.request.animation.GlideAnimation;
import com.bumptech.glide.request.target.Target;
import com.bumptech.glide.request.target.ViewTarget;

import java.io.File;

/**
 * Created by white on 16/7/25.
 */
public class GlideLoader implements IImageLoader {

    private static final String TAG = "GlideLoader";

    public GlideLoader() {

    }

    @Override
    public void loadImage(final View view, String url) {
        loadImage(view, url, 0);
    }

    @Override
    public void loadImage(final View view, String url, final IImageLoaderCallback callback) {
        loadImage(view, url, 0, callback);
    }

    @Override
    public void loadImage(final View view, String url, final int defaultResId) {
        loadImage(view, url, defaultResId, null);
    }

    @Override
    public void loadImage(String url, final IImageLoaderCallback callback) {
        RequestManager requestManager = Glide.with(ContextFinder.getApplication());
        requestManager.load(url).asBitmap().listener(new RequestListener<String, Bitmap>() {
            @Override
            public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                callback.onFinish(null);
                return false;
            }

            @Override
            public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                callback.onFinish(bitmap);
                return true;
            }
        }).into(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }

    public void loadRoundImage(String url,int width,int height, int dp, final IImageLoaderCallback callback) {
        RequestManager requestManager = Glide.with(ContextFinder.getApplication());
        requestManager.load(url).diskCacheStrategy(DiskCacheStrategy.RESULT);
        requestManager.load(url).override(width, height).fitCenter();
        requestManager.load(url).asBitmap().transform(new GlideRoundTransform(ContextFinder.getApplication(),dp)).listener(new RequestListener<String, Bitmap>() {
            @Override
            public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                if(callback!=null) {
                    callback.onFinish(null);
                }
                return false;
            }

            @Override
            public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                if(callback!=null) {
                    callback.onFinish(bitmap);
                }
                return true;
            }
        }).into(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }


    @Override
    public File loadImageSync(String url) {
        FutureTarget<File> future =
                Glide.with(ContextFinder.getApplication()).load(url).downloadOnly(Integer.MAX_VALUE, Integer.MAX_VALUE);
        try {
            File cacheFile = future.get();
            return cacheFile;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public void loadImage(View view, String url, int defaultResId, final IImageLoaderCallback callback) {

        DiskCacheStrategy diskCacheStrategy = DiskCacheStrategy.ALL;

        RequestManager requestManager = getRequestManager(view);
        if (view instanceof ImageView) {
            final ImageView imageView = (ImageView) view;
            BitmapRequestBuilder builder = requestManager.load(url).asBitmap().animate(android.R.anim.fade_in).fitCenter();
            if (defaultResId > 0) {
                builder.error(defaultResId).placeholder(defaultResId);
            }
            if (callback != null) {
                builder.listener(new RequestListener<String, Bitmap>() {
                    @Override
                    public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                        callback.onFinish(null);
                        LogTool.d(TAG, "onException " + e + " " + s + " " + b);
                        return false;
                    }

                    @Override
                    public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                        callback.onFinish(bitmap);
                        LogTool.d(TAG, "onFinish");
                        return false;
                    }
                });
                builder.diskCacheStrategy(diskCacheStrategy).into(new ViewTarget<View, Bitmap>(imageView) {
                    @Override
                    public void onResourceReady(Bitmap resource, GlideAnimation anim) {
                    }
                });
            } else {
                builder.diskCacheStrategy(diskCacheStrategy).into(imageView);
            }
        } else {
            BitmapRequestBuilder builder = requestManager.load(url).asBitmap().animate(android.R.anim.fade_in);
            if (defaultResId > 0) {
                builder.error(defaultResId).placeholder(defaultResId);
            }
            if (callback != null) {
                builder.listener(new RequestListener<String, Bitmap>() {
                    @Override
                    public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                        callback.onFinish(null);
                        return false;
                    }

                    @Override
                    public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                        LogTool.d(TAG, "onFinish");
                        callback.onFinish(bitmap);
                        return false;
                    }
                });
            }
            builder.diskCacheStrategy(diskCacheStrategy).into(new ViewTarget<View, Bitmap>(view) {
                @Override
                public void onResourceReady(Bitmap resource, GlideAnimation anim) {
                    if (callback == null)
                        view.setBackgroundDrawable(new BitmapDrawable(resource));
                }
            });
        }
    }


    @Override
    public void loadImage(View view, int resId, int defaultResId, final IImageLoaderCallback callback) {

        DiskCacheStrategy diskCacheStrategy = DiskCacheStrategy.ALL;

        RequestManager requestManager = getRequestManager(view);
        if (view instanceof ImageView) {
            final ImageView imageView = (ImageView) view;
            BitmapRequestBuilder builder = requestManager.load(resId).asBitmap().animate(android.R.anim.fade_in).fitCenter();
            if (defaultResId > 0) {
                builder.error(defaultResId).placeholder(defaultResId);
            }
            if (callback != null) {
                builder.listener(new RequestListener<Integer, Bitmap>() {
                    @Override
                    public boolean onException(Exception e, Integer s, Target<Bitmap> target, boolean b) {
                        callback.onFinish(null);
                        LogTool.d(TAG, "onException " + e + " " + s + " " + b);
                        return false;
                    }

                    @Override
                    public boolean onResourceReady(Bitmap bitmap, Integer s, Target<Bitmap> target, boolean b, boolean b1) {
                        callback.onFinish(bitmap);
                        LogTool.d(TAG, "onFinish");
                        return false;
                    }
                });
                builder.diskCacheStrategy(diskCacheStrategy).into(new ViewTarget<View, Bitmap>(imageView) {
                    @Override
                    public void onResourceReady(Bitmap resource, GlideAnimation anim) {
                    }
                });
            } else {
                builder.diskCacheStrategy(diskCacheStrategy).into(imageView);
            }
        } else {
            BitmapRequestBuilder builder = requestManager.load(resId).asBitmap().animate(android.R.anim.fade_in);
            if (defaultResId > 0) {
                builder.error(defaultResId).placeholder(defaultResId);
            }
            if (callback != null) {
                builder.listener(new RequestListener<String, Bitmap>() {
                    @Override
                    public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                        callback.onFinish(null);
                        return false;
                    }

                    @Override
                    public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                        LogTool.d(TAG, "onFinish");
                        callback.onFinish(bitmap);
                        return false;
                    }
                });
            }
            builder.diskCacheStrategy(diskCacheStrategy).into(new ViewTarget<View, Bitmap>(view) {
                @Override
                public void onResourceReady(Bitmap resource, GlideAnimation anim) {
                    if (callback == null)
                        view.setBackgroundDrawable(new BitmapDrawable(resource));
                }
            });
        }
    }

    @Override
    public void loadImage(int resId, final IImageLoaderCallback callback) {
        RequestManager requestManager = Glide.with(ContextFinder.getApplication());
        requestManager.load(resId).asBitmap().listener(new RequestListener<Integer, Bitmap>() {
            @Override
            public boolean onException(Exception e, Integer s, Target<Bitmap> target, boolean b) {
                callback.onFinish(null);
                return false;
            }

            @Override
            public boolean onResourceReady(Bitmap bitmap, Integer s, Target<Bitmap> target, boolean b, boolean b1) {
                callback.onFinish(bitmap);
                return true;
            }
        }).into(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }

    @Override
    public void loadGif(ImageView imageView, String url, int defaultResId) {
        // 注意：加载gif必须使用SOURCE，不然会卡顿
        DiskCacheStrategy diskCacheStrategy = DiskCacheStrategy.SOURCE;

        RequestManager requestManager = getRequestManager(imageView);
        GifRequestBuilder builder = requestManager.load(url).asGif().animate(android.R.anim.fade_in).centerCrop();
        if (defaultResId > 0) {
            builder.error(defaultResId).placeholder(defaultResId);
        }
        builder.diskCacheStrategy(diskCacheStrategy).into(imageView);

    }

    @Override
    public void loadCircleImage(ImageView view, String url) {
        Glide.with(view.getContext())
                .load(url)
                .crossFade()
                .transform(new GlideCircleTransform(view.getContext()))
                .into(view);
       /* DiskCacheStrategy diskCacheStrategy = DiskCacheStrategy.ALL;

        RequestManager requestManager = getRequestManager(view);
        BitmapRequestBuilder builder = requestManager.load(url).asBitmap();
        builder.transform(new GlideCircleTransform(ContextFinder.getApplication())).animate(android.R.anim.fade_in).centerCrop().diskCacheStrategy(diskCacheStrategy).into(view);*/
    }

    @Override
    public void loadRoundImage(ImageView view, int round,int defaultImageRes, String url,final IImageLoaderCallback callback ) {
        RequestManager requestManager = Glide.with(ContextFinder.getApplication());
        requestManager.load(url).diskCacheStrategy(DiskCacheStrategy.RESULT);
        requestManager.load(url).asBitmap().transform(new GlideRoundTransform(ContextFinder.getApplication(),round)).error(defaultImageRes).placeholder(defaultImageRes).listener(new RequestListener<String, Bitmap>() {
            @Override
            public boolean onException(Exception e, String s, Target<Bitmap> target, boolean b) {
                if(callback!=null){
                    callback.onFinish(null);
                }
                return false;
            }

            @Override
            public boolean onResourceReady(Bitmap bitmap, String s, Target<Bitmap> target, boolean b, boolean b1) {
                if(callback!=null){
                    callback.onFinish(bitmap);
                }
                return false;
            }
        }).into(view);
    }

    private RequestManager getRequestManager(View view) {
        if(view != null){
            Glide.with(view.getContext());
        }
        return Glide.with(ContextFinder.getApplication());
    }


}
