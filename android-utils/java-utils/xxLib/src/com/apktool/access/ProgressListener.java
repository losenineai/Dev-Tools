package com.apktool.access;

import org.json.JSONObject;

/**
 * 用于监听多开进度的监听者
 */
public interface ProgressListener {
    void onProgressUpdate(int progress);
    void onFinish(JSONObject resultJson);
    /**
     {
        "is_multi_success":"true",
        "inject_speed_success":"true",
        "inject_plugin_success":"true",
        "msg":"多开成功/失败",
        "apk_path":"多开成功后的安装包路径"
     }
     */
}