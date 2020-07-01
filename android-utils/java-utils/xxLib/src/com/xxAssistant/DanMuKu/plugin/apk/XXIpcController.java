package com.xxAssistant.DanMuKu.plugin.apk;

import android.net.LocalSocket;

public abstract class XXIpcController {
	
	private static final String TAG = "XXIpcController";

	protected int mPid;
	protected int mRid;
	protected XXIpcClient mIpcClient = null;
	protected IXXIpcCtrlConnectObsv mObsv;
	
	public XXIpcController(int pid) {
		this.mPid = pid;
		this.mRid = 0;
	}
	
	
	public abstract String getSocketBaseName();
	
	
	/**
	 * 发送消息，不需要带换行，内部会加上换行
	 * @param line
	 */
	public void send(String line) {
		if (null != mIpcClient){
			XXPLog.i(TAG, line);
			mIpcClient.sendString(line);
			mIpcClient.sendString("\n");
		} else {
			XXPLog.i(TAG, "mIpcClient is null");
		}
	}
	
	public void send(int cmd) {
		String str = mRid + " " + cmd;
		XXPLog.i(TAG, "send " + str);
		mRid++;
		send(str);
	}
	
	
	public void send(int cmd, int value) {
		String str = mRid + " "  + cmd  + " " + value;
		mRid++;
		XXPLog.i(TAG, "send " + str);
		send(str);
	}
	
	
	/**
	 * 进行socket连接
	 * @description
	 */
	public void connnect(IXXIpcCtrlConnectObsv obsv) {
		this.mObsv = obsv;
		if (null == mIpcClient) {
			mIpcClient = new XXIpcClient(this, getSocketBaseName() + "_" + mPid);
		}
		mIpcClient.beginConnect();
	}


    /**
     * 新增api，用于Server接受socket开启client通讯
     * @param socket
     */
    public void start(LocalSocket socket){
        if (mIpcClient == null) {
            mIpcClient = new XXIpcClient(this, null);
        }
        mIpcClient.beginConnect(socket);
    }


	/**
	 * 断开socket连接
	 * @description
	 */
	public void disconnect() {
		if (null != mIpcClient) {
			mIpcClient.closeConnect();
		}
	}
	
	
	/**
	 * 收到socket server的消息
	 * @param line 每次内容都是一行
	 */
	public abstract void onReceive(String line);
	
	
	/**
	 *  连接上socket的回调
	 */
	public void onConnected(){
		if (null != mObsv) {
			mObsv.onConnected();
		}
	}
	
	
	public void onConnectFail() {
		if (null != mObsv) {
			mObsv.onConnectFail();
		}
	}
	
	
	/** 
	 * 正在连接中
	 */
	public void onConnecting(){
		if (null != mObsv) {
			mObsv.onConnecting();
		}
	}
	
	
	/**
	 *  和socket断开、或者没有连接上
	 */
	public void onClosed(){
		if (null != mObsv) {
			mObsv.onClosed();
		}
	}
}
