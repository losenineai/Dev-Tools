package com.auric.intell.commonlib.manager.retrofit;


import com.auric.intell.commonlib.BuildConfig;
import com.auric.intell.commonlib.robot.RobotInfo;
import com.auric.intell.commonlib.robot.Token;
import com.google.gson.JsonObject;

import org.json.JSONObject;

import java.util.Map;

import okhttp3.RequestBody;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Part;
import retrofit2.http.PartMap;
import retrofit2.http.Path;
import retrofit2.http.Query;
import retrofit2.http.QueryMap;
import retrofit2.http.Streaming;
import retrofit2.http.Url;
import rx.Observable;

/**
 * Created by Ｔａｍｉｃ on 2016-07-08.
 * {@link # https://github.com/NeglectedByBoss/RetrofitClient}
 */
public interface BaseApiService {

    public static final String Base_URL = "https://lsdb.oduola.com/";


    @GET("{url}")
    Observable<ResponseBody> executeGet(
            @Path("url") String url,
            @QueryMap Map<String, String> maps
    );


    @POST("{url}")
    Observable<ResponseBody> executePost(
            @Path("url") String url,
            //  @Header("") String authorization,
            @QueryMap Map<String, String> maps);

    @POST("{url}")
    Observable<ResponseBody> json(
            @Path("url") String url,
            @Body RequestBody jsonStr);

    @Multipart
    @POST("{url}")
    Observable<ResponseBody> upLoadFile(
            @Path("url") String url,
            @Part("image\"; filename=\"image.jpg") RequestBody requestBody);

    @POST("{url}")
    Call<ResponseBody> uploadFiles(
            @Path("url") String url,
            @Path("headers") Map<String, String> headers,
            @Part("filename") String description,
            @PartMap() Map<String, RequestBody> maps);

    @Streaming
    @GET
    Observable<ResponseBody> downloadFile(@Url String fileUrl);

}
