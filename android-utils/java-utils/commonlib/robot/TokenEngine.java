package com.auric.intell.commonlib.robot;

import android.util.Log;

import com.auric.intell.commonlib.manager.retrofit.BaseSubscriber;
import com.auric.intell.commonlib.manager.retrofit.ExceptionHandle;
import com.auric.intell.commonlib.manager.retrofit.RetrofitClient;
import com.auric.intell.commonlib.manager.retrofit.RxTransformer;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.MD5Util;
import com.auric.intell.commonlib.utils.SPUtils;

import java.util.concurrent.CountDownLatch;

import rx.Subscriber;
import rx.schedulers.Schedulers;

import static com.auric.intell.commonlib.robot.Robot.KEY_TOKEN_ROBOT;

/**
 * Created by zhangxiliang on 2017/4/28.
 * Token 有效期为一个小时，客户端每隔一小时刷新一次，刷新后旧Token失效,刷新有效期为一个月内
 * 一个月后，需要重新登录获取刷新有效期
 * @Deprecated "Token机制去除，使用sn替换"
 *
 */
public class TokenEngine {

    private static final String TAG ="TokenEngine" ;
    private static final String TOKEN_EXP_KEY ="exp";
    private static final String TOKEN_NBF_KEY ="nbf";

    private String mBaseUrl;
    private static final long TOKNE_SUB_TIME = 60l;
    private TokenEngine(){}

    public static TokenEngine getInstance(){
        return HolderInstance.tokenEngine;
    }

    static class HolderInstance{
        private static final TokenEngine tokenEngine=new TokenEngine();
    }


    public String refreshToken(String expToken){

        final CountDownLatch countDownLatch=new CountDownLatch(1);

        RetrofitClient.getInstance(ContextFinder.getApplication(), Robot.sBaseUrl).create(CommonApiService.class).refreshToken(expToken).compose(RxTransformer.<Token>transformer()).subscribeOn(Schedulers.newThread())
                .subscribe(new BaseSubscriber<Token>() {
            @Override
            public void onError(ExceptionHandle.ResponeThrowable e) {
                Log.e(TAG,e.status_code+"");
                getTokenByLogin(countDownLatch);

               // countDownLatch.countDown();
            }

            @Override
            public void onNext(Token token) {
                SPUtils.put(KEY_TOKEN_ROBOT,token.getToken());
                countDownLatch.countDown();
            }
        });

       try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        String token = (String) SPUtils.get(KEY_TOKEN_ROBOT, "");
        return token;

    }




    public void getTokenByLogin(final CountDownLatch countDownLatch){
        RobotInfo robotInfo=Robot.getInfo();
        String passwd = new String(robotInfo.getSerial_no()).toLowerCase().toString();

        //Log.e(TAG,robotInfo.getWifiMac());
        String  md5Password = MD5Util.encode(passwd);

        RetrofitClient.getInstance(ContextFinder.getApplication(), Robot.sBaseUrl).create(CommonApiService.class).login(Robot.getInfo().getSerial_no(),md5Password).
                subscribeOn(Schedulers.immediate()).
                subscribe(new Subscriber<Token>() {
                    @Override
                    public void onCompleted() {

                    }

                    @Override
                    public void onError(Throwable e) {
                        // SPUtils.put(Robot.KEY_TOKEN_ROBOT,"");
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onNext(Token token) {
                        SPUtils.put(KEY_TOKEN_ROBOT,token.getToken());
                        countDownLatch.countDown();
                    }
                });
    }

    interface CallBack{

        public void onCompleted(String newToken);

    }

}
