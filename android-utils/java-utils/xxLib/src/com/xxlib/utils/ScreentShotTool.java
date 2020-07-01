package com.xxlib.utils;


import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.PixelFormat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.ShellTool;
import com.xxlib.utils.base.ShellTool.CommandResult;
import com.xxlib.utils.base.ShellTool.IShellListener;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;

public class ScreentShotTool {

    private static final String TAG = "ScreentShotUtil";

    private static final String CLASS1_NAME = "android.view.SurfaceControl";

    private static final String CLASS2_NAME = "android.view.Surface";

    private static final String METHOD_NAME = "screenshot";

    private static ScreentShotTool instance;

    private Display mDisplay;

    private DisplayMetrics mDisplayMetrics;

    private Matrix mDisplayMatrix;

    private WindowManager wm;

    private SimpleDateFormat format;

    private ScreentShotTool() {

    }

    public static ScreentShotTool getInstance() {
        synchronized (ScreentShotTool.class) {
            if (instance == null) {
                instance = new ScreentShotTool();
            }
        }
        return instance;
    }

    private Bitmap screenShot(int width, int height) {
        Log.i(TAG, "android.os.Build.VERSION.SDK : " + android.os.Build.VERSION.SDK_INT);
        Class<?> surfaceClass = null;
        Method method = null;
        try {
            Log.i(TAG, "width : " + width);
            Log.i(TAG, "height : " + height);
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR2) {

                surfaceClass = Class.forName(CLASS1_NAME);
            } else {
                surfaceClass = Class.forName(CLASS2_NAME);
            }
            method = surfaceClass.getDeclaredMethod(METHOD_NAME, int.class, int.class);
            method.setAccessible(true);
            return (Bitmap) method.invoke(null, width, height);
        } catch (NoSuchMethodException e) {
            LogTool.e(TAG, e.toString());
        } catch (IllegalArgumentException e) {
            LogTool.e(TAG, e.toString());
        } catch (IllegalAccessException e) {
            LogTool.e(TAG, e.toString());
        } catch (InvocationTargetException e) {
            LogTool.e(TAG, e.toString());
        } catch (ClassNotFoundException e) {
            LogTool.e(TAG, e.toString());
        } catch (Exception e) {
            LogTool.e(TAG, e.toString());
        }
        return null;
    }

    /**
     * Takes a screenshot of the current display and shows an animation.
     */
    @SuppressLint("NewApi")
    public boolean takeScreenshot(Context context, String fileFullPath, IShellListener listener) {
        boolean IS_USE_SECOND_WAY = true;
        boolean result = false;

        LogTool.i(TAG, "开始截屏");
        File fileDir = context.getFilesDir();
        if (!fileDir.exists()) {
            fileDir.mkdirs();
        }
//        String fileName = "screenshotTmp.png";
//        File shotFile = new File(fileDir, fileName);
        File shotFile = new File(StringUtils.format("/sdcard/%s/screenshotTmp.png", context.getPackageName()));

        // root方案
        if (ShellTool.checkRootPermission()) {
//			if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH){
            if (listener != null) {
                listener.onRootSuccess();
            }

            boolean isScreencapByRawFile = true; // 截屏方式开关, true--raw读取， false-- srcreencap -p方式
            if (isScreencapByRawFile) {
//                String rawFilePath = fileDir.getAbsolutePath() + "/screen.raw";
                String rawFilePath = StringUtils.format("/sdcard/%s/screen.raw", context.getPackageName());
                boolean isExecScreepcapSucc = screenCapRawFile(rawFilePath);
                if (isExecScreepcapSucc) {
                    Bitmap bitmap = parseRawFileToBitmap(context, rawFilePath);
                    long cur = System.currentTimeMillis();
                    result = saveBitmap2file(context, bitmap, fileFullPath);
                    LogTool.i(TAG, "读取文件   保存BitMap，耗时: " + (System.currentTimeMillis() - cur));
                }
            } else {
                result = screenShotByScreencapPng(shotFile, fileFullPath);
            }
//			}
        }

        // 如果使用root方案不行，使用以下方案，调用高系统的api
        if (!result && IS_USE_SECOND_WAY) {
            result = useSystemApiToScreencap(context, fileFullPath);
        }
        return result;
    }


    @SuppressLint("NewApi")
    private boolean useSystemApiToScreencap(Context context, String fileFullPath) {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
//				if(android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.JELLY_BEAN_MR2 && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.ICE_CREAM_SANDWICH){
            LogTool.i(TAG, "开始截屏2 begin");
            long cur = System.currentTimeMillis();
            wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
            mDisplay = wm.getDefaultDisplay();
            mDisplayMatrix = new Matrix();
            mDisplayMetrics = new DisplayMetrics();
            // We need to orient the screenshot correctly (and the Surface api seems to take screenshots
            // only in the natural orientation of the device :!)
            mDisplay.getRealMetrics(mDisplayMetrics);
            float[] dims =
                    {
                            mDisplayMetrics.widthPixels, mDisplayMetrics.heightPixels
                    };
            float degrees = getDegreesForRotation(mDisplay.getRotation());
            boolean requiresRotation = (degrees > 0);
            if (requiresRotation) {
                // Get the dimensions of the device in its native orientation
                mDisplayMatrix.reset();
                mDisplayMatrix.preRotate(-degrees);
                mDisplayMatrix.mapPoints(dims);
                dims[0] = Math.abs(dims[0]);
                dims[1] = Math.abs(dims[1]);
            }

            Bitmap mScreenBitmap = screenShot((int) dims[0], (int) dims[1]);
            // If we couldn't take the screenshot, notify the user
            if (mScreenBitmap == null || mScreenBitmap.getWidth() < 100 || mScreenBitmap.getHeight() < 100) {
//					Toast.makeText(context, "截屏失败", Toast.LENGTH_SHORT).show();
                return false;
            }
            Log.e("wxj", "api截屏 bitmap:" + mScreenBitmap);
            if (requiresRotation) {
                // Rotate the screenshot to the current orientation
                Bitmap ss = Bitmap.createBitmap(mDisplayMetrics.widthPixels, mDisplayMetrics.heightPixels,
                        Bitmap.Config.ARGB_8888);
                Canvas c = new Canvas(ss);
                c.translate(ss.getWidth() / 2, ss.getHeight() / 2);
                c.rotate(degrees);
                c.translate(-dims[0] / 2, -dims[1] / 2);
                c.drawBitmap(mScreenBitmap, 0, 0, null);
                c.setBitmap(null);
                mScreenBitmap = ss;
                if (ss != null && !ss.isRecycled()) {
                    ss.recycle();
                }
            }

            // Optimizations
            mScreenBitmap.setHasAlpha(false);
            mScreenBitmap.prepareToDraw();

            boolean isSaveSucc = saveBitmap2file(context, mScreenBitmap, fileFullPath);
            LogTool.i(TAG, "screen cap by system api, isSaveSucc " + isSaveSucc);
            return isSaveSucc;
        }
        return false;
    }

    /**
     * 用screepcap生成一个raw文件，放回raw文件的了路劲
     * @return
     */
    private boolean screenCapRawFile(String path) {
        boolean isSucc = false;
        long cur = System.currentTimeMillis();
        CommandResult cmdResult = ShellTool.execCommand("/system/bin/screencap " + path,
                "chmod 777 " + path, true);
        if (cmdResult.result == 0 && new File(path).length() > 100) {
            LogTool.i(TAG, "执行screencap命令，生成raw，耗时: " + (System.currentTimeMillis() - cur));
            isSucc = true;
        }
        return isSucc;
    }
    /**
     *获取屏幕宽度和高度
     * @return int[0]:宽度， int[1]:高度
     * create by chenrenzhan 2015-11-30
     */
    @SuppressLint("NewApi")
    private int[] getDisplayScreenResolution(Context context) {
        int[] screen = new int[2];
        int version = android.os.Build.VERSION.SDK_INT;

        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        Display display = wm.getDefaultDisplay();

        DisplayMetrics displayMetrics = new DisplayMetrics();

        int height = 0;
        int navigationBarHeight = 0;
        if(version < 17){
            display.getMetrics(displayMetrics);
            height = displayMetrics.heightPixels;
            navigationBarHeight = getNavigationBarHeight(context);
            height += navigationBarHeight;
        } else{
            display.getRealMetrics(displayMetrics);
            height = displayMetrics.heightPixels;
        }

        int width = displayMetrics.widthPixels;
        Log.d(TAG, "Run1 first get resolution:" + width + " * " + height + ", version " + version + ", " + (version >= 17));
        Log.i(TAG, "getNavigationBarHeight = " + height);
        screen[0] = width;
        screen[1] = height;

        return screen;
    }

    /**
     *获取虚拟按钮栏高度
     * create by chenrenzhan 2015-11-30
     */
    private int getNavigationBarHeight(Context context) {
        Resources resources = context.getResources();
        int h = 0;
        int resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android");
        if (resourceId > 0) {
            h = resources.getDimensionPixelSize(resourceId);
        }
        return h;
    }

    @SuppressLint("NewApi")
    private Bitmap parseRawFileToBitmap(Context context, String rawFilePath) {
        long cur = System.currentTimeMillis();

        int[] screenMetrics = getDisplayScreenResolution(context);
        int screenWidth = screenMetrics[0];
        int screenHeight = screenMetrics[1];

        int smallScreen = screenWidth;
        int largeScreen = screenHeight;
        if (screenHeight < screenWidth) {
            smallScreen = screenHeight;
            largeScreen = screenWidth;
        }

        wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        mDisplay = wm.getDefaultDisplay();
        int pixelformat = mDisplay.getPixelFormat();
        PixelFormat localPixelFormat1 = new PixelFormat();
        PixelFormat.getPixelFormatInfo(pixelformat, localPixelFormat1);
        int deepth = localPixelFormat1.bytesPerPixel;// 位深

        LogTool.i(TAG, "screenWidth " + screenWidth);
        LogTool.i(TAG, "screenHeight " + screenHeight);
        LogTool.i(TAG, "deepth " + deepth);

        int screenSize = screenWidth * screenHeight;
        byte[] piex = null;
        int[] colors = null;
        Bitmap bitmap = null;


        File fbFile = new File(rawFilePath);
        DataInputStream dStream = null;
        FileInputStream buf = null;// 读取文件内容
        try {
            buf = new FileInputStream(fbFile);
            dStream = new DataInputStream(buf);
            int beginOffset = buf.available() - screenSize * deepth;
            LogTool.i(TAG, "beginOffset " + beginOffset);
            dStream.read(new byte[beginOffset]);

            int readWay = 1; // 从文件流里读取方式：0--一次读完数组， 1--分多次读到数组， 2--分多次读到流
            if (readWay == 0) {
                //一次读完
                piex = new byte[screenSize * deepth];
                colors = new int[screenSize];
                dStream.readFully(piex);
                for (int m = 0; m < colors.length; m++) {
                    int r = (piex[m * deepth] & 0xFF);
                    int g = (piex[m * deepth + 1] & 0xFF);
                    int b = (piex[m * deepth + 2] & 0xFF);
                    int a = (piex[m * deepth + 3] & 0xFF);
                    colors[m] = (a << 24) + (r << 16) + (g << 8) + b;
                }
               bitmap = Bitmap.createBitmap(colors, smallScreen, largeScreen,
                        Bitmap.Config.ARGB_8888);
            } else if(readWay == 1){
                //分多次读取
                colors = new int[screenSize];
                for (int i = 0; i < deepth; ++i) {
                    piex = new byte[screenSize];
                    int length = dStream.read(piex);
                    int size = length / 4;
                    for (int m = 0; m < size; m++) {
                        int r = (piex[m * deepth] & 0xFF);
                        int g = (piex[m * deepth + 1] & 0xFF);
                        int b = (piex[m * deepth + 2] & 0xFF);
                        int a = (piex[m * deepth + 3] & 0xFF);
                        colors[size * i + m] = (a << 24) + (r << 16) + (g << 8) + b;
                    }
                }
                bitmap = Bitmap.createBitmap(colors, smallScreen, largeScreen,
                        Bitmap.Config.ARGB_8888);
            }
            else if(readWay == 2){
                //TODO

            }
            dStream.close();
            buf.close();
        } catch (Exception e) {
            e.printStackTrace();
            LogTool.e(TAG, e.toString());
        }

        LogTool.i(TAG, "读取文件  生成BitMap，耗时: " + (System.currentTimeMillis() - cur));
        LogTool.i(TAG, "截屏结束，耗时: " + (System.currentTimeMillis() - cur));
        if(bitmap != null){
            bitmap.setHasAlpha(false);
            bitmap.prepareToDraw();
        }

        return bitmap;
    }

    /**
     * 直接运行截屏命令，生成png图片
     */
    private boolean screenShotByScreencapPng(File shotFile, String fileFullPath) {
        Log.i(TAG, "执行截屏命令~");
        boolean result = false;
        long cur = System.currentTimeMillis();
        CommandResult cmdResult = ShellTool.execCommand(
                "/system/bin/screencap -p " + shotFile.getAbsolutePath(),
                "chmod 777 " + shotFile.getAbsolutePath(), true);
        LogTool.i(TAG, "开始截屏1,cmdResult.result:" + cmdResult.result);
        LogTool.i(TAG, "开始截屏1,cmdResult.error:" + cmdResult.errorMsg);
        LogTool.i(TAG, "开始截屏1,file:" + shotFile.exists());
        LogTool.i(TAG, "开始截屏1,file。length:" + shotFile.length());
        LogTool.i(TAG, "run screencap cost " + (System.currentTimeMillis() - cur));
        if (cmdResult.result == 0 && shotFile.exists() && shotFile.length() > 100)
            result = true;

        if (result) {
            cur = System.currentTimeMillis();
            boolean cpResult = false;
            try {
                cpResult = FileUtils.copyFile(shotFile.getAbsolutePath(), fileFullPath);
            } catch (Exception e) {
                e.printStackTrace();
                LogTool.w(TAG, e.toString());
            }
            if (cpResult) {
                shotFile.delete();
            }
            LogTool.i(TAG, "开始截屏1,fileFullPath " + fileFullPath);
            LogTool.i(TAG, "copy screencap cost " + (System.currentTimeMillis() - cur) + " copy result " + cpResult);

        }
        return result;
    }

    /**
     * bitmap保存为文件
     *
     * @param context
     * @param bmp
     * @param fileName
     * @return
     * @description
     */
    public boolean saveBitmap2file(Context context, Bitmap bmp, String fileName) {
        if (context == null || bmp == null || bmp.isRecycled() || fileName == null) {
            return false;
        }
        long curTemp = System.currentTimeMillis();
        Log.e(TAG, "截屏成功，开始保存图片");
        int quality = 100;
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        bmp.compress(Bitmap.CompressFormat.PNG, quality, baos);
        InputStream is = new ByteArrayInputStream(baos.toByteArray());
        byte[] buffer = new byte[1024];
        int len = 0;
        File file = new File(fileName);
        if (!file.exists()) {
            try {
                file.getParentFile().mkdir();
                file.getParentFile().createNewFile();
            } catch (IOException e) {
                Log.e(TAG, e.toString());
            }
        } else {
            try {
                file.getParentFile().delete();
                file.getParentFile().createNewFile();
            } catch (IOException e) {
                Log.e(TAG, e.toString());
            }
        }
        LogTool.i(TAG, "cost 1 " + (System.currentTimeMillis()-curTemp));
        curTemp = System.currentTimeMillis();
        FileOutputStream stream = null;
        try {
            stream = new FileOutputStream(file);
            while ((len = is.read(buffer)) != -1) {
                stream.write(buffer, 0, len);
            }
            stream.flush();
            LogTool.i(TAG, "cost 2, read to output stream" + (System.currentTimeMillis()-curTemp));
            return true;
        } catch (FileNotFoundException e) {
            Log.i(TAG, e.toString());
        } catch (IOException e) {
            Log.i(TAG, e.toString());
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    Log.i(TAG, e.toString());
                }
            }
            if (stream != null) {
                try {
                    stream.close();
                } catch (IOException e) {
                    Log.i(TAG, e.toString());
                }
            }
            if (bmp != null && !bmp.isRecycled()) {
                bmp.recycle();
            }
        }

        LogTool.i(TAG, "cost 3, read to output stream" + (System.currentTimeMillis()-curTemp));
        return false;

    }

    /**
     * @return the current display rotation in degrees
     */
    private float getDegreesForRotation(int value) {
        switch (value) {
            case Surface.ROTATION_90:
                return 360f - 90f;
            case Surface.ROTATION_180:
                return 360f - 180f;
            case Surface.ROTATION_270:
                return 360f - 270f;
        }
        return 0f;
    }

}
