package com.multilaunch;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.NotificationCompat;
import android.text.TextUtils;

import com.apktool.ApktoolManager;
import com.apktool.ApktoolUpdate;
import com.apktool.LoadApktoolResult;
import com.apktool.access.IMultiApkHelper;
import com.apktool.access.ProgressListener;
import com.xxlib.R;
import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.FileUtils;
import com.xxlib.utils.InstallUtils;
import com.xxlib.utils.SPCenter.SPCenter;
import com.xxlib.utils.ToastUtils;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.ShellTool;
import com.xxlib.utils.floatview.CheckAppRunning;

import org.json.JSONObject;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * 管理多开应用
 * Created by chenrenzhan on 15/12/1.
 */
public class MultiLaunchManager {

    private static final String TAG = "MultiLaunchManager";

    public final static String MULTI_LAUNCH_PKG_NAME = "multi_launch_pkg_name";
    public final static String MULTI_LAUNCH_APK_PATH = "multi_launch_apk_path";
    public final static String MULTI_LAUNCH_APK_INSTALL_START_STAMP = "multi_launch_apk_install_start_stamp"; // APP多开复制完成开始安装的时间戳
    public final static long MAX_INSTALL_TIME = 5 * 60 * 1000; //最大的安装时间，超过这一时间则视为安装失败，重置状态

    private final static int HANDLE_SUCCESS = 2001;
    private final static int HANDLE_FAILED = 2002;

    private static MultiLaunchManager mSelf;
    private LoadApktoolResult mLoadApktoolResult = null;

    private boolean mIsCopying = false;
    private String mCurCopyPkgName = null;
    private String mCurCopyAppName = null;
    private String mSrcAppName = null;  // 被多开的应用名
    private String mTempPkgName = null; // 由于安装多开成功后，会清楚多开相关的数据，所以临时保存多开的包名用于安装成功后的比较
    private long mInstallStartStamp = -1; // 复制完成时间戳
    private boolean mIsEnableSpeed = false;

    private MultiLaunchItemData.CopyState mCopyState;
    private int mCurProgress = 0;

    private List<MultiLaunchItemData> mItemList;
    private int mPosition = -1;
    private ProgressUpdateListener mProgressUpdateListener;
    private ProgressUpdateListener mGlobalUpdateListener;

