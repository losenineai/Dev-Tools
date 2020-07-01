package com.xxlib.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Typeface;

import com.xxlib.R;

/**
 * @author rosejames 2015-6-25 -- 下午2:38:36
 * @Description 和bitmap有关的工具类
 * <p/>
 * 版本号 作者 日期 相关修改 1.0 rosejames 2015-6-25 创建
 */
public class BitmapUtils {
    public static Bitmap drawColorBitmap(int width, int color) {
        Bitmap bitmap = Bitmap.createBitmap(width, width, Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        canvas.drawColor(color);
        canvas.drawBitmap(bitmap, 0, 0, null);
        return bitmap;
    }

    // 给图片添加水印
    public static Bitmap addWaterMarkToBitmap(Context context, Bitmap src, String str) {
        if (src == null) {
            return src;
        }
        int w = src.getWidth();
        int h = src.getHeight();
        Bitmap bmpTemp = Bitmap.createBitmap(w, h, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmpTemp);
        Paint p = new Paint();
        Typeface font = Typeface.create(Typeface.DEFAULT, Typeface.BOLD);
        p.setColor(context.getResources().getColor(R.color.screenWaterMark));
        p.setTypeface(font);
        p.setTextSize(30);
        canvas.drawBitmap(src, 0, 0, p);
        canvas.drawText(str, w / 2 - 100, h - 10, p);
        canvas.save(Canvas.ALL_SAVE_FLAG);
        canvas.restore();
        src.recycle();
        src = null;
        return bmpTemp;
    }

    public static Bitmap rotateBitmap(Bitmap bmp, float degree) {
        Matrix matrix = new Matrix();
        matrix.postRotate(degree);
        return Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight(), matrix, true);
    }


    public static Bitmap cutBimtp(Bitmap bitmap, int viewWidth, int viewHeight) {
        int imgWidth = bitmap.getWidth();
        int imgHeight = bitmap.getHeight();
        double radioVisible = viewHeight * 1.0 / viewWidth * 1.0;
        double radioImg = imgHeight * 1.0 / imgWidth * 1.0;
        if (radioVisible > radioImg) {
            int leftPost = (int) ((imgWidth - (imgHeight / radioVisible)) / 2);
            return Bitmap.createBitmap(bitmap, leftPost, 0, imgWidth - leftPost * 2, imgHeight);
        } else if (radioVisible < radioImg) {
            int topPos = (int) ((imgHeight - (imgWidth * radioVisible)) / 2);
            return Bitmap.createBitmap(bitmap, 0, topPos, imgWidth, imgHeight - topPos * 2);
        } else {
            return bitmap;
        }
    }

}


