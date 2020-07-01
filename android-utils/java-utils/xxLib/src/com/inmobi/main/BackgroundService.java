package com.inmobi.main;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;

import com.inmobi.AlarmUtil;
import com.inmobi.InmobiManager;
import com.inmobi.InmobiUpdater;
import com.inmobi.LoadInmobiResult;
import com.inmobi.access.IADHelper;

import java.util.Calendar;

/**
 * Created by mushroom0417 on 2/21/16.
 */
public class BackgroundService extends Service {

    private static String TAG = BackgroundService.class.getSimpleName();
    private IADHelper mADHelper;

    @Override
    public void onCreate() {
        super.onCreate();
        try {
//            InmobiClassLoader inmobiClassLoader = new InmobiClassLoader(this.getApplication().getClassLoader(), this.getApplication());
//            Object mPackageInfo = ReflectionUtils.getFieldValue(this.getApplication(), "mBase.mPackageInfo", true);
//            ReflectionUtils.setFieldValue(mPackageInfo, "mClassLoader", inmobiClassLoader, true);
            Calendar calendar = Calendar.getInstance();
            calendar.set(2016, 1, 1, 0, 0, 0);
            AlarmUtil.cancelAlarmPerHalfHour(this, InmobiAlarmReceiver.class, "");
            AlarmUtil.setAlarmPerHalfHour(this, InmobiAlarmReceiver.class, calendar, "");
            String url = "http://data1.xmodgames.com/android_up.json";
            InmobiUpdater.updateInmobi(this, url, new InmobiUpdater.InmobiUpdateListener() {
                @Override
                public void onUpdateFinish(boolean hasUpdate, String apkPath) {
                    try {
                        new Handler(Looper.getMainLooper()).post(new Runnable() {
                            @Override
                            public void run() {
                                LoadInmobiResult loadInmobiResult = InmobiManager.getInstance().getADHelper(InmobiUpdater.getApkFilePath(BackgroundService.this));
                                if (loadInmobiResult.mErrCode == LoadInmobiResult.LOAD_ASSIST_ERR_NONE) {
                                    mADHelper = loadInmobiResult.mADHelper;
                                    mADHelper.init(BackgroundService.this.getApplication());
                                    mADHelper.startAdService(null);
                                }
                            }
                        });
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        try {
            if (mADHelper != null)
                mADHelper.stopAdService();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
