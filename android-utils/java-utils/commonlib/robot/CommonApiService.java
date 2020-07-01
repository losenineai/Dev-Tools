package com.auric.intell.commonlib.robot;

import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Query;
import rx.Observable;

/**
 * Created by zhangxiliang on 2017/5/2.
 */

public interface CommonApiService {


    /***
     * 登录获取token
     * @param serial_no
     * @param password
     * @return
     */
    @POST("r/token")
    Observable<Token> login(@Query("serial_no") String serial_no , @Query("password") String password);


    /**
     * 旧Token换取新Token
     * @param expreToken
     * @return
     */
    @PUT("r/token")
    Observable<Token> refreshToken(@Query("token") String expreToken);

    /***
     * 时钟同步
     * @return
     */
    @GET("/clock")
    Observable<SyncClock> synClock();


}
