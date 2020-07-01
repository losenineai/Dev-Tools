package com.auric.intell.commonlib.uikit;

import com.auric.intell.commonlib.manager.retrofit.ExceptionHandle;

import java.util.List;

/**
 * Created by zhangxiliang on 2017/3/22.
 */

public  interface  DataSource{

    interface BaseDataCallBack<S>{
        void onLoadCompleted(S sList);
        void onLoadFail(ExceptionHandle.ResponeThrowable e);
    }



}
