package com.dx.mobile.risk;

import android.content.Context;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.linker.ReLinker;
import com.dx.mobile.risk.linker.ReLinkerErrorHandler;

import java.util.HashMap;

/**
 * Created by blue on 2018/11/29.
 */

public class DXRisk {

    private static final String TAG = "DXRisk";

    /* 用户相关字段 */
    public static final String KEY_USER_ID      = "U1";     // user_id
    public static final String KEY_USER_EMAIL   = "U2";     // email
    public static final String KEY_USER_PHONE   = "U3";     // phone
    public static final String KEY_USER_EXTEND1 = "U100";   // extend1
    public static final String KEY_USER_EXTEND2 = "U101";   // extend2
    public static final String KEY_USER_APP_LISTS_CHECK = "U201";    //作弊器检查。
    public static final String KEY_USER_IP_ROUTE_CHECK = "U202";

    /* 私有化url */
    public static final String KEY_URL = "KEY_URL";

    /* 接入saas服务器所在国家 */
    public static final String KEY_COUNTRY = "KEY_COUNTRY";
    public static final String VALUE_COUNTRY_INDONESIA = "INDONESIA";
    public static final String VALUE_COUNTRY_CHINA = "CHINA";

    /* 私有化服务器是否双写 */
    public static final String KEY_BACKUP = "KEY_BACKUP";
    public static final String KEY_BACKUP_APPID = "KEY_BACKUP_APPID";
    public static final String VALUE_ENABLE_BACKUP = "VALUE_ENABLE_BACKUP";

    /**
     * 请求token超时时间
     */
    public static final String KEY_DELAY_MS_TIME = "KEY_DELAY_MS_TIME";

    /**
     * 启动异步上报功能
     */
    public static final String KEY_ENABLE_ASYNC = "KEY_ENABLE_ASYNC";

    /**
     * FIXME: 专门给触手的api，因客户setup调用时机不在application，可能会触发4.4loadLibrary的bug
     *        报 UnsatisfiedLinkError的错误
     */
    public static boolean loadLibrary(Context context) {

        boolean ret = true;

        String version = BuildConfig.VERSION_NAME;

        try {
            DXRiskInternal.loadLibrary();
        } catch (UnsatisfiedLinkError e) {
            ReLinker.force();
            ret = ReLinker.loadLibrary(context, "DXRiskComm-" + version, e);
        }


        return ret;
    }

    /**
     * 初始化参数，环境
     * @param context
     * @return
     */
    public static boolean setup(Context context) {
        return setup(context, null, null);
    }

    /**
     * 初始化参数，环境
     * @param context
     * @param appId
     * @return
     * @deprecated 新版已弃用，建议使用 setup(context)
     */
    @Deprecated
    public static boolean setup(Context context, String appId) {
        return setup(context, appId, null);
    }

    /**
     * 初始化参数，环境
     * @param context
     * @param appId
     * @param paramsMap
     * @return
     * @deprecated 新版已弃用，建议使用 setup(context)
     */
    @Deprecated
    public static boolean setup(Context context, String appId, HashMap<String, String> paramsMap) {

        if (context == null) {
            throw new DXRiskErrorException("Parameters context can not be null from setup method.");
        }

        boolean result = false;

        Context ctx = context.getApplicationContext();

        if(loadLibrary(context)) {

            ContextFinder.setTokenContext(ctx);

            try {
                result = DXRiskInternal.setup(ctx, appId, paramsMap);
            } catch (LinkageError e) {
                ReLinkerErrorHandler.handleSetupError(ctx, e);
            }
        }

        return result;
    }

    /**
     * 获取token
     * @return
     * @throws DXRiskErrorException
     * @deprecated 新版已弃用，建议使用 getToken(String appId) 或者 getToken(String appId, HashMap<String, String> paramsMap)
     */
    @Deprecated
    public static String getToken() throws DXRiskErrorException {
        return getToken(null, null);
    }

    /**
     * 获取token
     * @param appId
     * @return
     * @throws DXRiskErrorException
     */
    public static String getToken(String appId) throws DXRiskErrorException {
        return getToken(appId, null);
    }

    /**
     * 获取token
     * @param appId
     * @param paramsMap
     * @return
     * @throws DXRiskErrorException
     */
    public static String getToken(String appId, HashMap<String, String> paramsMap) throws DXRiskErrorException {

        try {
            return DXRiskInternal.getToken(appId, paramsMap);
        } catch (LinkageError e) {
            ReLinkerErrorHandler.handleGetTokenError(ContextFinder.getApplication(), e);
        }

        return "";
    }

    /**
     * 获取采集字段较少的token
     * @return
     * @throws DXRiskErrorException
     */
    public static String getLightToken(String appId, HashMap<String, String> paramsMap) throws DXRiskErrorException {
        return DXRiskInternal.getLightToken(appId, paramsMap);
    }

    /**
     * 释放资源， 防止内存泄漏
     * FIXME 不同Appid的getToken情况下，调用release会有问题
     */
//    public static void release(){
//        ContextFinder.release();
//    }

}
