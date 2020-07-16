package com.dx.mobile.risk.utils;

import android.content.Context;

import java.lang.reflect.Field;

/**
 * 
 * 通过静态方式获取当前的Application实例，兼容范围(1.6-7.1)
 * 
 * @author boyliang
 *
 */
public final class ContextFinder {
	private static volatile Context sContext; 

	public static synchronized Context getApplication(){
		if(sContext == null){
			synchronized (ContextFinder.class) {
				/*if(sContext == null){
					Object application_thread = getStaticFieldValue("com.android.internal.os.RuntimeInit", "mApplicationObject");
					Object activity_thread = getFieldValue(application_thread, "this$0", "android.app.ActivityThread");
					sContext = getFieldValue(activity_thread, "mInitialApplication", "android.app.Application");
				}*/

				if(sContext == null) {
					try {
						sContext = (Context) Class.forName("android.app.ActivityThread").getMethod("currentApplication").invoke(null, (Object[]) null);
					} catch (Exception e) {
//					    e.printStackTrace();
						sContext = null;
					}
				}

				if(sContext == null) {
					try {
						sContext = (Context) Class.forName("android.app.AppGlobals").getMethod("getInitialApplication").invoke(null, (Object[]) null);
					} catch (Exception e) {
//                        e.printStackTrace();
						sContext = null;
					}
				}
			}
		}
		
		return sContext;
	}
	
	public static void setTokenContext(Context context){
		sContext = context;
	}


	private static Field getFieldByNameAndType(String cls, String filedname, String type) {
		Field field = null;

		try {
			Class<?> clazz = Class.forName(cls);
			field = clazz.getDeclaredField(filedname);

			if (field == null && type != null) {
				Class<?> type_clazz = Class.forName(type);
				for (Field f : clazz.getDeclaredFields()) {
					if (f.getClass().isInstance(type_clazz)) {
						field = f;
						break;
					}
				}
			}

			if (field != null) {
				field.setAccessible(true);
			}

		} catch (ClassNotFoundException e) {
//			e.printStackTrace();
		} catch (Exception e) {
//			e.printStackTrace();
		}

		return field;
	}

	@SuppressWarnings("unchecked")
	private static <T> T getStaticFieldValue(String cls, String fieldname) {
		Field field = getFieldByNameAndType(cls, fieldname, null);
		T res = null;

		if (field != null) {
			try {
				res = (T) field.get(null);
			} catch (IllegalAccessException e) {
//				e.printStackTrace();
			} catch (Exception e) {
//				e.printStackTrace();
			}
		}

		return res;
	}

	@SuppressWarnings("unchecked")
	private static <T> T getFieldValue(Object obj, String fieldname, String type){
		Field field = getFieldByNameAndType(obj.getClass().getName(), fieldname, type);
		T res = null;
		
		if(field != null){
			try {
				res = (T) field.get(obj);
			} catch (IllegalAccessException e) {
//				e.printStackTrace();
			} catch (Exception e) {
//				e.printStackTrace();
			}
		}
		
		return res;
	}

    public static void release(){
        sContext = null;
    }
}
