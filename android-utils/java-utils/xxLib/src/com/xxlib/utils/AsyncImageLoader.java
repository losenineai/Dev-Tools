package com.xxlib.utils;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.text.TextUtils;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.nostra13.universalimageloader.cache.disc.impl.UnlimitedDiscCache;
import com.nostra13.universalimageloader.cache.memory.MemoryCache;
import com.nostra13.universalimageloader.cache.memory.impl.LRULimitedMemoryCache;
import com.nostra13.universalimageloader.core.DisplayImageOptions;
import com.nostra13.universalimageloader.core.ImageLoader;
import com.nostra13.universalimageloader.core.ImageLoaderConfiguration;
import com.nostra13.universalimageloader.core.assist.FailReason;
import com.nostra13.universalimageloader.core.assist.ImageScaleType;
import com.nostra13.universalimageloader.core.assist.ImageSize;
import com.nostra13.universalimageloader.core.assist.LoadedFrom;
import com.nostra13.universalimageloader.core.assist.QueueProcessingType;
import com.nostra13.universalimageloader.core.display.BitmapDisplayer;
import com.nostra13.universalimageloader.core.download.BaseImageDownloader;
import com.nostra13.universalimageloader.core.imageaware.ImageAware;
import com.nostra13.universalimageloader.core.listener.ImageLoadingListener;
import com.nostra13.universalimageloader.utils.DiskCacheUtils;
import com.nostra13.universalimageloader.utils.MemoryCacheUtils;
import com.xxlib.utils.base.LogTool;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.zip.CRC32;

/**
 * 
 * 使用 Universal Image Loader
 * https://github.com/nostra13/Android-Universal-Image-Loader
 * 
 */
public class AsyncImageLoader {

    public static final String MAIN_DIRECT = Environment.getExternalStorageDirectory().toString() + "/" + ContextFinder.getApplication().getPackageName() + "/";
    public static final String IMAGE_DIRECT = MAIN_DIRECT + "images/";
	public static final int MAXMEMORY_RATE_FOR_CHACHA = 9;
	public static final int MAXMEMORY_RATE_FOR_FLOAT_VIEW = 18;

	protected static final String TAG = "AsyncImageLoader";
	boolean isAppView = false;
	// 要加载的图片宽度 默认是150
	private int requireImageWidth = 150;

	/**
	 * 图片缓存技术的核心类，用于缓存所有下载好的图片，在程序内存达到设定值时会将最少最近使用的图片移除掉。
	 */
	// private static LruCache<String, Bitmap> mMemoryCache;
	private ImageLoader mImageLoader = ImageLoader.getInstance();
	private DisplayImageOptions mOptionsForNet, mOptionsForLocal;

	private static AsyncImageLoader mAsyncImageLoader = null;

	// 用单例比较好
	public static AsyncImageLoader getInstance() {
		if (mAsyncImageLoader == null)
			mAsyncImageLoader = new AsyncImageLoader();
		return mAsyncImageLoader;
	}

	/**
	 * @description zzd 强制清理图片缓存的内存
	 */
	public static void cleanMemory() {
		ImageLoader.getInstance().clearMemoryCache();
	}

	/**
	 * 将一张图片存储到LruCache中。
	 * 
	 * @param key
	 *            LruCache的键，这里传入图片的URL地址。
	 * @param bitmap
	 *            LruCache的键，这里传入从网络上下载的Bitmap对象。
	 */
	public void addBitmapToMemoryCache(String key, Bitmap bitmap) {
		if (key.startsWith("http")) {
			mImageLoader.getMemoryCache().put(
					MemoryCacheUtils.generateKey(key, new ImageSize(bitmap.getWidth(), bitmap.getHeight())), bitmap);
		} else {
			mImageLoader.getMemoryCache()
					.put(MemoryCacheUtils.generateKey("file://" + key,
							new ImageSize(bitmap.getWidth(), bitmap.getHeight())), bitmap);
		}

	}

	/**
	 * 从LruCache中获取一张图片，如果不存在就返回null。
	 * 
	 * @param key
	 *            LruCache的键，这里传入图片的URL地址。
	 * @return 对应传入键的Bitmap对象，或者null。
	 */
	public Bitmap getBitmapFromMemoryCache(String key) {
		try {
			if (key.startsWith("http")) {
				return MemoryCacheUtils.findCachedBitmapsForImageUri(key, mImageLoader.getMemoryCache()).get(0);
			} else {
				return MemoryCacheUtils.findCachedBitmapsForImageUri("file://" + key, mImageLoader.getMemoryCache())
						.get(0);
			}
		} catch (Exception e) {
			return null;
		}
	}

