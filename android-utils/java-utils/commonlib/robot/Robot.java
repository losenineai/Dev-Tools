package com.auric.intell.commonlib.robot;

import android.text.TextUtils;
import android.util.Base64;
import android.util.Log;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.auric.intell.commonlib.utils.AppUtil;
import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.DeviceUtils;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.SPUtils;
import com.auric.intell.commonlib.utils.StringUtil;
import com.auric.intell.commonlib.utils.net.NetWorkUtil;


/**
 * Created by zhangxiliang on 2017/4/26.
 */

public class Robot {
    private static final String TAG = "Robot";

    public static final String KEY_TOKEN_ROBOT = "token_robot";
    public static final String KEY_NIM_ACCID="nim_accid";
    public static final String KEY_NIM_PASSWD="Nim_passwd";
    private static final String KEY_TOKEN_EXP_KEY = "exp";
    private static final String KEY_TOKEN_NBF_KEY = "nbf";
//    public static final String KEY_BIND_GUID="bind_guid";
    public static final String KEY_REGISTERED="registered";
    public static final String KEY_APP_VESION_NAME="KEY_APP_VESION_NAME";

    public static  final String KEY_BOOLEAN_PRESET_INIT="key_boolean_preset_init";
    public static  final String KEY_BOOLEAN_FIRST_GUID_NET_SUCCESS = "KEY_BOOLEAN_FIRST_GUID_NET_SUCCESS";

    private static final long TOKNE_SUB_TIME = 60l;

    public static String sBaseUrl = "https://lapi.oduola.com";
    public static void init(String baseUrl){
        sBaseUrl = baseUrl;
    }

    /**
     * 32231100010001	40:ed:98:80:00:14
     32231100010002	40:ed:98:80:00:15
     32231100010003	40:ed:98:80:00:16
     32231100010004	40:ed:98:80:00:17
     32231100010005	40:ed:98:80:00:18
     32231100010006	40:ed:98:80:00:19
     32231100010007	40:ed:98:80:00:1a
     32231100010008	40:ed:98:80:00:1b
     32231100010009	40:ed:98:80:00:1c
     32231100010010	40:ed:98:80:00:1d

     * @return
     */
    public static RobotInfo getInfo() {

        RobotInfo robotInfo = new RobotInfo();

        String serial_no = DeviceUtils.getIMEI(ContextFinder.getApplication());
        String wifiMac = DeviceUtils.getMacAddress(ContextFinder.getApplication());
        String blueMac = DeviceUtils.getBlueAddress();
        String totalMemery = DeviceUtils.getTotalInternalMemorySize() + "";
        String freeMemery = DeviceUtils.getAvailableInternalMemorySize() + "";

        String androidVersion = DeviceUtils.getSDKVersion() + "";
        String osVersion = AppUtil.getAPPVersionNameFromAPP(ContextFinder.getApplication());
        String model = "ZN322311";
//        String guid=(String)SPUtils.get(KEY_BIND_GUID,"");

        robotInfo.setModel(model);
        robotInfo.setOs_version(osVersion);
        robotInfo.setAndroid_version(androidVersion);
        robotInfo.setBlueMac(blueMac);
        robotInfo.setSerial_no(serial_no);
        robotInfo.setWifiMac(wifiMac);


        robotInfo.setFree_flash_space(freeMemery);
        robotInfo.setTotal_flash_space(totalMemery);


        return robotInfo;
    }


    /**
     * 是否已经注册
     * @return
     */
    public static boolean isRegistered(){
        boolean hasNim = !TextUtils.isEmpty(SPUtils.get(KEY_NIM_ACCID, "").toString())
        && !TextUtils.isEmpty(SPUtils.get(KEY_NIM_PASSWD, "").toString());

        LogUtils.d("hasNim:"+hasNim);
        return (Boolean) SPUtils.get(KEY_REGISTERED, false) && hasNim;
    }

    /**
     * 是否第一引导联网成功
     * @return
     */
    public static boolean isFirstGuideNetSuccess() {
        return (Boolean) SPUtils.get(KEY_BOOLEAN_FIRST_GUID_NET_SUCCESS, false);
    }

    /**
     * 设置第一次引导联网成功
     */
    public static void setFirstGuideNetSuccess() {
        SPUtils.put(KEY_BOOLEAN_FIRST_GUID_NET_SUCCESS, true);
    }

//    public static String getToken() {
//        LogUtils.d("");
//        if (!Robot.isRegistered()) {
//            return "";
//        }
//        String token = (String) SPUtils.get(KEY_TOKEN_ROBOT, "");
//
//        // 无网络
//        if (!NetWorkUtil.isConnected(ContextFinder.getApplication())) {
//            LogUtils.d("无网络");
//            return token;
//        }
//        // token失效
//        else if (isTokenExp(token)) {
//            LogUtils.d("失效");
//            String newToken = TokenEngine.getInstance().refreshToken(token);
//            return newToken;
//        }
//        // 其他
//        else {
//            LogUtils.d("本地缓存");
//            return token;
//        }
//
//    }

    private static boolean isTokenExp(String token) {
        if(StringUtil.isEmpty(token)){
            return true;
        }
        try {
            String calamString = new String(Base64.decode(token.split("\\.")[1], Base64.DEFAULT));
            Log.d(TAG, calamString);
            JSONObject jsonObject = JSON.parseObject(calamString);
            long expTime = Long.parseLong(jsonObject.get(KEY_TOKEN_EXP_KEY).toString());
            long nbfTime = Long.parseLong(jsonObject.get(KEY_TOKEN_NBF_KEY).toString());
            long currTime = System.currentTimeMillis() / 1000;
            // token没过期
            if (currTime < expTime - TOKNE_SUB_TIME) {
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }


        return true;
    }


}
