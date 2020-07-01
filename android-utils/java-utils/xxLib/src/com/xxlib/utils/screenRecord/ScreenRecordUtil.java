package com.xxlib.utils.screenRecord;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.MediaPlayer;
import android.media.ThumbnailUtils;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

import com.xxlib.R;
import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.FileUtils;
import com.xxlib.utils.ImageFactory;
import com.xxlib.utils.MD5Helper;
import com.xxlib.utils.SPCenter.SPCenter;
import com.xxlib.utils.SPCenter.XXLibSpkey;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.ShellTool;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;

/**
 * @author linbin 2015-4-14 -- 上午10:43:32
 * @Description 视频录制工具类 针对4.x
 * @modify xiaQing 2015-5-28
 */

@SuppressLint({"SdCardPath", "HandlerLeak"})
public class ScreenRecordUtil {
    private static final String TAG = "ScreenRecordUtil";

    private final static String PACKAGE_NAME = ApplicationUtils.getApplication().getPackageName();
    // 录制的视频的画质
    private final static String IS_SCREENRECORDER_QUALITY_NORMAL = "IS_SCREENRECORDER_QUALITY_NORMAL";
    private static String MAIN_DIRECT = Environment.getExternalStorageDirectory().toString() + PACKAGE_NAME;
    private static String DOWNLOAD_DIRECT = MAIN_DIRECT + "download/";

    private WindowManager mWindowManager;
    // 屏幕宽 屏幕高
    private static float WINDOW_WIDTH;
    private static float WINDOW_HEIGHT;

    public static final boolean isAvcFormat = false;

    public static String sNoRootString = "";

    public enum ScreenType {Normal, Height}

    private boolean mIsRootFail = false;

    private static boolean sIsFixScreenParam = false;

    private static int sFixWidth;
    private static int sFixHeight;

    int mScreenWidth;
    int mScreenHeight;
    //	int width = 0;
//	int height = 0;
    String mPath;
    Process mProcess;
    OutputStream mStdin;
    InputStream mStdout;

    Context mContext;

    private ComposeListener mComposeListener;

    public ScreenRecordUtil(Context context) {
        this(context, null);
    }

    public ScreenRecordUtil(Context context, String path) {
        mContext = context;

        if (path == null) {
            String defaultPath = "/sdcard/" + PACKAGE_NAME + "/videos";
            this.mPath = defaultPath + "/xx_" + System.currentTimeMillis() + ".mp4";
        } else {
            this.mPath = path;
        }

        mWindowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
        WINDOW_WIDTH = mWindowManager.getDefaultDisplay().getWidth();
        WINDOW_HEIGHT = mWindowManager.getDefaultDisplay().getHeight();

        // 5.0以上的不用改可执行文件来录屏
        if (android.os.Build.VERSION.SDK_INT >= 21)
            return;

        // 拷贝可执行文件screenrec
        String screenRecOutPath = "/data/data/" + PACKAGE_NAME + "/screenrec";
        copyFileToDataDataXmod(context, R.raw.screenrec, screenRecOutPath);
        // 拷贝可执行文件audio， 4.0和4.0以下没有audio这个文件
        if (android.os.Build.VERSION.SDK_INT > 15) {
            String audioOutPath = "/data/data/" + PACKAGE_NAME + "/audio";
            copyFileToDataDataXmod(context, R.raw.audio, audioOutPath);
        }

        // 修改可执行文件的权限为777
        Thread thread = new Thread() {
            public void run() {
                Runtime localRuntime = Runtime.getRuntime();
                try {
                    localRuntime.exec("chmod 777 /data/data/" + PACKAGE_NAME + "/screenrec");
                    if (android.os.Build.VERSION.SDK_INT > 15)    // 4.0和4.0以下没有audio这个文件
                        localRuntime.exec("chmod 777 /data/data/" + PACKAGE_NAME + "/audio");
                } catch (IOException e) {
                    e.printStackTrace();
                    LogTool.e(TAG, "thread:" + e.getMessage());
                    return;
                }
            }

            ;
        };
        thread.start();
    }

