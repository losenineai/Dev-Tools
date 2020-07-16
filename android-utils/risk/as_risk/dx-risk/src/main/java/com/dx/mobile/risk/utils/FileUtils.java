package com.dx.mobile.risk.utils;

import android.Manifest;
import android.content.Context;
import android.os.Environment;
import android.os.storage.StorageManager;

import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Method;

/**
 * Created by boyliang on 2017/5/31.
 * 文件管理辅助类
 */

public final class FileUtils {

    private FileUtils(){

    }

    public static void close(Closeable file){
        if(file != null ){
            try{
                file.close();
            }catch (Exception e){
//                e.printStackTrace();
            }
        }
    }

    public static byte[] readAllFromFile(String path){
        return readAllFromFile(new File(path));
    }

    public static byte[] readAllFromFile(File file){
        byte[] bytes = null;
        FileInputStream fis = null;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        try{

            byte[] buf = new byte[256];
            int len;

            fis = new FileInputStream(file);
            while((len = fis.read(buf)) > 0){
                bos.write(buf, 0, len);
            }

            bos.flush();
            bytes = bos.toByteArray();

        }catch (FileNotFoundException e){
//            e.printStackTrace();
        }catch (Exception e){
//            e.printStackTrace();
        } finally {
            close(fis);
        }

        return bytes;
    }

    public static void writeAllToFile(File file, byte[] data){
        FileOutputStream fos = null;

        try{
            fos = new FileOutputStream(file);
            fos.write(data);
            fos.flush();
        }catch (FileNotFoundException e){
//            e.printStackTrace();
        }catch (Exception e){
//            e.printStackTrace();
        }finally {
            close(fos);
        }
    }

    public static boolean existSDCard(){
        try{
            return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
        }catch (Exception e){
            //Bugfix: 在三星 Galaxy S4(GT-I9500)上会有NullPointerException
//            e.printStackTrace();
        }

        return false;
    }

    public static File getSDCardRootDir(){
        if(FileUtils.isExist(Environment.getExternalStorageDirectory())){
            return Environment.getExternalStorageDirectory();
        }

        return new File("/sdcard/");
    }

    /**
     * 获取内置或外置sdcard路径
     * @param mContext
     * @param isOutlay true 为获取外置sdcard,false为内置sdcard
     * @return 如无法获取则返回null
     */
    private static String getSDCardRootPath(Context mContext, boolean isOutlay) {

        StorageManager mStorageManager = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
        Class<?> storageVolumeClazz = null;

        try {
            storageVolumeClazz = Class.forName("android.os.storage.StorageVolume");
            Method getVolumeList = mStorageManager.getClass().getMethod("getVolumeList");
            Method getPath = storageVolumeClazz.getMethod("getPath");
            Method isRemovable = storageVolumeClazz.getMethod("isRemovable");
            Object result = getVolumeList.invoke(mStorageManager);

            int length = Array.getLength(result);

            for (int i = 0; i < length; i++) {
                Object storageVolumeElement = Array.get(result, i);
                String path = (String) getPath.invoke(storageVolumeElement);
                boolean removable = (Boolean) isRemovable.invoke(storageVolumeElement);

                if (isOutlay == removable) {
                    return path;
                }
            }

        } catch (Exception e) {
            //e.printStackTrace();
        }

        return "/sdcard/";
    }

    public static int getSDCardStatus() {
        int status = 0;
        try{
            /**
             * 默认使用内置sdcard，不使用外置sdcard
             */
            if(getSDCardRootDir().exists() &&
                    PermissionUtils.checkPermissions(Manifest.permission.READ_EXTERNAL_STORAGE) &&
                    PermissionUtils.checkPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE)){
                status |= 1 << 0;
            }

            /**
             * 使用remote访问StorageManager 方式，会导致android.os.DeadObjectException 报错
             */
//            String sdCardPath = getSDCardRootPath(ContextFinder.getApplication(), false);
//            String exSDCardPath = getSDCardRootPath(ContextFinder.getApplication(), true);
//
//
//            // 二进制 第0位表示内置sd  第1位表示外置sd
//            if(!StringUtils.isEmpty(sdCardPath) &&
//                    PermissionUtils.checkPermissions(Manifest.permission.READ_EXTERNAL_STORAGE) &&
//                    PermissionUtils.checkPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE))
//                status |= 1 << 0;
//
//            if(!StringUtils.isEmpty(exSDCardPath) &&
//                    PermissionUtils.checkPermissions(Manifest.permission.READ_EXTERNAL_STORAGE) &&
//                    PermissionUtils.checkPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE))
//                status |= 1 << 1;
        }catch(Throwable t){
//            t.printStackTrace();
        }
        return status;
    }

    public static boolean isExist(String path){
        return !StringUtils.isEmpty(path) && new File(path).exists();
    }

    public static boolean isExist(File path){
        return path.exists();
    }

    /**
     * 获取指定文件夹内其中一个指定后缀名文件
     * @param path
     * @param end 要获取的文件名后缀(不包含".")
     * @param filter 要过滤掉的文件名
     * @return void
     */
    public static String getFileFromDir(String path, String end, String filter){

        File fileDir = new File(path);
        File[] files = fileDir.listFiles();
        String result = "";

        if(files == null)
            return result;

        for (File file : files) {

            String fileName = file.getName();

            if (file.isFile() && fileName.endsWith(end) && !fileName.equals(filter)) {
                result = fileName;
            }

        }
        return result;

    }
}
