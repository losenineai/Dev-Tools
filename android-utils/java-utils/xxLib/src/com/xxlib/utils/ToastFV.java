package com.xxlib.utils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.widget.TextView;

import com.xxlib.R;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ToastFV {
	private static final String TAG = "ToastFV";

	public static class ToastModel {
		String text;
		int duration;
	}

	/**
	 * 排除重复大量的弹taost
	 */
	private static String mLastToast;

	private static Queue<ToastModel> mToastQueue;
	private static Context mContext;
	private static Handler mHandler;
	private static WindowManager mWindowManager;
	private static TextView mToastText;
	private static View mToastView;
	private static float mDp;

	private static boolean mIsStart = false;
	private static boolean mIsAddView = false;

	private static final int TOAST_SHOW = 101;
	private static final int TOAST_HIDE = 102;

	public static void init(Context context) {
		mContext = context;
		mLastToast = "";
		mToastQueue = new ConcurrentLinkedQueue<ToastModel>();
		mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
		buildTextView();
		mHandler = new Handler(context.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				switch (msg.what) {
				case TOAST_HIDE:
					dotoHideToast();
					break;
				case TOAST_SHOW:
					dotoShowToast((ToastModel) msg.obj);
					break;
				default:
					break;
				}
			}
		};
		DisplayMetrics metrics = new DisplayMetrics();
		mWindowManager.getDefaultDisplay().getMetrics(metrics);
		mDp = metrics.density;
	}

	protected static void dotoHideToast() {
		mLastToast = "";
		mToastView.setVisibility(View.GONE);
		mIsAddView = false;
		mWindowManager.removeView(mToastView);
	}

	protected static void dotoShowToast(ToastModel toast) {
		mToastText.setText(toast.text);
		if (!mIsAddView) {
			LayoutParams params = new LayoutParams();
			params.type = LayoutParams.TYPE_PHONE;
			params.format = PixelFormat.RGBA_8888;
			params.flags = LayoutParams.FLAG_NOT_TOUCH_MODAL
					| LayoutParams.FLAG_NOT_FOCUSABLE;
			params.gravity = Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM;
			params.width = LayoutParams.WRAP_CONTENT;
			params.height = LayoutParams.WRAP_CONTENT;
			params.y = (int) (30 * mDp);
			mWindowManager.addView(mToastView, params);
			mToastView.setVisibility(View.VISIBLE);
			mIsAddView = true;
		} else {
			mToastView.setVisibility(View.VISIBLE);
		}
	}

	@SuppressLint("InflateParams")
	private static void buildTextView() {
		mToastView = LayoutInflater.from(mContext).inflate(R.layout.layout_toast, null);
		mToastText = (TextView) mToastView.findViewById(R.id.tv_toast);
	}

	public static void showToastCustom(LayoutParams params, final View view, int duration) {
		mWindowManager.addView(view, params);
		mHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				mWindowManager.removeView(view);
			}
		}, duration);
	}

	public static void show(String text, int duration) {
		if (text.equals(mLastToast)) {
			return;
		}
		mLastToast = text;

		ToastModel toast = new ToastModel();
		toast.text = text;
		toast.duration = duration;
		mToastQueue.offer(toast);
		start();
	}

	public static void show(String text) {
		show(text, 2000);
	}

	public static void show(int resId) {
		show(mContext.getResources().getString(resId), 2000);
	}

	private synchronized static void start() {
		if (mIsStart) {
			return;
		}
		mIsStart = true;
		new Thread() {
			@Override
			public void run() {
				super.run();
				while (!mToastQueue.isEmpty()) {
					Message msg = mHandler.obtainMessage();
					msg.what = TOAST_SHOW;
					ToastModel toast = mToastQueue.poll();
					msg.obj = toast;
					mHandler.sendMessage(msg);
					try {
						sleep(toast.duration);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					mHandler.sendEmptyMessage(TOAST_HIDE);
				}
				mIsStart = false;
			}
		}.start();
	}

}
