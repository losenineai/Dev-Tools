package com.xxAssistant.DanMuKu.plugin.apk;

import android.content.Context;
import android.view.View;

public interface IXXPlugin {
	
	/**
	 * 初始化接口
	 * @description  
	 * @param gamePid 游戏的进程id 
	 * @param gamePkgName 游戏的包名
	 * @param obsv 
	 */
	public void init(int gameUid, int gamePid, String gamePkgName, final XXPluginInitObsv obsv);
	
	
	/**
	 * 获取到一个view，这个view是辅助的父view，辅助如果更改UI，要更改的父View内部的view
	 */
	public View getView(Context context, float dp, IXXClickBlankListener listener);
	
	
	/**
	 * @return Toast的字符串。一些辅助没有ui，只需要弹出一个Toast
	 */
	public String getToastStr();
	
	
	/**
	 * 辅助内部进行清空状态之类
	 */
	public void destroy();
}
