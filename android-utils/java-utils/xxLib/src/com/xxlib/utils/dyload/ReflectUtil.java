package com.xxlib.utils.dyload;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Created by mushroom0417 on 8/10/15.
 */
@SuppressWarnings({"unchecked", "rawtypes"})
public class ReflectUtil {

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

    public static Object invokeMethod(Object instance, String methodName, Object[] args) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException {
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
        return getMethod(instance, methodName, classTypes).invoke(instance, args);
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

    public static Object invokeMethod(String className, String methodName, ClassLoader loader, Object[] args) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, ClassNotFoundException {
        Class classObject = Class.forName(className, true, loader); // class的全名(包名+类名)
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

    public static boolean setField(Object instance, Class targetClass, String fieldName, Object value) {
        try {
            Field field;
            if (targetClass == null)
                field = instance.getClass().getDeclaredField(fieldName);
            else
                field = targetClass.getDeclaredField(fieldName);
            field.setAccessible(true);
            field.set(instance, value);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static Object getStaticFieldValue(String className, String fieldName) {
        Object result = null;
        try {
            Class<?> targetClass = ReflectUtil.class.getClassLoader().loadClass(className);
            if (null != targetClass) {
                Field targetField = targetClass.getField(fieldName);
                targetField.setAccessible(true);
                return targetField.get(null);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }


    public static Object getField(Object instance, Class targetClass, String fieldName) {
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
            e.printStackTrace();
        }
        return result;
    }

}

