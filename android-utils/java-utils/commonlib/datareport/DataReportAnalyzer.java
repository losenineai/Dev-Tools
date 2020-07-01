package com.auric.intell.commonlib.datareport;

import android.content.Context;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.ReflectUtil;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;


/**
 * @author white
 * @description：数据上报入口
 * @date 2017/8/18
 */

public class DataReportAnalyzer {

    /**
     * 单例模式
     */
    private static DataReportAnalyzer sInstance;
    public static DataReportAnalyzer getInstance() {
        synchronized (DataReportAnalyzer.class) {
            if (sInstance == null) {
                synchronized (DataReportAnalyzer.class) {
                    sInstance = new DataReportAnalyzer();
                }
            }
        }
        return sInstance;
    }


    private Set<IDataReportEngine> mEngineSet = new HashSet<>();

    private DataReportAnalyzer(){
    }

    /**
     * 配置需要上报的所有引擎
     * @param configs
     * @return
     */
    public DataReportAnalyzer configEngine(DataReportEngineConfig... configs) {
        if (configs != null && configs.length > 0) {
            mEngineSet.clear();
            for (DataReportEngineConfig config : configs) {
                Object obj = ReflectUtil.getInstance(config.getClas());
                if (obj != null) {
                    IDataReportEngine engine = (IDataReportEngine)obj;
                    // 初始化统计SDK的配置
                    engine.config(ContextFinder.getApplication(), config.getAppKey(), config.getChannelID());
                    mEngineSet.add(engine);
                }
            }
        }
        return this;
    }



    public void onResume(Context context) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onResume(context);
        }
    }

    public void onPause(Context context) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onPause(context);
        }
    }

    public void onEvent(String eventID) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onEvent(eventID);
        }
    }

    public void onEvent(String eventID,String key,String value) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onEvent(eventID,key,value);
        }
    }



    public void onEvent(String eventID, Map map) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onEvent(eventID, map);
        }
    }




    public void onEvent(String eventID, Map map, int value) {
        if (mEngineSet.size() <= 0) {
            return;
        }
        for (IDataReportEngine engine : mEngineSet) {
            engine.onEvent(eventID, map, value);
        }
    }
}
