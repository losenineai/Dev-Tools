package com.auric.intell.commonlib.manager.retrofit;

import android.content.Context;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogUtils;

import rx.Subscriber;

/**
 * BaseSubscriber
 * Created by Tamic on 2016-08-03.
 */
public abstract class BaseSubscriber<T> extends Subscriber<T> {

    private Context context;
    private boolean isNeedCahe;

    public BaseSubscriber(){
        this.context= ContextFinder.getApplication();
    }

    public BaseSubscriber(Context context) {
        this.context = context;
    }

    @Override
    public void onError(Throwable e) {
        LogUtils.e(e);
        // todo error somthing
        //onError(new ExceptionHandle.ResponeThrowable(e, ExceptionHandle.ERROR.UNKNOWN));
        onError(ExceptionHandle.handleException(e));
//        if(e instanceof ExceptionHandle.ResponeThrowable){
//            onError((ExceptionHandle.ResponeThrowable)e);
//        } else {
//            onError(new ExceptionHandle.ResponeThrowable(e, ExceptionHandle.ERROR.UNKNOWN));
//        }
    }


    @Override
    public void onStart() {
        super.onStart();

//        Toast.makeText(context, "http is start", Toast.LENGTH_SHORT).show();

        // todo some common as show loadding  and check netWork is NetworkAvailable
        // if  NetworkAvailable no !   must to call onCompleted
     /*   if (!NetworkUtil.isNetworkAvailable(context)) {
            //Toast.makeText(context, "无网络，读取缓存数据", Toast.LENGTH_SHORT).show();
            onCompleted();
        }*/

    }

    @Override
    public void onCompleted() {

//        Toast.makeText(context, "http is Complete", Toast.LENGTH_SHORT).show();
        // todo some common as  dismiss loadding
    }


    public abstract void onError(ExceptionHandle.ResponeThrowable e);

}
