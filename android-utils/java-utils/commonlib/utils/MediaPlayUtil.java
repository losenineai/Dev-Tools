package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.text.TextUtils;

/**
 * 播放音频url
 * @author white
 * @date 16/8/1
 */
public class MediaPlayUtil {

    private static final String TAG = "MediaPlayUtil";
    private static MediaPlayer sMediaPlayer;
    private static Context sContext;
    private static String sUrl;
    private static MediaPlayCallback sMediaCallback;
    private static boolean sBufferingPause;

    /**
     * 有序播放
     * @param context
     * @param urls
     * @param curIndex
     * @param callback
     */
    public static void playAndNext(final Context context, final String[] urls, final int curIndex, final MediaPlayCallback callback) {
        if(urls == null || curIndex >= urls.length || TextUtils.isEmpty(urls[curIndex])){
            if(callback!=null) {
                callback.onFinish();
            }
            return;
        }
        try {
            // 因为是static,防止资源未release导致不能播放
            stopAll();
            Uri uri = Uri.parse(urls[curIndex]);
            sMediaPlayer = new MediaPlayer();
            sMediaPlayer.reset();
            sMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            sMediaPlayer.setDataSource(context, uri);
//            sMediaPlayer.setDataSource(urls[curIndex]);
            sMediaPlayer.prepareAsync();
            sMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    LogTool.d(TAG, "setOnPreparedListener "+mp);
                    if(mp != null){
                        mp.start();
                    }
                    if(callback!=null) {
                        callback.onStart();
                    }
                }
            });
            sMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    LogTool.d(TAG, "setOnCompletionListener " + mp.isPlaying());
                    stopAll();
                    int next = curIndex + 1;
                    playAndNext(context, urls, next, callback);
                }
            });
            sMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
                @Override
                public boolean onError(MediaPlayer mp, int what, int extra) {
                    LogTool.d(TAG, "onError what:" + what + "extra:" + extra);
                    stopAll();
                    if(callback!=null) {
                        callback.onFinish();
                    }
                    return false;
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
            if(callback != null) {
                callback.onFinish();
            }
        }
    }

    /**
     * 播放单曲
     * @param context
     * @param url
     * @param loop
     * @param callback
     */
    public static void playOne(final Context context, final String url, final boolean loop, final MediaPlayCallback callback) {
        if(context == null || TextUtils.isEmpty(url)){
            if(callback!=null) {
                callback.onFinish();
            }
            return;
        }
        try {
            // 因为是static,防止资源未release导致不能播放
            stopAll();
            Uri uri = Uri.parse(url);
            sMediaPlayer = new MediaPlayer();
            sMediaPlayer.reset();
            sMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            sMediaPlayer.setDataSource(context, uri);
            sMediaPlayer.setLooping(loop);
//            sMediaPlayer.setDataSource(urls[curIndex]);
            sMediaPlayer.prepareAsync();
            sMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    LogTool.d(TAG, "setOnPreparedListener "+mp);
                    if(mp != null){
                        mp.start();
                    }
                    if(callback!=null) {
                        callback.onStart();
                    }
                }
            });
            sMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    LogTool.d(TAG, "setOnCompletionListener " + mp.isPlaying());
                    stopAll();
                }
            });
            sMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
                @Override
                public boolean onError(MediaPlayer mp, int what, int extra) {
                    LogTool.d(TAG, "onError what:" + what + "extra:" + extra);
                    stopAll();
                    if(callback!=null) {
                        callback.onFinish();
                    }
                    return false;
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
            if(callback != null) {
                callback.onFinish();
            }
        }
    }

    /**
     * 暂停当前
     */
    public static void pause(){
        try {
            if (sMediaPlayer != null ){
                if(sMediaPlayer.isPlaying()) {
                    sMediaPlayer.pause();
                    sBufferingPause =false;
                }
                else{
                    sMediaPlayer.reset();
                    sBufferingPause =true;
                }
            }
        } catch (Exception e) {
        }

    }

    /**
     * 恢复开始
     */
    public static void start(){
        try {
            if (sMediaPlayer != null )
                if(sBufferingPause){
                    play(sContext, sUrl, sMediaCallback);
                }else{
                    sMediaPlayer.start();
                }
            sBufferingPause =false;
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    /**
     * 播放url队列
     * @param context
     * @param urls
     * @param callback
     */
    public static void playQueue(final Context context, final String[] urls, final MediaPlayCallback callback){
        if(urls == null || urls.length == 0){
            if(callback != null) {
                callback.onFinish();
            }
            return;
        }
        for (String url : urls){
            LogTool.d(TAG, "url:"+url);
        }
        playAndNext(context, urls, 0, callback);
    }

    /**
     * 播放url
     * @param context
     * @param url
     * @param callback
     */
    public static void play(final Context context, String url, final MediaPlayCallback callback) {
        LogUtils.d("url:" + url+" tid:"+Thread.currentThread().getId());
        if(context == null || TextUtils.isEmpty(url)){
            if(callback != null) {
                callback.onFinish();
            }
            return;
        }
        sContext = context;
        sUrl = url;
        sMediaCallback = callback;
        playAndNext(context, new String[]{url}, 0, callback);
    }

    /**
     * 播放资源ID
     * @param context
     * @param resID
     * @param callback
     */
    public static void play(Context context, int resID, MediaPlayCallback callback) {
        play(context, MediaUrlParser.from(context, resID), callback);
    }

    /**
     * 播放资源ID
     * @param context
     * @param resID
     * @param callback
     */
    public static void play(Context context, int resID, boolean loop, MediaPlayCallback callback) {
        playOne(context, MediaUrlParser.from(context, resID), loop, callback);
    }

    /**
     * 停止当前所有音频
     */
    public static void stopAll(){
        LogUtils.d("");
        try{
            if (sMediaPlayer != null && sMediaPlayer.isPlaying()) {
                sMediaPlayer.pause();
                sMediaPlayer.stop();
            }
        }catch(Exception e){
            e.printStackTrace();
        }finally {
            try {
                if (sMediaPlayer != null) {
                    sMediaPlayer.release();
                    sMediaPlayer = null;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            sContext = null;
            sMediaCallback = null;
        }
    }


    /**
     * 获取当前播放的进度位置
     * @return
     */
    public static int getCurrentPos(){
        if (sMediaPlayer != null) {
            return sMediaPlayer.getCurrentPosition();
        }
        return -1;
    }

    /**
     * 获取当前播放的总时长
     * @return
     */
    public static int getMediaDuration(){
        try {
            if (sMediaPlayer != null && sMediaPlayer.isPlaying()){
                return sMediaPlayer.getDuration();
            }
        }catch (Exception e){
            // 继续播放
            play(sContext, sUrl, sMediaCallback);
        }
        return 0;
    }

    /**
     * 是否正在播放
     * @return
     */
    public static boolean isPlaying(){
        try {
            return sMediaPlayer != null && sMediaPlayer.isPlaying();
        } catch (Exception e) {
        }
        return false;
    }

    /**
     * 设置当前播放进度位置
     * @param pos
     */
    public static void setMediaPlayPos(int pos){
        try {
            if (sMediaPlayer != null && sMediaPlayer.isPlaying())
                sMediaPlayer.seekTo(pos);
        }catch (Exception e){
            e.printStackTrace();
        }
    }


    /***
     * generate uri from resource ID class
     */
    public static class MediaUrlParser{

        /**
         * generate Uri as String from resource id
         * @param context
         * @param resId
         * @return
         */
        public static String from(Context context, int resId) {
            return "android.resource://"+ context.getPackageName() + "/" + resId;
        }

        /**
         * generate Uri as String from resource filename
         * @param context
         * @param rawfileName  raw/filename （不要文件后缀)
         * @return
         */
        public static String from(Context context,String rawfileName){
            return "android.resource://"+ context.getPackageName() + "/" + rawfileName;
        }


    }

    /**
     * 回调接口
     */
    public interface MediaPlayCallback {
        void onFinish();
        void onStart();
    }
}
