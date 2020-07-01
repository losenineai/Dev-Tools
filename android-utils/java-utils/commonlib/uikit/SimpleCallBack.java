package com.auric.intell.commonlib.uikit;

import com.auric.intell.commonlib.manager.retrofit.ExceptionHandle;
import com.auric.intell.commonlib.manager.retrofit.rxconvert.BusinessException;

/**
 * Created by zhangxiliang on 2017/12/30.
 */

public interface SimpleCallBack<S> {

    void onLoadCompleted(S sList);
    void onLoadFail(BusinessException e);
}
