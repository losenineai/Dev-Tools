package com.xxlib.utils.floatview;

import android.app.AppOpsManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
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
 * guokai, 检测系统是否EMUI系统(华为手机)
 * @Description 
 * 
 */
public class CheckEMUI {
	private static final String TAG = "CheckEMUI";

	private static final String KEY_EMUI_VERSION = "ro.build.version.emui";

	public static boolean isEMUI() {
		try {
			final BuildProperties prop = BuildProperties.newInstance();
			return prop.getProperty(KEY_EMUI_VERSION, null) != null;
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
	 * 得到EMUI主版本号
	 * @description  
	 * @return
	 * @throws IOException
	 */
    public static int getEmuiMainVer() {
        String line = null;
        BufferedReader reader = null;
        try {
            Process p = Runtime.getRuntime().exec("getprop "+KEY_EMUI_VERSION );
            reader = new BufferedReader(new InputStreamReader(p.getInputStream()), 1024);
            line = reader.readLine();
            if(line.startsWith("EmotionUI_"))
            	return Integer.valueOf(line.substring(10,11));
            else {
				return -1;
			}
            	
        } catch (IOException e) {
        	LogTool.e(TAG, e.toString());
        } finally {
            try {
				reader.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
        }
        return -1;
    }  
	
    /**
     * 判断EMUI的悬浮窗权限(API>=19有效)
     * 对flyme的悬浮窗权限判断也有效果
     *
     * @param context 上下文
     */
    public static boolean isEmuiFloatWindowOpAllowed(Context context) {
        final int version = Build.VERSION.SDK_INT;

        if (version >= 19) {
            return checkOp(context, 24);//24表示悬浮窗权限在AppOpsManager中
        } else {
//        	//0x8000000表示1000000000000000000000000000如果&第28位所得值为1则该位置被置为1，悬浮窗打开
//            return (context.getApplicationInfo().flags & 0x8000000) == 1<<27;
        	
        	if(getEmuiMainVer()<3)
        		return true;
        	else
        		return false;
        }
    }
    
    protected static boolean checkOp(Context context, int op) {
        final int version = Build.VERSION.SDK_INT;
        if (version >= 19) {
            AppOpsManager manager = (AppOpsManager) context.getSystemService(Context.APP_OPS_SERVICE);
            try {
            	Object object = invokeMethod(manager, "checkOp", op, Binder.getCallingUid(), getPackageName(context));
                return AppOpsManager.MODE_ALLOWED == (Integer) object;
            } catch (Exception e) {
            	LogTool.e("CheckEMUI", e.toString());
            }
        } else {
        	LogTool.e("CheckEMUI", "Below API 19 cannot invoke!");
        }
        return false;
    }
    
    /**
     * 对华为EMUI有效, 对魅族flyme有效，对MIUI无效
     * 悬浮窗权限 op=24
     * @description  
     * @param context
     * @return
     */
    public static boolean setOp(Context context) {
    	
    	int op = 24;
        final int version = Build.VERSION.SDK_INT;
        if (version >= 19) {
            AppOpsManager manager = (AppOpsManager) context.getSystemService(Context.APP_OPS_SERVICE);
            try {
            	Object object = invokeMethod2(manager, "setMode", op, 
            			Binder.getCallingUid(), getPackageName(context), AppOpsManager.MODE_ALLOWED);
                return  true;
            } catch (Exception e) {
            	LogTool.e("CheckEMUI", e.toString());
            }
        } else {
        	LogTool.e("CheckEMUI", "Below API 19 cannot invoke!");
        }
        return false;
    }
	
    public static String getPackageName(Context context) throws PackageManager.NameNotFoundException {
        PackageInfo pInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
        return pInfo.packageName;
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
    
    public static Object invokeMethod2(AppOpsManager manager, String method, int op, int uid, String packageName, int mode){
        Class c = manager.getClass();
        try {
        	Class[] classes = new Class[] {int.class, int.class, String.class, int.class};
        	Object[] x2 = {op, uid, packageName, mode};
            Method m = c.getDeclaredMethod(method, classes);
            return m.invoke(manager, x2);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }
    
    public static void openNotificationMgr(Context context) {
    	try {
			Intent mIntent = new Intent();
			ComponentName comp = new ComponentName("com.huawei.systemmanager", 
					"com.huawei.notificationmanager.ui.NotificationManagmentActivity");
			mIntent.setComponent(comp);
			mIntent.setAction("android.intent.action.VIEW");
			context.startActivity(mIntent);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
}
