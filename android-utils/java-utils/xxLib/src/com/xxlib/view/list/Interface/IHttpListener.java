package com.xxlib.view.list.Interface;

/**
 * 
 * @Description wxj, 面向外部UI的http请求回调接口
 *
 * 版本号    作者    日期    相关修改
 *  1.0   white   2014-12-9  创建
 *
 * @author white
 * 2014-12-9   --  上午10:39:33
 */
public interface IHttpListener {

	/**
	 * 网络异常
	 * @description
	 */
	public void onNetException();
	
	/**
	 * 请求失败
	 * @description  
	 * @param cmd -1为解包异常，非业务逻辑
	 * @param obj
	 */
	public void onFailure(int cmd, Object obj);
	
	/**
	 * 请求成功
	 * @description  
	 * @param cmd 默认为0
	 * @param obj
	 */
	public void onSuccess(int cmd, Object obj);
	
}
