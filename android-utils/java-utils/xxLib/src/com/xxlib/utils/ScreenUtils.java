package com.xxlib.utils;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Display;
import android.view.WindowManager;

import java.lang.reflect.Field;


/**
 * ScreenUtils
 * <ul>
 * <strong>Convert between dp and sp</strong>
 * <li>{@link ScreenUtils#dpToPx(Context, float)}</li>
 * <li>{@link ScreenUtils#pxToDp(Context, float)}</li>
 * </ul>
 *
 * @author <a href="http://www.trinea.cn" target="_blank">Trinea</a> 2014-2-14
 */
public class ScreenUtils {

    private static float mDp;
    private static int mScreenWidth;
    private static int mScreenHeight;
    private static boolean mIsInit = false;

    public static float getDp() {
        if (!mIsInit) {
            initData();
        }
        return mDp;
    }

    public static int getScreenWidth() {
        if (!mIsInit) {
            initData();
        }
        return mScreenWidth;
    }

    public static int getScreenHeight() {
        if (!mIsInit) {
            initData();
        }
        return mScreenHeight;
    }

    private static void initData() {
        mIsInit = true;
        WindowManager wm = (WindowManager) ApplicationUtils.getApplication().getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);
        mDp = dm.density;
        mScreenHeight = dm.heightPixels;
        mScreenWidth = dm.widthPixels;
    }

    public static float dpToPx(Context context, float dp) {
        if (context == null) {
            return -1;
        }
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, context.getResources().getDisplayMetrics());
//		return dp * context.getResources().getDisplayMetrics().density;
    }

    public static float pxToDp(Context context, float px) {
        if (context == null) {
            return -1;
        }
        return px / context.getResources().getDisplayMetrics().density;
    }

    public static int dpToPxInt(Context context, float dp) {
        return (int) (dpToPx(context, dp) + 0.5f);
    }

    public static float pxToDpCeilInt(Context context, float px) {
        return (int) (pxToDp(context, px) + 0.5f);
    }


    /**
     * wxj，用于获取系统状态栏的高度。
     *
     * @return 返回状态栏高度的像素值。
     */
    public static int getStatusBarHeight(Context context) {
        if (context == null) {
            return 0;
        }
        int height = 0;

        try {
            Class<?> c = Class.forName("com.android.internal.R$dimen");
            Object o = c.newInstance();
            Field field = c.getField("status_bar_height");
            int x = (Integer) field.get(o);
            height = context.getResources().getDimensionPixelSize(x);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return height;
    }


    /**
     * 设置是否显示或者隐藏系统状态栏
     *
     * @param flag
     */
    public static void showStatusBar(Activity acti, boolean flag) {
        WindowManager.LayoutParams lp = acti.getWindow().getAttributes();
        if (flag) {
            lp.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
//			getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        } else {
            lp.flags &= ~WindowManager.LayoutParams.FLAG_FULLSCREEN;
//			getWindow().clearFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        }
        acti.getWindow().setAttributes(lp);
    }

    /**
     * 当前界面是否为竖屏
     *
     * @param context
     * @return
     * @description
     */
    public static boolean isWindowVertical(Context context) {
        if (context == null) {
            return false;
        }

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
//		int rotation = wm.getDefaultDisplay().getRotation();
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);

        return metrics.heightPixels > metrics.widthPixels;
    }

    private static int screenWidth;
    private static int screenHeight;

    /**
     * 获取屏幕高度
     *
     * @param mContext
     * @return
     */
    public static int getScreenHeight(Context mContext) {

        if (screenHeight != 0)
            return screenHeight;
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        screenWidth = wm.getDefaultDisplay().getWidth();
        screenHeight = wm.getDefaultDisplay().getHeight();
        return screenHeight;
    }

    /**
     * 获取屏幕高度
     *
     * @param mContext
     * @return
     */
    public static int getScreenWidth(Context mContext) {

        if (screenWidth != 0)
            return screenWidth;
        WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        screenWidth = wm.getDefaultDisplay().getWidth();
        screenHeight = wm.getDefaultDisplay().getHeight();
        return screenWidth;
    }


    private static float screenMDc = 0;

    /**
     * 获取屏幕密度
     *
     * @param mContext
     * @return
     */
    public static float getScreenMdp(Context mContext) {
        if (screenMDc != 0)
            return screenMDc;
        WindowManager windowManger = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        Display mDs = windowManger.getDefaultDisplay();
        DisplayMetrics mDc = new DisplayMetrics();
        mDs.getMetrics(mDc);
        screenMDc = mDc.density;
        return screenMDc;
    }

    public static float dp2px(Resources resources, float dp) {
        final float scale = resources.getDisplayMetrics().density;
        return  dp * scale + 0.5f;
    }

    public static float sp2px(Resources resources, float sp){
        final float scale = resources.getDisplayMetrics().scaledDensity;
        return sp * scale;
    }



    private static int sScreenRealWidth;
    private static int sScreenRealHeight;

    /**
     * 包括虚拟按键，用于查看屏幕分辨率
     * @param context
     * @return
     */
    public static int getScreenRealWidth(Context context){
        if(sScreenRealWidth > 0){
            return sScreenRealWidth;
        }
        if(context == null){
            return 0;
        }
        WindowManager windowManger = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics displayMetrics = new DisplayMetrics();
        if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1){
            windowManger.getDefaultDisplay().getRealMetrics(displayMetrics);
        }
        else{
            windowManger.getDefaultDisplay().getMetrics(displayMetrics);
        }
        sScreenRealWidth = displayMetrics.widthPixels;
        sScreenRealHeight = displayMetrics.heightPixels;

        return sScreenRealWidth;
    }

    /**
     * 包括虚拟按键，用于查看屏幕分辨率
     * @param context
     * @return
     */
    public static int getScreenRealHeight(Context context){
        if(sScreenRealHeight > 0){
            return sScreenRealHeight;
        }
        if(context == null){
            return 0;
        }
        WindowManager windowManger = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics displayMetrics = new DisplayMetrics();
        if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1){
            windowManger.getDefaultDisplay().getRealMetrics(displayMetrics);
        }
        else{
            windowManger.getDefaultDisplay().getMetrics(displayMetrics);
        }
        sScreenRealWidth = displayMetrics.widthPixels;
        sScreenRealHeight = displayMetrics.heightPixels;

        return sScreenRealHeight;
    }


    /**
     * 关闭、打开屏幕
     */
    private void setScreenOn(Activity activity, boolean on)
    {
        if(activity == null){
            return;
        }
        WindowManager wm = (WindowManager)activity.getSystemService(Context.WINDOW_SERVICE);
        WindowManager.LayoutParams params = activity.getWindow().getAttributes();
        params.flags |= WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON;
        params.screenBrightness = on ? 1 : 0.005f;
        activity.getWindow().setAttributes(params);
    }




}
