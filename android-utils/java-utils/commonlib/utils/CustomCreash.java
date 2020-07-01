package com.auric.intell.commonlib.utils;

import android.content.Context;

import cat.ereza.customactivityoncrash.CustomActivityOnCrash;

/**
 * Created by zhangxiliang on 2017/1/4.
 */
public class CustomCreash {

    public static  void init(Context context){
        CustomActivityOnCrash.install(context);
        CustomActivityOnCrash.setEnableAppRestart(false);

    }
}
