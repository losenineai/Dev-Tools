package com.xxlib.gpgame.inter;

import android.content.ActivityNotFoundException;
import android.content.ComponentName;
import android.content.Intent;
import android.net.Uri;

import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.StringUtils;
import com.xxlib.utils.UrlUtils;
import com.xxlib.utils.XXLibJobExecutor;
import com.xxlib.utils.base.Base64;
import com.xxlib.utils.base.XXTea;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by rosejames on 16/5/25.
 */
public class GPConnectManager {

    public static final String GP_SCHEME = "gpgame://";
    private boolean mIsUseScheme = true;

    public static final String INTENT_KEY_IS_NOT_FROM_SCHEME = "INTENT_KEY_IS_NOT_FROM_SCHEME";
    public static final String INTENT_KEY_JSON_PARAMS = "INTENT_KEY_JSON_PARAMS";

    /**
     * @param connectParams
     * @param callback      回调方法
     */
    public void invoke(final GPConnectParams connectParams, final IGPConnectCallback callback) {
        XXLibJobExecutor.getExecutor().execute(new Runnable() {
            @Override
            public void run() {
                GPConnectResult result = new GPConnectResult();
                if (checkLeagel(connectParams)) {
                    try {
                        //构造参数
                        String paramsStr = buildParamsStr(connectParams);
                        paramsStr = Base64.encode(XXTea.XXTeaEncrypt(paramsStr.getBytes(), paramsStr.getBytes().length, UrlUtils.KEY.getBytes()));
                        if (mIsUseScheme) {
                            //构造Scheme Uri
                            Uri uri = Uri.parse(GP_SCHEME + paramsStr);
                            //启动Activity
                            Intent intent = new Intent(Intent.ACTION_VIEW, uri);
                            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            ApplicationUtils.getApplication().startActivity(intent);
                        } else {
                            Intent intent = new Intent();
                            ComponentName cn = new ComponentName("com.flamingo.gpgame", "com.flamingo.gpgame.view.activity.GPGameConnectActivity");
                            intent.setComponent(cn);
                            intent.putExtra(INTENT_KEY_IS_NOT_FROM_SCHEME, true);
                            intent.putExtra(INTENT_KEY_JSON_PARAMS, paramsStr);
                            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                            ApplicationUtils.getApplication().startActivity(intent);
                        }
                        result.setErrorCode(GPConnectResult.GP_CONNECT_RESULT_SUC);
                    } catch (Exception e) {
                        if (e instanceof JSONException) {
                            result.setErrorCode(GPConnectResult.GP_CONNECT_RESULT_FAILED_JSON_EXCEPTION);
                        } else if (e instanceof ActivityNotFoundException) {
                            result.setErrorCode(GPConnectResult.GP_CONNECT_RESULT_FAILED_START_NO_SCHEME);
                        } else {
                            result.setErrorCode(GPConnectResult.GP_CONNECT_RESULT_FAILED_UNKNOWN);
                        }
                        if (e != null)
                            result.setErrMessage(e.getMessage());
                    }
                } else {
                    result.setErrorCode(GPConnectResult.GP_CONNECT_RESULT_FAILED_PARAMS_ILLEGAL);
                }
                if (callback != null) {
                    callback.onFinish(result);
                }
            }
        });
    }


    /**
     * 检查跳转参数是否合法
     *
     * @param connectParams
     * @return
     */
    private boolean checkLeagel(GPConnectParams connectParams) {

        return true;
    }

    /**
     * 构建传递的参数
     *
     * @param connectParams
     * @return
     * @throws JSONException
     */
    public String buildParamsStr(GPConnectParams connectParams) throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(GPConnectParams.INTENT_KEY_TYPE, connectParams.getConnectType());
        if (connectParams.getBannerId() != 0) {
            jsonObject.put(GPConnectParams.INTENT_KEY_BANNER_ID, connectParams.getBannerId());
        }
        if (!StringUtils.isEmpty(connectParams.getPageName())) {
            jsonObject.put(GPConnectParams.INTENT_KEY_PAGE_NAME, connectParams.getPageName());
        }
        if (!StringUtils.isEmpty(connectParams.getExJson())) {
            jsonObject.put(GPConnectParams.INTENT_KEY_EX_JSON, connectParams.getExJson());
        }
        if (connectParams.getPid() != 0) {
            jsonObject.put(GPConnectParams.INTENT_KEY_PID, connectParams.getPid());
        }
        if (connectParams.getUin() != 0) {
            jsonObject.put(GPConnectParams.INTENT_KEY_UIN, connectParams.getUin());
        }
        if (!StringUtils.isEmpty(connectParams.getLoginKey())) {
            jsonObject.put(GPConnectParams.INTENT_KEY_LOGIN_KEY, connectParams.getLoginKey());
        }
        if (!StringUtils.isEmpty(connectParams.getAppId())) {
            jsonObject.put(GPConnectParams.INTENT_KEY_APP_ID, connectParams.getAppId());
        }
        if (!StringUtils.isEmpty(connectParams.getUserName())) {
            jsonObject.put(GPConnectParams.INTENT_KEY_USER_NAME, connectParams.getUserName());
        }
        return jsonObject.toString();
    }

    public GPConnectParams buildParams(String paramsJson) throws JSONException {
        GPConnectParams gpConnectParams = new GPConnectParams();
        JSONObject jsonObject = new JSONObject(paramsJson);
        gpConnectParams.setConnectType(jsonObject.getInt(GPConnectParams.INTENT_KEY_TYPE));
        if (jsonObject.has(GPConnectParams.INTENT_KEY_BANNER_ID)) {
            gpConnectParams.setBannerId(jsonObject.getInt(GPConnectParams.INTENT_KEY_BANNER_ID));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_PAGE_NAME)) {
            gpConnectParams.setPageName(jsonObject.getString(GPConnectParams.INTENT_KEY_PAGE_NAME));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_PID)) {
            gpConnectParams.setPid(jsonObject.getInt(GPConnectParams.INTENT_KEY_PID));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_UIN)) {
            gpConnectParams.setUin(jsonObject.getLong(GPConnectParams.INTENT_KEY_UIN));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_LOGIN_KEY)) {
            gpConnectParams.setLoginKey(jsonObject.getString(GPConnectParams.INTENT_KEY_LOGIN_KEY));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_APP_ID)) {
            gpConnectParams.setAppId(jsonObject.getString(GPConnectParams.INTENT_KEY_APP_ID));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_EX_JSON)) {
            gpConnectParams.setExJson(jsonObject.getString(GPConnectParams.INTENT_KEY_EX_JSON));
        }
        if (jsonObject.has(GPConnectParams.INTENT_KEY_USER_NAME)) {
            gpConnectParams.setUserName(jsonObject.getString(GPConnectParams.INTENT_KEY_USER_NAME));
        }
        return gpConnectParams;
    }

}
