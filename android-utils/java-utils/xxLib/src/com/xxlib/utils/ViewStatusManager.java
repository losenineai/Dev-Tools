package com.xxlib.utils;

import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

import java.util.ArrayList;

/**
 * 控制状态视图的显示与隐藏
 * 
 * @Description
 * @author guokai
 */
public class ViewStatusManager {

	// 状态改变监听器, 用于扩展
	public interface StatusChangeListener {
		public void onStatusChange(ViewStatus status);
	}

	// 视图的各种状态, 只能处于其中的一种
	public enum ViewStatus {
		Loading, Sending, NoNet, NoData, Finish, HotKey/*热词*/;
	}

	private ViewStatus mStatus = null;
	private View mLoading, mSending, mNoNet, mNoData, mFinish, mHotKey;
	private Runnable mClickNoNet, mClickNoData;
	private ArrayList<StatusChangeListener> mListeners = new ArrayList<StatusChangeListener>();

	private void showIfNotNull(View v) {
		if (v != null) {
			v.setVisibility(View.VISIBLE);
		}
	}

	private void hideIfNotNull(View v) {
		if (v != null) {
			v.setVisibility(View.GONE);
		}
	}

	// 通过builder创建
	private ViewStatusManager(View loading, View sending, View nonet,
							  View nodata, View finish, View hotkey, Runnable clickNoNet, Runnable clickNoData) {
		mLoading = loading;
		mSending = sending;
		mNoNet = nonet;
		mNoData = nodata;
		mFinish = finish;
		mHotKey = hotkey;
		mClickNoNet = clickNoNet;
		mClickNoData = clickNoData;

		if (mLoading != null)
			mLoading.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					return true;
				}
			});

		if (mSending != null)
			mSending.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					return true;
				}
			});

		if (mNoNet != null)
			mNoNet.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					if (mClickNoNet != null) {
						mClickNoNet.run();
					}
					return true;
				}
			});

		if (mNoData != null)
			mNoData.setOnTouchListener(new OnTouchListener() {

				@Override
				public boolean onTouch(View v, MotionEvent event) {
					if (mClickNoData != null) {
						mClickNoData.run();
					}
					return true;
				}
			});
	}

	// 核心方法
	public void status(ViewStatus s) {
		mStatus = s;
		hideIfNotNull(mLoading);
		hideIfNotNull(mSending);
		hideIfNotNull(mNoNet);
		hideIfNotNull(mNoData);
		hideIfNotNull(mFinish);
		hideIfNotNull(mHotKey);

		switch (s) {
		case Loading:
			showIfNotNull(mLoading);
			break;
		case Sending:
			showIfNotNull(mSending);
			break;
		case NoNet:
			showIfNotNull(mNoNet);
			break;
		case NoData:
			showIfNotNull(mNoData);
			break;
		case Finish:
			showIfNotNull(mFinish);
		case HotKey:
			showIfNotNull(mHotKey);
			break;
		}

		// 扩展
		for (StatusChangeListener listener : mListeners) {
			listener.onStatusChange(s);
		}
	}

	public boolean isLoading() {
		if (mSending != null && mSending.getVisibility() == View.VISIBLE) {
			return true;
		}

		if (mLoading != null && mLoading.getVisibility() == View.VISIBLE) {
			return true;
		}

		return false;
	}

	public ViewStatus getStatus() {
		return mStatus;
	}

	public void addStatusListener(StatusChangeListener listener) {
		if (listener != null) {
			mListeners.add(listener);
		}
	}

	public void removeStatusListener(StatusChangeListener listener) {
		if (listener != null) {
			mListeners.remove(listener);
		}
	}
	
	public static class Builder{
		private View mLoading, mSending, mNoNet, mNoData, mFinish, mHotKey;
		private Runnable mClickNoNet, mClickNoData;
		
		public Builder setLoadingView(View view) {
			mLoading=view;
			return this;
		}
		
		public Builder setSendingView(View view) {
			mSending=view;
			return this;
		}
		
		public Builder setNoNetView(View view) {
			mNoNet=view;
			return this;
		}
		
		public Builder setNoDataView(View view) {
			mNoData=view;
			return this;
		}
		
		public Builder setFinishView(View view) {
			mFinish=view;
			return this;
		}
		
		public Builder setHotKeyView(View view) {
			mHotKey=view;
			return this;
		}
		
		public Builder setNoNetClick(Runnable runnable) {
			mClickNoNet=runnable;
			return this;
		}
		
		public Builder setNoDataClick(Runnable runnable) {
			mClickNoData=runnable;
			return this;
		}
		
		public ViewStatusManager build() {
			return new ViewStatusManager(mLoading, mSending, mNoNet, mNoData, mFinish, mHotKey, mClickNoNet, mClickNoData);
		}
	}
	
}