    private Handler mHandler = new Handler(ContextFinder.getApplication().getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case HANDLE_SUCCESS:
                    String apkPath = (String) msg.obj;
                    if (TextUtils.isEmpty(apkPath)) {
                        return;
                    }
                    if (ShellTool.checkRootPermission()) {
                        // 如果已经root，则静默安装
                        InstallUtils.installSilent(ApplicationUtils.getApplication(), apkPath);
                    } else {
                        if (!CheckAppRunning.isXxRunningOnForeground(ApplicationUtils.getApplication(), ApplicationUtils.getApplication().getPackageName())) {
                            sendInstallNotification(apkPath);
                        } else {
                            InstallUtils.installAppCallSystem(ApplicationUtils.getApplication(), new File(apkPath));
                        }
                    }
                    ToastUtils.show(ApplicationUtils.getApplication(), getCurCopyAppName() + ApplicationUtils.getApplication().getString(R.string.view_more_multi_launch_success));
                    break;
                case HANDLE_FAILED:
                    ToastUtils.show(ApplicationUtils.getApplication(), getCurCopyAppName() + ApplicationUtils.getApplication().getString(R.string.view_more_multi_launch_fail));
                    resetData();
                    resetSPData();
                    break;
                default:
                    break;
            }
        }
    };

    private MultiLaunchManager() {
    }

    public static MultiLaunchManager getInstance() {
        if (mSelf == null) {
            mSelf = new MultiLaunchManager();
        }
        return mSelf;
    }

    public List<MultiLaunchItemData> getItemList() {
        return mItemList;
    }

    public void initData(List<? extends IAppInfo> appList){
        if(mItemList == null){
            mItemList = new ArrayList<>();
        }
        mPosition = -1;
        mItemList.clear();
        for(IAppInfo app : appList){
            addItemDataByFilter(app);
        }
    }


    /**
     *
     * 根据后台配置，过滤一些游戏，不显示在多开列表
     * @param appInfo
     */
    public void addItemDataByFilter(IAppInfo appInfo){
//        LogTool.i(TAG, "match " + "com.xxAssistant".matches("com.xxAss.*"));
//        LogTool.i(TAG, "match " + "com.xxAssistant".matches(".*com.xxAss.*"));
//        LogTool.i(TAG, "match " + "com.xxAssistant".matches(".*xxAssistant"));
//        LogTool.i(TAG, "match " + "com.xxAssistant".matches("com.xxAssist"));

        for(String pkgFilter : ApktoolUpdate.getPkgNameFilterList()){
            if(appInfo.getPackname().matches(pkgFilter)){
                return;
            }
        }
        mItemList.add(new MultiLaunchItemData(appInfo));
    }


    public boolean isCopying() {
        return mIsCopying;
    }

    public void setIsCopying(boolean isCopying) {
        mIsCopying = isCopying;
    }

    public String getCurCopyPkgName() {
        return mCurCopyPkgName;
    }

    public void setCurCopyPkgName(String curCopyPkgName) {
        mCurCopyPkgName = curCopyPkgName;
    }

    public String getCurCopyAppName() {
        return mCurCopyAppName;
    }

    public void setCurCopyAppName(String curCopyAppName) {
        mCurCopyAppName = curCopyAppName;
    }

    public MultiLaunchItemData.CopyState getCurCopyState() {
        return mCopyState;
    }

    public void setCurCopyState(MultiLaunchItemData.CopyState curCopyState) {
        mCopyState = curCopyState;
    }

    public int getCurProgress() {
        return mCurProgress;
    }

    public void setCurProgress(int curProgress) {
        mCurProgress = curProgress;
    }

    public String getApkPath() {
        return SPCenter.getString(MULTI_LAUNCH_APK_PATH, null);
    }

    public void setApkPath(String apkPath) {
        LogTool.i(TAG, "setApkPath "+apkPath);
        SPCenter.putString(MULTI_LAUNCH_APK_PATH, apkPath);
    }

    public void setPkgNameToSP(String pkgName) {
        SPCenter.putString(MULTI_LAUNCH_PKG_NAME, pkgName);
    }

    public String getPkgNameFromSP() {
        return SPCenter.getString(MULTI_LAUNCH_PKG_NAME, null);
    }

    public void setInstallStartStampToSP(long stamp) {
        mInstallStartStamp = stamp;
        SPCenter.putString(MULTI_LAUNCH_APK_INSTALL_START_STAMP, String.valueOf(mInstallStartStamp));
    }

    public long getInstallStartStampFromSP() {
        String stampStr = SPCenter.getString(MULTI_LAUNCH_APK_INSTALL_START_STAMP, "-1");
        mInstallStartStamp = Long.valueOf(stampStr);
        return mInstallStartStamp;
    }

    public String getMultiLaunchPkgName() {
        if (TextUtils.isEmpty(getApkPath())) {
            return null;
        }
        String pkgName = null;
        PackageManager packageManager = ApplicationUtils.getApplication().getPackageManager();
        PackageInfo packageInfo = packageManager.getPackageArchiveInfo(getApkPath(),
                PackageManager.GET_ACTIVITIES);
        if (packageInfo != null) {
            pkgName = packageInfo.applicationInfo.packageName;
        }
        return pkgName;
    }

    public void setTempPkgName(String tempPkgName) {
        mTempPkgName = tempPkgName;
    }

    public String getTempPkgName() {
        return mTempPkgName;
    }

    public void resetData() {
        if (mPosition != -1 && mItemList != null && mItemList.size() >= mPosition) {
//            findPosition();
            mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_NORMAL);
        }
        mPosition = -1;
        mCopyState = MultiLaunchItemData.CopyState.STATE_NORMAL;
        mInstallStartStamp = -1;
//        setCurCopyPkgName(null);
        setCurCopyAppName(null);
        setCurProgress(0);
