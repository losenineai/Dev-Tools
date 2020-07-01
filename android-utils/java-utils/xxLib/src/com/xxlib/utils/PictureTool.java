package com.xxlib.utils;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.ThumbnailUtils;
import android.os.Build;
import android.support.v4.util.LruCache;
import android.text.style.ImageSpan;
import android.widget.ImageView;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * 
 * @Description 图片获取优化
 * 
 *              版本号 作者 日期 相关修改 1.0 linbin 2014-9-12 创建
 * 
 * @author linbin 2014-9-12
 */
public class PictureTool {

	private static LruCache<String, Bitmap> mMemoryCache;
	private static final int MAXMEMORY_RATE = 9;

	public static BitmapDrawable get(Context context, int resId) {
		BitmapFactory.Options opt = new BitmapFactory.Options();

		opt.inPreferredConfig = Bitmap.Config.ARGB_8888;

		opt.inPurgeable = true;

		opt.inInputShareable = true;

		// 获取资源图片
		InputStream is = context.getResources().openRawResource(resId);

		Bitmap bitmap = BitmapFactory.decodeStream(is, null, opt);

		try {
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return new BitmapDrawable(context.getResources(), bitmap);
	}

	public static ImageSpan getEmojiImageSpan(Context context, int resId) {
		return new ImageSpan(context, getEmojiBitmap(context, resId, 0));
	}

	public static ImageSpan getEmojiImageSpan(Context context, int resId, int length) {
		return new ImageSpan(context, getEmojiBitmap(context, resId, length));
	}

	public static Bitmap getEmojiBitmap(Context context, int resId, int length) {

		if (mMemoryCache == null) {
			int maxMemory = (int) Runtime.getRuntime().maxMemory();
			int cacheSize = maxMemory / MAXMEMORY_RATE;
			// 设置图片缓存大小为程序最大可用内存的1/MAXMEMORY_RATE
			mMemoryCache = new LruCache<String, Bitmap>(cacheSize) {
				@Override
				protected int sizeOf(String key, Bitmap bitmap) {
					if (bitmap != null)
						return bitmap.getRowBytes() * bitmap.getHeight();
					else
						return 1;
				}
			};
		}

		if (mMemoryCache.get(resId + "_" + length) != null) {
			return mMemoryCache.get(resId + "_" + length);
		}
		BitmapFactory.Options opt = new BitmapFactory.Options();

		opt.inPreferredConfig = Bitmap.Config.ARGB_8888;

		opt.inPurgeable = true;

		opt.inInputShareable = true;

		// 获取资源图片
		InputStream is = context.getResources().openRawResource(resId);

		Bitmap bitmap = BitmapFactory.decodeStream(is, null, opt);

		try {
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		if (length != 0)
			bitmap = ThumbnailUtils.extractThumbnail(bitmap, length, length); // 缩放图片，将这个过程放在这里是因为这个过程很耗时，做一次然后缓冲下来就能够减缓卡的效果

		mMemoryCache.put(resId + "_" + length, bitmap);

		return bitmap;
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	@SuppressWarnings("deprecation")
	private void setAlpha(ImageView imageView, float alpha) {

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {

			imageView.setAlpha(alpha);

		} else {

			imageView.setAlpha(Math.round(alpha * 255));

		}

	}

	/**
	 * drawable转byte数组
	 * 
	 * @description
	 * @param drawable
	 * @return
	 */
	public static byte[] drawableToBytes(Drawable drawable) {
		// 第一步，将Drawable对象转化为Bitmap对象
		Bitmap bmp = (((BitmapDrawable) drawable).getBitmap());
		// 第二步，声明并创建一个输出字节流对象
		ByteArrayOutputStream os = new ByteArrayOutputStream();
		// 第三步，调用compress将Bitmap对象压缩为PNG格式，第二个参数为PNG图片质量，第三个参数为接收容器，即输出字节流os
		bmp.compress(Bitmap.CompressFormat.PNG, 100, os);
		// 第四步，将输出字节流转换为字节数组，并直接进行存储数据库操作，注意，所对应的列的数据类型应该是BLOB类型
		return os.toByteArray();
	}

	/**
	 * byte数组转drawable
	 * 
	 * @description
	 * @param blob
	 * @return
	 */
	public static Drawable bytesToDrawable(byte[] blob) {
		// 第一步，调用BitmapFactory的解码方法decodeByteArray把字节数组转换为Bitmap对象
		Bitmap bmp = BitmapFactory.decodeByteArray(blob, 0, blob.length);
		// 第二步，调用BitmapDrawable构造函数生成一个BitmapDrawable对象，该对象继承Drawable对象，所以在需要处直接使用该对象即可
		BitmapDrawable bd = new BitmapDrawable(bmp);
		return bd;
	}

	public static Bitmap fileToBitmap(String path) {
		Bitmap bitmap = BitmapFactory.decodeFile(path);
		return bitmap;
	}

	/**
	 * 将Bitmap 转换成二进制数组
	 * 
	 * @description
	 * @param bitmap
	 * @return
	 */
	public static byte[] BitmapToBytes(Bitmap bitmap) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);
		return baos.toByteArray();
	}

}
