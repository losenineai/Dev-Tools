package com.xxlib.utils.cache;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * 磁盘缓存工具类
 * 参考：http://blog.csdn.net/guolin_blog/article/details/28863651
 * Created by hgkmail on 10/16/15.
 */
public class DiskCacheUtils {
    /**
     * 最大缓存空间
     */
    public static final int MAX_SIZE = 10 * 1024 * 1024; // 10M

    private Context mContext;

    // 核心
    private DiskLruCache lruCache = null;

    // 单例
    private static DiskCacheUtils diskCacheUtil = null;

    public static synchronized DiskCacheUtils getInstance(Context context) {
        if(diskCacheUtil==null)
            diskCacheUtil=new DiskCacheUtils(context);

        return diskCacheUtil;
    }

    /**
     * 创建一个磁盘缓存
     * @param ctx
     */
    private DiskCacheUtils(Context ctx) {
        mContext = ctx;
        try {
            File cacheDir = getDiskCacheDir(mContext, "disk_cache");
            if (!cacheDir.exists()) {
                cacheDir.mkdirs();
            }
            lruCache = DiskLruCache.open(cacheDir, getAppVersion(mContext), 1, MAX_SIZE);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 写入缓存
     * @param key
     * @param buffer
     */
    public synchronized void put(final String key, final byte[] buffer) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                DiskLruCache.Editor editor = null;
                try {
                    String hashKey = hashKeyForDisk(key);
                    editor = lruCache.edit(hashKey);
                    if (editor != null) {
                        OutputStream outputStream = editor.newOutputStream(0);
                        outputStream.write(buffer);
                        editor.commit();
                    }
                    lruCache.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    /**
     * 读取缓存
     * @param key
     * @return
     */
    public synchronized void get(final String key, final CacheGot callback) {
        new Thread() {
            @Override
            public void run() {
                DiskLruCache.Snapshot snapShot=null;
                String hashKey = hashKeyForDisk(key);
                try {
                    snapShot = lruCache.get(hashKey);
                    if (snapShot != null) {
                        InputStream is = snapShot.getInputStream(0);
                        callback.cacheGot(inputToByteArray(is));
                    }
                    else {
                        callback.cacheGot(null);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                    callback.cacheGot(null);
                }
            }
        }.start();
    }

    /**
     * 移除某个缓存
     * @param key
     */
    public synchronized void remove(String key) {
        try {
            String hashKey = hashKeyForDisk(key);
            lruCache.remove(hashKey);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 返回磁盘缓存总大小
     * @return
     */
    public synchronized long size() {
        return lruCache.size();
    }

    /**
     * 清除磁盘缓存
     */
    public synchronized void clear() {
        try {
            lruCache.delete();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 同步操作日志, 在Activity的onPause()方法中去调用一次flush()方法就可以了
     */
    public synchronized void flush() {
        try {
            lruCache.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 计算key的MD5
     * @param key
     * @return
     */
    public static String hashKeyForDisk(String key) {
        String cacheKey;
        try {
            final MessageDigest mDigest = MessageDigest.getInstance("MD5");
            mDigest.update(key.getBytes());
            cacheKey = bytesToHexString(mDigest.digest());
        } catch (NoSuchAlgorithmException e) {
            cacheKey = String.valueOf(key.hashCode());
        }
        return cacheKey;
    }

    /**
     * 把每个字节转化成16进制的字符串
     * @param bytes
     * @return
     */
    public static String bytesToHexString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < bytes.length; i++) {
            String hex = Integer.toHexString(0xFF & bytes[i]);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    /**
     * 获取磁盘缓存目录，有考虑SD卡是否存在
     * @param context
     * @param uniqueName
     * @return
     */
    private static File getDiskCacheDir(Context context, String uniqueName) {
        String cachePath;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            cachePath = context.getExternalCacheDir().getPath();
        } else {
            cachePath = context.getCacheDir().getPath();
        }
        return new File(cachePath + File.separator + uniqueName);
    }

    /**
     * 获取当前App的版本
     * @param context
     * @return
     */
    private static int getAppVersion(Context context) {
        try {
            PackageInfo info = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            return info.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return 1;
    }

    /**
     * 把 InputStream 转化为 byte[]
     * @param input
     * @return
     * @throws IOException
     */
    public static byte[] inputToByteArray(InputStream input) throws IOException {
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        byte[] buffer = new byte[4096];
        int n = 0;
        while (-1 != (n = input.read(buffer))) {
            output.write(buffer, 0, n);
        }
        return output.toByteArray();
    }

    public interface CacheGot {
        void cacheGot(byte[] buffer);
    }
}
