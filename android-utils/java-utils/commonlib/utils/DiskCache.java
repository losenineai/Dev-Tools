package com.auric.intell.commonlib.utils;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by zhangxiliang on 2017/4/1.
 */

public class DiskCache {

    private String mCacheRootDir;

    private DiskCache (){


    }

    public static  DiskCache getInstance(){
        return Instance.diskCache;
    }

    public  void init(Context context){
        if (context == null) {
            context = ContextFinder.getApplication();
        }
        mCacheRootDir=getDiskCacheDir(context)+File.separator;
        for (StorageType storageType : StorageType.values()) {
             makeDirectory(mCacheRootDir + storageType.getStoragePath());
        }
        makeDirectory(getSellShowDir(context));
    }

    public String getSellShowDir(Context context) {
        if (context == null)
            context = ContextFinder.getApplication();
        String sellshow;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            sellshow = Environment.getExternalStorageDirectory() + File.separator + "sellshow";
        } else {
            sellshow = context.getFilesDir().getAbsolutePath() + File.separator + "sellshow";
        }
        return sellshow;
    }

    public  String getDiskCacheDir(Context context) {
        if (context == null){
            context = ContextFinder.getApplication();
        }
        String cachePath = "";
        if (Environment.MEDIA_MOUNTED.equals(Environment
                .getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            File file = context.getExternalCacheDir();
            if (file != null) {
                try {
                    cachePath = file.getPath();
                } catch (NullPointerException e) {
                    cachePath = Environment.getExternalStorageDirectory().getPath() +
                            "/Android/data/" + context.getPackageName() + "/cache/";
                }
            } else {
                cachePath = Environment.getExternalStorageDirectory().getPath() +
                        "/Android/data/" + context.getPackageName() + "/cache/";
            }
        } else {
            try {
                cachePath = context.getCacheDir().getPath();
            } catch (NullPointerException e) {
                ToastUtil.show("读取文件权限被拒绝");
            }
        }
        return cachePath;
    }

    public String getOutSDPath() {
        String mount = new String();

        try {
            Runtime runtime = Runtime.getRuntime();
            Process proc = runtime.exec("mount");
            InputStream is = proc.getInputStream();
            InputStreamReader isr = new InputStreamReader(is);
            String line;

            BufferedReader br = new BufferedReader(isr);
            while ((line = br.readLine()) != null) {
                if (line.contains("secure"))
                    continue;
                if (line.contains("asec"))
                    continue;

                if (line.contains("fat")) {
                    String columns[] = line.split(" ");
                    if (columns != null && columns.length > 1) {
                        mount = mount.concat("*" + columns[1] + "\n");
                    }
                } else if (line.contains("fuse")) {
                    String columns[] = line.split(" ");
                    if (columns != null && columns.length > 1) {
                        mount = mount.concat(columns[1] + "\n");
                    }
                }
            }
            Log.i("外置SD卡路径", mount);

        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return mount;
    }

    /**
     * 创建目录
     *
     * @param path
     * @return
     */
    private boolean makeDirectory(String path) {
        File file = new File(path);
        boolean exist = file.exists();
        if (!exist) {
            exist = file.mkdirs();
        }
        return exist;
    }

    public  void clean(StorageType fileType){
        File dir=new File(getDirectoryByDirType(fileType));
        FileUtil.deleteFileOrDir(dir);
    }

    /**
     * 返回指定类型的文件夹路径
     *
     * @param fileType
     * @return
     */
    public String getDirectoryByDirType(StorageType fileType) {
        return mCacheRootDir + fileType.getStoragePath();
    }

    static  class Instance{

        private static DiskCache diskCache=new DiskCache();
    }

}
