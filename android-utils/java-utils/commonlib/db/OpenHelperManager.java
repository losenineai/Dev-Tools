package com.auric.intell.commonlib.db;

//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

import android.content.Context;
import android.content.res.Resources;

import com.j256.ormlite.android.apptools.OrmLiteSqliteOpenHelper;
import com.j256.ormlite.logger.Logger;
import com.j256.ormlite.logger.LoggerFactory;

import java.lang.reflect.Constructor;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

/**
 * 为了兼容不同数据库获取不同的OrmLiteSqliteOpenHelper
 * 1. 重写 com.j256.ormlite.android.apptools.OpenHelperManager 类的loadHelper方法
 * 2. 屏蔽 innerSetHelperClass 方法的功能
 */
public class OpenHelperManager {
    private static final String HELPER_CLASS_RESOURCE_NAME = "open_helper_classname";
    private static Logger logger = LoggerFactory.getLogger(com.j256.ormlite.android.apptools.OpenHelperManager.class);
    private static Class<? extends OrmLiteSqliteOpenHelper> helperClass = null;
    private static volatile OrmLiteSqliteOpenHelper helper = null;
    private static boolean wasClosed = false;
    private static int instanceCount = 0;

    public OpenHelperManager() {
    }

    public static void setOpenHelperClass(Class<? extends OrmLiteSqliteOpenHelper> openHelperClass) {
        if (openHelperClass == null) {
            helperClass = null;
        } else {
            innerSetHelperClass(openHelperClass);
        }

    }

    public static void setHelper(OrmLiteSqliteOpenHelper helper) {
        helper = helper;
    }

    public static synchronized <T extends OrmLiteSqliteOpenHelper> T getHelper(Context context, Class<T> openHelperClass) {
        innerSetHelperClass(openHelperClass);
        return loadHelper(context, openHelperClass);
    }

    /** @deprecated */
//    @Deprecated
//    public static synchronized OrmLiteSqliteOpenHelper getHelper(Context context) {
//        if (helperClass == null) {
//            if (context == null) {
//                throw new IllegalArgumentException("context argument is null");
//            }
//
//            Context appContext = context.getApplicationContext();
//            innerSetHelperClass(lookupHelperClass(appContext, context.getClass()));
//        }
//
//        return loadHelper(context, helperClass);
//    }

    /** @deprecated */
    @Deprecated
    public static void release() {
        releaseHelper();
    }

    public static synchronized void releaseHelper() {
        --instanceCount;
        logger.debug("helper instance count = {}", new Object[]{instanceCount});
        if (instanceCount <= 0) {
            if (helper != null) {
                logger.debug("Zero instances.  Closing helper.", new Object[0]);
                helper.close();
                helper = null;
                wasClosed = true;
            }

            if (instanceCount < 0) {
                logger.error("Too many calls to release helper.  Instance count = {}", new Object[]{instanceCount});
            }
        }

    }

    private static void innerSetHelperClass(Class<? extends OrmLiteSqliteOpenHelper> openHelperClass) {
//        if (helperClass == null) {
//            helperClass = openHelperClass;
//        } else if (helperClass != openHelperClass) {
//            throw new IllegalStateException("Helper class was " + helperClass + " but is trying to be reset to " + openHelperClass);
//        }

    }

    private static <T extends OrmLiteSqliteOpenHelper> T loadHelper(Context context, Class<T> openHelperClass) {
//        if (helper == null) {
//
//        }

        if (wasClosed) {
            logger.info("helper has already been closed and is being re-opened.", new Object[0]);
        }

        if (context == null) {
            throw new IllegalArgumentException("context argument is null");
        }

        Context appContext = context.getApplicationContext();
        helper = constructHelper(appContext, openHelperClass);
        logger.debug("Zero instances.  Created helper.", new Object[0]);
        instanceCount = 0;

        ++instanceCount;
        logger.debug("helper instance count = {} ", new Object[]{instanceCount});
        T castHelper = (T)helper;
        return castHelper;
    }

    private static OrmLiteSqliteOpenHelper constructHelper(Context context, Class<? extends OrmLiteSqliteOpenHelper> openHelperClass) {
        Constructor constructor;
        try {
            constructor = openHelperClass.getConstructor(Context.class);
        } catch (Exception var5) {
            var5.printStackTrace();
            throw new IllegalStateException("Could not find constructor that takes a Context argument for helper class " + openHelperClass, var5);
        }

        try {
            return (OrmLiteSqliteOpenHelper)constructor.newInstance(context);
        } catch (Exception var4) {
            throw new IllegalStateException("Could not construct instance of helper class " + openHelperClass, var4);
        }
    }

    private static Class<? extends OrmLiteSqliteOpenHelper> lookupHelperClass(Context context, Class<?> componentClass) {
        Resources resources = context.getResources();
        int resourceId = resources.getIdentifier("open_helper_classname", "string", context.getPackageName());
        if (resourceId != 0) {
            String className = resources.getString(resourceId);

            try {
                Class<? extends OrmLiteSqliteOpenHelper> castClass = (Class<? extends OrmLiteSqliteOpenHelper>)Class.forName(className);
                return castClass;
            } catch (Exception var13) {
                throw new IllegalStateException("Could not create helper instance for class " + className, var13);
            }
        } else {
            for(Class componentClassWalk = componentClass; componentClassWalk != null; componentClassWalk = componentClassWalk.getSuperclass()) {
                Type superType = componentClassWalk.getGenericSuperclass();
                if (superType != null && superType instanceof ParameterizedType) {
                    Type[] types = ((ParameterizedType)superType).getActualTypeArguments();
                    if (types != null && types.length != 0) {
                        Type[] arr$ = types;
                        int len$ = types.length;

                        for(int i$ = 0; i$ < len$; ++i$) {
                            Type type = arr$[i$];
                            if (type instanceof Class) {
                                Class<? extends OrmLiteSqliteOpenHelper> clazz = (Class)type;
                                if (OrmLiteSqliteOpenHelper.class.isAssignableFrom(clazz)) {
                                    return clazz;
                                }
                            }
                        }
                    }
                }
            }

            throw new IllegalStateException("Could not find OpenHelperClass because none of the generic parameters of class " + componentClass + " extends OrmLiteSqliteOpenHelper.  You should use getHelper(Context, Class) instead.");
        }
    }
}