    /**
     * 申请root权限给screenrec
     */
    private boolean askRootForScreenRec() {
        String[] arrayOfString = new String[1];
        arrayOfString[0] = "/data/data/" + PACKAGE_NAME + "/screenrec\n";
        try {
            if (ShellTool.checkRootPermission()) {
                mProcess = ShellTool.mRootProcess.getProcess();
                mStdin = mProcess.getOutputStream();
                mStdout = mProcess.getInputStream();
                mStdin.write(arrayOfString[0].getBytes("utf-8"));
                mStdin.flush();
                byte result[] = new byte[10];
                int count = mStdout.read(result);
                if (count > 0) {
                    LogTool.i(TAG, new String(result));
                }
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
            LogTool.e(TAG, "askRootForScreenRec:" + e.getMessage());
            return false;
        }
    }

    private void copyFileToDataDataXmod(Context context, int fileResId, String outFilePath) {
        try {
            File outFile = new File(outFilePath);
            if (!outFile.exists()) {
                outFile.createNewFile();

                InputStream in = context.getResources().openRawResource(fileResId);
                OutputStream out = new FileOutputStream(outFile);

                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }

                in.close();
                out.close();
            } else {
                LogTool.i(TAG, "copyFileToDataDataXmod outFilePath exists:" + outFilePath);
            }
        } catch (Exception e) {
            LogTool.e(TAG, "copyFileToDataDataXmod:" + e.getMessage());
        }
    }

    private void getScreenParams() {
        if (!sIsFixScreenParam) {
            Display display = mWindowManager.getDefaultDisplay();
            DisplayMetrics dm = new DisplayMetrics();
            try {
                @SuppressWarnings("rawtypes")
                Class c = Class.forName("android.view.Display");
                @SuppressWarnings("unchecked")
                Method method = c.getMethod("getRealMetrics", DisplayMetrics.class);
                method.invoke(display, dm);
                mScreenWidth = dm.widthPixels;
                mScreenHeight = dm.heightPixels;
                LogTool.i(TAG, "dm.widthPixels:" + mScreenWidth);
                LogTool.i(TAG, "dm.heightPixels:" + mScreenHeight);
            } catch (Exception e) {
                e.printStackTrace();
                mScreenWidth = (int) WINDOW_WIDTH;
                mScreenHeight = (int) WINDOW_HEIGHT;
            }
        } else {
            mScreenHeight = sFixHeight;
            mScreenWidth = sFixWidth;
        }
    }

    public boolean isRootFail() {
        return mIsRootFail;
    }

    public void startRecordScreen(final ScreenRecordListener listener, final boolean isNormal, String path) {
        this.mPath = path;
        Thread thread = new Thread() {
            public void run() {
                // 申请root权限给到screenrec这个可执行文件，audio不需要申请，被screenrec内部用到
                if (!askRootForScreenRec()) {
                    mIsRootFail = true;
                    listener.onFail();
                } else {

                    // rotation
                    WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
                    int rotation = wm.getDefaultDisplay().getRotation() * 90;
                    if (rotation == 90 || rotation == 270) {
                        rotation = 360 - rotation;
                    }
                    LogTool.i(TAG, "rotation:" + rotation);

                    ScreenType type;
                    if (isNormal) {
                        type = ScreenType.Normal;
                    } else {
                        type = ScreenType.Height;
                    }

                    // width, height
                    getScreenParams();
                    int width = (type == ScreenType.Normal ? 720 : 1080);
//                    int width = 720;
                    int height;
                    if (mScreenWidth < mScreenHeight) {
                        height = width * mScreenHeight / mScreenWidth;
                    } else {
                        height = width * mScreenWidth / mScreenHeight;
                    }
                    height = height - height % 4;
                    LogTool.i(TAG, "width:" + width);
                    LogTool.i(TAG, "height:" + height);

                    // 标记是否是竖屏的
                    // 兼容xmod，当前仅xmod使用
                    if (mScreenHeight > mScreenWidth) {
                        SPCenter.putBoolean(XXLibSpkey.IS_VIDEO_VERTICAL + MD5Helper.encode(mPath), true, SPCenter.FLOATVIEW_PREFERENCE);
                    }

                    // bitrate
                    int bitrate;
                    if (isAvcFormat) {
                        bitrate = (type == ScreenType.Normal ? 500000 : 1000000);
                    } else {
                        bitrate = (type == ScreenType.Normal ? 6000 : 15000);
                    }

                    // codec:  2 avc编码; -2 mpeg-4编码
                    int codecFormat = (isAvcFormat ? 2 : -2);

                    byte[] arrayOfByte = (mPath + "\n").getBytes();
                    try {
                        mStdin.write(arrayOfByte);
                        mStdin.flush();

                        arrayOfByte = (rotation + "\n").getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "m\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = (height + "\n").getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = (width + "\n").getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "0\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "0\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "30\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "CPU\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "RGBA\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = (bitrate + "\n").getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "16000\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = (codecFormat + "\n").getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        arrayOfByte = "0\n".getBytes();
                        mStdin.write(arrayOfByte);
                        mStdin.flush();
                        LogTool.i(TAG, "arrayOfByte:" + arrayOfByte.toString());
                        listener.onSuccess();
                    } catch (Exception e) {
                        e.printStackTrace();
                        listener.onFail();
                    }
                }

            }
        };
        thread.start();
    }

