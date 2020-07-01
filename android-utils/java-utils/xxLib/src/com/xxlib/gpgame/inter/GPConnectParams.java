package com.xxlib.gpgame.inter;

/**
 * Created by rosejames on 16/5/25.
 */
public class GPConnectParams {

    /**
     * 定义Key
     */
    public static final String INTENT_KEY_TYPE = "type";
    public static final String INTENT_KEY_BANNER_ID = "banner_id";
    public static final String INTENT_KEY_PAGE_NAME = "page_name";
    public static final String INTENT_KEY_PID = "pid";
    public static final String INTENT_KEY_UIN = "uin";
    public static final String INTENT_KEY_LOGIN_KEY = "loginkey";
    public static final String INTENT_KEY_APP_ID = "appid";
    public static final String INTENT_KEY_EX_JSON = "page_params";
    public static final String INTENT_KEY_USER_NAME = "username";

    /**
     * 定义跳转类型
     */
    public static final int CONNECT_TYPE_NONE = 0;      //无操作，到主界面
    public static final int CONNECT_TYPE_BANNER = 1;    //用banner_id进行获取
    public static final int CONNECT_TYPE_PAGE = 2;      //去到具体页面
    public static final int CONNECT_TYPE_RECHARGE = 3;  //去充值

    /**
     * 必填参数
     */
    private int mConnectType;           //连接到果盘游戏的类型 CONNECT_TYPE_NONE CONNECT_TYPE_BANNER CONNECT_TYPE_PAGE CONNECT_TYPE_RECHARGE
    private int mPid;                   //源头App的产品ID


    private long mUin;                  //用户Uin
    private String mLoginKey = "";      //用户LoginKey
    private String mAppId = "";         //如果从sdk跳转，需要填写sdk的appId
    private String mUserName = "";       //用户名
    private int mBannerId;              //type = CONNECT_TYPE_BANNER 需要填写的参数
    private String mPageName = "";      //type = CONNECT_TYPE_PAGE 需要填写的参数
    private String mExJson = "";        //type = CONNECT_TYPE_PAGE 会读取的参数（非必填）


    public int getConnectType() {
        return mConnectType;
    }

    public GPConnectParams setConnectType(int connectType) {
        mConnectType = connectType;
        return this;
    }

    public int getPid() {
        return mPid;
    }

    public GPConnectParams setPid(int pid) {
        mPid = pid;
        return this;
    }

    public long getUin() {
        return mUin;
    }

    public GPConnectParams setUin(long uin) {
        mUin = uin;
        return this;
    }

    public String getLoginKey() {
        return mLoginKey;
    }

    public GPConnectParams setLoginKey(String loginKey) {
        mLoginKey = loginKey;
        return this;
    }

    public String getAppId() {
        return mAppId;
    }

    public GPConnectParams setAppId(String appId) {
        mAppId = appId;
        return this;
    }

    public int getBannerId() {
        return mBannerId;
    }

    public GPConnectParams setBannerId(int bannerId) {
        mBannerId = bannerId;
        return this;
    }

    public String getPageName() {
        return mPageName;
    }

    public GPConnectParams setPageName(String pageName) {
        mPageName = pageName;
        return this;
    }

    public String getExJson() {
        return mExJson;
    }

    public GPConnectParams setExJson(String exJson) {
        mExJson = exJson;
        return this;
    }

    public String getUserName() {
        return mUserName;
    }

    public GPConnectParams setUserName(String userName) {
        mUserName = userName;
        return this;
    }
}
