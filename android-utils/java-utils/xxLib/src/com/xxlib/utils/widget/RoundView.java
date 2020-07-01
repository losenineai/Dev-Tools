package com.xxlib.utils.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.widget.TextView;

import com.xxlib.R;

/**
 * 
 * 最左和最右是半圆的按钮
 * @author gk
 *
 */
public class RoundView extends TextView {

	int lineColor;
	int solidColor;
	float lineWidth;
	
	Paint mPaint;
	float radius=-1;
	float width, height;
	
	public RoundView(Context context, AttributeSet attrs) {
		super(context, attrs);
		mPaint=new Paint();
		mPaint.setAntiAlias(true);
		
		TypedArray ta=context.obtainStyledAttributes(attrs, R.styleable.RoundView);
		lineWidth=ta.getDimensionPixelSize(R.styleable.RoundView_lineWidth, 4);
		lineColor=ta.getColor(R.styleable.RoundView_lineColor, Color.BLACK);
		solidColor=ta.getColor(R.styleable.RoundView_solidColor, Color.RED);
		ta.recycle();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		if(radius<0)
			radius=getHeight()/2.0f;
		
		if (lineWidth > 0) {
			mPaint.setColor(lineColor);
			canvas.drawCircle(radius, radius, radius, mPaint);
			canvas.drawCircle(getWidth() - radius, radius, radius, mPaint);
		}
		
		mPaint.setColor(solidColor);
		canvas.drawCircle(radius, radius, radius-lineWidth, mPaint);
		canvas.drawCircle(getWidth()-radius, radius, radius-lineWidth, mPaint);
		
		if (lineWidth > 0) {
			mPaint.setColor(lineColor);
			canvas.drawRect(radius, 0, getWidth() - radius, getHeight(), mPaint);
		}
		
		mPaint.setColor(solidColor);
		canvas.drawRect(radius, 0+lineWidth, getWidth()-radius, getHeight()-lineWidth, mPaint);
		
		super.onDraw(canvas);
	}

    public void setLineColor(int color) {
        lineColor=color;
        invalidate();
    }

}
