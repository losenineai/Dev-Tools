package com.dx.mobile.risk;

import android.content.Context;
import android.os.Build;
import android.os.Looper;
import android.util.Log;

import com.dx.mobile.annotation.JXC_RISK;
import com.dx.mobile.risk.utils.StringUtils;

import java.util.HashMap;

/**
 * Created by blue on 2017/10/18.
 */

public class DXRiskInternal {

    private static final String TAG = "DXRisk";

    private static long mCookie = -1;
    private static volatile boolean mIsSetup = false;
    private static String mAppId;
    private static HashMap<String, String> mParamsMap;

    public static void loadLibrary() {
        System.loadLibrary("DXRiskComm-" + BuildConfig.VERSION_NAME);
    }

    private DXRiskInternal() {
    }

    /**
     * 数据初始化
     * @return
     */
    public static boolean setup(final Context context, String appId, HashMap<String, String> paramsMap) {

        mAppId = appId;
        mParamsMap = paramsMap;

        if (mIsSetup) {
            return true;
        }

        // TODO verify so 暂时不开放，因为耗时超过80ms
//        if (!c.a()) {
//            throw new DXRiskErrorException("DxRiskComm-" + BuildConfig.VERSION_NAME + ".so is illegal.");
//        }

        // instance DXRisk
        mCookie = createObjectNative();
        if (mCookie == -1) {
            throw new DXRiskErrorException("setup error.");
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                long time = System.currentTimeMillis();
                setupNative(mCookie, context);
                Log.i(TAG, "sncost: " + (System.currentTimeMillis() - time));
            }
        }).start();

        mIsSetup = true;

        return mIsSetup;
    }

    /**
     * @return
     * @throws DXRiskErrorException
     */
    public static String getToken(String appId, HashMap<String, String> paramsMap) throws DXRiskErrorException {
        if (StringUtils.isEmpty(appId)) {
            appId = mAppId;
        }
        if (paramsMap == null) {
            paramsMap = mParamsMap;
        }
        return getTokenInternal(appId, paramsMap, true);
    }

    /**
     * @return
     * @throws DXRiskErrorException
     */
    @JXC_RISK
    public static String getLightToken(String appId, HashMap<String, String> paramsMap) throws DXRiskErrorException {
        if (StringUtils.isEmpty(appId)) {
            appId = mAppId;
        }
        if (paramsMap == null) {
            paramsMap = mParamsMap;
        }
        return getTokenInternal(appId, paramsMap, false);
    }

    /**
     * @param appId 调用者唯一标识
     * @param paramsMap token额外参数，用于token预加载
     * @return
     * @throws DXRiskErrorException
     */
    @JXC_RISK
    private static String getTokenInternal(String appId, HashMap<String, String> paramsMap, boolean bGetFullToken) throws DXRiskErrorException {

        if (!mIsSetup) {
            throw new DXRiskErrorException("Before call SDK getToken, Must call setup method!");
        }


        if (StringUtils.isEmpty(appId)) {
            throw new DXRiskErrorException("Parameters appId can not be null from getToken method.");
        }

        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH)
            throw new DXRiskErrorException("SDK version must bigger the 14");

        // 平台相关的检查
        {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.CUPCAKE &&
                    Looper.getMainLooper().getThread() == Thread.currentThread()) {
                throw new DXRiskErrorException("getToken must be called in no-UI thread!");
            }

            if (mCookie == -1) {
                throw new DXRiskErrorException("DXRisk init error");
            }
        }

        // 检测 KEY_DELAY_MS_TIME 合法性
        if (paramsMap != null && paramsMap.containsKey(DXRisk.KEY_DELAY_MS_TIME)) {
            try {
                Integer.parseInt(paramsMap.get(DXRisk.KEY_DELAY_MS_TIME));
            } catch (Exception e) {
                throw new DXRiskErrorException("Value of DXRisk.KEY_DELAY_MS_TIME must be int value");
            }
        }

        String token;

        synchronized (DXRiskInternal.class) {
            try {
                if (paramsMap == null) {
                    paramsMap = new HashMap<>();
                }
                token = getTokenNative(mCookie, appId, paramsMap, bGetFullToken);
            } catch (Throwable e) {
                throw new DXRiskErrorException("Native Exception", e);
            }
        }

        return token;
    }

    private static native long createObjectNative();

    private static native void setupNative(long id, Context context);

    private static native String getTokenNative(long id, String appId, HashMap<String, String> paramsMap, boolean bGetFullToken);

}
