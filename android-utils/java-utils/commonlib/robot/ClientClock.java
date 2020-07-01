package com.auric.intell.commonlib.robot;

import com.auric.intell.commonlib.manager.retrofit.BaseSubscriber;
import com.auric.intell.commonlib.manager.retrofit.ExceptionHandle;
import com.auric.intell.commonlib.manager.retrofit.RetrofitClient;
import com.auric.intell.commonlib.manager.retrofit.RxSchedulers;
import com.auric.intell.commonlib.manager.retrofit.RxTransformer;
import com.auric.intell.commonlib.uikit.DataSource;
import com.auric.intell.commonlib.utils.ContextFinder;

import rx.Subscriber;

/**
 * Created by zhangxiliang on 2017/6/12.
 */

public class ClientClock {

    public static final String CLOCK_URL = "http://lsdb.oduola.com/";

    public static void synclock(final DataSource.BaseDataCallBack<SyncClock> callBack) {
        RetrofitClient.getInstance(ContextFinder.getApplication(), CLOCK_URL).create(CommonApiService.class).synClock().compose(RxSchedulers.io_main()).subscribe(new BaseSubscriber<SyncClock>() {
            @Override
            public void onError(ExceptionHandle.ResponeThrowable e) {

                callBack.onLoadFail(e);
            }

            @Override
            public void onNext(SyncClock syncClock) {
                callBack.onLoadCompleted(syncClock);
            }
        });
    }

}
