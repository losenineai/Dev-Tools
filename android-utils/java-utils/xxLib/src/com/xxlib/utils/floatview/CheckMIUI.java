package com.xxlib.utils.floatview;

import android.app.AppOpsManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Binder;
import android.os.Build;
import android.os.Environment;

import com.xxlib.utils.base.LogTool;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;


/**
 * wxj, 检测系统是否MIUI系统
 * @Description 
 *
 * 版本号    作者    日期    相关修改
 *  1.0   wxj  2014-5-30  创建
 *
 * @author wxj
 * 2014-5-30
 */
public class CheckMIUI {
	private static final String TAG = "CheckMIUI";

	private static final String KEY_MIUI_VERSION_CODE = "ro.miui.ui.version.code";
	private static final String KEY_MIUI_VERSION_NAME = "ro.miui.ui.version.name";
	private static final String KEY_MIUI_INTERNAL_STORAGE = "ro.miui.internal.storage";

	public static boolean isMIUI() {
		try {
			final BuildProperties prop = BuildProperties.newInstance();
			return prop.getProperty(KEY_MIUI_VERSION_CODE, null) != null
					|| prop.getProperty(KEY_MIUI_VERSION_NAME, null) != null
					|| prop.getProperty(KEY_MIUI_INTERNAL_STORAGE, null) != null;
		} catch (final IOException e) {
			return false;
		}
	}
	
	public static class BuildProperties {

		private final Properties properties;

		public BuildProperties() throws IOException {
			properties = new Properties();
			properties.load(new FileInputStream(new File(Environment.getRootDirectory(), "build.prop")));
		}

		public boolean containsKey(final Object key) {
			return properties.containsKey(key);
		}

		public boolean containsValue(final Object value) {
			return properties.containsValue(value);
		}

		public Set<Entry<Object, Object>> entrySet() {
			return properties.entrySet();
		}

		public String getProperty(final String name) {
			return properties.getProperty(name);
		}

		public String getProperty(final String name, final String defaultValue) {
			return properties.getProperty(name, defaultValue);
		}

		public boolean isEmpty() {
			return properties.isEmpty();
		}

		public Enumeration<Object> keys() {
			return properties.keys();
		}

		public Set<Object> keySet() {
			return properties.keySet();
		}

		public int size() {
			return properties.size();
		}

		public Collection<Object> values() {
			return properties.values();
		}

		public static BuildProperties newInstance() throws IOException{
			return new BuildProperties();
		}

	}
	
	/**
	 * 得到MIUI版本
	 * @description  
	 * @return V5  V6
	 * @throws IOException
	 */
    public static String getMIUIVersion() {
        String line = "UNKNOWN";
        BufferedReader reader = null;
        try {
            Process p = Runtime.getRuntime().exec("getprop ro.miui.ui.version.name" );
            reader = new BufferedReader(new InputStreamReader(p.getInputStream()), 1024);
            line = reader.readLine();
            if (line == null) 
            	line = "UNKNOWN";
            return line;
        } catch (Exception e) {
        	LogTool.e(TAG, e.toString());
        } finally {
            try {
            	if (null != reader)
            		reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
        }
        return line;
    }  
	
    /**
     * 判断MIUI的悬浮窗权限
     *
     * @param context 上下文
     */
    public static boolean isMiuiFloatWindowOpAllowed(Context context) {
        final int version = Build.VERSION.SDK_INT;

        if (version >= 19) {
            return checkOp(context, 24);//24表示悬浮窗权限在AppOpsManager中
        } else {
        	//0x8000000表示1000000000000000000000000000如果&第28位所得值为1则该位置被置为1，悬浮窗打开
            return (context.getApplicationInfo().flags & 0x8000000) == 1<<27;
        }
    }
    
    protected static boolean checkOp(Context context, int op) {
        final int version = Build.VERSION.SDK_INT;
        if (version >= 19) {
            AppOpsManager manager = (AppOpsManager) context.getSystemService(Context.APP_OPS_SERVICE);
            try {
            	Object object = invokeMethod(manager, "checkOp", op, Binder.getCallingUid(), context.getPackageName());
                return AppOpsManager.MODE_ALLOWED == (Integer) object;
            } catch (Exception e) {
            	LogTool.e("CheckMIUI", e.toString());
            }
        } else {
        	LogTool.e("CheckMIUI", "Below API 19 cannot invoke!");
        }
        return false;
    }
    
    public static Object invokeMethod(AppOpsManager manager, String method, int op, int uid, String packageName){
        Class c = manager.getClass();
        try {
        	Class[] classes = new Class[] {int.class, int.class, String.class};
        	Object[] x2 = {op, uid, packageName};
            Method m = c.getDeclaredMethod(method, classes);
            return m.invoke(manager, x2);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }
    
    

	public static boolean openMiuiFloatPermissionV5(Context context) {
		try {
			LogTool.i(TAG, "openMiuiFloatPermissionV5");
			Intent intent = new Intent(android.provider.Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
			Uri uri = Uri.fromParts("package", context.getPackageName(), null);
			intent.setData(uri);
			context.startActivity(intent);
			return true;
		} catch (Exception e) {
			LogTool.w(TAG, "openMiuiFloatPermissionV5 ex " + e.toString());
		}
		return false;
	}
	
	public static boolean openMiuiFloatPermissionV6(Context context) {
		try {
			LogTool.i(TAG, "openMiuiFloatPermissionV6");
			Intent intent = new Intent("miui.intent.action.APP_PERM_EDITOR");
			intent.setClassName("com.miui.securitycenter", "com.miui.permcenter.permissions.AppPermissionsEditorActivity");
			intent.putExtra("extra_pkgname", context.getPackageName());
			context.startActivity(intent);
			return true;
		} catch (Exception e) {
			LogTool.w(TAG, "openMiuiFloatPermissionV6 ex " + e.toString());
		}
		return false;
	}
}
