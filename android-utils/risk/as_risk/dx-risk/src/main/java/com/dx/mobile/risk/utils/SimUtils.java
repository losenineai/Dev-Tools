package com.dx.mobile.risk.utils;

import android.Manifest;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

/**
 * Created by amlloc on 17-12-13.
 */

public class SimUtils {

    private static final String TAG = "DXRisk";

    public final static int SIM_CARD1 = 0;
    public final static int SIM_CARD2 = 1;
    public final static int SIM_CARD3 = 2;

//    private static final String GET_NETWORK_OPERATORNAME = "getNetworkOperatorName";
//    private static final String GET_SIMSTATE = "getSimState";
//    private static final String GET_SUBID = "getSubId";
//    private static final String GET_SUBSCRIBER_ID = "getSubscriberId";
//    private static final String GET_DEVICE_ID = "getDeviceId";
//    private static final String GET_IMEI = "getImei";
//    private static final String GET_SIMSERIAL_NUMBER = "getSimSerialNumber";
//    private static final String GET_PHONE_COUNT = "getPhoneCount";

    /*获取当前手机sim卡对应的网络类型*/
    /*需要权限ACCESS_NETWORK_STATE*/
    private static final int NETWORK_TYPE_UNAVAILABLE = -1;
    // private static final int NETWORK_TYPE_MOBILE = -100;
    private static final int NETWORK_TYPE_WIFI = -101;

    private static final int NETWORK_CLASS_WIFI = -101;
    private static final int NETWORK_CLASS_UNAVAILABLE = -1;
    /**
     * Unknown network class.
     */
    private static final int NETWORK_CLASS_UNKNOWN = 0;
    /**
     * Class of broadly defined "2G" networks.
     */
    private static final int NETWORK_CLASS_2_G = 1;
    /**
     * Class of broadly defined "3G" networks.
     */
    private static final int NETWORK_CLASS_3_G = 2;
    /**
     * Class of broadly defined "4G" networks.
     */
    private static final int NETWORK_CLASS_4_G = 3;


    // 可以适配低版本手机
    /**
     * Network type is unknown
     */
    public static final int NETWORK_TYPE_UNKNOWN = 0;
    /**
     * Current network is GPRS
     */
    public static final int NETWORK_TYPE_GPRS = 1;
    /**
     * Current network is EDGE
     */
    public static final int NETWORK_TYPE_EDGE = 2;
    /**
     * Current network is UMTS
     */
    public static final int NETWORK_TYPE_UMTS = 3;
    /**
     * Current network is CDMA: Either IS95A or IS95B
     */
    public static final int NETWORK_TYPE_CDMA = 4;
    /**
     * Current network is EVDO revision 0
     */
    public static final int NETWORK_TYPE_EVDO_0 = 5;
    /**
     * Current network is EVDO revision A
     */
    public static final int NETWORK_TYPE_EVDO_A = 6;
    /**
     * Current network is 1xRTT
     */
    public static final int NETWORK_TYPE_1xRTT = 7;
    /**
     * Current network is HSDPA
     */
    public static final int NETWORK_TYPE_HSDPA = 8;
    /**
     * Current network is HSUPA
     */
    public static final int NETWORK_TYPE_HSUPA = 9;
    /**
     * Current network is HSPA
     */
    public static final int NETWORK_TYPE_HSPA = 10;
    /**
     * Current network is iDen
     */
    public static final int NETWORK_TYPE_IDEN = 11;
    /**
     * Current network is EVDO revision B
     */
    public static final int NETWORK_TYPE_EVDO_B = 12;
    /**
     * Current network is LTE
     */
    public static final int NETWORK_TYPE_LTE = 13;
    /**
     * Current network is eHRPD
     */
    public static final int NETWORK_TYPE_EHRPD = 14;
    /**
     * Current network is HSPA+
     */
    public static final int NETWORK_TYPE_HSPAP = 15;

    private static TelephonyManager teleMgr = null;
    private static Object tele2Mgr = null;
    private static Object teleMsimMgr = null;

    public static void init(TelephonyManager mgr) {
        teleMgr = mgr;

        try {
            tele2Mgr = ContextFinder.getApplication().getSystemService("phone2");
        } catch (Throwable e) { }

        try {
            Class.forName("android.telephony.MSimTelephonyManager");
            teleMsimMgr = ContextFinder.getApplication().getSystemService("phone_msim");
        } catch (Throwable e) { }

    }

