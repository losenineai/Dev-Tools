package com.auric.intell.commonlib.uikit.widget;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Build;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.View;

import com.auric.intell.commonlib.R;
import com.auric.intell.commonlib.utils.ScreenUtil;

/**
 * Created by Tin on 2017/9/26.
 */


public class CountDownView extends View {

    Paint mInnerCiclePaint;
    Paint mOutterCiclePaint;
    Paint mTextPaint;
    Paint mSecondTextPaint;
    Context mContext;

    int base_color = 0x000000;
    int count_down_color = 0x000000;
    int text_color = 0x000000;
    int seconds_text_color = 0x000000;
    float text_size = 17;
    float seconds_text_size = 14;

    float progress = 100f;   //当前进度
    float timeType = 100f / 30f;   //时间的制度格式
    String time = "" + (int)(progress / timeType);  //剩余时间
    String second = "秒";

    float screenW, screenH;  //屏幕的宽和高
    float left, top, right, bottom;  //计时器的四个顶点坐标
    float radius;
    float roundX, roundY;

    float strokeWidth;

    public CountDownView(Context context) {
        super(context);
        init(context, null);
    }

    public CountDownView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    public CountDownView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs);
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public CountDownView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    private void init(Context context, AttributeSet attrs) {
        if (context == null) {
            return;
        }
        mContext = context;
        if (attrs != null) {
            parseAttributeset(context.obtainStyledAttributes(attrs, R.styleable.CountDownView));
        }

        screenW = ScreenUtil.getScreenWidth(mContext);
        screenH = ScreenUtil.getScreenHeight(mContext);
        radius = screenW / 4f;
        roundX = screenW / 2f;  roundY = screenH / 6f;
        strokeWidth = screenW / 20f;

        mInnerCiclePaint = new Paint();
        mInnerCiclePaint.setColor(base_color);
        mInnerCiclePaint.setStyle(Paint.Style.STROKE);
        mInnerCiclePaint.setAntiAlias(true);

        mOutterCiclePaint = new Paint(mInnerCiclePaint);
        mOutterCiclePaint.setColor(count_down_color);
        mOutterCiclePaint.setStrokeCap(Paint.Cap.ROUND);

        mTextPaint = new Paint();
        mTextPaint.setColor(text_color);
        mTextPaint.setTextSize(text_size);
        mTextPaint.setAntiAlias(true);

        mSecondTextPaint = new Paint();
        mSecondTextPaint.setColor(seconds_text_color);
        mSecondTextPaint.setTextSize(seconds_text_size);
        mSecondTextPaint.setAntiAlias(true);

    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);


    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        left = roundX - radius;  top = roundY - radius;
        right = roundX + radius;  bottom = roundY + radius;

        mInnerCiclePaint.setStrokeWidth(strokeWidth);
        mOutterCiclePaint.setStrokeWidth(strokeWidth);

        canvas.drawCircle(roundX, roundY, radius, mInnerCiclePaint);

        //设置扇形的四个顶点位置

        RectF rectF = new RectF(left, top, right, bottom);
        //绘制一个空心扇形
        canvas.drawArc(rectF, -90, progress * 3.6f, false, mOutterCiclePaint);

        time = "" + (int)(progress / timeType);
        float horizontalTextOffset = mTextPaint.measureText(time) / 2f;
        float horizontalSecondTextOffset = mSecondTextPaint.measureText(second) / 2f;

        float verticalText = (mTextPaint.descent() - mTextPaint.ascent()) / 2f - mTextPaint.descent();  //得到文本中心离文本基线（baseline)的距离
        float verticalTextOffset = verticalText;
        if (!TextUtils.isEmpty(second)) {
            float verticalsecondText = (mSecondTextPaint.descent() - mSecondTextPaint.ascent()) / 2f
                    - mSecondTextPaint.descent();
            float verticalSecondTextOffset = 2 * verticalsecondText + verticalText + 5; //加5拉开time和秒之间的距离，好看一点
            verticalTextOffset = verticalsecondText;
            canvas.drawText(second, roundX - horizontalSecondTextOffset, roundY + verticalSecondTextOffset, mSecondTextPaint);
        }
        canvas.drawText(time, roundX - horizontalTextOffset, roundY + verticalTextOffset , mTextPaint);
    }

    private void parseAttributeset(TypedArray a) {
        base_color = a.getColor(R.styleable.CountDownView_base_color, base_color);
        count_down_color = a.getColor(R.styleable.CountDownView_count_down_color, count_down_color);
        text_color = a.getColor(R.styleable.CountDownView_text_color, text_color);
        text_size = (int) a.getDimension(R.styleable.CountDownView_text_size, text_size);
        text_size = ScreenUtil.sp2px(mContext.getResources(), text_size);
        seconds_text_color = a.getColor(R.styleable.CountDownView_seconds_text_color, seconds_text_color);
        seconds_text_size = (int) a.getDimension(R.styleable.CountDownView_seconds_text_size, seconds_text_size);
        seconds_text_size = ScreenUtil.sp2px(mContext.getResources(), seconds_text_size);
    }

    public void setRectF(float left, float top, float right, float bottom) {
        this.left = left;   this.top = top;
        this.right = right;  this.bottom = bottom;
    }

    public void setProgress(float progress) {
        this.progress = progress * timeType;
        invalidate();
    }

    public float getProgress() {
        return progress;
    }

    public void setTime(float totalTime) {
        timeType = 100 / totalTime;
        invalidate();
    }

    public int getCurrentTime() {
        return (int)(progress / timeType);
    }

    public void setSecondText(String secondText) {
        this.second = secondText;
        invalidate();
    }

    public void setRadius(float radius) {
        this.radius = radius;
        invalidate();
    }

    public float getRadius() {
        return radius;
    }

    public void setRoundX(float roundX) {
        this.roundX = roundX;
        invalidate();
    }

    public float getRoundX() {
        return roundX;
    }

    public void setRoundY(float roundY) {
        this.roundY = roundY;
        invalidate();
    }

    public float getRoundY() {
        return roundY;
    }

    public float getStrokeWidth() {
        return strokeWidth;
    }

    public void setStrokeWidth(float strokeWidth) {
        this.strokeWidth = strokeWidth;
    }
}
