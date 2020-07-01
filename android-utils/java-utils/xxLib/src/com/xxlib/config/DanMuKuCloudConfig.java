package com.xxlib.config;

import android.content.Context;
import android.text.TextUtils;

import com.xxlib.utils.ContextFinder;
import com.xxlib.utils.FileUtils;
import com.xxlib.utils.base.LogTool;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class DanMuKuCloudConfig {

	private static final String TAG = "DanMuKuCloudConfig";
    // 缓存文件下载长度
    public final static int DOWNLOAD_LENGTH = 1024;
    // 连接超时
    public final static int CONNECT_TIMEOUT = 10000;

    /** 悬浮窗配置本地存储文件 */
    public final static String DANMU_CONFIG_PATH = "/data/data/%s/float_config.json";
    /** 悬浮窗配置下载路劲 */
    public final static String DANMU_CONFIG_DOWNLOAD_PATH = "/data/data/%s/float_config_download.json";

	private static final String WORDING = "wording";
	private static final String CONTROL = "control";
	private static final String PACKAGE = "package";
	private static final String TIMESTAMP = "timestamp";

	public static class GameDanmukuConfig {
		public String mPkgName;
		public String mWording;
		public int mControl;
		public String mTimeStamp;

		public JSONObject toJSONObject() {
			JSONObject obj = new JSONObject();
			try {
				if (!TextUtils.isEmpty(mPkgName))
					obj.put(PACKAGE, mPkgName);
				if (!TextUtils.isEmpty(mWording))
					obj.put(WORDING, mWording);
				if (!TextUtils.isEmpty(mTimeStamp))
					obj.put(TIMESTAMP, mTimeStamp);
				obj.put(CONTROL, mControl);
			} catch (Exception e) {
			}
			return obj;
		}

		public static GameDanmukuConfig fromJson(JSONObject obj) {
			GameDanmukuConfig config = new GameDanmukuConfig();
			try {
				if (obj.has(PACKAGE))
					config.mPkgName = obj.getString(PACKAGE);
				if (obj.has(CONTROL))
					config.mControl = obj.getInt(CONTROL);
				if (obj.has(WORDING))
					config.mWording = obj.getString(WORDING);
				if (obj.has(TIMESTAMP))
					config.mTimeStamp = obj.getString(TIMESTAMP);
			} catch (Exception e) {
			}
			return config;
		}
	}

	private static DanMuKuCloudConfig instance = null;
	private List<GameDanmukuConfig> mConfigs;

	public static DanMuKuCloudConfig getInstance() {
		synchronized (DanMuKuCloudConfig.class) {
			if (null == instance) {
				synchronized (DanMuKuCloudConfig.class) {
					instance = new DanMuKuCloudConfig();
				}
			}
			return instance;
		}
	}

	private DanMuKuCloudConfig() {
		mConfigs = new ArrayList<GameDanmukuConfig>();
	}

	public List<GameDanmukuConfig> getConfig() {
		return mConfigs;
	}

	public void initDanmuConfig(String configFileUrl) {
        String pkgName = ContextFinder.getApplication().getPackageName();
        String localConfigPath = String.format(DANMU_CONFIG_PATH, pkgName);
        String configDownloadPath = String.format(DANMU_CONFIG_DOWNLOAD_PATH, pkgName);

		// 先读取本地配置
		List<GameDanmukuConfig> list = initDanmuConfigFromJson(localConfigPath);
		for (GameDanmukuConfig config : list) {
			LogTool.i(TAG, "mPkgName " + config.mPkgName);
			LogTool.i(TAG, "mWording " + config.mWording);
			LogTool.i(TAG, "mControl " + config.mControl);
			LogTool.i(TAG, "mTimeStamp " + config.mTimeStamp);
		}
		if (list.size() > 0)
			mConfigs.addAll(list);

		// 下载
		downloadConfigFile(configFileUrl, configDownloadPath);
		// 解析下载的配置
		List<GameDanmukuConfig> cloudConfigs = parseConfig(configDownloadPath);
		LogTool.i(TAG, "parse config finish.........");
		for (GameDanmukuConfig config : cloudConfigs) {
			LogTool.i(TAG, "mPkgName " + config.mPkgName);
			LogTool.i(TAG, "mWording " + config.mWording);
			LogTool.i(TAG, "mControl " + config.mControl);
			LogTool.i(TAG, "mTimeStamp " + config.mTimeStamp);
		}
		// 合并在一起，放在内存
		List<GameDanmukuConfig> mergeList = mergeConfigs(cloudConfigs, mConfigs);
		if (mergeList.size() > 0) {
			mConfigs = mergeList;
		}

		// 写入到文件
		saveConfigIntoFile(mConfigs, localConfigPath);
	}

	private List<GameDanmukuConfig> mergeConfigs(List<GameDanmukuConfig> cloudConfigs,
			List<GameDanmukuConfig> localConfigs) {
		List<GameDanmukuConfig> mergeList = new ArrayList<GameDanmukuConfig>();
		GameDanmukuConfig cloudConfig = null;
		for (int i = 0; i < cloudConfigs.size(); ++i) {
			// 每个游戏的配置，都要判断是否要覆盖本地的control，具体咨询zemin
			cloudConfig = checkGameConfig(cloudConfigs.get(i), localConfigs);
			if (null != cloudConfig)
				mergeList.add(cloudConfig);
		}
		return mergeList;
	}

	/**
	 * @author jammy
	 *         <p>
	 *         timestamp和wording必定每次都覆盖本地
	 *         <p>
	 *         timestamp和本地的不一致，force=1，control覆盖本地配置
	 *         <p>
	 *         timestamp和本地的不一致，force=0，control不覆盖本地配置
	 *         <p>
	 *         timestamp和本地的一致，control不覆盖本地配置
	 */
	private GameDanmukuConfig checkGameConfig(GameDanmukuConfig config, List<GameDanmukuConfig> localConfigs) {
		if (config.mPkgName == null) {
			return null;
		}
        Context context = ContextFinder.getApplication();
		boolean isForce = isForce(config.mControl);
		for (GameDanmukuConfig localConfig : localConfigs) {
			if (config.mPkgName.equals(localConfig.mPkgName)) {
				boolean isSameTimestamp = (null != localConfig.mTimeStamp)
						&& localConfig.mTimeStamp.equals(config.mTimeStamp);
				if (isForce && !isSameTimestamp) {
					// force，而且时间戳不一样，那么用云端下发的，用config.mControl即可
					// 用云端下发的，将该游戏加速器首次提示，标记为还没显示 
					JsonSpCenter jsonSpCenter = new JsonSpCenter(context);
					String key = JsonSpCenter.FLOAT_SPEED_TIPS_HAS_SHOW+config.mPkgName;
					jsonSpCenter.putBoolean(key, false);
				} else {
					// 其他情况，用本地的配置。
					config.mControl = localConfig.mControl;
				}
				break;
			}
		}
		return config;
	}

	/*
	 * value & (1<<0)，非0表示force，0表示不force value & (1<<1)，非0表示整个悬浮窗不出现。0表示悬浮窗出现。
	 * value & (1<<2)，非0表示加速器不出现。0表示加速器出现。 value & (1<<3)，非0表示脚本不出现。0表示脚本出现。
	 * value & (1<<4)，非0表示辅助不出现。0表示辅助是否出现 value & (1<<5)
	 * ，非0表示wording每次都提示。0表示wording首次提示。
	 */
	public static boolean isForce(int control) {
		return (control & 1) != 0;
	}

	public static boolean isShowFloat(int control) {
		return (control & 1 << 1) == 0;
	}

	public static boolean isShowSpeed(int control) {
		return (control & 1 << 2) == 0;
	}

	public static boolean isShowScript(int control) {
		return (control & 1 << 3) == 0;
	}

	public static boolean isShowPlugin(int control) {
		return (control & 1 << 4) == 0;
	}

	/**
	 * @param control
	 * @return
	 * <p>true  加速器提示,是否只提示一次
	 * <p>false 加速器提示，每次都提示
	 */
	public static boolean isShowSpeedTipEveryTime(int control) {
		return (control & 1 << 5) != 0;
	}
	
	/**
	 *  是否显示加速器提示
	 *  @return 
	 *  <p> true 表示要提示
	 *  <p> false 表示不提示
	 */
	public static boolean isShowSpeedTip(int control) {
		return (control & 1 << 6) != 0;
	}

	private void saveConfigIntoFile(List<GameDanmukuConfig> configs, String localConfigPath) {
		if (null == configs || configs.size() == 0) {
			return;
		}
		JSONArray arr = new JSONArray();
		for (GameDanmukuConfig config : configs) {
			arr.put(config.toJSONObject());
		}

		File jsonConfigFile = new File(localConfigPath);
		FileUtils.saveContent(arr.toString(), jsonConfigFile, null);
	}

	private List<GameDanmukuConfig> initDanmuConfigFromJson(String localConfigPath) {
		List<GameDanmukuConfig> list = new ArrayList<GameDanmukuConfig>();
		try {
			File jsonConfigFile = new File(localConfigPath);
			if (!jsonConfigFile.exists()) {
				return list;
			}
			String str = FileUtils.readFile(jsonConfigFile, null);
			JSONArray arr = new JSONArray(str);
			for (int i = 0; i < arr.length(); ++i) {
				JSONObject obj = arr.getJSONObject(i);
				// 读取下发的游戏配置
				GameDanmukuConfig config = GameDanmukuConfig.fromJson(obj);
				if (config.mPkgName != null && config.mControl != 0) {
					list.add(config);
				}
			}
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		}
		return list;
	}

	private List<GameDanmukuConfig> parseConfig(String path) {
		List<GameDanmukuConfig> configs = new ArrayList<GameDanmukuConfig>();
		if (!new File(path).exists()) {
			return configs;
		}
		try {
			String str = FileUtils.readFile(new File(path), "utf-8");
			JSONArray jsonArray = new JSONArray(str);
			GameDanmukuConfig config = null;
			JSONObject jsonObj = null;
			for (int i=0; i<jsonArray.length(); ++i) {
				config = new GameDanmukuConfig(); 
				try {
					 jsonObj = jsonArray.getJSONObject(i);
					 config = GameDanmukuConfig.fromJson(jsonObj);
					 if (!TextUtils.isEmpty(config.mPkgName)) {
						 configs.add(config);
					 }
				} catch (Exception e) {
					LogTool.i(TAG, "parse index " + i + " ex : "+ e.toString());
				}
			}
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		} finally {
			new File(path).delete();
		}
		return configs;
	}

	private void downloadConfigFile(String configFileUrl, String configDownloadPath) {
		URL url;
		HttpURLConnection conn = null;
		try {
			url = new URL(configFileUrl);
			conn = (HttpURLConnection) url.openConnection();
			conn.setRequestProperty("Range", "bytes=" + 0 + "-"); // 从0开始下载
			conn.setRequestMethod("GET");
			conn.setConnectTimeout(CONNECT_TIMEOUT);

			File file = new File(configDownloadPath);
			InputStream is = conn.getInputStream();
			FileOutputStream fos = new FileOutputStream(file);
			byte[] buffer = new byte[DOWNLOAD_LENGTH];
			int len = 0;
			while ((len = is.read(buffer)) != -1) {
				fos.write(buffer, 0, len);
			}
			is.close();
			fos.flush();
			fos.close();
		} catch (Exception e) {
			LogTool.w(TAG, e.toString());
		} finally {
			try {
				if (null != conn) {
					conn.disconnect();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

    public static GameDanmukuConfig getGameDanmukuConfig(String pkgName) {
        List<GameDanmukuConfig> list = DanMuKuCloudConfig.getInstance().getConfig();
        for (GameDanmukuConfig c : list) {
            if (!TextUtils.isEmpty(c.mPkgName)) {
                LogTool.i(TAG, "c.mPkgName " + c.mPkgName);
                if (c.mPkgName.startsWith("*") && c.mPkgName.endsWith("*") && c.mPkgName.length() > 2) {
                    String key = c.mPkgName.substring(1, c.mPkgName.length() - 1);
                    LogTool.i(TAG, "key " + key);
                    if (pkgName.contains(key)) {
                        return c;
                    }
                } else if (c.mPkgName.startsWith("*") && c.mPkgName.length() > 1) {
                    String key = c.mPkgName.substring(1);
                    LogTool.i(TAG, "key " + key);
                    if (pkgName.endsWith(key)) {
                        return c;
                    }
                } else if (c.mPkgName.endsWith("*") && c.mPkgName.length() > 1) {
                    String key = c.mPkgName.substring(0, c.mPkgName.length() - 1);
                    LogTool.i(TAG, "key " + key);
                    if (pkgName.startsWith(key)) {
                        return c;
                    }
                } else {
                    LogTool.i(TAG, "key " + c.mPkgName);
                    if (c.mPkgName.equals(pkgName)) {
                        return c;
                    }
                }
            }
        }
        return null;
    }
}
