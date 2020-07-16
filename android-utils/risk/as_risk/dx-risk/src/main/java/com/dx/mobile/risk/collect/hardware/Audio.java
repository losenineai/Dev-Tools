package com.dx.mobile.risk.collect.hardware;

import android.content.Context;
import android.media.AudioManager;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Audio {


    private static AudioManager audioManager;

    public static String getAudioInfo() {

        JSONObject action_json = new JSONObject();

        try {
            if (audioManager == null)
                audioManager = (AudioManager) ContextFinder.getApplication().getSystemService(Context.AUDIO_SERVICE);
            JSONUtils.jsonPut(action_json, JNIConst.KEY_APP_KOCK_INDEX3, JNIConst.VALUE_APP_KOCK);
            JSONUtils.jsonPut(action_json, "K800", audioManager.getStreamVolume(1));
            JSONUtils.jsonPut(action_json, "K801", audioManager.getRingerMode());

        } catch (Exception e) { }

        return action_json.toString();
    }

}
