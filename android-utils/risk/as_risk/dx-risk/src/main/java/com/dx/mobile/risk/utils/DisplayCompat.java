package com.dx.mobile.risk.utils;

import android.util.DisplayMetrics;
import android.view.Display;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


/**
 * 针对Display类的兼容性处理
 */
public final class DisplayCompat {
    private static Method s_getRealMetrics_m = null;
    private static Method s_getName_m = null;

    static {
        try{
            s_getRealMetrics_m = Display.class.getDeclaredMethod("getRealMetrics", DisplayMetrics.class);
            s_getName_m = Display.class.getDeclaredMethod("getName");
        }catch (NoSuchMethodException e){
//            e.printStackTrace();
        }
    }

    private Display mDisplay;

    public DisplayCompat(Display display){
        mDisplay = display;
    }

    public void getRealMetrics(DisplayMetrics metrics){
        if(s_getRealMetrics_m != null){
            try{
                s_getRealMetrics_m.invoke(mDisplay, metrics);
            }catch (InvocationTargetException e){
//                e.printStackTrace();
            }catch (Exception e){
//                e.printStackTrace();
            }
        }else{
            mDisplay.getMetrics(metrics);
        }
    }

    public String getName(){
        if(s_getName_m != null){

            try{
                s_getName_m.invoke(mDisplay);
            }catch (InvocationTargetException e){
//                e.printStackTrace();
            }catch (Exception e){
//                e.printStackTrace();
            }
            return "nil";
        }else{
            return "nil";
        }
    }
}
