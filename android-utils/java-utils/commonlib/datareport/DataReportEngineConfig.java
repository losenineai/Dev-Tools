package com.auric.intell.commonlib.datareport;

import com.auric.intell.commonlib.datareport.umeng.UMengEngine;

/**
 * @author white
 * @description：
 * @date 2017/8/23
 */

public enum DataReportEngineConfig {
    // TODO: 2017/8/23 自由渠道需求实现
    MASTER(null),     // 自有渠道
    UMENG(UMengEngine.class);       // 友盟



    private Class<?> clas;
    private String appKey;
    private String channelID;
    DataReportEngineConfig(Class<?> clas) {
        this.clas = clas;
    }

    public DataReportEngineConfig config(String appKey, String channelID) {
        this.appKey = appKey;
        this.channelID = channelID;
        return this;
    }

    public Class<?> getClas() {
        return clas;
    }

    public DataReportEngineConfig setClas(Class<?> clas) {
        this.clas = clas;
        return this;
    }

    public String getAppKey() {
        return appKey;
    }

    public DataReportEngineConfig setAppKey(String appKey) {
        this.appKey = appKey;
        return this;
    }

    public String getChannelID() {
        return channelID;
    }

    public DataReportEngineConfig setChannelID(String channelID) {
        this.channelID = channelID;
        return this;
    }
}
