package com.auric.intell.commonlib.uikit.widget;

import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathMeasure;
import android.os.Build;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

import com.auric.intell.commonlib.R;
import com.auric.intell.commonlib.utils.LogUtils;
import com.auric.intell.commonlib.utils.ScreenUtil;


/**
 * Created by Tin on 2017/9/27.
 * 使用了drawPath，如果要使用SuccessView需要设置不使用硬件加速或者在所要获取的path上执行mPathRight.rLineTo(0, 0);
 * 关闭硬件加速的方法：
 * android:hardwareAccelerated="false"
 * 或者 view.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
 * 或者 getWindow().setFlags(WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED, WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED);
 */

public class ResultView extends View implements ValueAnimator.AnimatorUpdateListener {

    private Paint mPaint;
    private Path mPathCircle;
    private Path mPathRight;
    private Path mPathRightDst;
    private Path mPathCircleDst;
    /**
     * Path管理
     */
    private PathMeasure mPathMeasure;

    private Context mContext;

    private int main_color = Color.parseColor("#7BD24F");
    private float radius = 0;
    private float strokeWidth = 0;
    private float roundX, roundY = 0;
    private float start_angel = 90;
    private boolean rotate_deasil = true;
    private volatile boolean repeating = false;
    private volatile boolean appearence = true;

    /**
     * 当前绘制进度占总Path长度百分比
     */
    private float mCirclePercent;
    private float mRightPercent;

    private ValueAnimator mCircleAnimator;
    private ValueAnimator mRightAnimator;

    private Paint.Style paintStyle = Paint.Style.STROKE;

    private float parentW, parentH;
//    private float left, top, right, bottom;  //计时器的四个顶点坐标

    private float originX, originY;  //路径的起点位置坐标
    private float nextX, nextY;  //路径的下一点坐标
    private float endX, endY;

    private float x1, x2, y1, y2;
    private float lineX, lineY;
    private float lineX1, lineY1;

    private boolean fisrtInit = true;

    private static final int RESULT_CICLE = 0;
    private static final int RESULT_RIGHT = 1;
    private static final int RESULT_ERROR = 2;
    private int result = RESULT_CICLE;

    public ResultView(Context context) {
        this(context, null);
    }

    public ResultView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public ResultView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        if (context == null) {
            return;
        }
        mContext = context;

        parentW = ScreenUtil.getScreenWidth(mContext);
        parentH = ScreenUtil.getScreenHeight(mContext);

        strokeWidth = parentW / 20f;

        if (attrs != null)
            parseAttributeset(context.obtainStyledAttributes(attrs, R.styleable.ResultView));

        mPaint = new Paint();
        mPaint.setColor(main_color);
        mPaint.setStyle(paintStyle);
        mPaint.setAntiAlias(true);

        mPathCircle = new Path();
        mPathRight = new Path();
        mPathCircleDst = new Path();
        mPathRightDst = new Path();

        mPathMeasure = new PathMeasure();

        mCircleAnimator = ValueAnimator.ofFloat(0, 1);
        mCircleAnimator.setDuration(1000);
        mCircleAnimator.addUpdateListener(this);
        mCircleAnimator.start();

        mRightAnimator = ValueAnimator.ofFloat(0, 1);
        mRightAnimator.setDuration(300);
        mRightAnimator.addUpdateListener(this);

    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (fisrtInit) {
            parentW = getWidth();
            parentH = getHeight();
            strokeWidth = parentW / 25f;
            float offsetX;   float offsetY;
            LogUtils.e("onDraw parentW = " + parentW + "  parentH = " + parentH);
            if (parentW > parentH) {
                radius = (parentH - strokeWidth * 2) / 2f;
                roundX = parentW / 2f;  roundY = parentH / 2f;
                offsetX = roundX - radius;  offsetY = strokeWidth + 2 * strokeWidth;
            } else {
                radius = (parentW - strokeWidth * 2) / 2f;
                roundX = parentH / 2f;  roundY = parentW / 2;
                offsetX = strokeWidth - strokeWidth;  offsetY = roundX - radius - strokeWidth;
            }
            originX = offsetX + radius * 4 / 7;  originY = offsetY + radius * 6 / 8;
            nextX = offsetX + radius * 65 / 72;  nextY = offsetY + radius * 11 / 10;
            endX = offsetX + radius * 3 / 2;  endY = offsetY + radius / 3;
            offsetY -= 2 * strokeWidth;
            x1 = offsetX + radius * 3 / 5;   y1 = offsetY + radius * 3 / 5;
            x2 = offsetX + radius * 7 / 5; y2 = offsetY + radius * 7 / 5;
            lineX = x1;   lineY = y1;
            lineX1 = x2;    lineY1 = y1;
            fisrtInit = false;
            LogUtils.e("onDraw parentW = " + parentW + " parentH = " + parentH);
        }

