package com.auric.intell.commonlib.manager.http;

import java.util.HashMap;

/**
 * @author : jinlongfeng
 * @version : V0.0.1
 * @description: HTTP head 类
 * @time : 16/7/22-00:53
 * @modifier white
 *
 * @modifier jinlong
 * @desc 添加 JSON 获取类型
 * @time 09-17-22-21
 */
public class HttpHeader extends HashMap<String, String> {

    public HttpHeader() {
    }

    public static HttpHeader getRbtCommonHeader() {
        HttpHeader header = new HttpHeader();
        header.put("Content-Type", "application/x-www-form-urlencoded");
        header.put("Accept", "application/vnd.robot.v1+json API V1");
        return header;
    }

    public static HttpHeader getRbtJSONCommonHeader() {
        HttpHeader header = new HttpHeader();
        header.put("Content-Type", "application/json");
        header.put("Accept", "application/vnd.robot.v1+json API V1");
        return header;
    }

    public HttpHeader putToken(String token) {
        put("Authorization", "Bearer " + token);
        return this;
    }

    public HttpHeader putSn(String sn) {
        put("sn", sn);
        return this;
    }
}