    public void startRecordScreen(final ScreenRecordListener listener) {

        Thread thread = new Thread() {
            public void run() {
                // 申请root权限给到screenrec这个可执行文件，audio不需要申请，被screenrec内部用到
                askRootForScreenRec();

                // rotation
                WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
                int rotation = wm.getDefaultDisplay().getRotation() * 90;
                if (rotation == 90 || rotation == 270) {
                    rotation = 360 - rotation;
                }
                LogTool.i(TAG, "rotation:" + rotation);

                ScreenType type;
                if (SPCenter.getBoolean(IS_SCREENRECORDER_QUALITY_NORMAL, true, SPCenter.FLOATVIEW_PREFERENCE)) {
                    type = ScreenType.Normal;
                } else {
                    type = ScreenType.Height;
                }

                // width, height
                getScreenParams();
//				int width = (type == ScreenType.Normal ? 720 : 1080);
                int width = 720;
                int height;
                if (mScreenWidth < mScreenHeight) {
                    height = width * mScreenHeight / mScreenWidth;
                } else {
                    height = width * mScreenWidth / mScreenHeight;
                }
                height = height - height % 4;
                LogTool.i(TAG, "width:" + width);
                LogTool.i(TAG, "height:" + height);

                // 标记是否是竖屏的
                // 兼容xmod，当前仅xmod使用
                if (mScreenHeight > mScreenWidth) {
                    SPCenter.putBoolean(XXLibSpkey.IS_VIDEO_VERTICAL + MD5Helper.encode(mPath), true, SPCenter.FLOATVIEW_PREFERENCE);
                }

                // bitrate
                int bitrate;
                if (isAvcFormat) {
                    bitrate = (type == ScreenType.Normal ? 500000 : 1000000);
                } else {
                    bitrate = (type == ScreenType.Normal ? 6000 : 15000);
                }

                // codec:  2 avc编码; -2 mpeg-4编码
                int codecFormat = (isAvcFormat ? 2 : -2);

                byte[] arrayOfByte = (mPath + "\n").getBytes();
                try {
                    mStdin.write(arrayOfByte);
                    mStdin.flush();

                    arrayOfByte = (rotation + "\n").getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "m\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = (height + "\n").getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = (width + "\n").getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "0\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "0\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "30\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "CPU\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "RGBA\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = (bitrate + "\n").getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "16000\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = (codecFormat + "\n").getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    arrayOfByte = "0\n".getBytes();
                    mStdin.write(arrayOfByte);
                    mStdin.flush();
                    LogTool.i(TAG, "arrayOfByte:" + arrayOfByte.toString());
                    listener.onSuccess();
                } catch (Exception e) {
                    e.printStackTrace();
                    listener.onFail();
                }

            }
        };
        thread.start();
    }

    private final static int MSG_DELAY = 1;
    private final static int MSG_NOTIFY_OUTSIDE = 2;

