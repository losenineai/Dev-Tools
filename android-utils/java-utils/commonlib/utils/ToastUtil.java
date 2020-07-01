package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.os.Handler;
import android.view.Gravity;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.auric.intell.commonlib.R;

/**
 * Created by white on 15/7/15.
 *
 * @description 直接在主进程显示的Toast，不需要再特地在主线程中弹出了
 */
public class ToastUtil {
    private static Toast mToast;
    public static void showShortToast(final Context context, final String text) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast=Toast.makeText(context, text, Toast.LENGTH_SHORT);
                mToast.show();
            }
        });
    }

    public static void showLongToast(final Context context, final String text) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast=Toast.makeText(context, text, Toast.LENGTH_LONG);
                mToast.show();
            }
        });
    }

    public static void showShortToast(final Context context, final int resId) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast=Toast.makeText(context, resId, Toast.LENGTH_SHORT);
                mToast.show();
            }
        });
    }

    public static void showLongToast(final Context context, final int resId) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast=Toast.makeText(context, resId, Toast.LENGTH_LONG);
                mToast.show();
            }
        });
    }

    public static void show(Context context, int resId) {
        try {
            show(context, context.getResources().getText(resId), Toast.LENGTH_SHORT);
        } catch (Exception e) {
        }

    }

    public static void show(Context context, int resId, int duration) {
        try {
            show(context, context.getResources().getText(resId), duration);
        } catch (Exception e) {
        }
    }

    public static void show(Context context, CharSequence text) {
        try {
            show(context, text, Toast.LENGTH_SHORT);
        } catch (Exception e) {
        }
    }

    public static void show(final Context context, final CharSequence text, final int duration) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast=Toast.makeText(context, text, duration);
                mToast.show();
            }
        });
    }

    public static void show(CharSequence text, int duration) {
        show(ContextFinder.getApplication(), text, duration);
    }

    public static void show(CharSequence text) {
        show(text, Toast.LENGTH_SHORT);
    }

    public static void cancel(){
        if(mToast!=null)
            mToast.cancel();
    }

    /**
     * 带图片Toast
     * @param text
     */
    public static void showImage(CharSequence text){
        showImage(text, R.drawable.customactivityoncrash_error_image);
    }

    public static void showImage(final CharSequence text, final int resId){
        new Handler(ContextFinder.getApplication().getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                mToast = Toast.makeText(ContextFinder.getApplication(),"  " + text + "  ", Toast.LENGTH_SHORT);
                mToast .setGravity(Gravity.CENTER, 0, 0);
                LinearLayout toastView = (LinearLayout) mToast.getView();
                ImageView imageCodeProject = new ImageView(ContextFinder.getApplication());
                imageCodeProject.setImageResource(resId);
                toastView.addView(imageCodeProject, 0);
                mToast.show();
            }
        });

    }

}
