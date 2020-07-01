package com.xxlib.utils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.ExifInterface;
import android.support.v4.util.LruCache;
import android.text.TextUtils;
import android.util.Log;
import android.widget.ImageView;

import com.xxlib.utils.base.LogTool;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * 图片工厂类，用于构建bitmap实例，内置实现了缓存机制和异步加载
 * 注意：必须在主线程实例化本类
 * @author wxj
 *
 */
public class ImageFactory {

	/**
	 * 单例模式
	 */
	private static ImageFactory mInstance;
	public static ImageFactory getInstance(){
		if(mInstance == null){
			mInstance = new ImageFactory();
		}
		return mInstance;
	}
	
	// 缩略图缓存， 3.0之后的版本不能使用弱引用，要使用最少最近使用的缓存策略来优化图片缓存
	private LruCache<String, Bitmap> mBitmapCache;
	// 大图片Bitmap缓存
	private LruCache<String, Bitmap> mBigBitmapCache;
	/**
	 * 最大内存使用率，防止OOM
	 */
	private int MAXMEMORY_RATE = 9;
	
	// 是否有缩略图
	private boolean hasThumbnail = false;
	
	/**
	 * 结果回调处理
	 */
//	private Handler mHandler = new Handler(xxApplication.mContext.getMainLooper());
	
