package com.xxlib.utils.xpk;

/**
 * 
 * @Description 安装Xpk格式的数据模型
 *
 * 版本号    作者    日期    相关修改
 *  1.0   linbin   2015-1-8  创建
 *
 * @author linbin
 * 2015-1-8   --  下午7:37:55
 */
public class XpkData {

	//obb文件的参数
	private String file;
	private String destination;
	private String md5;
	
	//dir文件夹的参数
	private String dir;
	private String dirDestination;
	
	public String getFile() {
		return file;
	}
	public String getDir() {
		return dir;
	}
	public void setDir(String dir) {
		this.dir = dir;
	}
	public String getDirDestination() {
		return dirDestination;
	}
	public void setDirDestination(String dirDestination) {
		this.dirDestination = dirDestination;
	}
	public void setFile(String file) {
		this.file = file;
	}
	public String getDestination() {
		return destination;
	}
	public void setDestination(String destination) {
		this.destination = destination;
	}
	public String getMd5() {
		return md5;
	}
	public void setMd5(String md5) {
		this.md5 = md5;
	}
	
	
}
