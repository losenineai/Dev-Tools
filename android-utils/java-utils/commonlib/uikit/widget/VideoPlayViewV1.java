package com.auric.intell.commonlib.uikit.widget;


import android.content.Context;
import android.content.res.TypedArray;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.AttributeSet;
import android.view.View;
import android.widget.MediaController;
import android.widget.VideoView;

import com.auric.intell.commonlib.R;
import com.auric.intell.commonlib.utils.LogTool;


/**
 * @author : jinlongfeng
 * @version : V2.0.8
 * @description: 封装视频播放控件
 * @time : 2016/10/29-17:31
 */

public class VideoPlayViewV1 extends VideoView {

    private static final String TAG = "VideoPlayViewV1";
    private MediaController mMediaController;
    private VideoPlayCallBack mVideoPlayCallBack;

    private int videoview_width, videoview_height;
    private boolean videoview_controlBarVisual = false;

    private volatile boolean isPaused;
    private boolean mIsFinish;


    public VideoPlayViewV1(Context context, AttributeSet attrs) {
        super(context, attrs);
//        mMediaController = this.
//        mMediaController.setVisibility(View.GONE);
//        this.setMediaController(mMediaController);
        //read custom attrs
        TypedArray t = context.obtainStyledAttributes(attrs,
                R.styleable.VideoPlayView);
        videoview_width = t.getDimensionPixelSize(R.styleable.VideoPlayView_VideoPlayViewwidth, 1280);

        videoview_height = t.getDimensionPixelSize(R.styleable.VideoPlayView_VideoPlayViewheight, 720);

        videoview_controlBarVisual = false;
        t.recycle();
    }


    public VideoPlayViewV1(Context context) {
        this(context, null);
    }

    public void setVideoPlayCallBack(VideoPlayCallBack videoPlayCallBack) {
        mVideoPlayCallBack = videoPlayCallBack;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);

        int widthDefault = getDefaultSize(0, widthMeasureSpec);
        int heightDefault = getDefaultSize(0, heightMeasureSpec);
        int width = getMySize(widthDefault, widthMeasureSpec);
        int height = getMySize(heightDefault, heightMeasureSpec);
        setMeasuredDimension(width, height);

    }


    /**
     * 初始化
     *
     * @param uri
     */
    public void init(Uri uri) {
        LogTool.d(TAG, "init uri:"+uri.toString());
        setVideoURI(uri);
//        setMediaController(mMediaController);
        setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                LogTool.d(TAG, "onCompletion");
                if (mVideoPlayCallBack != null) {
                    mVideoPlayCallBack.onPlayFinished();
                }
                isPaused = false;
                mIsFinish = true;
            }
        });
        setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mp, int what, int extra) {
                LogTool.d(TAG, "onError "+what +" "+extra);
                if (mVideoPlayCallBack != null) {
                    mVideoPlayCallBack.onPlayError();
                }
                isPaused = false;
                return true;
            }
        });

        setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                LogTool.d(TAG, "onPrepared ");
                if (mVideoPlayCallBack != null) {
                    mVideoPlayCallBack.onPlayPrepared();
                }
                mIsFinish = false;
            }
        });


//        if(videoview_controlBarVisual){
//            mMediaController.setVisibility(VISIBLE);
//        }else {
//            mMediaController.setVisibility(GONE);
//        }
    }

    public void startPlay() {
        start();
        mVideoPlayCallBack.onPlayBegin();
    }

    public void pausePlay() {
        isPaused = true;
        pause();
    }

    public void resumePlay() {
        if (isPaused) {
            start();
        }
        isPaused = false;
    }

    public boolean isPaused() {
        return isPaused;

    }

    public int getCurPosition() {
        return super.getCurrentPosition();
    }

    public int getDuration() {
        return super.getDuration();
    }

    public boolean isFinish() {
        return mIsFinish;
    }

    private int getMySize(int defaultSize, int measureSpec) {
        int mySize = defaultSize;
        int mode = MeasureSpec.getMode(measureSpec);
        int size = MeasureSpec.getSize(measureSpec);
        switch (mode) {
            case MeasureSpec.UNSPECIFIED: {//如果没有指定大小，就设置为默认大小
                mySize = defaultSize;
                break;
            }
            case MeasureSpec.AT_MOST: {//如果测量模式是最大取值为size
                //我们将大小取最大值,你也可以取其他值
                mySize = size;
                break;
            }
            case MeasureSpec.EXACTLY: {//如果是固定的大小，那就不要去改变它
                mySize = size;
                break;
            }
        }
        return mySize;
    }

    public void setControlBarVisual(int isVisual) {
        videoview_controlBarVisual = isVisual == View.VISIBLE;
//        if (videoview_controlBarVisual) {
//            mMediaController.setVisibility(View.VISIBLE);
//        }
//        else{
//            mMediaController.setVisibility(View.GONE);
//        }
    }

    public interface VideoPlayCallBack {

        void onPlayFinished();

        void onPlayBegin();

        void onPlayError();

        void onPlayPrepared();

    }

}