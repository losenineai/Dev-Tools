package com.xxlib.utils;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.media.SoundPool.OnLoadCompleteListener;

/**
 * 声音播放工具
 * @description
 * @author white
 * 2015-6-1
 * @modificator
 */
public class SoundPoolProxy {
	
	private static String TAG = "SoundPool";

	private SoundPool mSoundPool;			//播放器
	private AudioManager mAudioManager;		//系统音频管理
	private int mCurStreamID;				//当前加载的流id
	public static int MAX_SIZE_SOUND = 1;	//同时播放音频个数
	
	private OnLoadCompleteListener mLoadCompletedListener = new OnLoadCompleteListener(){
		@Override
		public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
			//停止当前播放
			if(mCurStreamID > 0){
				mSoundPool.stop(mCurStreamID);
			}
			
			//当前音量
			int curVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
			//最大音量
			int maxVolume = mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
			//音量比率
			float rate = curVolume * 1.0F / maxVolume;
			//播放,播放一次
			mSoundPool.play(sampleId, rate, rate, 1, 0, 1.0F);
		}
	};
	
	public SoundPoolProxy(){
		mSoundPool = new SoundPool(MAX_SIZE_SOUND, AudioManager.STREAM_MUSIC, 0);
		mSoundPool.setOnLoadCompleteListener(mLoadCompletedListener);
	}
	
	public void play(Context context, int resId){
		if(mSoundPool == null || context == null || resId < 0){
			return ;
		}
		
		//实例化系统音频管理器
		if(mAudioManager == null){
			mAudioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);   
		}
		//加载
		mCurStreamID = mSoundPool.load(context, resId, 1);
	}
	
	
	public void release(){
		if(mSoundPool != null){
			mSoundPool.stop(mCurStreamID);
			mSoundPool.release();
			mSoundPool = null;
		}
	}
	
	
	
	
}
