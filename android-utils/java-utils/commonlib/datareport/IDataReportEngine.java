package com.auric.intell.commonlib.datareport;

import android.content.Context;

import java.util.Map;

/**
 * @author white
 * @description：数据上报引擎接口
 * @date 2017/8/22
 */

public interface IDataReportEngine {

    /**
     * 配置信息
     * @param context
     * @param appKey
     * @param channelID
     */
     void config(Context context, String appKey, String channelID);

    /**
     * session的统计，统计应用时长
     *
     * @param context
     */
    void onResume(Context context);

    /**
     * session的统计，统计应用时长
     *
     * @param context
     */
    void onPause(Context context);

    /**
     * 自定义事件上报
     * @param eventID
     */

    void onEvent(String eventID,String key,String value);
    void onEvent(String eventID);
    void onEvent(String eventID, Map map);
    void onEvent(String eventID, Map map, int value);
}
