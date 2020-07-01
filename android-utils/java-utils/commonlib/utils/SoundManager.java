package com.auric.intell.commonlib.utils;

import java.util.HashMap;
import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

public class SoundManager implements SoundPool.OnLoadCompleteListener {
    /** SoundPool left volume */
	private static final float LEFT_VOLUME = 1.0f;
	
	/** SoundPool right volume */
	private static final float RIGHT_VOLUME = 1.0f;
	
	/** All sounds will have equal priority */
	private static final int STREAM_PRIORITY  = 0;	
	
	/** Potential LOOP_MODE */
	private static final int MODE_NO_LOOP = 0;
	
	/** Potential LOOP_MODE */
	@SuppressWarnings("unused")
	private static final int MODE_LOOP_FOREVER = -1;
	
	/** Whether sounds should loop */
	private static final int LOOP_MODE = MODE_NO_LOOP;
	
	/** SoundPool playback rate */
	private static final float PLAYBACK_RATE = 1.0f;
	
	private static final String TAG = "SoundManager";
	
	/** Inner SoundManager instance */
	private static SoundManager sInstance = null;

	/** Mapping of resource ids to sound ids returned by load() */
	private HashMap<Integer, Integer> mSoundMap = new HashMap<Integer,Integer>();


	int mCurrentStreamid;
	/** SoundPool instance */
	private SoundPool mSoundPool;
	
	/** Application Context */
	private Context mContext;
	
	/** Maximum concurrent streams that can play */
	private static final int MAX_STREAMS = 1;
	
	/** Private constructor for singleton */
	private SoundManager(Context context) {
		mContext = context.getApplicationContext();
		mSoundPool = new SoundPool(MAX_STREAMS, AudioManager.STREAM_MUSIC, 0);
		mSoundPool.setOnLoadCompleteListener(this);
	}

	/** Static access to internal instance */
	public static SoundManager getInstance(Context context) {
		if (sInstance == null) {
			sInstance = new SoundManager(context.getApplicationContext());
		}
		return sInstance;
	}

	/** Loads a sound. Called automatically by play() if not already loaded */
	public void load(int id) {
		mSoundMap.put(id, mSoundPool.load(mContext, id, 1));
	}

	/** 
	 * Test if sound is loaded, call with id from R.raw
	 *  
	 * @param resourceId
	 * @return true|false
	 */
	public boolean isSoundLoaded(int resourceId) {
		return mSoundMap.containsKey(resourceId);
	}
	
	/** Unload sound, prints warning if sound is not loaded */
	public void unload(int id) {
		if (mSoundMap.containsKey(id)) {
			int soundId = mSoundMap.remove(id);
			mSoundPool.unload(soundId);
		} else {
			Log.w(TAG, "sound: " + id + " is not loaded!");
		}
	}
	
	public void play(int resourceId) {
        stop(resourceId);
        if (isSoundLoaded(resourceId)) {
			mCurrentStreamid = mSoundPool.play(mSoundMap.get(resourceId), LEFT_VOLUME, RIGHT_VOLUME, STREAM_PRIORITY, LOOP_MODE, PLAYBACK_RATE);
		} else {
			load(resourceId);
		}
	}


	public void stop(int resourceId){
		if(isSoundLoaded(resourceId)){
			mSoundPool.stop(mCurrentStreamid);
		}
	}

    public void stop() {
        try {
            mSoundPool.stop(mCurrentStreamid);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

	/**
	 * If the sound is being loaded for the first time, we should wait until it
	 * is completely loaded to play it.
	 */
	@Override
	public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
		mCurrentStreamid=mSoundPool.play(sampleId, LEFT_VOLUME, RIGHT_VOLUME, STREAM_PRIORITY, LOOP_MODE, PLAYBACK_RATE);
	}
}