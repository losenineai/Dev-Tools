package com.xxlib.utils;

import android.app.Activity;
import android.graphics.Rect;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.FrameLayout;

import com.xxlib.utils.base.LogTool;

public class KeyBoardUtils {
    private static final String TAG = "KeyBoardUtils";

    // For more information, see https://code.google.com/p/android/issues/detail?id=5497
    // To use this class, simply invoke assistActivity() on an Activity that already has its content view set.

    private View mChildOfContent;
    private FrameLayout mContent;
    private int usableHeightPrevious;
    private int contentOriginalHeight;
    private FrameLayout.LayoutParams frameLayoutParams;
    private OnKeyBoardStateChangeListener mOnKeyBoardStateChangeListener;
    private boolean mEnable;

    public KeyBoardUtils(Activity activity) {
        mContent = (FrameLayout) activity.findViewById(android.R.id.content);
        mChildOfContent = mContent.getChildAt(0);
        if (mChildOfContent != null) {
            frameLayoutParams = (FrameLayout.LayoutParams) mChildOfContent.getLayoutParams();
            contentOriginalHeight = mChildOfContent.getHeight();
            LogTool.i(TAG, "(constructor)child of content height = " + contentOriginalHeight);
        }
    }

    public void enable() {
        if (mChildOfContent != null) {
            mChildOfContent.getViewTreeObserver().addOnGlobalLayoutListener(mListener);
            mEnable = true;
        }
    }

    public void disable() {
        if (mChildOfContent != null) {
            mChildOfContent.getViewTreeObserver().removeGlobalOnLayoutListener(mListener);
            mEnable = false;
        }
    }

    public boolean isEnable() {
        return mEnable;
    }

    public void resume() {
        if (mChildOfContent != null)
            mChildOfContent.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mIsInputMethodOpen = false;
                    LogTool.i(TAG, "resume");
                    frameLayoutParams.height = FrameLayout.LayoutParams.MATCH_PARENT;
                    mChildOfContent.requestLayout();
                    usableHeightPrevious = -1;
                    contentOriginalHeight = mChildOfContent.getHeight();
                    possiblyResizeChildOfContent();
                    LogTool.i(TAG, "(on resume)child of content height = " + contentOriginalHeight);
                }
            }, 300);

    }

    public void registerOnKeyBoardStateChangeListener(OnKeyBoardStateChangeListener listener) {
        this.mOnKeyBoardStateChangeListener = listener;
    }

    public void unregisterOnKeyBoardStateChangeListener() {
        this.mOnKeyBoardStateChangeListener = null;
    }

    private boolean mIsInputMethodOpen = false;

    private ViewTreeObserver.OnGlobalLayoutListener mListener = new ViewTreeObserver.OnGlobalLayoutListener() {
        public void onGlobalLayout() {
            possiblyResizeChildOfContent();
        }
    };

    private void possiblyResizeChildOfContent() {
        int usableHeightNow = computeUsableHeight();

        LogTool.i(TAG, "onGlobalLayout :" + usableHeightNow + ", " + usableHeightPrevious);
        if (usableHeightNow != usableHeightPrevious) {
            if (usableHeightNow < contentOriginalHeight) {
                // keyboard probably just became visible
                mIsInputMethodOpen = true;
                if (mOnKeyBoardStateChangeListener != null)
                    mOnKeyBoardStateChangeListener.onKeyBoardShow();
                /*frameLayoutParams.topMargin = usableHeightPrevious - usableHeightNow;
                if (frameLayoutParams.topMargin < 0)
                    frameLayoutParams.topMargin = 0;*/
                frameLayoutParams.height = contentOriginalHeight;
                mChildOfContent.requestLayout();
                frameLayoutParams.height = usableHeightNow;
            } else {
                mIsInputMethodOpen = false;
                if (mOnKeyBoardStateChangeListener != null)
                    mOnKeyBoardStateChangeListener.onKeyBoardHide();
                // keyboard probably just became hidden
                /*frameLayoutParams.topMargin = 0;*/
                frameLayoutParams.height = FrameLayout.LayoutParams.MATCH_PARENT;
            }
            mChildOfContent.requestLayout();
            usableHeightPrevious = usableHeightNow;
        }
    }

    private int computeUsableHeight() {
        Rect r = new Rect();
        mChildOfContent.getWindowVisibleDisplayFrame(r);
        return (r.bottom - r.top);
    }

    public boolean isInputMethodOpen() {
        return mIsInputMethodOpen;
    }

    public interface OnKeyBoardStateChangeListener {
        void onKeyBoardShow();
        void onKeyBoardHide();
    }
}