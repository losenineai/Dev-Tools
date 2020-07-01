package com.auric.intell.commonlib.manager.http;

/**
 * Created by white on 16/7/19.
 */
public interface IHttpListener {

    void onSuccess(HttpResult result);

    void onFailed(HttpResult result);

}
