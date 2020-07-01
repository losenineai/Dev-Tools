package com.xxlib.utils;

import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff.Mode;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;


/**
 * 图片处理工具
 * @author wxj
 * 2014
 */
public class ImageUtil {

	
	/**
	 * 获得矩形圆角图片
	 * @param bitmap 位图
	 * @return 返回圆角图片
	 */
	public static Bitmap getRoundedCornerBitmap(Bitmap bitmap) {
		
		if(bitmap == null){
			return null;
		}
		// 圆角半径
		float roundPx = 10.0F;
		
		Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
				bitmap.getHeight(), Config.ARGB_8888);
		Canvas canvas = new Canvas(output);

		final int color = 0xff000000;
		final Paint paint = new Paint();
		final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
		final RectF rectF = new RectF(rect);

		paint.setAntiAlias(true);
		canvas.drawARGB(0, 0, 0, 0);
		paint.setColor(color);
		canvas.drawRoundRect(rectF, roundPx, roundPx, paint);

		
		paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
		canvas.drawBitmap(bitmap, rect, rect, paint);

//		if(!bitmap.isRecycled()){
//			bitmap.recycle();
//		}
		return output;
	}
	
	/**
	 * 获得矩形圆角图片
	 * @param bitmap 位图
	 * @return 返回圆角图片
	 */
	public static Bitmap getRoundedCornerBitmap(Bitmap bitmap,float round) {
		
		if(bitmap == null){
			return null;
		}
		float roundPx = 10.0F;
		
		if(round > 0.0)
		{
			// 圆角半径
			roundPx = round;
		}
		
		Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
				bitmap.getHeight(), Config.ARGB_8888);
		Canvas canvas = new Canvas(output);

		final int color = 0xff000000;
		final Paint paint = new Paint();
		final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
		final RectF rectF = new RectF(rect);

		paint.setAntiAlias(true);
		canvas.drawARGB(0, 0, 0, 0);
		paint.setColor(color);
		canvas.drawRoundRect(rectF, roundPx, roundPx, paint);

		
		paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
		canvas.drawBitmap(bitmap, rect, rect, paint);

//		if(!bitmap.isRecycled()){
//			bitmap.recycle();
//		}
		return output;
	}
	
	/**
	 * 获得圆形的Bitmap
	 * @param bitmap
	 * @param roundPx
	 * @return
	 */
	public static Bitmap getOvalBitmap(Bitmap bitmap) {
		
		if(bitmap == null){
			return null;
		}
		
		Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
				bitmap.getHeight(), Config.ARGB_8888);
		Canvas canvas = new Canvas(output);

		final int color = 0xffffffff;
		final Paint paint = new Paint();
		final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
		rect.bottom-=4;
		rect.right-=4;
		rect.top+=4;
		rect.left+=4;
		final RectF rectF = new RectF(rect);

		paint.setAntiAlias(true);
		canvas.drawARGB(0, 0, 0, 0);
		paint.setColor(color);
		canvas.drawOval(rectF, paint);

		
		paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
		canvas.drawBitmap(bitmap, rect, rect, paint);