        mPaint.setStrokeWidth(strokeWidth);
        mPaint.setStrokeCap(Paint.Cap.ROUND);

        //设置扇形的四个顶点位置
/*        RectF rectF = new RectF(left, top, right, bottom);
        //绘制一个空心扇形
        canvas.drawArc(rectF, -90, progress * 3.6f, false, mPaint);*/

        //画一个圆
        mPathCircle.addCircle(roundX, roundY, radius, Path.Direction.CW);
        mPathMeasure.setPath(mPathCircle, true);
        int start = 0;
        //获取到指定范围内的一段轮廓，存入到dst参数中
        mPathMeasure.getSegment(start, mCirclePercent * (mPathMeasure.getLength() + start), mPathCircleDst, true);
        canvas.drawPath(mPathCircleDst, mPaint);

        if (result == RESULT_RIGHT) {
            mPaint.setStrokeJoin(Paint.Join.ROUND);  //设置拐角为圆角

            mPathRight.moveTo(originX, originY);
            mPathRight.lineTo(nextX, nextY);
            mPathRight.lineTo(endX, endY);
            mPathMeasure.nextContour();  //移动到下一个轮廓
            mPathMeasure.setPath(mPathRight, false);
            //避免KITKAT及以下机型mPathMeasure.getLength()无效
            if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.KITKAT)
                mPathRight.rLineTo(0, 0);
            mPathMeasure.getSegment(0, mRightPercent * mPathMeasure.getLength(), mPathRightDst, true);
            canvas.drawPath(mPathRightDst, mPaint);
        }

        if (result == RESULT_ERROR) {
            mPaint.setStrokeJoin(Paint.Join.ROUND);  //设置拐角为圆角
            if (lineX <= x2 && lineY <= y2) {
                lineX += 10;   lineY+= 10;
            }
            canvas.drawLine(x1, y1, lineX, lineY, mPaint);
            if (lineX1 >= x1 && lineY1 <= y2) {
                lineX1 -= 10;  lineY1 +=10;
            }
            canvas.drawLine(x2, y1, lineX1, lineY1, mPaint);

            //每隔10毫秒界面刷新
            postInvalidateDelayed(10);
        }

    }

    @Override
    public void onAnimationUpdate(ValueAnimator animation) {
        if (animation.equals(mCircleAnimator)) {
            mCirclePercent = (float) animation.getAnimatedValue();
            invalidate();
            if (mCirclePercent == 1) {
                if (appearence) {
                    result = RESULT_RIGHT;
                    mRightAnimator.start();
                } else {
                    result = RESULT_ERROR;
                }
            }
        } else if (animation.equals(mRightAnimator)) {
            if (result == RESULT_RIGHT) {
                mRightPercent = (float) animation.getAnimatedValue();
                invalidate();
            }
        }
    }

    private void parseAttributeset(TypedArray a) {
        main_color = a.getColor(R.styleable.ResultView_main_color, main_color);
        radius = a.getDimension(R.styleable.ResultView_radius_size, radius);
        strokeWidth = a.getDimension(R.styleable.ResultView_stroke_size, strokeWidth);
        roundX = a.getDimension(R.styleable.ResultView_x_axis, roundX);
        roundY = a.getDimension(R.styleable.ResultView_y_axis, roundY);
        start_angel = a.getFloat(R.styleable.ResultView_start_angel, start_angel);
        rotate_deasil = a.getBoolean(R.styleable.ResultView_rotate_deasil, rotate_deasil);
        repeating = a.getBoolean(R.styleable.ResultView_repeating, repeating);
        appearence = a.getBoolean(R.styleable.ResultView_appearence, appearence);
        a.recycle();
    }

/*    public void setRectF(float left, float top, float right, float bottom) {
        this.left = left;   this.top = top;
        this.right = right;  this.bottom = bottom;
        invalidate();
    }*/

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

    public float getOriginX() {
        return originX;
    }

    public void setOriginX(float originX) {
        this.originX = originX;
    }

    public float getOriginY() {
        return originY;
    }

    public void setOriginY(float originY) {
        this.originY = originY;
    }

    public float getNextX() {
        return nextX;
    }

    public void setNextX(float nextX) {
        this.nextX = nextX;
    }

    public float getNextY() {
        return nextY;
    }

    public void setNextY(float nextY) {
        this.nextY = nextY;
    }

    public float getEndX() {
        return endX;
    }

    public void setEndX(float endX) {
        this.endX = endX;
    }

    public float getEndY() {
        return endY;
    }

    public void setEndY(float endY) {
        this.endY = endY;
    }
}
