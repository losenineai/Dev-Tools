package com.xxAssistant.DanMuKu.plugin.apk;

/**
 * 初始化结果回调
 *
 * @author jammy
 * 2015-2-7   --  上午10:35:12
 */
public interface XXPluginInitObsv {
	
	public enum InitResult {
		InitFail, InitSuccShowView, InitSuccShowToast
	};
	
	
	public void onInitFinish(InitResult initResult);
}
