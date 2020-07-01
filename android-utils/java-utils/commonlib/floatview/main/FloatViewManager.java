package com.auric.intell.commonlib.floatview.main;

import android.content.Context;
import android.view.View;
import android.view.WindowManager;

import java.util.Stack;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogTool;

/**
 * 悬浮窗管理器
 * @author white
 * @date 16/7/27
 */
public class FloatViewManager {

    private static final String TAG = "FloatViewManager";
    private static FloatViewManager sInstance;

    public static FloatViewManager getInstance() {
        if (sInstance == null) {
            synchronized (FloatViewManager.class) {
                if (sInstance == null) {
                    sInstance = new FloatViewManager();
                }
            }
        }
        return sInstance;
    }

    private Context mContext;
    private IFloatViewFactory mFloatViewFactory;
    private IFloatViewBase mFirstFLoatView;
    private WindowManager mWindowManager;
    private Stack<IFloatViewBase> mStack = new Stack<IFloatViewBase>();

    /**
     * 初始化,使用悬浮窗框架前必须先init
     * @param context
     * @param floatViewFactory
     */
    public void init(Context context, IFloatViewFactory floatViewFactory) {
        mContext = context;
        mFloatViewFactory = floatViewFactory;
        mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
    }

    private FloatViewManager() {
    }

    /**
     * 获取Context
     * @return
     */
    public Context getContext() {
        return ContextFinder.getApplication();
    }

    /**
     * 新增View在上层
     * @param tag
     */
    public void addView(int tag) {
        addView(tag, null);
    }

    /**
     * 新增View在上层
     * @param tag
     * @param obj
     */
    public void addView(int tag, Object obj) {
        LogTool.d(TAG, "addView tag:"+tag);
        for (IFloatViewBase floatView : mStack) {
            if (floatView.getFloatViewTag() == tag) {
                floatView.getMainView().setVisibility(View.VISIBLE);
                return;
            }
        }
        IFloatViewBase floatViewBase = mFloatViewFactory.createFloatView(tag, obj);
        doAddView(floatViewBase);
    }

    /**
     * 移除View
     */
    public void removeView(int tag) {
        LogTool.d(TAG, "removeView");
        for (IFloatViewBase floatView : mStack) {
            if (floatView.getFloatViewTag() == tag) {
                doRemoveView(floatView);
            }
        }
    }

    /**
     * 移除View
     */
    public void hideView(int tag) {
        LogTool.d(TAG, "hideView");
        for (IFloatViewBase floatView : mStack) {
            if (floatView.getFloatViewTag() == tag) {
                floatView.getMainView().setVisibility(View.GONE);
                floatView.onViewDestroy();
            }
        }
    }

    /**
     * 移除View
     */
    public void showView(int tag) {
        LogTool.d(TAG, "showView");
        for (IFloatViewBase floatView : mStack) {
            if (floatView.getFloatViewTag() == tag) {
                floatView.getMainView().setVisibility(View.VISIBLE);
                floatView.onViewCreate();
            }
        }
    }


    /**
     * 移除头顶View,显示下一层View
     */
    public void backToLastView() {
        LogTool.d(TAG, "backToLastView");
        // 第一个不处理
        if (mStack.size() > 0 && mStack.peek() == mFirstFLoatView) {
            return;
        }
        IFloatViewBase topView = mStack.pop();
        doRemoveView(topView);
        if (mStack.size() > 0) {
            IFloatViewBase nextView = mStack.peek();
            if (nextView != mFirstFLoatView) {
                addView(nextView.getFloatViewTag());
            }
        }
    }




    /**
     * 移除所有View
     */
    public void removeAllView() {
        LogTool.d(TAG, "removeAllView");
        for (IFloatViewBase floatView : mStack) {
            doRemoveView(floatView);
        }
    }

    /**
     * 更新View
     * @param view
     * @param params
     */
    public void updateView(View view, WindowManager.LayoutParams params) {
        try {
            mWindowManager.updateViewLayout(view, params);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private void doAddView(final IFloatViewBase floatViewBase) {
        LogTool.d(TAG, "doAddView " + floatViewBase.getFloatViewTag());
        if (floatViewBase == null) {
            return;
        }
        // 硬件加速
        if (android.os.Build.VERSION.SDK_INT >= 11) {
            floatViewBase.getWinLayoutParams().flags = floatViewBase.getWinLayoutParams().flags | WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED;
        }

        try {
            mWindowManager.addView(floatViewBase.getMainView(), floatViewBase.getWinLayoutParams());
            floatViewBase.onViewCreate();
            mStack.add(floatViewBase);
            if (mStack.size() == 1) {
                mFirstFLoatView = floatViewBase;
            }
            LogTool.d(TAG, "doAddView success");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void doRemoveView(IFloatViewBase floatViewBase) {
        if (floatViewBase != null) {
            try {
                mWindowManager.removeView(floatViewBase.getMainView());
                floatViewBase.onViewDestroy();
                mStack.remove(floatViewBase);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void doHideView(IFloatViewBase floatViewBase) {
        if (floatViewBase != null) {
            floatViewBase.getMainView().setVisibility(View.GONE);
        }
    }
}
