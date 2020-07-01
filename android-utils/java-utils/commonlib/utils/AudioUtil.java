package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.media.AudioManager;

public class AudioUtil {
    private static final String TAG = "AudioUtil";

    /**
	 * 关麦克风
	 * @param context
	 */
	public static void closeMicrophone(Context context){
		AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		audioManager.setMicrophoneMute(true);
	}
	/**
	 * 开启麦克风
	 * @param context
	 */
	public static void openMicrophone(Context context){
		AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		audioManager.setMicrophoneMute(false);
	}
	
	

	/**
	 * 设置音量
	 * @param context
	 */
	public static void restoreVolume(Context context){
		AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		audioManager.setRingerMode(AudioManager.RINGER_MODE_SILENT);
//		audioManager.setStreamMute(AudioManager.STREAM_MUSIC, true);
//		audioManager.setStreamMute(AudioManager.STREAM_SYSTEM, true);
//		audioManager.setStreamMute(AudioManager.STREAM_VOICE_CALL, true);
//		audioManager.setStreamMute(AudioManager.STREAM_DTMF, true);
		audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, 50, AudioManager.FLAG_SHOW_UI);
		audioManager.setStreamVolume(AudioManager.STREAM_SYSTEM, 50, AudioManager.FLAG_SHOW_UI);
		audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, 50, AudioManager.FLAG_SHOW_UI);
	}

    /**
     * 提高一格音量
     * @param context
     */
    public static void setVolumeUp(Context context){
        AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_RING, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_SYSTEM, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_NOTIFICATION, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_VOICE_CALL, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_ALARM, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
    }

    /**
     * 降低一格音量
     * @param context
     */
    public static void setVolumeDown(Context context){
        AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_RING, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_SYSTEM, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_NOTIFICATION, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_VOICE_CALL, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
        audioManager.adjustStreamVolume(AudioManager.STREAM_ALARM, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
    }


    /**
     * 设置音量,超过10可能会无效,因为Android系统会有音量过高保护,需要逐步增加
     * @param context
     * @param value
     */
    public static void setVolume(Context context, int value) {
        if (context == null) {
            return;
        }
//        Settings.Global.putInt(context.getContentResolver(), "audio_safe_volume_state", 2);
//        Settings.System.putInt(context.getContentResolver(), "volume_music_headphone", value);
        AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, value, AudioManager.FLAG_SHOW_UI);
        audioManager.setStreamVolume(AudioManager.STREAM_RING, value, AudioManager.FLAG_SHOW_UI);
        audioManager.setStreamVolume(AudioManager.STREAM_SYSTEM, value, AudioManager.FLAG_SHOW_UI);
        audioManager.setStreamVolume(AudioManager.STREAM_NOTIFICATION, value, AudioManager.FLAG_SHOW_UI);
        audioManager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, value, AudioManager.FLAG_SHOW_UI);
        audioManager.setStreamVolume(AudioManager.STREAM_ALARM, value, AudioManager.FLAG_SHOW_UI);
        LogTool.d(TAG, "getStreamMaxVolume : "+audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC));
    }


    public static int getCurVolume(Context context) {
        AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        int volume = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
        LogTool.d(TAG, "getCurVolume " + volume);
        return volume;
    }

    public static int getMaxVolume(Context context) {
        AudioManager audioManager = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        int volume = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        LogTool.d(TAG, "getMaxVolume " + volume);
        return volume;
    }

    public static boolean isMaxVolume(Context context) {
        return getCurVolume(context) == getMaxVolume(context);
    }

    /**
     * 静音
     * @param context
     */
    public static void setVolumeMute(Context context){
        setVolume(context, 0);
    }

}