//        setApkPath(null);
    }

    public void resetSPData() {
//        setApkPath(null);
        setPkgNameToSP(null);
        setInstallStartStampToSP(-1);
    }

    public void setEnableSpeed(boolean enable){
        mIsEnableSpeed = enable;
    }

    public boolean getEnableSpeed(){
        return mIsEnableSpeed;
    }

    private int findPosition() {
        if (TextUtils.isEmpty(mCurCopyPkgName)) {
            return -1;
        }
        for (int i = 0; i < mItemList.size(); i++) {
            MultiLaunchItemData itemData = mItemList.get(i);
            String pkgName = itemData.getAppInfo().getPackname();
            if (TextUtils.isEmpty(pkgName)) {
                continue;
            }
            if (pkgName.equals(mCurCopyPkgName)) {
                mPosition = i;
                break;
            }
        }
        return mPosition;
    }

    public void registerProgressUpdateListener(ProgressUpdateListener listener) {
        mProgressUpdateListener = listener;
        if (isCopying()) {
            mListener.onProgressUpdate(mCurProgress);
        } else if (getPkgNameFromSP() != null && getCurCopyPkgName() != null && getPkgNameFromSP().equals(getCurCopyPkgName())) {
            mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_COMPLETE);
            if (mProgressUpdateListener != null) {
                mProgressUpdateListener.onInstall();
            }
        }
    }

    public void unRegisterProgressUpdateListener() {
        mProgressUpdateListener = null;
    }

    public void setGlobalUpdateListener(ProgressUpdateListener listener) {
        mGlobalUpdateListener = listener;
    }

    public boolean hasGlobalUpdateListener() {
        return mGlobalUpdateListener != null;
    }

    private ProgressListener mListener = new ProgressListener() {
        @Override
        public void onProgressUpdate(int progress) {
            if (mPosition <= -1) {
                findPosition();
            }
            setCurProgress(progress);
            mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_PROCESS);

            notifyListenerProgressUpdate(progress);
        }

        @Override
        public void onFinish(JSONObject resultJson) {
            setIsCopying(false);
            if (mPosition <= -1) {
                findPosition();
            }
            try {
                boolean isSuccess = resultJson.getBoolean("is_multi_success");
                boolean isSpeedSuccess = resultJson.getBoolean("inject_speed_success");
                boolean isPluginSuccess = resultJson.getBoolean("inject_assist_success");
                String apkPath = resultJson.getString("apk_path");

                if (isSuccess) {
                    setPkgNameToSP(mCurCopyPkgName);
                    setApkPath(apkPath);
                    if (mPosition > -1) {
                        mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_COMPLETE);
                    }

                } else {
                    if (mPosition > -1) {
                        mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_FAILED);
                    }
                }

                notifyListenerFinish(isSuccess, apkPath, mCurCopyPkgName);

            } catch (Exception e) {
                e.printStackTrace();
                if (mPosition > -1) {
                    mItemList.get(mPosition).setCopyState(MultiLaunchItemData.CopyState.STATE_FAILED);
                }

                notifyListenerFinish(false, "", mCurCopyPkgName);
            }
        }
    };

    private void notifyListenerProgressUpdate(int progress) {
        if (mProgressUpdateListener != null) {
            mProgressUpdateListener.onProgressUpdate(progress);
        }

        if (mGlobalUpdateListener != null) {
            mGlobalUpdateListener.onProgressUpdate(progress);
        }
    }

    private void notifyListenerFinish(boolean isSuccess, String apkPath, String pkgName) {
        Bundle bundle = new Bundle();
        bundle.putBoolean(ProgressUpdateListener.KEY_BOOL_SUCCESS, isSuccess);
        bundle.putString(ProgressUpdateListener.KEY_APK_PATH, apkPath);
        bundle.putString(ProgressUpdateListener.KEY_PKG_NAME, pkgName);
        bundle.putString(ProgressUpdateListener.KEY_APP_NAME, mSrcAppName);

        // 如果mProgressUpdateListener为null时，MultiLaunchManager使用内部mHandler处理事件；
        // 如果mProgressUpdateListener不为null，则向外抛出事件
        if (mProgressUpdateListener != null) {
            mProgressUpdateListener.onFinish(bundle);
        } else {
            Message msg = mHandler.obtainMessage();
            if (isSuccess) {
                msg.what = HANDLE_SUCCESS;
                msg.obj = apkPath;
            } else {
                msg.what = HANDLE_FAILED;
            }
            mHandler.sendMessage(msg);
        }

        if (mGlobalUpdateListener != null) {
            mGlobalUpdateListener.onFinish(bundle);
        }
    }

    public void startMultiLaunch(Context context, String pkgName, String srcAppName, String distAppName, boolean enableSpeed, boolean enableAssist, String assistUrl, int assistId, boolean hasLocalAssist) {
        setIsCopying(true);
        if (mLoadApktoolResult == null) {
            mLoadApktoolResult = ApktoolManager.getInstance().getMultiApkHelper(ApktoolUpdate.getApkFilePath(context), "com.apktool.MultiApkHelper");
        }
        if (mLoadApktoolResult.mErrCode == LoadApktoolResult.LOAD_ASSIST_ERR_NONE) {
            mSrcAppName = srcAppName;
            setCurCopyPkgName(pkgName);
            setCurCopyAppName(distAppName);
            setEnableSpeed(enableSpeed);
            IMultiApkHelper iMultiApkHelper = mLoadApktoolResult.iMultiApkHelper;
            iMultiApkHelper.isAppInjected(pkgName); // 判断是否多开过 Todo for test
            JSONObject configJson = new JSONObject();
            try {
                configJson.put("package_name", pkgName);
                configJson.put("target_name", distAppName);
                configJson.put("is_speed", enableSpeed);
                configJson.put("is_assist", enableAssist);
                configJson.put("is_splash", false);
                configJson.put("assist_url", assistUrl);
                configJson.put("has_local_assist", hasLocalAssist);
                configJson.put("assist_uid", assistId);
            } catch (Exception e) {
                e.printStackTrace();
            }
            LogTool.i(TAG, "startMultiLaunch:\n" + configJson);
            iMultiApkHelper.generateNewApk(configJson, mListener);
        }
        else{
            mListener.onFinish(null);
        }
    }

    public boolean isMultiLaunchPkgName(String pkgName){
        if (mLoadApktoolResult == null) {
            mLoadApktoolResult = ApktoolManager.getInstance().getMultiApkHelper(ApktoolUpdate.getApkFilePath(ContextFinder.getApplication()), "com.apktool.MultiApkHelper");
        }
        if (mLoadApktoolResult.mErrCode == LoadApktoolResult.LOAD_ASSIST_ERR_NONE) {
            return mLoadApktoolResult.iMultiApkHelper.isAppInjected(pkgName);
        }
        return false;
    }

    public interface ProgressUpdateListener {

        String KEY_BOOL_SUCCESS = "key_success";
        String KEY_APK_PATH = "key_apk_name";
        String KEY_PKG_NAME = "key_pkg_name";
        String KEY_APP_NAME = "key_app_name";

        void onProgressUpdate(int progress);

        void onFinish(Bundle result);

        void onInstall();
    }

    /**
     * 发送安装通知
     *
     * @param apkPath
     */
    public void sendInstallNotification(String apkPath) {
        if (TextUtils.isEmpty(apkPath)) {
            return;
        }

        NotificationManager manager = (NotificationManager)  ApplicationUtils.getApplication().getSystemService(Context.NOTIFICATION_SERVICE);
        NotificationCompat.Builder builder = new NotificationCompat.Builder( ApplicationUtils.getApplication());
        builder.setContentTitle(MultiLaunchManager.getInstance().getCurCopyAppName() + ApplicationUtils.getApplication().getString(R.string.view_more_multi_launch_notification));
        builder.setSmallIcon( ApplicationUtils.getApplication().getApplicationInfo().icon);
        Intent intent = new Intent();
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(android.content.Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.fromFile(new File(apkPath)), "application/vnd.android.package-archive");
        PendingIntent pendingIntent = PendingIntent.getActivity( ApplicationUtils.getApplication(), 0, intent, PendingIntent.FLAG_ONE_SHOT);
        builder.setContentIntent(pendingIntent);
        Notification notification = builder.build();
        notification.flags = Notification.FLAG_AUTO_CANCEL;
        manager.notify(1, notification);
    }

    /**
     * 根据包名判断是否已经安装APP
     *
     * @param context     context
     * @param packageName 对应的包名
     */
    public static boolean hadInstall(Context context, String packageName) {
        PackageManager packageManager = context.getPackageManager();
        List<ApplicationInfo> apkList = packageManager.getInstalledApplications(PackageManager.GET_ACTIVITIES);
        for (ApplicationInfo packageInfo : apkList) {
            if (packageInfo.packageName.equalsIgnoreCase(packageName)) {
                return true;
            }
        }
        return false;
    }

    public boolean deleteApkFile() {
        try {
            File rootDir = new File(Environment.getExternalStorageDirectory().getAbsoluteFile() + ApplicationUtils.getApplication().getPackageName() + "/apk/");
            File[] files = rootDir.listFiles();
            if (files != null) {
                for (File file : files) {
                    FileUtils.DeleteFolder(file.getAbsolutePath());
                }
            }
        } catch (Exception er) {
            er.printStackTrace();
        }
        return true;
    }

    /**
     * 重置管理保存的所有的状态
     */
    public void resetAll() {
        resetData();
        resetSPData();

        deleteApkFile(); //删除已下载的apk包
    }

    /**
     * 判断是否已经安装超时
     *
     * @return
     */
    public boolean isInstallOvertime() {
        long startInstallStamp = getInstallStartStampFromSP();
        long curStamp = System.currentTimeMillis();
        if (startInstallStamp == -1) { //表示不计时
            return false;
        } else if (curStamp - startInstallStamp > MAX_INSTALL_TIME) {
            return true;
        }
        return false;
    }

    /**
     * SP的值为-1，不作计时
     */
    public void setNoKeepTime() {
        setInstallStartStampToSP(-1);
    }
}