    public static Object invokeMethodWithParam(Object obj, String method, int param) {

        if(obj == null)
            return null;

        try {

            Class<?> telePhonyClass = Class.forName(obj.getClass().getName());
            Class<?>[] paramter = new Class[1];
            paramter[0] = int.class;
            Method getMethod = telePhonyClass.getMethod(method, paramter);

            Object objPhone[] = new Object[1];
            objPhone[0] = param;
            Object objResult = getMethod.invoke(obj, objPhone);

            return objResult;

        } catch (Exception e) {
//            e.printStackTrace();
        }

        return null;
    }

    public static Object invokeMethod(Object obj, String method) {

        if(obj == null)
            return null;

        try {

            Class<?> telePhonyClass = Class.forName(obj.getClass().getName());
            Method getMethod = telePhonyClass.getMethod(method);

            Object objResult = getMethod.invoke(obj);

            return objResult;

        } catch (Exception e) {
//            e.printStackTrace();
        }

        return null;
    }

    //执行TelephonyManager类里带参数的方法
    public static Object invokeTeleMgrMethodWithParam(String method, int param) {

        return invokeMethodWithParam(teleMgr, method, param);

    }

    //执行TelephonyManager类里不带参数的方法
    public static Object invokeTeleMgrMethod(String method) {

        return invokeMethod(teleMgr, method);
    }

    //执行MsimTelephonyManager类里带参数的方法
    public static Object invokeTeleMsimMgrMethodWithParam(String method, int param) {

        return invokeMethodWithParam(teleMsimMgr, method, param);
    }

    //执行MsimTelephonyManager类里带参数的方法
    public static Object invokeTele2MgrMethod(String method) {

        return invokeMethod(tele2Mgr, method);
    }

    /*执行SubscriptionManager类里带int参数的方法
     *
     * 利用反射机制找到SubscriptionManager的含参构造函数，并传入context作为形参进行实例化
     * 这样避免import SubscriptionManager，从而达到消除5.0以下低版本错误警告的目的
     *
     * @param Method 方法名 param 参数
     * @return 调用方法后得到的结果
     */
    public static Object invokeSubscriptionMgrMethod(String method, int param) {

        Context cxt = ContextFinder.getApplication();

        try {

            Class<?> subscriptionManagerClass = Class.forName("android.telephony.SubscriptionManager");
            Constructor constructor = subscriptionManagerClass.getConstructor(Context.class);
            Object subscriptionManagerObj = constructor.newInstance(cxt);

            return invokeMethodWithParam(subscriptionManagerObj, method, param);

        } catch (Exception e) {
//            e.printStackTrace();
        }

        return null;
    }

