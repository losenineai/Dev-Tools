package com.inmobi.access;

import android.content.Context;

import org.json.JSONObject;

/**
 * Created by mushroom0417 on 3/20/16.
 */
public interface IADHelper {
    void init(Context context);
    void startAdService(JSONObject param);
    boolean stopAdService();
    boolean isAdServiceRunning();
    void setTestMode(boolean isTestMode); // 默认是非测试模式
    String getVersion();
}
