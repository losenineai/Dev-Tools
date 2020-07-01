package com.xxlib.utils;

import android.content.Context;
import android.os.Build;
import android.os.Handler;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

/**
 * Created by rosejames on 15/7/15.
 *
 * @description 直接在主进程显示的Toast，不需要再特地在主线程中弹出了
 */
public class ToastUtils {

    public static void showShortToast(final Context context, final String text) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, text, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public static void showLongToast(final Context context, final String text) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, text, Toast.LENGTH_LONG).show();
            }
        });
    }

    public static void showShortToast(final Context context, final int resId) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, resId, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public static void showLongToast(final Context context, final int resId) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, resId, Toast.LENGTH_LONG).show();
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

    public static void show(Context context, CharSequence text, boolean isFloatView) {
        if (isFloatView) {
            ToastFV.show(text.toString());
        } else {
            show(context, text);
        }
    }

    public static void show(final Context context, final CharSequence text, final int duration) {
        new Handler(context.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context, text, duration).show();
            }
        });
    }

    public static void show(CharSequence text, int duration) {
        show(ContextFinder.getApplication(), text, duration);
    }

    public static void show(CharSequence text) {
        show(text, Toast.LENGTH_SHORT);
    }


    public static void show(int resId) {
        show(ApplicationUtils.getApplication().getResources().getString(resId), Toast.LENGTH_SHORT);
    }

    public static void showToastCustom(Context context, final WindowManager windowManager, WindowManager.LayoutParams params, final View view, int duration) {
        try {
            params.type = WindowManager.LayoutParams.TYPE_APPLICATION;
            params.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
            if (Build.VERSION.SDK_INT >= 19)
                params.flags |= WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS;
            windowManager.addView(view, params);
            new Handler(context.getMainLooper()).postDelayed(new Runnable() {
                @Override
                public void run() {
                    windowManager.removeView(view);
                }
            }, duration);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


//	public static void show(Context context, int resId, Object... args) {
//		show(context, String.format(context.getResources().getString(resId), args), Toast.LENGTH_SHORT);
//	}
//
//	public static void show(Context context, String format, Object... args) {
//		show(context, String.format(format, args), Toast.LENGTH_SHORT);
//	}
//
//	public static void show(Context context, int resId, int duration, Object... args) {
//		show(context, String.format(context.getResources().getString(resId), args), duration);
//	}
//
//	public static void show(Context context, String format, int duration, Object... args) {
//		show(context, String.format(format, args), duration);
//	}
}
