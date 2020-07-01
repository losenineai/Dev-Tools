package com.inmobi;

import android.content.Context;
import android.text.TextUtils;

import com.xxlib.utils.ApplicationUtils;
import com.xxlib.utils.FileUtils;
import com.xxlib.utils.MD5Helper;
import com.xxlib.utils.VersionComparer;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.io.AssetsUtil;
import com.xxlib.utils.net.HttpGetUtil;

import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;

/**
 * Created by white on 15-12-22.
 */
public class InmobiUpdater {

    private static final String TAG = "InmobiUpdater";

    private static final String APK_NAME_PREFIX = "inmobi.";
    private static final String APK_NAME_SUFFIX = ".apk";
    private static final String FILE_NAME_CONFIG = "config.json";

    private static final boolean IS_TEST_BY_ASSETS = false;  // 用于通过获取assets测试


    public static void updateInmobi(final Context context, final String url, final InmobiUpdateListener listener){
        // 用于测试
        if(IS_TEST_BY_ASSETS){
            AssetsUtil.copyFromAssets(context, "inmobi.apk", getApkFilePath(context));
            if (listener != null)
                listener.onUpdateFinish(true, getApkFilePath(context));
            return;
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                if(context == null || TextUtils.isEmpty(url)){
                    return;
                }
                /**
                 *
                 * {"version":"1.0.0",
                 "resource":[{"url":"http://cdn.xxzhushou.cn/inmobi-release_1.0.0_100.apk","md5":"d4fa76e9361ccf3e86738ab8e790dd26"}]}
                 */
                // 获取配置信息
                String jsonStr = HttpGetUtil.get(url);
                // TODO test
//                jsonStr = "{\"version\":\"1.0.4\",\n" +
//                        "                 \"resource\":[{\"url\":\"http://cdn.xxzhushou.cn/inmobi-release_1.0.0_100.apk\",\"md5\":\"f7cdfff2413f448c5c4caf2a2f8e4f27\"}],\n" +
//                        "                 \"enable_speed\":false,\n" +
//                        "                 \"enable_assist\":true,\n" +
//                        "                 \"filter\":[\"com.xxAssis\",\".*\\.xx\\.multi[0-9]+\"]}";
                LogTool.i(TAG, jsonStr);

                try{
                    JSONObject jsonObj = new JSONObject(jsonStr);
                    File localFile = new File(getApkFilePath(context));
                    // 获取服务端版本
                    String serverVersion = jsonObj.getString("version");
                    // 是否更新
                    if(isNeedUpdate(context, localFile, serverVersion)){
                        String apkUrl = jsonObj.getJSONArray("resource").getJSONObject(0).getString("url");
                        String apkMd5 = jsonObj.getJSONArray("resource").getJSONObject(0).getString("md5");
                        if(updateFile(context, serverVersion, apkUrl, apkMd5)){
                            // 保存最新配置文件
                            saveConfigFile(jsonStr);
                            LogTool.i(TAG, "更新inmobi成功 " + getApkFilePath(context));
                            if(listener != null)
                                listener.onUpdateFinish(false, getApkFilePath(context));
                            return;
                        }
                        LogTool.i(TAG, "更新inmobi失败 " + getApkFilePath(context));
                    }
                    else{
                        if(listener != null)
                            listener.onUpdateFinish(false, null);
                        LogTool.i(TAG, "无更新 " + getApkFilePath(context));
                    }

                }catch(Exception er){
                    er.printStackTrace();
                    LogTool.i(TAG, LogTool.getStackTraceString(er));
                }
            }
        }).start();
    }


    public static String getApkFilePath(Context context){
        if(context == null){
            return null;
        }
        File dir = context.getDir("inmobi", Context.MODE_PRIVATE);
        FileUtils.createDir(dir.getAbsolutePath());
        return dir.getAbsolutePath() + "/" + APK_NAME_PREFIX + getLocalVersion(context) + APK_NAME_SUFFIX;
    }


    public static JSONObject getConfigJsonOjb(){
        File configFile = new File(new File(getApkFilePath(ApplicationUtils.getApplication())).getParent() + "/" + FILE_NAME_CONFIG);
        String jsonStr = FileUtils.readFile(configFile, FileUtils.CHARSET);
        try{
            return new JSONObject(jsonStr);
        }catch (Exception er){
            er.printStackTrace();
        }
        return null;
    }

    private static void saveConfigFile(String jsonStr){
        if (TextUtils.isEmpty(jsonStr)) {
            return ;
        }
        String parentDir = new File(getApkFilePath(ApplicationUtils.getApplication())).getParent();
        FileUtils.createDir(parentDir);
        try {
            FileUtils.saveContent(jsonStr, new FileOutputStream(parentDir + "/" + FILE_NAME_CONFIG), FileUtils.CHARSET);
            LogTool.i(TAG, "保存配置文件成功 " + parentDir + "/" + FILE_NAME_CONFIG);
        }catch (Exception er){
            er.printStackTrace();
        }
    }


    private static boolean isNeedUpdate(Context context, File localApk, String serverVersion){
        if(!localApk.exists()){
            return true;
        }

        // 获取本地多开apk的版本号
        String localVersion = getLocalVersion(context);

        return VersionComparer.compareTo(localVersion, serverVersion) == -1;
    }


    public static String getLocalVersion(Context context) {
        File dir = context.getDir("inmobi", Context.MODE_PRIVATE);
        if(!dir.exists() || !dir.isDirectory()){
            return "";
        }
        String[] files = dir.list();
        if(files == null){
            return "";
        }

        for (String filePath : files) {
            if (filePath.startsWith(APK_NAME_PREFIX) && filePath.endsWith(APK_NAME_SUFFIX) && filePath.length()>(APK_NAME_PREFIX.length()+APK_NAME_SUFFIX.length())) {
                String version = filePath.substring(APK_NAME_PREFIX.length(), filePath.length()-APK_NAME_SUFFIX.length());
                return version;
            }
        }
        return "";
    }

    private static boolean updateFile(Context context, String version, String url, String md5){
        // 先下载到tmp缓存文件中
        File apkParentDir = new File(getApkFilePath(context)).getParentFile();
        File tmpFile = new File(apkParentDir.getAbsolutePath() + "/ " + APK_NAME_PREFIX + "tmp");
        if(!HttpGetUtil.downloadFile(url, tmpFile.getAbsolutePath())){
            return false;
        }
        // md5检验
        if(!MD5Helper.compareMD5(MD5Helper.getFileMD5(tmpFile), md5)){
            return false;
        }

        // 下载成功后把原来的文件删除
        FileUtils.DeleteFolder(getApkFilePath(context));
        // 最后替换新的版本文件
        File targetFile = new File(apkParentDir.getAbsolutePath() + "/" + APK_NAME_PREFIX + version + APK_NAME_SUFFIX);
        boolean result = tmpFile.renameTo(targetFile);

        // 修改权限
        targetFile.setReadable(true);
        targetFile.setWritable(true);
        targetFile.setExecutable(true);

        return result;
    }


    public interface InmobiUpdateListener {
        void onUpdateFinish(boolean hasUpdate, String apkPath);
    }



}
