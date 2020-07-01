package com.apktool.access;

import org.json.JSONObject;

/**
 * Created by mushroom0417 on 12/15/15.
 * 多开的接口
 */
public interface IMultiApkHelper {
    String generateNewApk(JSONObject configJson, ProgressListener progressListener);
    String getVersion();
    boolean isAppInjected(String packageName);
    /** extendedInfo结构
     {
        "package_name":"", require
        "target_name":"", require
        "is_speed":true, require
        "is_assist":true, optional
        "is_splash":true, optional
        "assist_url":"http://xxzhushou.cn/assist.zip", optional
        "has_local_assist":"false", require
        "assist_uid":107, require
     }
     */
    // 写在游戏apk的assets的json
    /** 
     { 
        "xxPkgName":"com.xmodlite", 
        "speed":{ 
            "so_name":"", 
            "so_path":"app_speed/so_name", 
            "so_construct":"construct_name", 
            "apk_name":"", 
            "apk_path":"", 
            "apk_entry":"" 
        }, 
        "assist":{ 
            "so_name":"", 
            "so_path":"app_speed/so_name", 
            "so_construct":"construct_name", 
            "apk_name":"", 
            "apk_path":"", 
            "apk_entry":"" 
        } 
     } 
     */
}
