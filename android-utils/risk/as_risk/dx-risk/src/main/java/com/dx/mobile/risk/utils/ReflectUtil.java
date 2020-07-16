package com.dx.mobile.risk.utils;

import java.lang.reflect.*;

/**
 * Created by white on 8/10/15.
 */
public class ReflectUtil {
    
    public static final boolean LOG_OPEN = false;

    public static Method getMethod(Object instance, String methodName, Class[] classTypes) throws NoSuchMethodException {
        Method accessMethod = getMethod(instance.getClass(), methodName, classTypes);
        //参数值为true，禁用访问控制检查
        accessMethod.setAccessible(true);
        return accessMethod;
    }

    private static Method getMethod(Class thisClass, String methodName, Class[] classTypes) throws NoSuchMethodException {
        if (thisClass == null) {
            throw new NoSuchMethodException("Error method !");
        }

        try {
            return thisClass.getDeclaredMethod(methodName, classTypes);
        } catch (NoSuchMethodException e) {
            return getMethod(thisClass.getSuperclass(), methodName, classTypes);
        }
    }

    public static Object invokeMethod(Object instance, String methodName, Object... args) {
        Class[] classTypes = null;
        if (args != null) {
            classTypes = new Class[args.length];
            for (int i = 0; i < args.length; i++) {
                if (args[i] != null) {
                    if (args[i].getClass().equals(Integer.class))
                        classTypes[i] = int.class;
                    else if (args[i].getClass().equals(Float.class))
                        classTypes[i] = float.class;
                    else if (args[i].getClass().equals(Double.class))
                        classTypes[i] = double.class;
                    else if (args[i].getClass().equals(Boolean.class))
                        classTypes[i] = boolean.class;
                    else if (args[i].getClass().equals(Character.class))
                        classTypes[i] = char.class;
                    else if (args[i].getClass().equals(Byte.class))
                        classTypes[i] = byte.class;
                    else if (args[i].getClass().equals(Short.class))
                        classTypes[i] = short.class;
                    else if (args[i].getClass().equals(Long.class))
                        classTypes[i] = long.class;
                    else
                        classTypes[i] = args[i].getClass();
                }
            }
        }
        try {
            return getMethod(instance, methodName, classTypes).invoke(instance, args);
        } catch (IllegalAccessException e) {
            if (LOG_OPEN)
                e.printStackTrace();
        } catch (InvocationTargetException e) {
            if (LOG_OPEN)
                e.printStackTrace();
        } catch (NoSuchMethodException e) {
            if (LOG_OPEN)
                e.printStackTrace();
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return null;
    }

    public static Object invokeMethod(String className, String methodName, Object[] args) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, ClassNotFoundException {
        Class classObject = Class.forName(className); // class的全名(包名+类名)
        Class[] classTypes = null;
        if (args != null) {
            classTypes = new Class[args.length];
            for (int i = 0; i < args.length; i++) {
                if (args[i] != null) {
                    if (args[i].getClass().equals(Integer.class))
                        classTypes[i] = int.class;
                    else if (args[i].getClass().equals(Float.class))
                        classTypes[i] = float.class;
                    else if (args[i].getClass().equals(Double.class))
                        classTypes[i] = double.class;
                    else if (args[i].getClass().equals(Boolean.class))
                        classTypes[i] = boolean.class;
                    else if (args[i].getClass().equals(Character.class))
                        classTypes[i] = char.class;
                    else if (args[i].getClass().equals(Byte.class))
                        classTypes[i] = byte.class;
                    else if (args[i].getClass().equals(Short.class))
                        classTypes[i] = short.class;
                    else if (args[i].getClass().equals(Long.class))
                        classTypes[i] = long.class;
                    else
                        classTypes[i] = args[i].getClass();
                }
            }
        }
        Method method = classObject.getDeclaredMethod(methodName, classTypes);
        method.setAccessible(true);
        return method.invoke(classObject, args);
    }

    public static Object invokeMethod(Object object, String methodName, ClassLoader loader, Object[] args)  {
        if (object == null) {
            return null;
        }

        try {
            Class classObject = object.getClass();
            Class[] classTypes = null;
            if (args != null) {
                classTypes = new Class[args.length];
                for (int i = 0; i < args.length; i++) {
                    if (args[i] != null) {
                        if (args[i].getClass().equals(Integer.class))
                            classTypes[i] = int.class;
                        else if (args[i].getClass().equals(Float.class))
                            classTypes[i] = float.class;
                        else if (args[i].getClass().equals(Double.class))
                            classTypes[i] = double.class;
                        else if (args[i].getClass().equals(Boolean.class))
                            classTypes[i] = boolean.class;
                        else if (args[i].getClass().equals(Character.class))
                            classTypes[i] = char.class;
                        else if (args[i].getClass().equals(Byte.class))
                            classTypes[i] = byte.class;
                        else if (args[i].getClass().equals(Short.class))
                            classTypes[i] = short.class;
                        else if (args[i].getClass().equals(Long.class))
                            classTypes[i] = long.class;
                        else
                            classTypes[i] = args[i].getClass();
                    }
                }
            }
            Method method = classObject.getDeclaredMethod(methodName, classTypes);
            method.setAccessible(true);
            return method.invoke(object, args);
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return null;
    }

    public static boolean setFieldValue(Object instance, String fieldName, Object value) {
        if (instance == null) {
            return false;
        }
        return setFieldValue(instance, instance.getClass(), fieldName, value);
    }

    public static boolean setFieldValue(Object instance, Class targetClass, String fieldName, Object value) {
        try {
            Field field;
            if (targetClass == null)
                field = instance.getClass().getDeclaredField(fieldName);
            else
                field = targetClass.getDeclaredField(fieldName);
            field.setAccessible(true);
            field.set(instance, value);
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
            return false;
        }
        return true;
    }

    public static Object getStaticFieldValue(String className, String fieldName) {
        try {
            Class<?> targetClass = ReflectUtil.class.getClassLoader().loadClass(className);
            return getStaticFieldValue(targetClass, fieldName);
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return null;
    }

    public static Object getStaticFieldValue(Class<?> targetClass, String fieldName) {
        Object result = null;
        try {
            if (null != targetClass) {
                Field targetField = targetClass.getDeclaredField(fieldName);
                targetField.setAccessible(true);
                return targetField.get(null);
            }
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return result;
    }

    public static Object getFieldValue(Object instance, String fieldName) {
        if (instance == null) {
            return null;
        }
        return getFieldValue(instance, instance.getClass(), fieldName);
    }

    public static Object getFieldValue(Object instance, Class targetClass, String fieldName) {
        Object result = null;
        try {
            Field field;
            if (targetClass == null)
                field = instance.getClass().getDeclaredField(fieldName);
            else
                field = targetClass.getDeclaredField(fieldName);
            field.setAccessible(true);
            result = field.get(instance);
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return result;
    }


    /**
     * <pre>
     * 用此方法有一个前提:
     * 参数类必须有显示声明的无参构造器或没有任何显示声明的构造器
     * </pre>
     */
    public static <T> T getInstance(Class<? extends T> clazz) {
        return getInstance(clazz, new Object[0]);
    }

    /**
     * <pre>
     * 用此方法有两个前提:
     * 1. 参数数组的项都不能为null
     * 2. 参数数组的项都不能是简单数据类型
     * </pre>
     */
    public static <T> T getInstance(Class<? extends T> clazz, Object... args) {
        Class<?>[] parameterTypes = new Class<?>[args.length];
        for (int i = 0; i < args.length; i++) {
            parameterTypes[i] = args[i].getClass();
        }
        return getInstance(clazz, parameterTypes, args);
//        return clazz.cast(object);
    }

    /**
     * <pre>
     * 用此方法有三个前提:
     * 1. 两个参数数组同时为null或同时不为null
     * 2. 两个数组的长度必须相等
     * 3. parameterTypes如果不空null, 则其各元素不能为null
     * </pre>
     */
    public static <T> T getInstance(Class<? extends T> clazz, Class<?>[] parameterTypes, Object[] args) {
        try {
            if (parameterTypes != null && args != null) {
                if (parameterTypes.length == args.length) {
                    Constructor<?> constructor = clazz.getDeclaredConstructor(parameterTypes);
                    constructor.setAccessible(true);
                    T instance = clazz.cast(constructor.newInstance(args));
                    return instance;
                } else {
                    throw new IllegalArgumentException("参数个数不匹配");
                }
            } else if (parameterTypes == null && args == null) {
                T instance = clazz.newInstance();
                return instance;
            } else {
                throw new IllegalArgumentException("两个参数数组必须同时为null或同时不为null");
            }
        } catch (Exception e) {
            if (LOG_OPEN)
                e.printStackTrace();
        }
        return null;
    }


    /***
     * 获取泛型数据类型
     * @param tClass
     * @return
     */
    public static Class getTClass(Class<?> tClass){
        Class<?> entityClass = (Class<?>) ((ParameterizedType) tClass.getGenericInterfaces()[0]).getActualTypeArguments()[0];

        return entityClass;
    }

}

