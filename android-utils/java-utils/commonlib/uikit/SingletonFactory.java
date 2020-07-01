package com.auric.intell.commonlib.uikit;

import java.lang.reflect.Constructor;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

public class SingletonFactory {

	public static final Logger LOG = Logger.getLogger(SingletonFactory.class.getCanonicalName());

	private static final Map<Class<?>, Object> INSTANCE_MAP = Collections.synchronizedMap(new HashMap<Class<?>, Object>());
    private static final Object[] EMPTY_ARGS = new Object[0];

	/**
	 * <pre>
	 * 用此方法有一个前提:
	 * 参数类必须有显示声明的无参构造器或没有任何显示声明的构造器
	 * </pre>
	 */
	public static <T> T getInstance(Class<? extends T> clazz) {
		return getInstance(clazz, EMPTY_ARGS);
	}

	/**
	 * <pre>
	 * 用此方法有两个前提:
	 * 1. 参数数组的项都不能为null
	 * 2. 参数数组的项都不能是简单数据类型
	 * </pre>
	 */
	public static <T> T getInstance(Class<? extends T> clazz, Object... args) {
		Object object = INSTANCE_MAP.get(clazz);
		if (object == null) {
			Class<?>[] parameterTypes = new Class<?>[args.length];
			for (int i = 0; i < args.length; i++) {
				parameterTypes[i] = args[i].getClass();
			}
			return getInstance(clazz, parameterTypes, args);
		}
		return clazz.cast(object);
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
		Object object = INSTANCE_MAP.get(clazz);
		if (object == null) { // 检查实例,如是不存在就进行同步代码区
			synchronized (clazz) { // 对其进行锁,防止两个线程同时进入同步代码区
				// 双重检查,非常重要,如果两个同时访问的线程,当第一线程访问完同步代码区后,生成一个实例;当第二个已进入getInstance方法等待的线程进入同步代码区时,也会产生一个新的实例
				if (object == null) {
					try {
						if (parameterTypes != null && args != null) {
							if (parameterTypes.length == args.length) {
								Constructor<?> constructor = clazz.getDeclaredConstructor(parameterTypes);
								constructor.setAccessible(true);
								T instance = clazz.cast(constructor.newInstance(args));
								INSTANCE_MAP.put(clazz, instance);
								return instance;
							} else {
								throw new IllegalArgumentException("参数个数不匹配");
							}
						} else if (parameterTypes == null && args == null) {
							T instance = clazz.newInstance();
							INSTANCE_MAP.put(clazz, instance);
							return instance;
						} else {
							throw new IllegalArgumentException("两个参数数组必须同时为null或同时不为null");
						}
					} catch (Exception e) {
						LOG.log(Level.SEVERE, "创建实例失败", e);
					}
				}
			}
		}
		return null;
	}
}