    public void finishScreenRecord(ComposeListener composeListener) {
        this.mComposeListener = composeListener;
        byte[] arrayOfByte = "stop\n".getBytes();
        try {

            this.mStdin.write(arrayOfByte);
            this.mStdin.flush();
            byte result[] = new byte[mStdout.available()];
            mStdout.read(result);
            LogTool.i(TAG, "stop output : " + new String(result));
        } catch (Exception e) {
            e.printStackTrace();
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                Message msg = Message.obtain();
                msg.what = MSG_DELAY;
                handler.sendMessageDelayed(msg, 1000);
            }

            Handler handler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    switch (msg.what) {
                        case MSG_DELAY:
                            sendEmptyMessage(MSG_NOTIFY_OUTSIDE);
                            break;
                        case MSG_NOTIFY_OUTSIDE:
                            if (mComposeListener != null) {
                                mComposeListener.finish();
                                mComposeListener = null;
                            }
                            break;
                        default:
                            break;
                    }

                }
            };

        }).start();
    }

    public String getVideoPath() {
        return mPath;
    }

    public interface ScreenRecordListener {
        void onSuccess();

        void onFail();
    }

    /**
     * @param filePath videoPath
     * @return 如果返回null 说明文件破坏或格式不正确
     * @description 获取视频的缩略图
     */
    public static Bitmap getVideoThumbnail(String filePath) {
        Bitmap bitmap;
        Bitmap newBitmap = null;
        try {
            bitmap = ThumbnailUtils.createVideoThumbnail(filePath, Images.Thumbnails.FULL_SCREEN_KIND);
            if (bitmap != null) {
                int oldWidth = bitmap.getWidth();
                int oldHeight = bitmap.getHeight();
                LogTool.i(TAG, "createVideoThumbnail width " + bitmap.getWidth());
                LogTool.i(TAG, "createVideoThumbnail height " + bitmap.getHeight());
                FileUtils.saveImage(bitmap, new File(DOWNLOAD_DIRECT, "aaaaa.png"));
                int newWidth;
                int newHeight;
                if (oldHeight > oldWidth) {
                    // 属于竖屏 分辨率要控制，宽度最大720 zemin
                    newWidth = 720;
                } else {
                    // 属于横屏，分辨率控制 zemin
                    newWidth = 960;
                }
                if (oldWidth < newWidth) {
                    return bitmap;
                }
                newHeight = oldHeight * newWidth / oldWidth;
                newBitmap = ThumbnailUtils.extractThumbnail(bitmap, newWidth, newHeight);

                if (bitmap != newBitmap && bitmap != null && !bitmap.isRecycled()) {
                    bitmap.recycle();
                }
            }
        } catch (Throwable e) {
            LogTool.e(TAG, LogTool.getStackTraceString(e));
        }

        if (newBitmap == null) {
            LogTool.e(TAG, "取不到视频缩略图： 视频格式不支持");
            newBitmap = Bitmap.createBitmap(600, 600, Config.ARGB_8888);
        }

        return newBitmap;
    }

    /**
     * 获取指定宽高的视频缩略图
     */
    public static Bitmap getVideoSmallBitmap(String filePath, int width, int height) {
//		Bitmap bitmap = getVideoThumbnail(filePath, Images.Thumbnails.MICRO_KIND);
        Bitmap bitmap = ThumbnailUtils.createVideoThumbnail(filePath, Images.Thumbnails.MICRO_KIND);
        Bitmap smallBitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height);
        if (smallBitmap == null) {
            smallBitmap = Bitmap.createBitmap(600, 600, Config.ARGB_8888);
        }
        return smallBitmap;
    }

    /**
     * @param filePath 视频路径
     * @return 时长
     * @description 获取视频的时长
     */
    public static int getVideoLength(Context context, String filePath) {
        try {
            File file = new File(filePath);
            if (!file.exists()) {
                LogTool.e(TAG, "取不到视频长度： 视频文件不存在");
                return 0;
            }

            MediaPlayer mediaPlayer = MediaPlayer.create(context, Uri.parse(filePath));
            if (mediaPlayer == null) {
                LogTool.e(TAG, "取不到视频长度：mediaPlayer 为空");
                return 0;
            }
            int duration = mediaPlayer.getDuration();
            mediaPlayer.release();

            LogTool.i(TAG, "getDuration:" + duration);
            return duration;
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
    }

    /**
     * =====================xmod 迁移过来的方法=======================
     **/
    public static Bitmap getVideoImage(String videoPath) {
        if (!new File(videoPath).exists()) {
            return Bitmap.createBitmap(192, 144, Bitmap.Config.ARGB_8888);
        }
        boolean isVideoVertical = SPCenter.getBoolean(XXLibSpkey.IS_VIDEO_VERTICAL + MD5Helper.encode(videoPath), false, SPCenter.FLOATVIEW_PREFERENCE);
        Bitmap bitmap = null;
        try {
            bitmap = ThumbnailUtils.createVideoThumbnail(videoPath, MediaStore.Images.Thumbnails.FULL_SCREEN_KIND);
        } catch (Throwable t) {
            LogTool.e(TAG, t.toString());
        }
        if (null == bitmap) {
            return Bitmap.createBitmap(192, 144, Bitmap.Config.ARGB_8888);
        }
        LogTool.i(TAG, "getHeight " + bitmap.getHeight());
        LogTool.i(TAG, "getWidth " + bitmap.getWidth());
        if (isVideoVertical && bitmap.getWidth() > bitmap.getHeight()) {
            // 竖屏，而且宽度比高度大，那么是图片不对。在9300的机器会这种
            try {
                Bitmap bitmapNew = ImageFactory.rotateBitmap(bitmap, 90);
                ImageFactory.destroyBitmap(bitmap);
                return bitmapNew;
            } catch (Throwable t) {
                LogTool.e(TAG, t.toString());
                return Bitmap.createBitmap(192, 144, Bitmap.Config.ARGB_8888);
            }
        }
        return bitmap;
    }

    public static Drawable getSmallVideoImage(String videoPath) {
        Bitmap bitmap = getVideoImage(videoPath);
        Bitmap smallBitmap = ThumbnailUtils.extractThumbnail(bitmap, 192, 144);    // 目前的缩列图ui大小
        ImageFactory.destroyBitmap(bitmap);
        if (smallBitmap == null) {
            smallBitmap = Bitmap.createBitmap(192, 144, Bitmap.Config.ARGB_8888);
        }
        return new BitmapDrawable(smallBitmap);
    }

    public static void setIsFixScreenParams(boolean isFixScreenParams) {
        sIsFixScreenParam = isFixScreenParams;
    }

    public static void setFixScreenParams(int width, int height) {
        sFixWidth = width;
        sFixHeight = height;
    }
}