    //获取sloIdx号对应Sim卡的状态，与卡槽无关
    @SuppressWarnings("deprecation")//表示不显示使用了不赞成使用的类或方法时的警告
    public static boolean getTelephonyStateBySloIdx(int slotIdx) {

        boolean isReady = false;
        int simState;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            simState = (slotIdx == 0) ? (int) invokeTeleMgrMethod("getSimState") : TelephonyManager.SIM_STATE_UNKNOWN;
        } else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            simState = (int) invokeTeleMgrMethodWithParam("getSimState", slotIdx);
        } else {
            simState = (int) invokeTeleMgrMethod("getSimState");
        }

        if (simState != TelephonyManager.SIM_STATE_ABSENT && (simState) != TelephonyManager.SIM_STATE_UNKNOWN) {
            isReady = true;
        }

        return isReady;
    }

    //注意这个函数只能用于Android5.0以上
    @SuppressWarnings("deprecation")
    public static int getSubidBySlotId(int slotIdx) {

        try {

            Object objResult = invokeSubscriptionMgrMethod("getSubId", slotIdx);

            if (objResult != null) {
                return ((int[]) objResult)[0];
            }

        } catch (Exception e) {
//            e.printStackTrace();
        }

        return -1;
    }

    /*
    @return   返回sim卡对应运营商名称
              此函数通过imsi号进行获取，好处在于输出稳定的字段：
              中国移动，中国联通，中国电信
    @worning  除此三大运营商外，其他运营商还待扩充
     */
    @SuppressWarnings("deprecation")
    public static String getOperatorName(int slotIdx) {

        String imsi = getSubscriberid(slotIdx);

        if (StringUtils.isEmpty(imsi)) {
            return "";
        }

        return verifyOperatorNameByImsi(imsi);
    }

    //根据imsi获取对应运营商名称
    public static String verifyOperatorNameByImsi(String imsi) {

        String operatorName = "";
        if (StringUtils.isEmpty(imsi)) {

            if (imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007")) {

                operatorName = "移动";

            } else if (imsi.startsWith("46001") || imsi.startsWith("46006")) {

                //中国联通
                operatorName = "联通";
            } else if (imsi.startsWith("46003") || imsi.startsWith("46005")) {

                //中国电信
                operatorName = "电信";

            } else if (imsi.startsWith("46020")) {

                operatorName = "铁通";
            }

        }
        return operatorName;
    }

    @SuppressWarnings("deprecation")
    public static String getDeviceId(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String deviceId;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            //这里不用getTelephonyStateBySloId取判断是因为devicesid与卡槽绑定而不是与sim卡绑定
            deviceId = (slotIdx == 0) ? (String) invokeTeleMgrMethod("getDeviceId") : "";
        } else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            deviceId = (String) invokeTeleMgrMethodWithParam("getDeviceId", slotIdx);
        } else {
            //Android API 26 以后获取Imei号来作为DevicesId
            deviceId = (String) invokeTeleMgrMethodWithParam("getImei", slotIdx);
        }

        return deviceId;
    }

    @SuppressWarnings("deprecation")
    public static String getSubscriberid(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String subscriberid = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {

            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                subscriberid = (slotIdx == 0) ? (String) invokeTeleMgrMethod("getSubscriberId") : "";
            } else {
                subscriberid = (String) invokeTeleMgrMethodWithParam("getSubscriberId", getSubidBySlotId(slotIdx));
            }
        }

        return subscriberid;
    }

    //获取Imsi号
    public static String getSimSerialNumber(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String simSerialNumber = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {

            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                simSerialNumber = (slotIdx == 0) ? (String) invokeTeleMgrMethod("getSimSerialNumber") : "";
            } else {
                simSerialNumber = (String) invokeTeleMgrMethodWithParam("getSimSerialNumber", getSubidBySlotId(slotIdx));
            }
        }

        return simSerialNumber;

    }

    /* annotation for google grounding - by blue 2018.10.31
    // 获取电话号码
    public static String getPhoneNumber() {

        Context context = ContextFinder.getApplication();

        TelephonyManager telephonyManager = (TelephonyManager)
                context.getSystemService(Context.TELEPHONY_SERVICE);

        if (PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE))
            return telephonyManager.getLine1Number();
        else
            return "";
    }

    // 获取语音电话号码
    public static String getVoiceMailNumber() {

        Context context = ContextFinder.getApplication();

        TelephonyManager telephonyManager = (TelephonyManager)
                context.getSystemService(Context.TELEPHONY_SERVICE);

        if (PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE))
            return telephonyManager.getVoiceMailNumber();
        else
            return "";
    }
    */

    //返回Sim卡数量，如果失败，则返回-1
    @SuppressWarnings("deprecation")
    public static int getSimCardCount() {
        //Android 23以上可以使用
        int phoneCount = -1;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            phoneCount = (int) invokeTeleMgrMethod("getPhoneCount");
        }

        return phoneCount;
    }

    /*网络状态方法调用入口*/
    // 有Sim卡且连接状态为数据连接时，则获取对应Sim卡的网络状态
    //连接状态为wifi时则获取wifi连接状态
    public static String getCurrentNetworkType(int slotId) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.ACCESS_NETWORK_STATE)) {
            Log.d("permission:", "ACCESS_NETWORK_STATE deny");
            return "";
        }

        String type = "unknown";

        switch (getNetworkClass(slotId)) {
            //不提供服务
            case NETWORK_CLASS_UNAVAILABLE:
                type = "unavailable";
                break;
            case NETWORK_CLASS_WIFI:
                type = "Wi-Fi";
                break;
            case NETWORK_CLASS_2_G:
                type = "2G";
                break;
            case NETWORK_CLASS_3_G:
                type = "3G";
                break;
            case NETWORK_CLASS_4_G:
                type = "4G";
                break;
            //不明网络类型
            case NETWORK_CLASS_UNKNOWN:
                type = "unknown";
                break;
        }

        return type;
    }

    //根据网络信息获取网络类型
    private static int getNetworkClassByType(int networkType) {

        switch (networkType) {
            case NETWORK_TYPE_UNAVAILABLE:
                return NETWORK_CLASS_UNAVAILABLE;
            case NETWORK_TYPE_WIFI:
                return NETWORK_CLASS_WIFI;
            case NETWORK_TYPE_GPRS:
            case NETWORK_TYPE_EDGE:
            case NETWORK_TYPE_CDMA:
            case NETWORK_TYPE_1xRTT:
            case NETWORK_TYPE_IDEN:
                return NETWORK_CLASS_2_G;
            case NETWORK_TYPE_UMTS:
            case NETWORK_TYPE_EVDO_0:
            case NETWORK_TYPE_EVDO_A:
            case NETWORK_TYPE_HSDPA:
            case NETWORK_TYPE_HSUPA:
            case NETWORK_TYPE_HSPA:
            case NETWORK_TYPE_EVDO_B:
            case NETWORK_TYPE_EHRPD:
            case NETWORK_TYPE_HSPAP:
                return NETWORK_CLASS_3_G;
            case NETWORK_TYPE_LTE:
                return NETWORK_CLASS_4_G;
            default:
                return NETWORK_CLASS_UNKNOWN;
        }
    }

    //调用系统函数获取网络连接状态相关信息
    private static int getNetworkClass(int slotId) {

        Context cxt = ContextFinder.getApplication();
        int networkType = NETWORK_TYPE_UNKNOWN;

        try {

            if(!PermissionUtils.checkPermissions(Manifest.permission.ACCESS_NETWORK_STATE))
                return NETWORK_CLASS_UNAVAILABLE;

            final NetworkInfo network = ((ConnectivityManager) cxt.getApplicationContext()
                    .getSystemService(Context.CONNECTIVITY_SERVICE))
                    .getActiveNetworkInfo();

            if (network != null && network.isAvailable() && network.isConnected()) {

                int type = network.getType();

                if (type == ConnectivityManager.TYPE_WIFI) {
                    networkType = NETWORK_TYPE_WIFI;
                } else if (type == ConnectivityManager.TYPE_MOBILE && getTelephonyStateBySloIdx(slotId)) {

                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        networkType = (int) invokeTeleMgrMethodWithParam("getDataNetworkType", getSubidBySlotId(slotId));
                    } else {
                        networkType = teleMgr.getNetworkType();
                    }
                }
            } else{
                networkType = NETWORK_TYPE_UNAVAILABLE;
            }

        } catch(Exception ex) { }

        return getNetworkClassByType(networkType);
    }

    //获取imei号
    public static String getTele2DeviceId() {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        return (String) invokeTele2MgrMethod("getDeviceId");
    }

    //获取imei号
    public static String getMsimDeviceId(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String msimDeviceId = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {
            msimDeviceId = (String) invokeTeleMsimMgrMethodWithParam("getDeviceId", getSubidBySlotId(slotIdx));
        }

        return msimDeviceId;
    }

    //获取imei号
    public static String getDeviceIdGemini(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String deviceIdGemini = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {
            deviceIdGemini = (String) invokeTeleMgrMethodWithParam("getDeviceIdGemini", getSubidBySlotId(slotIdx));
        }

        return deviceIdGemini;
    }

    //获取imei号
    public static String getDefault(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String deviceIdGemini = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {
            deviceIdGemini = (String) invokeTeleMgrMethodWithParam("getDefault", getSubidBySlotId(slotIdx));
        }

        return deviceIdGemini;
    }

    //获取imei号
    public static String getTele2SubscriberIdId() {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        return (String) invokeTele2MgrMethod("getSubscriberId");
    }

    //获取imsi号
    public static String getMsimSubscriberId(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String subscriberId = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {
            subscriberId = (String) invokeTeleMsimMgrMethodWithParam("getSubscriberId", getSubidBySlotId(slotIdx));
        }

        return subscriberId;
    }

    //获取imsi号
    public static String getSubscriberIdGemini(int slotIdx) {

        if (!PermissionUtils.checkPermissions(Manifest.permission.READ_PHONE_STATE)) {
            return "";
        }

        String subIdGemini = "";

        if (getTelephonyStateBySloIdx(slotIdx)) {
            subIdGemini = (String) invokeTeleMgrMethodWithParam("getSubscriberIdGemini", getSubidBySlotId(slotIdx));
        }

        return subIdGemini;
    }

}
