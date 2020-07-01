package com.xxlib.utils;

import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;

import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.floatview.CheckAppStateUtils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * clear system memory tool
 * 
 * @author linbin 2013
 */
public class ClearMemory {

	private static final String TAG = "ClearMemory";
	private Context context;

	public ClearMemory(Context context) {
		this.context = context;
	}

	public long getAvailMemory() {
		// 获取android当前可用内存大小
		ActivityManager am = (ActivityManager) this.context.getSystemService(Context.ACTIVITY_SERVICE);
		MemoryInfo mi = new MemoryInfo();
		am.getMemoryInfo(mi);
		// mi.availMem; 当前系统的可用内存

		// return Formatter.formatFileSize(context, mi.availMem);// 将获取的内存大小规格化
		return mi.availMem / (1024 * 1024);
	}

	public long getTotalMemory() {
		String str1 = "/proc/meminfo";// 系统内存信息文件
		String str2;
		String[] arrayOfString;
		long initial_memory = 0;

		try {
			FileReader localFileReader = new FileReader(str1);
			BufferedReader localBufferedReader = new BufferedReader(localFileReader, 8192);
			str2 = localBufferedReader.readLine();// 读取meminfo第一行，系统总内存大小

			arrayOfString = str2.split("\\s+");
			for (String num : arrayOfString) {
				// Log.i(str2, num + "\t");
			}

			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte
			localBufferedReader.close();

		} catch (IOException e) {
		}
		// return Formatter.formatFileSize(context, initial_memory);//
		// Byte转换为KB或者MB，内存大小规格化
		return initial_memory / (1024 * 1024);
	}

	public void clearMemory() {
		ActivityManager activityManger = (ActivityManager) this.context.getSystemService(this.context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> list = activityManger.getRunningAppProcesses();
		if (list != null)
			for (int i = 0; i < list.size(); i++) {
				RunningAppProcessInfo apinfo = list.get(i);
				String[] pkgList = apinfo.pkgList;
				// 通过比对当前获得的activity重要度与系统定义重要进程重要度进行对比
				// 这里的系统重要程度是300，当低于这个级别时，就会容易被系统回收清理，所以是属于不重要进程，进行回收
				if (apinfo.importance >= 300) {
					// Process.killProcess(apinfo.pid);
					for (int j = 0; j < pkgList.length; j++) {
						// 2.2以上是过时的,请用killBackgroundProcesses代替
						if (!pkgList[j].equals(context.getPackageName())) {
							activityManger.killBackgroundProcesses(pkgList[j]);
							LogTool.i("clearMemory", pkgList[j] + ", " + context.getPackageName());
						}
					}
				}
			}
	}

	public static int getGamePid(Context context, String pkgName) {
		int pid = 0;
		ActivityManager activityManger = (ActivityManager) context.getSystemService(context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> list = activityManger.getRunningAppProcesses();
		if (null != list) {
			for (RunningAppProcessInfo info : list) {
				if (null != info && info.processName != null && info.processName.equals(pkgName)) {
					LogTool.i(TAG, "info.processName " + info.processName + "  info.pid " +  info.pid);
                    LogTool.i(TAG, "info.importanceReasonPid " + info.importanceReasonPid);
                    // 萌三国特殊处理。萌三国有两个一样名字的进程。
                    if (pkgName.contains("org.thedream.sanguo") && info.importanceReasonPid != 0) {
                        pid = info.importanceReasonPid;
                    } else {
                        pid = info.pid;
                    }
					break;
				}
			}
		}
		LogTool.i(TAG, "pre game id:" + pid);
		// 某些机子没法通过getRunningAppProcess来获取pid，因此通过top命令取得
		if (pid == 0) {
			ArrayList<Integer> pids = CheckAppStateUtils.getPidsByPackageName(pkgName);
			pid = CheckAppStateUtils.getMiniOomAdjPid(pids);
			LogTool.i(TAG, "CheckAppStateUtils game id:" + pid);
		}
		return pid;
	}
}