	/**
	 * 根据传进来的宽度，计算出合比例压缩加载参数
	 * 
	 * @param options
	 *            BitmapFactory参数
	 * @param reqWidth
	 *            要求的宽度
	 * @return inSampleSize 压缩比例
	 */
	public static int calculateInSampleSize(BitmapFactory.Options options, int reqWidth) {
		// 源图片的宽度
		final int width = options.outWidth;
		int inSampleSize = 1;
		if (width > reqWidth) {
			// 计算出实际宽度和目标宽度的比率
			float ss = (float) width / (float) reqWidth;
			float sss = ss % 1;

			final int widthRatio = Math.round((float) width / (float) reqWidth);
			inSampleSize = widthRatio;

			if (sss > 0) {
				inSampleSize++;
			}
		}

		if (inSampleSize == 1 && options.outHeight > 3000)
			inSampleSize++;

		return inSampleSize;
	}

	/**
	 * 加载合适比例的图片
	 * 
	 * @param pathName
	 *            图片文件位置
	 * @param reqWidth
	 *            要求的宽度
	 * @return bitmap 返回的图片实例
	 */

	public static synchronized Bitmap decodeSampledBitmapFromResource(String pathName, int reqWidth) {
		// 第一次解析将inJustDecodeBounds设置为true，来获取图片大小
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(pathName, options);
		// 调用上面定义的方法计算inSampleSize值
		options.inSampleSize = calculateInSampleSize(options, reqWidth);
		// Log.e("加载的图片的宽高", options.outWidth+" "+options.outHeight+"");
		// 使用获取到的inSampleSize值再次解析图片
		options.inJustDecodeBounds = false;
		Bitmap b = null;
		InputStream is = null;
		try {
			File file = new File(pathName);
			is = new FileInputStream(file);
			b = BitmapFactory.decodeStream(is, null, options);
			// wxj, 调整显示方向
			// Log.e("wxj", "~~~~1~~"+b.isRecycled());
			b = adjustOrientation(pathName, b);
			// Log.e("wxj", "~~~~~2~"+b.isRecycled());
		} catch (OutOfMemoryError e) {

			System.runFinalization();
		} catch (FileNotFoundException e) {

		} finally {
			try {
				if (null != is) {
					is.close();
				}
			} catch (IOException e) {

			}
		}

		return b;
	}

	private AsyncImageLoader() {
		initParams();
	}

	private AsyncImageLoader(int requireImageWidth) {
		initParams();
	}

	private AsyncImageLoader(int requireImageWidth, boolean isLimit) {
		initParams();
	}

	private void initParams() {
		mOptionsForNet = new DisplayImageOptions.Builder().cloneFrom(DisplayImageOptions.createSimple())
				.cacheInMemory(true)
				// 设置下载的图片是否缓存在内存中
				.cacheOnDisk(true)
				// 设置下载的图片是否缓存在SD卡中
				.bitmapConfig(Bitmap.Config.RGB_565).imageScaleType(ImageScaleType.NONE).considerExifParams(true)
				.build();
		mOptionsForLocal = new DisplayImageOptions.Builder().cloneFrom(DisplayImageOptions.createSimple())
				.cacheInMemory(true)
				// 设置下载的图片是否缓存在内存中
				.cacheOnDisk(false)
				// 设置下载的图片是否缓存在SD卡中
				.bitmapConfig(Bitmap.Config.RGB_565).considerExifParams(true)
				.imageScaleType(ImageScaleType.IN_SAMPLE_POWER_OF_2).build();
	}

	/**
	 * 异步加载图片，设置ImageView的内容，如果加载失败，不显示默认图片
	 */
	public void setItemPicAsyncNotSetDefault(final String url, final ImageView imageView) {
		loadDrawable(url, imageView, new ImageCallback() {

			@Override
			public void imageLoaded(Drawable drawable, ImageView iv, String url) {
				if (drawable != null)
					iv.setBackgroundDrawable(drawable);
			}
		});
	}

	/**
	 * 异步加载图片，设置ImageView的内容，如果加载失败，默认显示 R.drawable.icon_logo_default 这个图片
	 */
//	public void setItemPicAsync(final String url, final ImageView imageView) {
//		imageView.setBackgroundResource(R.drawable.icon_logo_white);
//		loadDrawable(url, imageView, new ImageCallback() {
//
//			@Override
//			public void imageLoaded(Drawable drawable, ImageView iv, String url) {
//				if (drawable != null)
//					iv.setBackgroundDrawable(drawable);
//			}
//		});
//	}

