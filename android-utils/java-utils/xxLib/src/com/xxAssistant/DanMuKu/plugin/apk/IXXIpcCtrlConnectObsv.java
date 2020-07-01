package com.xxAssistant.DanMuKu.plugin.apk;

public interface IXXIpcCtrlConnectObsv {
	/**
	 *  连接上socket
	 */
	public abstract void onConnected();
	
	
	/** 
	 * 正在连接中
	 */
	public abstract void onConnecting();
	
	
	/**
	 *  从连接状态中断开
	 */
	public abstract void onClosed();
	

	/**
	 * 连接失败
	 * @description
	 */
	public abstract void onConnectFail();

}
