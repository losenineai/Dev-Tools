package com.dx.mobile.risk.dx;

import android.util.Log;
import com.dx.mobile.risk.BuildConfig;
import com.dx.mobile.risk.utils.FileUtils;
import com.dx.mobile.risk.utils.StringUtils;

import java.io.File;
import java.lang.reflect.Method;

/**
 * @author white
 * @description：校验so是否被篡改
 * @date 2018/12/25
 */
public class c {

    private static final String TAG = "DXRisk";
    /**
     * 需要检验对比的so md5值，编译后会inline新值
     */
    private static String a = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";  // arm
    private static String b = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // arm64
    private static String c = "cccccccccccccccccccccccccccccccc";     // x86

    /**
     * TODO verify so 暂时不开放，因为耗时超过80ms
     * 原型：verifySo
     * @return true校验通过
     */
    public static boolean a(){

        long time = System.currentTimeMillis();

        boolean result = false;

        String libPath = null;

        try {
            String libName = "DXRiskComm-" + BuildConfig.VERSION_NAME;
            ClassLoader cloader = c.class.getClassLoader();
            Method fmethod = ClassLoader.class.getDeclaredMethod("findLibrary", new Class[]{String.class});
            fmethod.setAccessible(true);
            libPath = (String)fmethod.invoke(cloader, libName);
            Log.d(TAG, "libPath:"+libPath);
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }

        if (libPath != null && new File(libPath).exists()) {
            byte[] soBytes = FileUtils.readAllFromFile(libPath);
            String soMd5 = StringUtils.md5(soBytes);
//            Log.d(TAG, "soMd5:"+soMd5);

            result = a.equals(soMd5) || b.equals(soMd5) || c.equals(soMd5);
        }

        Log.d(TAG, "acost:" + (System.currentTimeMillis() - time));

        return result;
    }



}
