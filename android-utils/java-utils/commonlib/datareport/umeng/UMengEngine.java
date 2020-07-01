package com.auric.intell.commonlib.datareport.umeng;

import android.content.Context;

import com.auric.intell.commonlib.datareport.IDataReportEngine;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.umeng.analytics.MobclickAgent;

import java.util.HashMap;
import java.util.Map;

/**
 * @author white
 * @description：友盟SDK实现
 * @date 2017/8/22
 */

public class UMengEngine implements IDataReportEngine {

    @Override
    public void config(Context context, String appKey, String channelID) {
        MobclickAgent.UMAnalyticsConfig config = new MobclickAgent.UMAnalyticsConfig(context, appKey, channelID,
                MobclickAgent.EScenarioType.E_UM_NORMAL);
        MobclickAgent.setCatchUncaughtExceptions(true);
        MobclickAgent.setDebugMode(true);
        MobclickAgent.startWithConfigure(config);
    }

    @Override
    public void onResume(Context context) {
        MobclickAgent.onResume(context);
    }

    @Override
    public void onPause(Context context) {
        MobclickAgent.onPause(context);
    }

    @Override
    public void onEvent(String eventID, String key, String value) {
        HashMap map=new HashMap();
        map.put(key,value);
        MobclickAgent.onEvent(ContextFinder.getApplication(), eventID, map);

    }

    @Override
    public void onEvent(String eventID) {
        MobclickAgent.onEvent(ContextFinder.getApplication(), eventID);
    }

    @Override
    public void onEvent(String eventID, Map map) {
        MobclickAgent.onEvent(ContextFinder.getApplication(), eventID, map);
    }

    @Override
    public void onEvent(String eventID, Map map, int value) {
        MobclickAgent.onEventValue(ContextFinder.getApplication(), eventID, map, value);
    }
}
