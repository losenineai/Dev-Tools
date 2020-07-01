package com.xxlib.utils;

import android.app.Application;

import java.lang.reflect.Field;

/**
 * 
 * 通过静态方式获取当前的Application实例，兼容范围(1.6-5.1)
 * 
 * @author boyliang
 *
 */
public final class ContextFinder {
	private static volatile Application sApplication;
    private static Application sYourApplication; // 当前app所处的application

    /**
     * 保存当前应用的application，在getApplication为null时使用
     * @param application your Application
     */
    public static void setYourApplication(Application application) {
        if (application == null) {
            return;
        }
        sYourApplication = application;
    }

	public static synchronized Application getApplication(){
		if(sApplication == null){
			synchronized (ContextFinder.class) {
				if(sApplication == null){
					Object application_thread = getStaticFieldValue("com.android.internal.os.RuntimeInit", "mApplicationObject");
                    if (application_thread == null && sYourApplication != null) {
                        return sYourApplication;
                    }
                    Object activity_thread = getFieldValue(application_thread, "this$0", "android.app.ActivityThread");
                    if (activity_thread == null && sYourApplication != null) {
                        return sYourApplication;
                    }
                    sApplication = getFieldValue(activity_thread, "mInitialApplication", "android.app.Application");
                    if (sApplication == null && sYourApplication != null) {
                        return sYourApplication;
                    }
                }
			}
		}
		
		return sApplication;
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
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			e.printStackTrace();
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
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
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
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			}
		}
		
		return res;
	}
}
