package com.auric.intell.commonlib.manager.retrofit;

import rx.Observable;
import rx.functions.Func1;

/**
 * Created by zhangxiliang on 2017/5/2.
 */

public class RxTransformer {
    public static  <T> Observable.Transformer<T,T> transformer() {

        return new Observable.Transformer() {

            @Override
            public Object call(Object observable) {
                return ((Observable) observable).onErrorResumeNext(new HttpResponseFunc<T>());
            }
        };
    }


    private static class HttpResponseFunc<T> implements Func1<Throwable, Observable<T>> {
        @Override public Observable<T> call(Throwable t) {
            return Observable.error(ExceptionHandle.handleException(t));
        }
    }
}
