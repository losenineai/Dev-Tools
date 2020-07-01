package com.xxlib.utils.RegisterModule;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import com.xxlib.utils.base.LogTool;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.io.StringWriter;

/**
 * 文件操作工具类
 *
 * @author wxj 2014
 */
public class FileUtil {
    private static final String TAG = "FileUtil";
    // 默认字符编码
    public static final String CHARSET = "utf-8";

    /**
     * 删除前先rename到另一个地方
     *
     * @param path
     * @return
     */
    public static boolean deleteFileSafely(String path) {
        try {
            File file = new File(path);
            if (file.exists()) {
                File fileTmp = new File(path + System.currentTimeMillis());
                file.renameTo(fileTmp);
                return fileTmp.delete();
            }
        } catch (Exception e) {
            LogTool.i(TAG, LogTool.getStackTraceString(e));
        }
        return false;
    }

    public static boolean deleteDirectorySafety(String filePath) {
        boolean flag = false;
        // 如果filePath不以文件分隔符结尾，自动添加文件分隔符
        if (!filePath.endsWith(File.separator)) {
            filePath = filePath + File.separator;
        }
        File dirFile = new File(filePath);
        if (!dirFile.exists() || !dirFile.isDirectory()) {
            return false;
        }
        flag = true;
        File[] files = dirFile.listFiles();
        // 遍历删除文件夹下的所有文件(包括子目录)
        for (int i = 0; i < files.length; i++) {
            if (files[i].isFile()) {
                // 删除子文件
                flag = deleteFileSafely(files[i].getAbsolutePath());
                if (!flag)
                    break;
            } else {
                // 删除子目录
                flag = deleteDirectorySafety(files[i].getAbsolutePath());
                if (!flag)
                    break;
            }
        }
        if (!flag)
            return false;
        // 删除当前空目录
        return false;
    }

    /**
     * 保存图片
     *
     * @param bitmap   图片
     * @param destFile 目标文件
     * @return
     */
    public static boolean saveImage(Bitmap bitmap, File destFile) {
        return saveImage(bitmap, destFile, Bitmap.CompressFormat.JPEG);
    }

