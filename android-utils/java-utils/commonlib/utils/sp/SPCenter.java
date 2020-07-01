package com.auric.intell.commonlib.utils.sp;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;

import java.util.Set;
import java.util.TreeSet;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogTool;

/**
 * SharePreference 的工具库
 * Created by white on 16/7/15.
 */
public class SPCenter {

    private static final String TAG = "SPCenter";

	private static String DEFUALT_FILE_NAME_SP = "settings";

	private static SharedPreferences sp_setting;

	private static SharedPreferences getSpSetting() {
		return getSpSetting(DEFUALT_FILE_NAME_SP);
	}

	private static SharedPreferences getSpSetting(String name) {
		synchronized (SPCenter.class) {
			sp_setting = ContextFinder.getApplication().getSharedPreferences(name, Context.MODE_MULTI_PROCESS);
			return sp_setting;
		}
	}
	
	public static void remove(String key) {
		getSpSetting().edit().remove(key).commit();
	}
	
	/**
	 * 系统低于11，没有getStringSet这个接口，会用spliter来隔开保存.
	 * 有少量4.1和4.0的也上报，干脆全部用自己写
	 * <p> 例如spliter是":" str1:str2:str3
	 */
	public static Set<String> getStringSet(String key, String spliter) {
		Set<String> strSet = new TreeSet<String>();
		String tmp = SPCenter.getString(key);
		if (!TextUtils.isEmpty(tmp)) {
			String[] strArr = tmp.split(spliter);
			for (String s : strArr) {
				strSet.add(s);
			}
		}
		return strSet;
	}
	
	/**
	 * 系统低于11，没有getStringSet这个接口，会用spliter来隔开保存.
	 * 有少量4.1和4.0的也上报，干脆全部用自己写
	 * <p> 例如spliter是":" str1:str2:str3
	 */
	public static boolean putStringSet(String key, Set<String> values, String spliter) {
		if (null != values && values.size() != 0) {
			Object[] objArr = values.toArray();
			if (null != objArr) {
				StringBuilder sb = new StringBuilder();
				for (int i=0; i<objArr.length; ++i) {
					if (null != objArr[i])
						sb.append(i!=0 ? spliter+objArr[i].toString() : objArr[i].toString());
				}
				return SPCenter.putString(key, sb.toString());
			}
		}
		return false;
	}

	public static boolean contains(String key) {
		return getSpSetting().contains(key);
	}

	public static boolean contains(String key, String spName) {
		return getSpSetting(spName).contains(key);
	}

	/**
	 * put string preferences
	 * 
	 * @param key
	 *            The name of the preference to modify
	 * @param value
	 *            The new value for the preference
	 * @return True if the new values were successfully written to persistent
	 *         storage.
	 */
	public static boolean putString(String key, String value) {
		SharedPreferences.Editor editor = getSpSetting().edit();
		editor.putString(key, value);
		return editor.commit();
	}

	public static boolean putString(String key, String value, String spName) {
		SharedPreferences.Editor editor = getSpSetting(spName).edit();
		editor.putString(key, value);
		return editor.commit();
	}

	/**
	 * get string preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @return The preference value if it exists, or null. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a string
	 * @see #getString(String, String)
	 */
	public static String getString(String key) {
		return getString(key, "");
	}

	/**
	 * get string preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @param defaultValue
	 *            Value to return if this preference does not exist
	 * @return The preference value if it exists, or defValue. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a string
	 */
	public static String getString(String key, String defaultValue) {
		return getSpSetting().getString(key, defaultValue);
	}

	public static String getString(String key, String defaultValue, String spName) {
		return getSpSetting(spName).getString(key, defaultValue);
	}

	/**
	 * put int preferences
	 * 
	 * @param key
	 *            The name of the preference to modify
	 * @param value
	 *            The new value for the preference
	 * @return True if the new values were successfully written to persistent
	 *         storage.
	 */
	public static boolean putInt(String key, int value) {
		SharedPreferences.Editor editor = getSpSetting().edit();
		editor.putInt(key, value);
		return editor.commit();
	}

	public static boolean putInt(String key, int value, String spName) {
		SharedPreferences.Editor editor = getSpSetting(spName).edit();
		editor.putInt(key, value);
		return editor.commit();
	}

	/**
	 * get int preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @return The preference value if it exists, or -1. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a int
	 * @see #getInt(String, int)
	 */
	public static int getInt(String key) {
		return getInt(key, -1);
	}

