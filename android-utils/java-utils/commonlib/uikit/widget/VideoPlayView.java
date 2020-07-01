package com.auric.intell.commonlib.uikit.widget;

import android.annotation.TargetApi;
import android.content.Context;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.auric.intell.commonlib.utils.ContextFinder;

/**
 * Created by white on 2018/1/31.
 */

public class VideoPlayView extends SurfaceView implements SurfaceHolder.Callback, MediaPlayer.OnErrorListener, MediaPlayer.OnCompletionListener {

    MediaPlayer mediaPlayer;
    VideoPlayListener mListener;
    private static final String TAG = "VideoPlayView";


    public VideoPlayView(Context context) {
        super(context);
        initView();
    }

    public VideoPlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public VideoPlayView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public VideoPlayView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        initView();
    }

    private void initView(){
        setZOrderOnTop(true);
        getHolder().addCallback(this);
    }


    public void play(final String url, boolean loop, VideoPlayListener listener ) {
//        stopMediaPlayer();
//        mediaPlayer = new MediaPlayer();
        mListener = listener;
        try {
            if (mediaPlayer == null) {
                mediaPlayer = new MediaPlayer();
            }
            mediaPlayer.reset();
            mediaPlayer.setScreenOnWhilePlaying(true);
            mediaPlayer.setDataSource(ContextFinder.getApplication(), Uri.parse(url));
            mediaPlayer.setLooping(loop);
            mediaPlayer.prepare();
            mediaPlayer.setOnCompletionListener(this);
            mediaPlayer.start();
        } catch (Exception e) {
            Log.e(TAG, Log.getStackTraceString(e));
        }
    }


    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if (mediaPlayer == null) {
            mediaPlayer = new MediaPlayer();
        }
        try {
            Log.e(TAG,"surfaceCreated");
            mediaPlayer.setDisplay(holder);
        } catch (IllegalStateException e) {

            Log.e(TAG, Log.getStackTraceString(e));
            mediaPlayer = new MediaPlayer();
            mediaPlayer.setDisplay(holder);
        }

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.e(TAG,"surfaceChanged");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.e(TAG,"surfaceDestroyed");
        stopMediaPlayer();
    }

    @Override
    public boolean onError(MediaPlayer mp, int what, int extra) {
        Log.e(TAG, "error code:" + what);
        stopMediaPlayer();
        if (mListener != null) {
            mListener.onError();
        }
        return false;
    }

    @Override
    public void onCompletion(MediaPlayer mediaPlayer) {
        if (mListener != null) {
            mListener.onCompletion();
        }
    }

    private void stopMediaPlayer(){
        if (mediaPlayer != null) {
            try {
                mediaPlayer.stop();
                mediaPlayer.release();
            } catch (IllegalStateException e) {
                e.printStackTrace();
            }
            mediaPlayer = null;
        }
    }

    public int getCurPosition() {
        if (mediaPlayer != null) {
            try {
                return mediaPlayer.getCurrentPosition();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return -1;
    }

    public int getDuration() {
        if (mediaPlayer != null) {
            try {
                mediaPlayer.getDuration();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return -1;
    }

    public void seekTo(int msec) {
        if (mediaPlayer != null) {
            try {
                mediaPlayer.seekTo(msec);
            } catch (IllegalStateException e) {
                e.printStackTrace();
            }
        }
    }

    public void pause(){
        if (mediaPlayer != null) {
            try {
                mediaPlayer.pause();
            } catch (IllegalStateException e) {
                e.printStackTrace();
            }
        }
    }

    public void resume(){
        if (mediaPlayer != null) {
            try {
                if (!mediaPlayer.isPlaying()) {
                    mediaPlayer.start();
                }
            } catch (IllegalStateException e) {
                e.printStackTrace();
            }
        }
    }



    public interface VideoPlayListener{
        void onError();
        void onCompletion();
    }
}
