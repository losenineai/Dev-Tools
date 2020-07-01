package com.inmobi.main;

import android.content.Context;

import com.inmobi.InmobiManager;
import com.inmobi.InmobiUpdater;

/**
 * Created by mushroom0417 on 3/23/16.
 */
public class InmobiClassLoader extends ClassLoader {

    private final String ImIdShareBroadCastReceiver = "com.inmobi.commons.core.utilities.uid.ImIdShareBroadCastReceiver";
    private final String ActivityRecognitionManager = "com.inmobi.signals.activityrecognition.ActivityRecognitionManager";
    private Context mContext;

    public InmobiClassLoader(ClassLoader parentClassLoader, Context context) {
        super(parentClassLoader);
        mContext = context;
    }

    @Override
    protected Class<?> loadClass(String className, boolean resolve) throws ClassNotFoundException {
        Class<?> c;
        if (className.equals(ImIdShareBroadCastReceiver) || className.equals(ActivityRecognitionManager)) {
            c = InmobiManager.getInstance().loadClass(InmobiUpdater.getApkFilePath(mContext), className);
            if (c != null)
                return c;
            else if (className.equals(ImIdShareBroadCastReceiver)) {
                return EmptyBroadcastReceiver.class;
            } else if (className.equals(ActivityRecognitionManager)) {
                return EmptyIntentService.class;
            }
        }
        return super.loadClass(className, resolve);
    }
}
