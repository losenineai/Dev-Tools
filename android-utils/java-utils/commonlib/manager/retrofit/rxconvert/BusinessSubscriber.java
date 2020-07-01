package com.auric.intell.commonlib.manager.retrofit.rxconvert;

import rx.Subscriber;

/**
 * Created by zhangxiliang on 2017/12/26.
 */

public abstract class BusinessSubscriber <T> extends Subscriber<T> {



    @Override
    public void onError(Throwable e) {

        onBusinessError(new BusinessException(e));
    }

    @Override
    public void onCompleted() {

    }

    public abstract void onBusinessError(BusinessException businessException);


}
