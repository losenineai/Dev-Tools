package com.auric.intell.commonlib.uikit.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

import com.auric.intell.commonlib.R;

/**
 * @author white
 * @description：
 * @date 2017/10/17
 */

public class DrawHookView extends View {


    //绘制圆弧的进度值
    private int progress = 0;
    //线1的x轴
    private int line1_x = 0;
    //线1的y轴
    private int line1_y = 0;
    //线2的x轴
    private int line2_x = 0;
    //线2的y轴
    private int line2_y = 0;

    // 动画速度，2px/10ms
    private static final int SPEED = 10;
    private int mMainColor = Color.GREEN;

    public DrawHookView(Context context) {
        super(context);
        initView(context, null);
    }

    public DrawHookView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView(context, attrs);
    }

    public DrawHookView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initView(context, attrs);
    }

    private void initView(Context context, AttributeSet attrs){
        if (attrs != null) {
            TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.DrawHookView);
            mMainColor = a.getColor(R.styleable.DrawHookView_dhv_main_color, Color.GREEN);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        progress += SPEED;

        /**
         * 绘制圆弧
         */
        Paint paint = new Paint();
        //设置画笔颜色
        paint.setColor(mMainColor);
        //设置圆弧的宽度
        paint.setStrokeWidth(25);
        //设置圆弧为空心
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeCap(Paint.Cap.ROUND);
        //消除锯齿
        paint.setAntiAlias(true);

        //获取圆心的x坐标
        int center = getWidth() / 2;
        int center1 = center - getWidth() / 5;
        //圆弧半径
        int radius = getWidth() / 2 - (int)paint.getStrokeWidth();

        //定义的圆弧的形状和大小的界限
        RectF rectF = new RectF(center - radius -1, center - radius -1 ,center + radius + 1, center + radius + 1);

        //根据进度画圆弧
        canvas.drawArc(rectF, 235, -360 * progress / 100, false, paint);

        /**
         * 绘制对勾
         */
        //先等圆弧画完，才话对勾
        if(progress >= 100) {
            if(line1_x < radius / 3) {
                line1_x += SPEED;
                line1_y += SPEED;
            }
            //画第一根线
            canvas.drawLine(center1, center, center1 + line1_x, center + line1_y, paint);

            if (line1_x >= radius / 3 && line1_x <= radius / 3 + SPEED) {
                line2_x = line1_x;
                line2_y = line1_y ;
                line1_x += SPEED;
                line1_y += SPEED;
            }
            if (line1_x >= (radius / 3) && line2_x <= radius) {
                line2_x += SPEED;
                line2_y -= SPEED;
            }
            //画第二根线
            canvas.drawLine(center1 + line1_x - 1, center + line1_y, center1 + line2_x, center + line2_y, paint);
        }

        //每隔10毫秒界面刷新
        postInvalidateDelayed(10);
    }


    public void startAnimation(){
        progress = 0;
        invalidate();
    }
}
