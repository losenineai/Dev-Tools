package com.auric.intell.commonlib.utils;

import android.annotation.TargetApi;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.StatFs;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.StringReader;
import java.io.StringWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

import static android.R.attr.path;

/**
 * 文件操作工具类
 *
 * @author wxj 2014
 */
public class FileUtil {
    // 默认字符编码
    public static final String CHARSET = "utf-8";
    private static final String TAG = "FileUtil";

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
            LogTool.d(TAG, LogTool.getStackTraceString(e));
        }
        return false;
    }

    /*
    * 获取文件夹大小
    * */
    public static long getFolderSize(String path){
        long size = 0;
        java.io.File file=new File(path);
        if(file.exists()){
            if(file.isDirectory()){
                try {
                    java.io.File[] fileList = file.listFiles();
                    for (int i = 0; i < fileList.length; i++) {
                        if (fileList[i].isDirectory()) {
                            size = size + getFolderSize(fileList[i].getAbsolutePath());
                        } else {
                            size = size + fileList[i].length();

                        }
                    }
                } catch (Exception e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
                // return size/1048576;
                return size;
            }
        }
       return 0;
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


    public static List<File> getAllFileByDir(String path, ArrayList arrayList) {
        if (arrayList == null) {
            arrayList = new ArrayList<File>();
        }
        if (TextUtils.isEmpty(path))
            return null;
        File src = new File(path);
        if (!src.exists())
            return null;
        File[] fileArray = src.listFiles();
        for (File f : fileArray) {
            if (f.isDirectory()) {
                getAllFileByDir(f.getAbsolutePath(), arrayList);
            } else if (f.isFile()) {
                arrayList.add(f);
            }
        }
        return arrayList;
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
     * 删除文件
     *
     * @param file 被删除的文件
     * @return
     */
    public static boolean deleteFileOrDir(File file) {
        if (file.isFile()) {
            return file.delete();
        }

        if (file.isDirectory()) {
            File[] childFiles = file.listFiles();
            if (childFiles == null || childFiles.length == 0) {
                return file.delete();
            }

            for (int i = 0; i < childFiles.length; i++) {
                deleteFileOrDir(childFiles[i]);
            }
            return file.delete();
        }

        return false;
    }

    /**
     * 递归删除目录下的所有文件及子目录下所有文件
     * @param dir 将要删除的文件目录
     * @return boolean Returns "true" if all deletions were successful.
     *                 If a deletion fails, the method stops attempting to
     *                 delete and returns "false".
     */
    private static boolean deleteDir(File dir) {
        if (dir.isDirectory()) {
            String[] children = dir.list();
            //递归删除目录中的子目录下
            for (int i=0; i<children.length; i++) {
                boolean success = deleteDir(new File(dir, children[i]));
                if (!success) {
                    return false;
                }
            }
        }
        // 目录此时为空，可以删除
        return dir.delete();
    }


    /**
     * 删除文件
     *
     * @param filepath 被删除的文件路径
     * @return
     */
    public static boolean deleteFile(String filepath) {

        File file = new File(filepath);

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
     * @param content
     * @param destFilePath
     * @param charset
     * @return
     */
    public static boolean saveContent(String content, String destFilePath) {
        return saveContent(content, new File(destFilePath), CHARSET);
    }

    /**
     * 保存字符串内容
     * @param content
     * @param destFile
     * @param charset
     * @return
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
                e.printStackTrace();
            }
            try {
                sw.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return "";
    }

    /**
     * 读取文件字符串
     *
     * @param filePath
     * @return
     */
    public static String readFileString(String filePath) {
        StringBuilder sb = new StringBuilder();
        if (TextUtils.isEmpty(filePath)) {
            return sb.toString();
        }
        File file = new File(filePath);
        if (file.exists()) {
            try {
                byte[] e = new byte[1024];
                FileInputStream fileInputStream = new FileInputStream(file);

                int len;
                while ((len = fileInputStream.read(e, 0, e.length)) != -1) {
                    String str = new String(e, 0, len, CHARSET);
                    sb.append(str);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return sb.toString();
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

    public static void writeFile(String data, String filePath) {
        if (TextUtils.isEmpty(data)) {
            return;
        }
        try {
            writeFile(data.getBytes("utf-8"), filePath);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String readFile(InputStream inputStream, String charset) {
        if (null == inputStream) {
            return "";
        }
        StringWriter sw = new StringWriter();
        InputStreamReader isr = null;
        try {
            isr = new InputStreamReader(inputStream, charset == null ? CHARSET : charset);
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
                e.printStackTrace();
            }
            try {
                sw.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return "";
    }


    public static final String SDCARD_0 = "/storage/sdcard0";
    public static final String SDCARD_1 = "/storage/sdcard1";

    /**
     * 获取指定路径存储全部空间大小
     *
     * @param filePath
     * @return GB
     */
    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    public static double getFileSizeAllTotal(String filePath) {
        String state = Environment.getExternalStorageState();
        double size_GB = 0l;
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            File file = new File(filePath);
            Log.d(TAG, file.getAbsolutePath());
            StatFs sf = new StatFs(file.getPath());
            long blockSize = sf.getBlockSizeLong();
            long blockCount = sf.getBlockCountLong();
            // long availCount = sf.getAvailableBlocksLong();
            size_GB = (blockSize * blockCount * 1.00) / (1024 * 1024 * 1024);
            //  Log.d("", "block大小:"+ blockSize+",block数目:"+ blockCount+",总大小:"+blockSize*blockCount/1024/1024+"MB");
            //  Log.d("", "可用的block数目：:"+ availCount+",剩余空间:"+ availCount*blockSize/1024/1024+"MB");
        }

        return size_GB;
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    public static double getFileSizeFreeTotal(String filePath) {
        String state = Environment.getExternalStorageState();
        double size_GB = 0l;
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            File file = new File(filePath);
            Log.d(TAG, file.getAbsolutePath());
            StatFs sf = new StatFs(file.getPath());
            long blockSize = sf.getBlockSizeLong();
            // long blockCount = sf.getBlockCountLong();
            long availCount = sf.getAvailableBlocksLong();
            size_GB = (blockSize * availCount * 1.00) / (1024 * 1024 * 1024);

            //  Log.d("", "block大小:"+ blockSize+",block数目:"+ blockCount+",总大小:"+blockSize*blockCount/1024/1024+"MB");
            //  Log.d("", "可用的block数目：:"+ availCount+",剩余空间:"+ availCount*blockSize/1024/1024+"MB");
        }
        return size_GB;
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    public static long getFileSizeFree4Long(String filePath) {
        String state = Environment.getExternalStorageState();
        long size_GB = 0L;
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            File file = new File(filePath);
            Log.d(TAG, file.getAbsolutePath());
            StatFs sf = new StatFs(file.getPath());
            long blockSize = sf.getBlockSizeLong();
            // long blockCount = sf.getBlockCountLong();
            long availCount = sf.getAvailableBlocksLong();
            size_GB = blockSize * availCount;

            //  Log.d("", "block大小:"+ blockSize+",block数目:"+ blockCount+",总大小:"+blockSize*blockCount/1024/1024+"MB");
            //  Log.d("", "可用的block数目：:"+ availCount+",剩余空间:"+ availCount*blockSize/1024/1024+"MB");
        }
        return size_GB;
    }

    /**
     * 获取文件或者文件夹占用多少字节
     *
     * @param file
     * @return
     */
    public static long getFileAvailSize(File file) {
        long size = 0;
        if (file == null || !file.exists()) {
            return size;
        }
        if (file.isDirectory()) {
            File[] fileList = file.listFiles();
            if (fileList != null) {
                for (int i = 0; i < fileList.length; i++) {
                    size = size + getFileAvailSize(fileList[i]);
                }
            }
        } else {
            try {
                FileInputStream fileIs = new FileInputStream(file);
                size = fileIs.available();
                fileIs.close();
            } catch (Exception e) {
            }
        }

        return size;
    }

    /**
     * 根据文件字节大小换算为K、MB等单位
     *
     * @param fileS
     * @return 文件单位转换后大小
     */
    public static String formatFileSize(long fileS){
        DecimalFormat df = new DecimalFormat("#0.0");
        String fileSizeString = "";
        if (fileS < 1024) {
            fileSizeString = df.format((double) fileS) + "B";
        } else if (fileS < 1048576) {
            fileSizeString = df.format((double) fileS / 1024) + "K";
        } else if (fileS < 1073741824) {
            fileSizeString = df.format((double) fileS / 1048576) + "M";
        } else {
            fileSizeString = df.format((double) fileS / 1073741824) +"G";
        }
        return fileSizeString;
    }

    /**
     * 根据路径获取文件大小
     * @param path
     * @return
     */
    public static String getFileSizeByPath(String path){
        File file = new File(path);
        if (file.exists()){
            return getFileSizeByFile(file);
        }
        return "0KB";
    }

    /**
     * 根据File获取文件大小
     * @param file
     * @return
     */
    public static String getFileSizeByFile(File file){
        return formatFileSize(getFileAvailSize(file));
    }

    public static String getCacheFileSize(String path[]){
        long cacheSize = 0;
        for (String mPath : path){
            File file = new File(mPath);
                if (file.exists()){
                    cacheSize += getFileAvailSize(file);
                }
        }
        if (cacheSize == 0) {
            return "0KB";
        }
        return formatFileSize(cacheSize);
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


    public static void createDir(String path) {
        File foder = new File(path);
        if (!foder.exists()) {
            foder.mkdirs();
        }
    }

    public static boolean isFile(String path) {
        if (TextUtils.isEmpty(path)) {
            return false;
        }
        File file = new File(path);
        return file.exists() && file.isFile();
    }

    public static boolean writeFile(File destFile, InputStream stream, boolean append) {
        if (destFile == null || stream == null) {
            return false;
        }
        OutputStream os = null;
        try {
            os = new FileOutputStream(destFile, append);
            byte data[] = new byte[1024];
            int length = -1;
            while ((length = stream.read(data)) != -1) {
                os.write(data, 0, length);
            }
            os.flush();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (os != null) {
                try {
                    os.close();
                    stream.close();
                } catch (IOException e) {
                    throw new RuntimeException("IOException occurred. ", e);
                }
            }
        }
        return false;
    }

    /**
     * copy file
     * @param srcPath
     * @param destPath
     * @return
     */
    public static boolean copyFile(String srcPath, String destPath) {
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(srcPath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        return writeFile(new File(destPath), inputStream, false);
    }

    /**
     * copy file
     * @param srcFile
     * @param destFile
     * @return
     */
    public static boolean copyFile(File srcFile, File destFile) {
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(srcFile);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        return writeFile(destFile, inputStream, false);
    }

    /**
     * 移动文件
     * @param srcFile
     * @param destFile
     * @return
     */
    public static boolean moveFile(File srcFile, File destFile) {
        boolean result = srcFile.renameTo(destFile);
        LogTool.d(TAG, "moveFile renameTo "+result);
        if (!result) {
            FileUtil.createDir(destFile.getParent());
            result = FileUtil.copyFile(srcFile, destFile);
            LogTool.d(TAG, "moveFile copyFile "+result);
        }

        return result;
    }

    /**
     * @param context
     * @param imageFile
     * @return content uri
     */
    public static Uri getContentUriFromFile(Context context, File imageFile){
        String filePath = imageFile.getAbsolutePath();
        Cursor cursor = context.getContentResolver().query(
                MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[]{MediaStore.Images.Media._ID},
                MediaStore.Images.Media.DATA + "=?", new String[]{filePath}, null);
        if (cursor != null && cursor.moveToFirst()){
            int id = cursor.getInt(cursor.getColumnIndex(MediaStore.Images.Media._ID));
            Uri baseUri = Uri.parse("content://media/external/images/media");
            return Uri.withAppendedPath(baseUri, "" + id);
        }else {
            if (imageFile.exists()){
                ContentValues values = new ContentValues();
                values.put(MediaStore.Images.Media.DATA,filePath);
                return context.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,values);
            }else {
                return null;
            }
        }
    }

    public static boolean isFileExist(String fileName){
        return (new File(fileName)).exists();
    }
}
