package com.xxAssistant.DanMuKu.plugin.apk;

import java.lang.reflect.Method;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.view.View;

import com.xxAssistant.DanMuKu.plugin.apk.XXPluginInitObsv.InitResult;

/**
 * 一个辅助ui apk加载之后，就获取到一个XXPlugin实例
 * @author jammy
 * 2015-2-7   --  上午10:29:39
 */
public abstract class XXPluginBase implements IXXPlugin {
	
	private static final String TAG = "XXPluginBase";
	protected XXIpcController mIpcController;
	
	protected int gameUid;			// 游戏的辅助id
	protected int gamePid;			// 游戏的进程id
	protected String gamePkgName;	// 游戏的包名
	
	protected Context mContext;		// 叉叉助手的application的context
	protected IXXClickBlankListener mBlankListener;
	
	
	public XXPluginBase(Context context) {
		this.mContext = context;
	}
	
	
	public Context getContext() {
		return mContext;
	}
	
	
	public void init(int gameUid, int gamePid, String gamePkgName, final XXPluginInitObsv obsv){
		this.gameUid = gameUid;
		this.gamePid = gamePid;
		this.gamePkgName = gamePkgName;
		
		mIpcController = getIpcController();
		mIpcController.connnect(new IXXIpcCtrlConnectObsv() {
			@Override
			public void onConnecting() {
				XXPLog.i(TAG, "onConnecting");
				// 连接中不通知外部
			}
			@Override
			public void onConnected() {
				XXPLog.i(TAG, "onConnected");
				onInitResult(true);
				notifyInitObsv(obsv, XX_PLUGIN_VIEW_TYPE_VIEW == getViewType() ? 
						InitResult.InitSuccShowView : InitResult.InitSuccShowToast);
			}
			@Override
			public void onClosed() {
				XXPLog.i(TAG, "onClosed");
				// 从连接中状态断开，不通知外部
			}
			@Override
			public void onConnectFail(){
				XXPLog.i(TAG, "onConnectFail");
				onInitResult(false);
				notifyInitObsv(obsv, InitResult.InitFail);
			}
		});
	}

	
	protected void notifyInitObsv(XXPluginInitObsv obsv, InitResult initResult) {
		if (null != obsv) {
			obsv.onInitFinish(initResult);
		}
	}
	
	
	@Override
	public void destroy() {
		mContext = null;
		mBlankListener = null;
		if (null != mIpcController) {
			mIpcController.disconnect();
		}
	}
	
	
	/**
	 * 获取底层ipc通讯的对象
	 * @description  
	 * @return
	 */
	public XXIpcController getIpcController() {
		if (null == mIpcController) {
			mIpcController = createIpcController(gamePid);
		}
		return mIpcController;
	}

	
	/***
	 * 回到小浮点页面, 触发xx助手传递进来的点击空白事件
	 * @description
	 */
	public void onReturnToSmallFloatView() {
		if (null != mBlankListener) {
			mBlankListener.onClickBlank();
		}
	}
	
	
	/**
	 * 初始化成功会调用这个函数，一般是socket是否连接成功
	 * @param isSucc
	 */
	protected abstract void onInitResult(boolean isSucc);
	
	
	/**
	 * 每个游戏的ipc管理者，处理底层socket的收发消息
	 * 每个游戏的辅助都需要实现一个
	 */
	protected abstract XXIpcController createIpcController(int pid);
	
	
	/**
	 * 获取到一个view，这个view是辅助的父view，辅助如果更改UI，要更改的父View内部的view
	 */
	public abstract View getView(Context context, float dp, IXXClickBlankListener listene);
	
	
	/**
	 * 有些辅助没有ui，只需要弹toast的提示语
	 */
	public abstract String getToastStr();
	
	
	/**
	 * @return 0表示view，1表示toast
	 */
	public abstract int getViewType();
	public static final int XX_PLUGIN_VIEW_TYPE_VIEW = 0;
	public static final int XX_PLUGIN_VIEW_TYPE_TOAST = 1;
	
	
	protected AssetManager createAssetManager(String dexPath) {
        try {
            AssetManager assetManager = AssetManager.class.newInstance();
            Method addAssetPath = assetManager.getClass().getMethod("addAssetPath", String.class);
            addAssetPath.invoke(assetManager, dexPath);
            return assetManager;
        } catch (Exception e) {
        	XXPLog.e(TAG, "createAssetManager fail " + e.toString());
            e.printStackTrace();
            return null;
        }
    }


	protected Resources createResources(AssetManager assetManager) {
        Resources superRes = mContext.getResources();
		Resources resources = new Resources(assetManager, superRes.getDisplayMetrics(), superRes.getConfiguration());
        return resources;
    }
	
	
	protected String getApkPath() {
		return "/data/data/"+mContext.getPackageName()+"/app_plugin/" + gameUid + "/xxp_plugin_" + gameUid + ".apk";
	}
}
