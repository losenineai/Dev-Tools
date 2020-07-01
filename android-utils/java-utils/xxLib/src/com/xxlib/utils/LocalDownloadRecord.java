package com.xxlib.utils;

import java.util.Hashtable;

public class LocalDownloadRecord {

	/**
	 * 用来记录下载的数据，如果本地点击了下载就放入到这个table中， 如果在更新时发现这里有该游戏的下载数据，就将其移除
	 */
	private Hashtable<String, Integer> sDownLoadTable;
	private static LocalDownloadRecord mSingleton;

	private LocalDownloadRecord() {
		sDownLoadTable = new Hashtable<String, Integer>();
	}

	public static LocalDownloadRecord getInstance() {
		if (mSingleton == null) {
			mSingleton = new LocalDownloadRecord();
		}
		return mSingleton;
	}

	/**
	 * @description 在table中查找游戏，如果有该游戏，则返回此游戏的下载次数，否则则返回-1
	 * @param name
	 *            游戏名称
	 * @return
	 */
	private int findGame(String name) {
		Integer times = sDownLoadTable.get(name);
		if (times != null) {
			return times;
		}
		return -1;
	}

	/**
	 * @description 为该游戏下载次数 + 1
	 * @param name
	 */
	public void addDownLoadTime(String name, int serverTimes) {
		Integer tableTimes = sDownLoadTable.get(name);
		if (tableTimes != null){
			sDownLoadTable.put(name, tableTimes + 1);
		}
		else{
			sDownLoadTable.put(name, serverTimes + 1);
		}

	}

	/**
	 * @description 传入服务器获取的下载次数，返回真正要显示的下载次数
	 * @param name
	 * @param serverTimes
	 * @return
	 */
	public int getDownLoadTimes(String name, int serverTimes) {
		int recordTimes = findGame(name);
		if (recordTimes < serverTimes) {
			sDownLoadTable.remove(name);
			return serverTimes;
		} else {
			return recordTimes;
		}
	}
}