	/**
	 * 异步加载图片，设置ImageView的内容
	 * 
	 * @param defaultDrawable
	 *            如果加载失败，显示默认图片
	 */
	public void setItemPicAsync(final String url, final ImageView imageView, final Drawable defaultDrawable) {
		imageView.setBackgroundDrawable(defaultDrawable);
		loadDrawable(url, imageView, new ImageCallback() {

			@Override
			public void imageLoaded(Drawable drawable, ImageView iv, String url) {
				if (drawable != null)
					iv.setBackgroundDrawable(drawable);
			}
		});
	}

	/**
	 * 异步加载图片，设置ImageView的内容
	 * 
	 * @param defaultDrawableResId
	 *            如果加载失败，显示默认图片
	 */
	public void setItemPicAsync(final String url, final ImageView imageView, final int defaultDrawableResId) {
		imageView.setBackgroundResource(defaultDrawableResId);
		loadDrawable(url, imageView, new ImageCallback() {

			@Override
			public void imageLoaded(Drawable drawable, ImageView iv, String url) {
				if (drawable != null)
					iv.setBackgroundDrawable(drawable);
			}
		});
	}

	public void loadDrawable(final String url, final TextView textView, final ImageSpanCallback callback) {
		if (url == null) {
			return;
		}
		if (!url.startsWith("http")) {
			loadDrawableFromNative(url, textView, callback);
			return;
		}

		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(textView.getContext().getApplicationContext(), 1);
			}
		}

		DisplayImageOptions op = new DisplayImageOptions.Builder().cloneFrom(mOptionsForNet).build();

		mImageLoader.loadImage(url, op, new ImageLoadingListener() {
			@Override
			public void onLoadingStarted(String s, View view) {

			}

			@Override
			public void onLoadingFailed(String s, View view, FailReason failReason) {
				callback.imageLoaded(null, textView, url);
			}

			@Override
			public void onLoadingComplete(String s, View view, Bitmap bitmap) {
				callback.imageLoaded(bitmap, textView, url);
			}

			@Override
			public void onLoadingCancelled(String s, View view) {

			}
		});

	}

	public void loadDrawable(final String url, final ImageView imageView, final ImageCallback callback) {
		if (TextUtils.isEmpty(url)) {
			return;
		}
		if (!url.startsWith("http")) {
			loadDrawableFromNative(url, imageView, callback);
			return;
		}

		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(imageView.getContext().getApplicationContext(), MAXMEMORY_RATE_FOR_CHACHA);
			}
		}

		DisplayImageOptions op = new DisplayImageOptions.Builder().cloneFrom(mOptionsForNet)
				.displayer(new BitmapDisplayer() {

					@Override
					public void display(Bitmap bitmap, ImageAware imageAware, LoadedFrom loadedFrom) {
						if (bitmap != null) {
							callback.imageLoaded(new BitmapDrawable(bitmap), imageView, url);
						} else
							callback.imageLoaded(null, imageView, url);
					}
				}).build();

		mImageLoader.displayImage(url, imageView, op);

	}

	public void loadDrawableFromNative(final String path, final TextView textView, final ImageSpanCallback callback) {
		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(textView.getContext().getApplicationContext(), 1);
			}
		}

		String dir = path;
		if (!dir.startsWith("file://")) {
			dir = "file://" + dir;
		}
		DisplayImageOptions op = new DisplayImageOptions.Builder().cloneFrom(mOptionsForLocal).build();

		mImageLoader.loadImage(dir, op, new ImageLoadingListener() {
			@Override
			public void onLoadingStarted(String s, View view) {

			}

			@Override
			public void onLoadingFailed(String s, View view, FailReason failReason) {
				callback.imageLoaded(null, textView, path);
			}

			@Override
			public void onLoadingComplete(String s, View view, Bitmap bitmap) {
				callback.imageLoaded(bitmap, textView, path);
			}

			@Override
			public void onLoadingCancelled(String s, View view) {

			}
		});

	}


	public void loadDrawableFromNative(final String path, final ImageView imageView, final ImageCallback callback) {
		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(imageView.getContext().getApplicationContext(), MAXMEMORY_RATE_FOR_CHACHA);
			}
		}

		String dir = path;
		if (!dir.startsWith("file://")) {
			dir = "file://" + dir;
		}
		DisplayImageOptions op = new DisplayImageOptions.Builder().cloneFrom(mOptionsForLocal)
				.displayer(new BitmapDisplayer() {
					@Override
					public void display(Bitmap bitmap, ImageAware imageAware, LoadedFrom loadedFrom) {
						if (bitmap != null)
							callback.imageLoaded(new BitmapDrawable(bitmap), imageView, path);
						else
							callback.imageLoaded(null, imageView, path);
					}
				}).build();
		mImageLoader.displayImage(dir, imageView, op);

	}

	public void displayImage(Context mContext ,String url, ImageView imageView, DisplayImageOptions op) {
		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(mContext, MAXMEMORY_RATE_FOR_CHACHA);
			}
		}
		mImageLoader.displayImage(url, imageView, op);
	}

	public void displayImage(Context mContext ,String url, ImageView imageView, DisplayImageOptions op, ImageLoadingListener listener) {
		synchronized (TAG) {
			if (!mImageLoader.isInited()) {
				initImageLoader(mContext, MAXMEMORY_RATE_FOR_CHACHA);
			}
		}
		mImageLoader.displayImage(url, imageView, op, listener);
	}

	/**
	 * 图片加载回调
	 */
	public interface ImageCallback {
		void imageLoaded(Drawable drawable, ImageView iv, String url);
	}

	/**
	 * 图片加载回调
	 */
	public interface ImageSpanCallback {
		void imageLoaded(Bitmap bitmap, TextView tv, String url);
	}


	/**
	 * 判断是否有SD卡
	 * 
	 * @return
	 */
	private boolean hasSDCard() {
		return Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState());
	}

	public static int computeSampleSize(BitmapFactory.Options options, int minSideLength, int maxNumOfPixels) {
		int initialSize = computeInitialSampleSize(options, minSideLength, maxNumOfPixels);
		int roundedSize;
		if (initialSize <= 8) {
			roundedSize = 1;
			while (roundedSize < initialSize) {
				roundedSize <<= 1;
			}
		} else {
			roundedSize = (initialSize + 7) / 8 * 8;
		}

		return roundedSize;
	}

	private static int computeInitialSampleSize(BitmapFactory.Options options, int minSideLength, int maxNumOfPixels) {
		double w = options.outWidth;
		double h = options.outHeight;

		int lowerBound = (maxNumOfPixels == -1) ? 1 : (int) Math.ceil(Math.sqrt(w * h / maxNumOfPixels));
		int upperBound = (minSideLength == -1) ? 128 : (int) Math.min(Math.floor(w / minSideLength),
				Math.floor(h / minSideLength));

		if (upperBound < lowerBound) {
			return lowerBound;
		}

		if ((maxNumOfPixels == -1) && (minSideLength == -1)) {
			return 1;
		} else if (minSideLength == -1) {
			return lowerBound;
		} else {
			return upperBound;
		}
	}

	/**
	 * 获取图片路径
	 *
	 * @param url
	 * @return
	 */
	private String getImagePath(String url) {
		CRC32 crc32 = new CRC32();
		try {
			crc32.update(StringtoBytes.getBytes(url.toString()));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return IMAGE_DIRECT + "fail.png.tmp";
		}
		String afterName = url.substring(url.lastIndexOf(".") + 1);
		if (afterName.length() > 4 || afterName.length() == 0)
			afterName = "png.tmp";
		return IMAGE_DIRECT + crc32.getValue() + "." + afterName + ".tmp";
	}

	/**
	 * 将图片下载到SD卡缓存起来。
	 * 
	 * @param imageUrl
	 *            图片的URL地址。
	 */
	public String downloadImage(String imageUrl) {

		HttpURLConnection con = null;
		FileOutputStream fos = null;
		BufferedOutputStream bos = null;
		BufferedInputStream bis = null;
		File imageFile = null;
		try {
			URL url = new URL(imageUrl);
			con = (HttpURLConnection) url.openConnection();
			con.setConnectTimeout(5 * 1000);
			con.setReadTimeout(15 * 1000);
			con.setDoInput(true);
			bis = new BufferedInputStream(con.getInputStream());
			imageFile = new File(getImagePath(imageUrl));
			fos = new FileOutputStream(imageFile);
			bos = new BufferedOutputStream(fos);
			byte[] b = new byte[1024];
			int length;
			while ((length = bis.read(b)) != -1) {
				bos.write(b, 0, length);
				bos.flush();
			}
		} catch (Exception e) {
			if (imageFile != null)
				imageFile.delete();
		} finally {
			try {
				if (bis != null) {
					bis.close();
				}
				if (bos != null) {
					bos.close();
				}
				if (con != null) {
					con.disconnect();
				}
			} catch (IOException e) {
			}
		}
		if (imageFile != null) {
			Bitmap bitmap = decodeSampledBitmapFromResource(imageFile.getPath(), requireImageWidth);
			if (bitmap != null) {
				addBitmapToMemoryCache(imageUrl + requireImageWidth, bitmap);
			}
			return imageFile.getAbsolutePath();
		} else {
			return "";
		}
	}

	public static MemoryCache getmMemoryCache() {
		return ImageLoader.getInstance().getMemoryCache();
	}

	// public static void setmMemoryCache(LruCache<String, Bitmap> mMemoryCache)
	// {
	// AsyncImageLoader.mMemoryCache = mMemoryCache;
	// }

	public int getRequireWidth() {
		return requireImageWidth;
	}

	/**
	 * 获取本地图片路径(兼容果盘的AsyncImageLoader)
	 * 
	 * @param url
	 * @return
	 */
	public static String getNativeImagePath(String url) {
		File file = DiskCacheUtils.findInCache(url, ImageLoader.getInstance().getDiskCache());
		if (file != null)
			return file.getAbsolutePath();
		return null;
	}

	/**
	 * wxj, 调整Bitmap显示的方向
	 * 
	 * @param path
	 * @param bitmap
	 */
	public static Bitmap adjustOrientation(String path, Bitmap bitmap) {
		// if (bitmap == null)
		// return bitmap;
		// int degree = ImageFactory.getImageDegree(path);
		// // Log.e("wxj", "degree:"+degree);
		// if (degree % 360 == 0) {
		// Log.i(TAG, "degree % 360 == 0 " + degree);
		// return bitmap;
		// }
		// Log.i(TAG, "degree % 360 != 0 " + degree);
		// Matrix matrix = new Matrix();
		// matrix.postRotate(degree);
		// Bitmap destBitmap = Bitmap.createBitmap(bitmap, 0, 0,
		// bitmap.getWidth(), bitmap.getHeight(), matrix, true);
		// // ImageFactory.destroyBitmap(bitmap);
		// return destBitmap;

		return bitmap;
	}

	public static void initImageLoader(Context ctx, int max_rate) {
		boolean isCacheOnDisk = false;
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
			isCacheOnDisk = true;
		} else {
			isCacheOnDisk = false;
		}

		DisplayImageOptions op = new DisplayImageOptions.Builder().cloneFrom(DisplayImageOptions.createSimple())
				.cacheInMemory(true).cacheOnDisk(isCacheOnDisk).considerExifParams(true)
				// .bitmapConfig(Bitmap.Config.RGB_565)
				.imageScaleType(ImageScaleType.NONE).build();

		initImageLoader(ctx, max_rate, op);
	}

	public static void initImageLoader(Context ctx, int max_rate, DisplayImageOptions op) {
		File cacheDir = new File(IMAGE_DIRECT);
		LogTool.i(TAG, "cacheDir: " + cacheDir.getAbsolutePath());
		LogTool.i(TAG, "Process ID :" + android.os.Process.myPid());

		int maxMemory = (int) Runtime.getRuntime().maxMemory();

		int discCacheFileCount = 0;
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
			discCacheFileCount = 100;
		} else {
			discCacheFileCount = 10;
		}

		ImageLoaderConfiguration config = new ImageLoaderConfiguration.Builder(ctx).memoryCacheExtraOptions(4096, 4096)
				.denyCacheImageMultipleSizesInMemory().tasksProcessingOrder(QueueProcessingType.LIFO).threadPoolSize(4)
				.threadPriority(Thread.NORM_PRIORITY - 1).memoryCache(new LRULimitedMemoryCache(5 * 1024 * 1024)) // 5M
				.memoryCacheSize(5 * 1024 * 1024) // 5M
				// .memoryCache(new LRULimitedMemoryCache(cacheSize))
				// .memoryCacheSize(cacheSize)
				.discCacheFileCount(discCacheFileCount) // 缓存的文件数量
				.discCache(new UnlimitedDiscCache(cacheDir))// 自定义缓存路径
				.defaultDisplayImageOptions(op).imageDownloader(new BaseImageDownloader(ctx, 5 * 1000, 30 * 1000)) // connectTimeout
				.build();
		ImageLoader.getInstance().init(config);// 全局初始化此配置
	}

    /**==========================start xmod 迁移过来的方法===============================**/
    public void setItemSrcAsync(final String url, final ImageView imageView, final int defaultDrawableResId) {
        imageView.setImageResource(defaultDrawableResId);
        loadDrawable(url, imageView, new ImageCallback() {

            @Override
            public void imageLoaded(Drawable drawable, ImageView iv, String url) {
                if (drawable != null)
                    iv.setImageDrawable(drawable);
            }
        });
    }
    /**==========================end xmod 迁移过来的方法===============================**/
}