	/**
	 * get int preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @param defaultValue
	 *            Value to return if this preference does not exist
	 * @return The preference value if it exists, or defValue. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a int
	 */
	public static int getInt(String key, int defaultValue) {
		return getSpSetting().getInt(key, defaultValue);
	}

	public static int getInt(String key, int defaultValue, String spName) {
		return getSpSetting(spName).getInt(key, defaultValue);
	}

	/**
	 * put long preferences
	 * 
	 * @param key
	 *            The name of the preference to modify
	 * @param value
	 *            The new value for the preference
	 * @return True if the new values were successfully written to persistent
	 *         storage.
	 */
	public static boolean putLong(String key, long value) {
		SharedPreferences.Editor editor = getSpSetting().edit();
		editor.putLong(key, value);
		return editor.commit();
	}

	public static boolean putLong(String key, long value, String spName) {
		SharedPreferences.Editor editor = getSpSetting(spName).edit();
		editor.putLong(key, value);
		return editor.commit();
	}

	/**
	 * get long preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @return The preference value if it exists, or -1. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a long
	 * @see #getLong(String, long)
	 */
	public static long getLong(String key) {
		return getLong(key, -1);
	}

	/**
	 * get long preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @param defaultValue
	 *            Value to return if this preference does not exist
	 * @return The preference value if it exists, or defValue. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a long
	 */
	public static long getLong(String key, long defaultValue) {
		return getSpSetting().getLong(key, defaultValue);
	}

	public static long getLong(String key, long defaultValue, String spName) {
		return getSpSetting(spName).getLong(key, defaultValue);
	}

	/**
	 * put float preferences
	 * 
	 * @param key
	 *            The name of the preference to modify
	 * @param value
	 *            The new value for the preference
	 * @return True if the new values were successfully written to persistent
	 *         storage.
	 */
	public static boolean putFloat(String key, float value) {
		SharedPreferences.Editor editor = getSpSetting().edit();
		editor.putFloat(key, value);
		return editor.commit();
	}

	/**
	 * get float preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @return The preference value if it exists, or -1. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a float
	 * @see #getFloat(String, float)
	 */
	public static float getFloat(String key) {
		return getFloat(key, -1);
	}

	/**
	 * get float preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @param defaultValue
	 *            Value to return if this preference does not exist
	 * @return The preference value if it exists, or defValue. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a float
	 */
	public static float getFloat(String key, float defaultValue) {
		return getSpSetting().getFloat(key, defaultValue);
	}

	/**
	 * put boolean preferences
	 * 
	 * @param key
	 *            The name of the preference to modify
	 * @param value
	 *            The new value for the preference
	 * @return True if the new values were successfully written to persistent
	 *         storage.
	 */
	public static boolean putBoolean(String key, boolean value) {
		SharedPreferences.Editor editor = getSpSetting().edit();
		editor.putBoolean(key, value);
		return editor.commit();
	}

	public static boolean putBoolean(String key, boolean value, String spName) {
		SharedPreferences.Editor editor = getSpSetting(spName).edit();
		editor.putBoolean(key, value);
		boolean result = editor.commit();

        LogTool.d(TAG, "put" + key + "" + value + " isSucc:"+result);
        return result;
	}

	/**
	 * get boolean preferences, default is false
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @return The preference value if it exists, or false. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a boolean
	 * @see #getBoolean(String, boolean)
	 */
	public static boolean getBoolean(String key) {
		return getBoolean(key, false);
	}

	/**
	 * get boolean preferences
	 * 
	 * @param key
	 *            The name of the preference to retrieve
	 * @param defaultValue
	 *            Value to return if this preference does not exist
	 * @return The preference value if it exists, or defValue. Throws
	 *         ClassCastException if there is a preference with this name that
	 *         is not a boolean
	 */
	public static boolean getBoolean(String key, boolean defaultValue) {
		return getSpSetting().getBoolean(key, defaultValue);
	}

	public static boolean getBoolean(String key, boolean defaultValue, String spName) {
		return getSpSetting(spName).getBoolean(key, defaultValue);
	}

	public static boolean remove(String key, String spName) {
		try {
			SharedPreferences.Editor editor = getSpSetting(spName).edit();
			editor.remove(key);
			return editor.commit();
		} catch (Throwable t) {

		}
		return false;
	}
}