	private ImageFactory(){
		// 最大运行内存
		int maxMemory = (int) Runtime.getRuntime().maxMemory();
//		int cacheSize = maxMemory / 2;
		int cacheSize = maxMemory / MAXMEMORY_RATE / 2;
//		Log.e("wxj", "ImageFactory 分配的缓存大小："+cacheSize);
		mBitmapCache = new LruCache<String, Bitmap>(cacheSize);
		mBigBitmapCache = new LruCache<String, Bitmap>(cacheSize);
	}
	
//	/**
//	 * 异步创建缩略图片
//	 * @param iv ImageView
//	 * @param srcPath 原图片文件路径
//	 * @param thumbPath 缩略图路径
//	 * @param callback 回调接口
//	 */
//	public  void  asyncCreateThumbnail(final ImageView iv, final String srcPath, final String thumbPath, final ImageCallback callback){
//		// 原图片文件路径不能为null
//		if(TextUtils.isEmpty(srcPath) || iv == null){
//			return;
//		}
//		
//		hasThumbnail = false;
//		if(!TextUtils.isEmpty(thumbPath)){
//			hasThumbnail = true;
//		}
//		
//		// 检测缓存
//		Bitmap bitmap = mBitmapCache.get(srcPath);
//		if(bitmap != null && !bitmap.isRecycled()){
//			callback.onCreateBitmap(iv, bitmap, srcPath);
//			return;
//		}
//		
//		// 若无缓存，重新构建Bitmap
//		// guokai, 用线程池比较好
//		new Thread(){
//			public void run(){
//				try{
//					Bitmap bitmap = null;
//					if(hasThumbnail){
//						FileInputStream fis = new FileInputStream(thumbPath);
//						bitmap = BitmapFactory.decodeStream(fis);
//						fis.close();
//						if(bitmap == null){
//							bitmap = createToThumbnail(new File(srcPath));
//						}
//					}
//					else{
//						bitmap = createToThumbnail(new File(srcPath));
//					}
//					// 加入缓存
////					Log.e("加入的图片大小", ""+bitmap.getHeight()*bitmap.getWidth()*4);
//					putCache(srcPath, bitmap);
//					final Bitmap bmp = bitmap;
//					// 回调
//					mHandler.post(new Runnable(){
//						public void run(){
//							callback.onCreateBitmap(iv, bmp, srcPath);
//						}
//					});
//					
//					
//				}catch(Exception err){
//					// 回调
//					mHandler.post(new Runnable(){
//						public void run(){
////							Log.e("wxj", "create failed");
//							callback.onCreateFailed(iv, srcPath);
//						}
//					});
//				}
//			}
//		}.start();
//		
//	}
	
	
	/**
	 * 从原图片文件中创建Bitmap缩略图
	 * @param file 原图片文件
	 * @return 像素256*256以内的缩略图
	 */
	public Bitmap createToThumbnail(File file) throws IOException{
		if(file == null || !file.exists()){
//			Log.e("wxj", "file exsits:"+file.exists());
			return null;
		}
		
		Bitmap bitmap = null;
		Bitmap destBitmap = null;
		try{
			// 测量原图长宽
			BitmapFactory.Options option = new BitmapFactory.Options();
			option.inJustDecodeBounds = true;	// 不返回bitmap，测量关键点
			BufferedInputStream in = new BufferedInputStream(new FileInputStream(file));
			BitmapFactory.decodeStream(in, null, option);
			in.close();
			int max = 256;
			int width = option.outWidth;
			int height = option.outHeight;
			
			// 计算压缩比例
			if(width > max && height > max){
				int sampleSizie = ((width > height ? width : height) / max) + 1;
				option.inSampleSize = sampleSizie;
			}
			
			// 构建bitmap
			option.inJustDecodeBounds = false;
			in = new BufferedInputStream(new FileInputStream(file));
			bitmap = BitmapFactory.decodeStream(in, null, option);
			in.close();
			
			// 调整方向
			destBitmap = AsyncImageLoader.adjustOrientation(file.getAbsolutePath(), bitmap);
			
		}catch(Exception err){
			try {
//				Log.e("wxj", "createToThumbnail error:"+err);
				throw err;
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
//		Log.e("wxj", "bitmap:"+bitmap);
//		Log.e("wxj", "path:"+file);
		return destBitmap;
		
	}
	
	
	
	
	/**
	 * 指定宽度上限，从文件加载不超过此上限的Bitmap
	 * @param path
	 * @param maxWidth
	 * @return
	 */
	private Bitmap loadBitmapUnderWidth(String path, int maxWidth){
		Bitmap bitmap = null;
		BufferedInputStream in = null;
		
		try{
			// 测量原图长宽
			BitmapFactory.Options option = new BitmapFactory.Options();
			// true,不返回bitmap，测量关键点
			option.inJustDecodeBounds = true;	
			in = new BufferedInputStream(new FileInputStream(path));
			BitmapFactory.decodeStream(in, null, option);
//			in.close();
			
			int initWidth = isRotateImage(path) ?  option.outHeight : option.outWidth;

//			Log.e("wxj", "init outWidth:"+option.outWidth);
//			Log.e("wxj", "init outHeight:"+option.outHeight);
			
			// 不超过宽度最大限，计算压缩比例
			if(initWidth > maxWidth){
				int sampleSize = initWidth / maxWidth;
				option.inSampleSize = sampleSize;
			}
//			Log.e("wxj", "压缩比例："+option.inSampleSize);
			// 构建bitmap
			option.inJustDecodeBounds = false;
			in = new BufferedInputStream(new FileInputStream(path));
			bitmap = BitmapFactory.decodeStream(in, null, option);
			
		}catch(Exception err){
			err.printStackTrace();
		}finally{
			try {
				in.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return bitmap;
	}
	
	
	/**
	 * 根据宽度来进行比例压缩，小于宽度不压缩
	 * @param path 原图片路径
	 * @param srcBitmap 原图片
	 * @param destWidth 目标宽度
	 * @return
	 */
	public Bitmap compressBitmapByWidth(String path, Bitmap srcBitmap, int destWidth){
		// 获取这个图片的宽和高
        float width = isRotateImage(path) ? srcBitmap.getHeight() : srcBitmap.getWidth();
        // 大于目标才按比例压缩到目标宽度
        if(width <= destWidth){
        	return srcBitmap;
        }
        // 创建操作图片用的matrix对象
        Matrix matrix = new Matrix();
        // 计算宽高缩放率
        float scaleWidth = ((float) destWidth) / width;
        // 比例缩放图片
        matrix.postScale(scaleWidth, scaleWidth);
        // 调整方向
        matrix.postRotate(getImageDegree(path));
        Bitmap bitmap = Bitmap.createBitmap(srcBitmap, 0, 0, srcBitmap.getWidth(), srcBitmap.getHeight(), matrix, true);
        // 销毁原图
        destroyBitmap(srcBitmap);
        return bitmap;
	}
	
	
	/**
	 * 是否旋转图片
	 * @param filePath
	 * @return
	 */
	public boolean isRotateImage(String filePath){
//		Log.e("wxj", "degress:"+getImageDegree(filePath));
		return getImageDegree(filePath) % 180 != 0;
	}
	
	
	/**
	 * 销毁位图
	 * @param bitmap
	 */
	public static void destroyBitmap(Bitmap bitmap){
		if(bitmap != null && !bitmap.isRecycled()){
			bitmap.recycle();
			bitmap = null;
		}
	}
	
	
	/**
     * 读取图片属性：旋转的角度
     * @param path 图片绝对路径
     * @return degree旋转的角度
     */
    public static int getImageDegree(String path) {
		if (TextUtils.isEmpty(path)) {
			return 0;
		}

        int degree  = 0;

        if (!TextUtils.isEmpty(path)) {
            try {
                ExifInterface exifInterface = new ExifInterface(path);
                int orientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
                switch (orientation) {
                    case ExifInterface.ORIENTATION_ROTATE_90:
                        degree = 90;
                        break;
                    case ExifInterface.ORIENTATION_ROTATE_180:
                        degree = 180;
                        break;
                    case ExifInterface.ORIENTATION_ROTATE_270:
                        degree = 270;
                        break;
                }
                // 兼容系统截屏时方向失效问题
//            try{
//            	String degreeStr = path.substring(path.lastIndexOf("_") + 1, path.lastIndexOf("."));
//                degree -= Integer.parseInt(degreeStr);
//            }catch(Exception err){
//            }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return degree;
    }
    

	
	
	/**
	 * 添加缓存, 线程安全
	 * @param srcPath
	 * @param bitmap
	 */
	private synchronized void putCache(String srcPath, Bitmap bitmap){
		if(TextUtils.isEmpty(srcPath) || bitmap == null){
			return;
		}
		mBitmapCache.put(srcPath, bitmap);
	}
	
	
	/**
	 * 清空图片缓存
	 * @param path
	 */
	public void clearCache(String path){
		Bitmap bitmap = mBitmapCache.remove(path);
		if(bitmap != null && !bitmap.isRecycled()){
			bitmap.recycle();
			bitmap = null;
		}
		
		bitmap = mBigBitmapCache.remove(path);
		if(bitmap != null && !bitmap.isRecycled()){
			bitmap.recycle();
			bitmap = null;
		}
	}
	



	/**
	 * 创建占全屏的bitmap
	 * 
	 * @param context
	 *            上下文
	 * @param file
	 *            原文件
	 * @return
	 * @throws IOException
	 */
	@SuppressLint("NewApi")
	public Bitmap createBitmapForScreen(Context context, File file) throws Exception {
		if (file == null || !file.exists() || context == null) {
			return null;
		}

		// 如果有缓存，返回缓存
		Bitmap bitmap = mBigBitmapCache.get(file.getAbsolutePath());
		if (bitmap != null && !bitmap.isRecycled()) {
			return bitmap;
		}

		try {
			int maxWidth = 960;
			bitmap = loadBitmapUnderWidth(file.getAbsolutePath(), maxWidth);
			Log.e("zzd", bitmap.getByteCount() + "before compress size :" + bitmap.getWidth() + ", " + bitmap.getHeight());
			bitmap = compressBitmapByWidth(file.getAbsolutePath(), bitmap, maxWidth);
			// bitmap = compressImage(bitmap);
			Log.e("zzd", bitmap.getByteCount() + "after compress size :" + bitmap.getWidth() + ", " + bitmap.getHeight());
		} catch (Exception err) {
			try {
				throw err;
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} catch (OutOfMemoryError err) {
			Log.e("OutOfMemoryError", "" + err);
		}

		return bitmap;

	}

	public static byte[] createBitmapByCompress(Context context, File file, int maxWidth, int quality) throws Exception, OutOfMemoryError {
		if (file == null || !file.exists() || context == null) {
			return null;
		}
		// 1920*1080 mi4
		// Bitmap bitmap = mBigBitmapCache.get(file.getAbsolutePath());
		// if (bitmap != null && !bitmap.isRecycled()) {
		// return ImageUtils.bitmapToByte(bitmap);
		// }
		byte[] data;
		try {
			data = bitmapToByte(file.getPath(), maxWidth, quality);
			// 销毁原图

		} catch (Exception err) {
			err.printStackTrace();
			throw err;
		} catch (OutOfMemoryError err) {
			err.printStackTrace();
			throw err;
		}
		return data;
	}

	public static byte[] createBitmapByCompress(Context context, String filePath, int maxWidth, int quality) throws Exception, OutOfMemoryError {
		// 1920*1080 mi4
		byte[] data;
		try {
			data = bitmapToByte(filePath, maxWidth, quality);
			// 销毁原图
			if (null == data) {
				Exception exception = new Exception();
				exception.initCause(new Throwable("no image data return"));
				throw exception;
			}
		} catch (Exception err) {
			err.printStackTrace();
			throw err;
		} catch (OutOfMemoryError err) {
			err.printStackTrace();
			throw err;
		}
		return data;
	}

	public static int[] getBitmapSize(Context context, String filePath) {
		int[] i = new int[2];
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(filePath, options);
		// Calculate inSampleSize
		int width = options.outWidth;
		int height = options.outHeight;
		i[0] = width;
		i[1] = height;
		return i;
	}

	/***** weaiken ***/
	// 计算图片的缩放值
	private static int calculateInSampleSize(BitmapFactory.Options options, int reqWidth) {
		final int width = options.outWidth;
		int inSampleSize = 1;

		if (width > reqWidth) {
			final int widthRatio = Math.round((float) width / (float) reqWidth);
			inSampleSize = widthRatio;
		}
		return inSampleSize;
	}

	public static Bitmap rotateBitmap(Bitmap bitmap, int degress) {
		if (bitmap != null) {
			Matrix m = new Matrix();
			m.postRotate(degress);
			bitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), m, true);
			return bitmap;
		}
		return bitmap;
	}

	// 根据路径获得图片并压缩，返回bitmap用于显示
	private static Bitmap getSmallBitmap(String filePath, int reqWidth) {
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(filePath, options);

		// Calculate inSampleSize
		options.inSampleSize = calculateInSampleSize(options, reqWidth);

		// Decode bitmap with inSampleSize set
		options.inJustDecodeBounds = false;

		return BitmapFactory.decodeFile(filePath, options);
	}

	// 把bitmap转换成String
	private static byte[] bitmapToByte(String filePath, int reqWidth, int quality) {

		Bitmap bm = getSmallBitmap(filePath, reqWidth);
		int degree = getImageDegree(filePath);
		if (degree != 0) {// 旋转照片角度
			bm = rotateBitmap(bm, degree);
		}
		byte[] imageByte = ImageUtils.bitmapToByte(bm);
		if(imageByte.length < 100 * 1024){
			return imageByte;
		}
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		bm.compress(Bitmap.CompressFormat.JPEG, quality, baos);
		return baos.toByteArray();
	}

//	private static int[] getBitmapSize(String path) {
//		int i[] = new int[2];
//		BitmapFactory.Options options = new BitmapFactory.Options();
//		options.inJustDecodeBounds = true;
//		BitmapFactory.decodeFile(path, options);
//		int height = options.outHeight;
//		int width = options.outWidth;
//		i[0] = width;
//		i[1] = height;
//		return i;
//	}
//
//	/*************************/
//
//	private Bitmap compressImage(Bitmap image) {
//
//		ByteArrayOutputStream baos = new ByteArrayOutputStream();
//		image.compress(Bitmap.CompressFormat.PNG, 100, baos);// 质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中
//		int options = 100;
//		while (baos.toByteArray().length / 1024 > 200) { // 循环判断如果压缩后图片是否大于100kb,大于继续压缩
//			baos.reset();// 重置baos即清空baos
//			image.compress(Bitmap.CompressFormat.PNG, options, baos);// 这里压缩options%，把压缩后的数据存放到baos中
//			options -= 50;// 每次都减少10
//		}
//		ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());// 把压缩后的数据baos存放到ByteArrayInputStream中
//		Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);// 把ByteArrayInputStream数据生成图片
//		return bitmap;
//	}


	/**
	 * 指定宽度上限，从文件加载不超过此上限的Bitmap
	 * 
	 * @param path
	 * @param maxWidth
	 * @return
	 */
	public Bitmap LoadBitmapUnderWidth(String path, int maxWidth) {
		Bitmap bitmap = null;
		BufferedInputStream in = null;

		try {
			// 测量原图长宽
			BitmapFactory.Options option = new BitmapFactory.Options();
			// true,不返回bitmap，测量关键点
			option.inJustDecodeBounds = true;
			in = new BufferedInputStream(new FileInputStream(path));
			BitmapFactory.decodeStream(in, null, option);
			// in.close();

			int initWidth = isRotateImage(path) ? option.outHeight : option.outWidth;

			// Log.e("wxj", "init outWidth:"+option.outWidth);
			// Log.e("wxj", "init outHeight:"+option.outHeight);

			// 不超过宽度最大限，计算压缩比例
			if (initWidth > maxWidth) {
				int sampleSize = initWidth / maxWidth;
				option.inSampleSize = sampleSize;
			}
			Log.e("wxj", "压缩比例：" + option.inSampleSize);
			// 构建bitmap
			option.inJustDecodeBounds = false;
			in = new BufferedInputStream(new FileInputStream(path));
			bitmap = BitmapFactory.decodeStream(in, null, option);

		} catch (Exception err) {
			err.printStackTrace();
		} finally {
			try {
				in.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		return bitmap;
	}

	
	/**
	 * 图片加载回调接口
	 * @author wxj
	 *
	 */
	public interface ImageCallback{
		// 当bitmap被创建
		public void onCreateBitmap(ImageView iv, Bitmap bitmap, Object... option);
		// 当创建bitmap失败
		public void onCreateFailed(ImageView iv, Object... option);
	}
	
	/**========================xmod method start==========================**/
    /**
     * 回收imageview的图片资源
     *
     * @description
     * @param imageView
     */
    public static void releaseImageViewResouce(ImageView imageView) {
        if (imageView == null)
            return;
        Drawable drawable = imageView.getDrawable();
        if (drawable != null && drawable instanceof BitmapDrawable) {
            BitmapDrawable bitmapDrawable = (BitmapDrawable) drawable;
            Bitmap bitmap = bitmapDrawable.getBitmap();
            LogTool.i("ImageFactory", "release");
            if (bitmap != null && !bitmap.isRecycled()) {
                bitmap.recycle();
            }
        }
    }
    /**========================xmod method end==========================**/
}
