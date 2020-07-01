package com.auric.intell.commonlib.floatview.base;

import android.content.Context;
import android.view.ViewGroup;
import android.view.WindowManager;

import com.auric.intell.commonlib.floatview.main.IFloatViewBase;


/**
 * 悬浮窗基类
 * @author white
 * @date 16/7/27
 */
public abstract class FloatViewBase extends android.widget.RelativeLayout implements IFloatViewBase {

	/**
	 * FloatView的布局参数，一定要给其赋值
	 */
	protected WindowManager.LayoutParams mLayoutParams;

	/**
	 * 必须要传上下文
	 */
	protected Context mContext;

	/**
	 * 用于保存外部传入数据
	 */
	public Object mData;

    public FloatViewBase(Context context) {
        this(context, null);
    }

	public FloatViewBase(Context context, Object obj) {
		super(context);
		mContext = context;
		mData = obj;
        setContentView(this);
        initViewParams();
	}

	/**
	 * 提供给子类重写，用于重新设置界面的宽高，不需要重新创建view
	 * @description
	 */
	protected void updateLayout() {
	}

    /**
     * 初始化参数
     */
    protected void initViewParams(){
        mLayoutParams = getWinLayoutParams();
        this.setFocusable(true);
        this.setFocusableInTouchMode(true);
        this.requestFocus();
    }

    @Override
    public ViewGroup getMainView() {
        return this;
    }

    /**
     * 设置内容View
     * @param rootView
     */
    abstract protected void setContentView(ViewGroup rootView);
}
