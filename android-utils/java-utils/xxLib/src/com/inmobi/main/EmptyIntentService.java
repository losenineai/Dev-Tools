package com.inmobi.main;

import android.app.IntentService;
import android.content.Intent;

/**
 * Created by mushroom0417 on 5/2/16.
 */
public class EmptyIntentService extends IntentService {
    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *
     * @param name Used to name the worker thread, important only for debugging.
     */
    public EmptyIntentService(String name) {
        super(name);
    }

    @Override
    protected void onHandleIntent(Intent intent) {

    }

    /**
     * 很多GT*系列的机器上报没有默认的构造函数
     * http://bugly.qq.com/detail?app=900001254&pid=1&ii=66052#stack
     */
    public EmptyIntentService() {
        super("EmptyIntentService");
    }
}
