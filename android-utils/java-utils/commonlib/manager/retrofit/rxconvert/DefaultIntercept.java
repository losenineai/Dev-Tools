package com.auric.intell.commonlib.manager.retrofit.rxconvert;

import com.auric.intell.commonlib.manager.retrofit.BaseInterceptor;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by zhangxiliang on 2017/12/27.
 */

public class DefaultIntercept extends BaseInterceptor{
    @Override
    public Map<String, String> getHeaders() {

        Map<String,String> headers=new HashMap<>();

        headers.put("Content-Type","application/x-www-form-urlencoded; charset=utf-8");
        headers.put("Accept","application/vnd.robot.v1+json");

        return headers;
    }
}
