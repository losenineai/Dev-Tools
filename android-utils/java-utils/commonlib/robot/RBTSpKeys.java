package com.auric.intell.commonlib.robot;

import android.content.SharedPreferences;

/**
 * @author white
 * @description：RBT的key常量
 * @date 2016/10/9
 */

public class RBTSpKeys {
    // 是否正在夜间休眠
    public static final String BOOLEAN_AT_NIGHT_REST = "BOOLEAN_AT_NIGHT_REST";
    // 是否正在一键休眠
    public static final String BOOLEAN_IN_ONE_KEY_REST = "BOOLEAN_IN_ONE_KEY_REST";
    // 一键休眠总时长
    public static final String LONG_ONEKEY_REST_TOTAL_TIME_MS = "LONG_ONEKEY_REST_TOTAL_TIME_MS";
    // 一键休眠结束时间
    public static final String LONG_ONEKEY_REST_END_TIME = "LONG_ONEKEY_REST_END_TIME";
    // 是否正在休息提醒的5分钟
    public static final String BOOLEAN_IS_IN_REST_PROMPT = "BOOLEAN_IS_IN_REST_PROMPT";
    // 休息提醒开始时间, 使用System.currentTimeMillis()
    public static final String LONG_REST_PROMPT_START_TIME = "LONG_REST_PROMPT_START_TIME";


    // token
    public static final String TOKEN = "STRING_TOKEN";

    // last reg time
    public static final String LAST_REG_TIME = "LONG_LAST_REG_TIME";

    // last reg time
    public static final String LAST_REFRESH_TIME = "LONG_LAST_REFRESH_TIME";

    // 标识是否已经进入 Launcher (标识已经成功激活)
    public static final String BOOLEAN_IS_ENTER_LAUNCHER = "IS_ENTER_LAUNCHER";

    // 标识 WiFi 是否可以enable
    public static final String BPPLEAN_IS_WIfI_ENABLE = "IS_WIFI_ENABLE";

    // 是否正在聊天通话中
    public static final String BOOLEAN_IS_CHAT_CALLING = "BOOLEAN_IS_CHAT_CALLING";
    // 是否正在视频监控中
    public static final String BOOLEAN_IS_VIDEO_MONITORING = "BOOLEAN_IS_VIDEO_MONITORING";

    // Launcher是否关闭背景音乐
    public static final String BOOLEAN_IS_DISABLE_LAUNCHER_BG_MUSIC = "BOOLEAN_IS_DISABLE_LAUNCHER_BG_MUSIC";

}