//		if(!bitmap.isRecycled()){
//			bitmap.recycle();
//		}
	        
		return output;
	}

	/**
	 * wxj, 压缩图片为缩略图，控制在10K一下的大小，适用于显示于小尺寸的ImageView
	 * @param imageView 显示的ImageView
	 */
	public static void compressToThumbnail(ImageView imageView){
		if(imageView == null){
			return;
		}
		
		Bitmap bitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
		if(bitmap == null){
			return;
		}
		
		// 图片大小上限,10k
		int maxSize = 10 * 1024;
		byte[] bytes = bitmap2ByteArray(bitmap);
		
		if(bytes.length > maxSize){
			BitmapFactory.Options opt = new BitmapFactory.Options();
			opt.inSampleSize = (bytes.length / maxSize) + 1;
			Bitmap bmp = BitmapFactory.decodeByteArray(bytes, 0, bytes.length, opt);
			// 设置压缩的bitmap
			imageView.setImageBitmap(bmp);
			// 最后销毁之前的bitmap。回收bitmap出现imageview使用recycled的异常，待解决
//			FileUtil.destroyBitmap(bitmap);
		}
		
	}
	
	/**
	 * wxj, bitmap转化成字节数组
	 * @param bitmap
	 * @return
	 */
	private static byte[] bitmap2ByteArray(Bitmap bitmap){
		if(bitmap == null || bitmap.isRecycled()){
			return new byte[0];
		}
		
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bitmap.compress(CompressFormat.PNG, 100, baos);
		try {
			baos.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
//			e.printStackTrace();
		}
		return baos.toByteArray();
	}
	
	
	/**
	 * convert Drawable to Bitmap
	 * 
	 * @param d
	 * @return
	 */
	public static Bitmap drawableToBitmap(Drawable d) {
		return d == null ? null : ((BitmapDrawable) d).getBitmap();
	}

    /**==========================xmod method start===========================**/
    public static Bitmap drawable2Bitmap(Drawable drawable) // drawable 转换成 bitmap
    {
        int width = drawable.getIntrinsicWidth(); // 取 drawable 的长宽
        int height = drawable.getIntrinsicHeight();
        Bitmap.Config config = drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
                : Bitmap.Config.RGB_565; // 取 drawable 的颜色格式
        Bitmap bitmap = Bitmap.createBitmap(width, height, config); // 建立对应
        // bitmap
        Canvas canvas = new Canvas(bitmap); // 建立对应 bitmap 的画布
        drawable.setBounds(0, 0, width, height);
        drawable.draw(canvas); // 把 drawable 内容画到画布中
        return bitmap;
    }

    public static Drawable zoomDrawable(Drawable drawable, int w, int h) {
        int width = drawable.getIntrinsicWidth();
        int height = drawable.getIntrinsicHeight();
        Bitmap oldbmp = drawableToBitmap(drawable); // drawable 转换成 bitmap
        Matrix matrix = new Matrix(); // 创建操作图片用的 Matrix 对象
        float scaleWidth = ((float) w / width); // 计算缩放比例
        float scaleHeight = ((float) h / height);
        matrix.postScale(scaleWidth, scaleHeight); // 设置缩放比例
        Bitmap newbmp = Bitmap.createBitmap(oldbmp, 0, 0, width, height,
                matrix, true); // 建立新的 bitmap ，其内容是对原 bitmap 的缩放后的图
        return new BitmapDrawable(newbmp); // 把 bitmap 转换成 drawable 并返回
    }

    public static Bitmap comp(Bitmap image) {

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
        if( baos.toByteArray().length / 1024>1024) {//判断如果图片大于1M,进行压缩避免在生成图片（BitmapFactory.decodeStream）时溢出
            baos.reset();//重置baos即清空baos
            image.compress(Bitmap.CompressFormat.JPEG, 50, baos);//这里压缩50%，把压缩后的数据存放到baos中
        }
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());
        BitmapFactory.Options newOpts = new BitmapFactory.Options();
        //开始读入图片，此时把options.inJustDecodeBounds 设回true了
        newOpts.inJustDecodeBounds = true;
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
        newOpts.inJustDecodeBounds = false;
        int w = newOpts.outWidth;
        int h = newOpts.outHeight;
        //现在主流手机比较多是800*480分辨率，所以高和宽我们设置为
        float hh = 800f;//这里设置高度为800f
        float ww = 480f;//这里设置宽度为480f
        //缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
        int be = 1;//be=1表示不缩放
        if (w > h && w > ww) {//如果宽度大的话根据宽度固定大小缩放
            be = (int) (newOpts.outWidth / ww);
        } else if (w < h && h > hh) {//如果高度高的话根据宽度固定大小缩放
            be = (int) (newOpts.outHeight / hh);
        }
        if (be <= 0)
            be = 1;
        newOpts.inSampleSize = be;//设置缩放比例
        //重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
        isBm = new ByteArrayInputStream(baos.toByteArray());
        bitmap = BitmapFactory.decodeStream(isBm, null, newOpts);
        return compressImage(bitmap);//压缩好比例大小后再进行质量压缩
    }

    private static Bitmap compressImage(Bitmap image) {

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
        int options = 100;
        while ( baos.toByteArray().length / 1024>100) {  //循环判断如果压缩后图片是否大于100kb,大于继续压缩
            baos.reset();//重置baos即清空baos
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//这里压缩options%，把压缩后的数据存放到baos中
            options -= 10;//每次都减少10
        }
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//把压缩后的数据baos存放到ByteArrayInputStream中
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//把ByteArrayInputStream数据生成图片
        return bitmap;
    }
    /**==========================xmod method end===========================**/
}