    /**
     * 保存图片
     *
     * @param bitmap   图片
     * @param destFile 目标文件
     * @return
     */
    public static boolean saveImage(Bitmap bitmap, File destFile, Bitmap.CompressFormat format) {
        if (bitmap == null || destFile == null) {
            return false;
        }

        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(destFile);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            bitmap.compress(format, 100, baos);
            int options = 100;
            // 如果大于80kb则再次压缩,最多压缩三次
            while (baos.toByteArray().length / 1024 > 50 && options != 10) {
                // 清空baos
                baos.reset();
                // 这里压缩options%，把压缩后的数据存放到baos中
                bitmap.compress(format, options, baos);
                options -= 30;
            }
            fos.write(baos.toByteArray());
            fos.close();
            baos.close();
            // 保存到文件之后，销毁bitmap
            // destroyBitmap(bitmap);
            return true;
        } catch (Exception e) {
            Log.e("wxj", "saveImage error:" + e);
        } finally {
            try {
                fos.close();
            } catch (Exception e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        return false;
    }


    /**
     * 保存图片PNG无损
     *
     * @param bitmap   图片
     * @param destFile 目标文件
     * @return
     */
    public static boolean saveImageByPng(Bitmap bitmap, File destFile) {
        if (bitmap == null || destFile == null) {
            return false;
        }

        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(destFile);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);
            fos.write(baos.toByteArray());
            fos.close();
            baos.close();
            // 保存到文件之后，销毁bitmap
//			destroyBitmap(bitmap);
            return true;
        } catch (Exception e) {
            Log.e("wxj", "saveImage error:" + e);
        } finally {
            try {
                fos.close();
            } catch (Exception e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        return false;
    }

    /**
     * 删除文件
     *
     * @param file 被删除的文件
     * @return
     */
    public static boolean deleteFile(File file) {
        if (file == null || !file.exists()) {
            return false;
        }
        file.delete();
        return true;
    }

    /**
     * 销毁位图
     *
     * @param bitmap
     */
    public static void destroyBitmap(Bitmap bitmap) {
        if (bitmap == null) {
            return;
        }

        if (!bitmap.isRecycled()) {
            bitmap.recycle();
            bitmap = null;
            System.gc();
            Log.e("wxj", "销毁bitmap成功");
        }
    }

    /**
     * 销毁绘图
     *
     * @param drawable 可以来自ImageView中的drawable，且是BitmapDrawable的实例
     */
    public static void destroyDrawable(Drawable drawable) {
        if (drawable == null) {
            return;
        }
        BitmapDrawable bd = null;
        try {
            bd = (BitmapDrawable) drawable;
        } catch (Exception e) {
            return;
        }

        destroyBitmap(bd.getBitmap());

    }

    /**
     * 保存字符串内容
     *
     * @param content  字符串内容
     * @param destFile 保存到的文件
     * @param charset  字符编码，可null，默认为utf-8
     */
    public static boolean saveContent(String content, File destFile, String charset) {
        if (content == null || destFile == null) {
            return false;
        }

        StringReader sr = new StringReader(content);
        OutputStreamWriter osw = null;
        try {
            osw = new OutputStreamWriter(new FileOutputStream(destFile), charset == null ? CHARSET : charset);
            char[] buf = new char[1024];
            int len = 0;
            while ((len = sr.read(buf)) != -1) {
                osw.write(buf, 0, len);
            }

            return true;
        } catch (Exception err) {
            err.printStackTrace();
        } finally {
            sr.close();
            try {
                osw.flush();
                osw.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        return false;
    }

    /**
     * 读取文件
     *
     * @param file    要读取的文件
     * @param charset 字符编码，可null，默认为utf-8
     * @return 返回字符串
     */
    public static String readFile(File file, String charset) {
        if (file == null || !file.exists()) {
            return "";
        }

        StringWriter sw = new StringWriter();
        InputStreamReader isr = null;
        try {
            isr = new InputStreamReader(new FileInputStream(file), charset == null ? CHARSET : charset);
            char[] buf = new char[1024];
            int len = 0;
            while ((len = isr.read(buf)) != -1) {
                sw.write(buf, 0, len);
            }

            return sw.toString();

        } catch (Exception err) {
            err.printStackTrace();
        } finally {
            try {
                isr.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            try {
                sw.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        return "";
    }

    /**
     * 读取源文件内容
     *
     * @param filename String 文件路径
     * @return byte[] 文件内容
     * @throws IOException
     */
    public static byte[] readFile(String filename) throws IOException {

        File file = new File(filename);
        if (filename == null || filename.equals("")) {
            throw new NullPointerException("无效的文件路径");
        }
        long len = file.length();
        byte[] bytes = new byte[(int) len];

        BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(file));
        int r = bufferedInputStream.read(bytes);
        if (r != len)
            throw new IOException("读取文件不正确");
        bufferedInputStream.close();

        return bytes;

    }

    /**
     * 将数据写入文件
     *
     * @param data byte[]
     * @throws IOException
     */
    public static void writeFile(byte[] data, String filename) throws IOException {
        File file = new File(filename);
        file.getParentFile().mkdirs();
        BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(new FileOutputStream(file));
        bufferedOutputStream.write(data);
        bufferedOutputStream.close();

    }

    /**
     * 从jar文件里读取class
     *
     * @param filename String
     * @return byte[]
     * @throws IOException
     */
    public byte[] readFileJar(String filename) throws IOException {
        BufferedInputStream bufferedInputStream = new BufferedInputStream(getClass().getResource(filename).openStream());
        int len = bufferedInputStream.available();
        byte[] bytes = new byte[len];
        int r = bufferedInputStream.read(bytes);
        if (len != r) {
            bytes = null;
            throw new IOException("读取文件不正确");
        }
        bufferedInputStream.close();
        return bytes;
    }

    /**
     * 读取网络流，为了防止中文的问题，在读取过程中没有进行编码转换，而且采取了动态的byte[]的方式获得所有的byte返回
     *
     * @param bufferedInputStream BufferedInputStream
     * @return byte[]
     * @throws IOException
     */
    public byte[] readUrlStream(BufferedInputStream bufferedInputStream) throws IOException {
        byte[] bytes = new byte[100];
        byte[] bytecount = null;
        int n = 0;
        int ilength = 0;
        while ((n = bufferedInputStream.read(bytes)) >= 0) {
            if (bytecount != null)
                ilength = bytecount.length;
            byte[] tempbyte = new byte[ilength + n];
            if (bytecount != null) {
                System.arraycopy(bytecount, 0, tempbyte, 0, ilength);
            }

            System.arraycopy(bytes, 0, tempbyte, ilength, n);
            bytecount = tempbyte;

            if (n < bytes.length)
                break;
        }
        return bytecount;
    }

}
