package com.xxlib.utils.xpk;

import android.content.Context;
import android.text.TextUtils;

import com.xxlib.utils.ApkCommentChannelIdUtils;
import com.xxlib.utils.MD5Helper;
import com.xxlib.utils.RegisterModule.FileUtil;
import com.xxlib.utils.SPCenter.SPCenter;
import com.xxlib.utils.SPCenter.XXLibSpkey;
import com.xxlib.utils.base.LogTool;
import com.xxlib.utils.base.XXTea;
import com.xxlib.utils.io.UpZip;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

/**
 * Created by jammy on 16/2/15.
 */
public class XpkAppTool {

    private static final String TAG = "XpkAppTool";


    public enum INSTALL_XPK_ERR_CODE {
        INSTALL_XPK_UPZIP_RES_DATA_FAIL,        // 解压失败
        INSTALL_XPK_UPZIP_CONFIG_FAIL,          // 解压失败
        INSTALL_XPK_UPZIP_APK_FAIL,             // 解压失败：可能是io写失败，也可能是空间不足
        INSTALL_XPK_DECRYPT_CONFIG_FAIL,        // 解密config失败，一般是包配置错误
        INSTALL_XPK_CONFIG_FAIL,                // 配置的文件类型不对，一般是包配置错误
        INSTALL_XPK_DEPLOY_MD5_NOT_EQUAL_FAIL,  // 拷贝完资源，md5对比不上。可能是写io失败，也可能是配置失败。
        INSTALL_XPK_INSTALL_EX,                 // 解压文件中途抛了异常，逻辑出错或者io出错。
        INSTALL_XPK_UPZIP_FINISH,               // 解压完成
        INSTALL_XPK_INSTALL_APK_ING,            // 安装apk中，暂无用到
        INSTALL_XPK_INSTALL_APK_FAIL,           // 安装apk失败，暂无用到
    };


    public static class InstallXpkResult {
        public INSTALL_XPK_ERR_CODE mCode;
        /** 安装xpk的时候，有拷贝的那些路劲，不包括文件夹 */
        public List<String> mCopyFilePath;
        public String mExStr;
    };


    /**
     * xpk的解压状态
     * 开始、解压配置文件、解压游戏资源、、解压游戏安装包完成
     */
    public enum XPK_UNZIP_STATE {
        BEGIN,
        UNZIP_CONFIG,
        UNZIP_RES,
        UNZIP_APK,
        FINISH
    }


    public interface XpkInstallObsv {
        void notifyInstallState(XPK_UNZIP_STATE state);
    }


    private static void notifyObsv(XpkInstallObsv obsv, XPK_UNZIP_STATE state) {
        if (null != obsv) {
            obsv.notifyInstallState(state);
        }
    }

    private static final String XPK_MD5 = "md5";
    private static final String XPK_FILE = "file";
    private static final String XPK_DIR = "dir";
    private static final String XPK_DEST = "dest";
    private static final String XPK_DATA_INFO = "dataInfo";
    private static final String XPK_CONFIG_FILE = "package.cfg";
    public static final String XPK_APK_FILE = "application.apk";


    public static String DecryptConfigFile(File file) {
        LogTool.i(TAG, "DecryptConfigFile");
        if (null == file || !file.exists()) {
            return "";
        }
        byte[] data;
        try {
            FileInputStream inputStream = new FileInputStream(file);
            data = new byte[inputStream.available()];
            inputStream.read(data);
            inputStream.close();
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
            return "";
        }
        byte[] xxResult = XXTea.XXTeaDecrypt(data, data.length, ApkCommentChannelIdUtils.KEY.getBytes());
        return new String(xxResult);
    }


    /**
     *
     * @description 检测xpk的文件部署是否完成 （这里考虑到 如果是有dir类型的文件需要部署 还是重头进行一次部署为好
     *              所以只针对单obb文件做部署检测）
     * @param destination
     * @return
     * @throws JSONException
     */
    public static boolean checkXpkDeploy(String destination) throws JSONException {
        LogTool.i(TAG, "checkXpkDeploy 检测部署 1");
        File cfgFile = new File(destination + XPK_CONFIG_FILE);
        File apkFile = new File(destination + XPK_APK_FILE);
        if (!cfgFile.exists())
            return false;
        if (!apkFile.exists())
            return false;

        String StringAfterDecrypt = XpkAppTool.DecryptConfigFile(cfgFile);
        if (TextUtils.isEmpty(StringAfterDecrypt)) {
            return false;
        }
        LogTool.i(TAG, "config: " + StringAfterDecrypt);
        JSONObject obj = new JSONObject(StringAfterDecrypt);

        // 检查文件是否部署成功
        JSONArray thumbnail = (JSONArray) obj.get(XPK_DATA_INFO);
        for (int i = 0; i < thumbnail.length(); ++i) {
            JSONObject dataInfoItem = thumbnail.getJSONObject(i);
            if (dataInfoItem.has(XPK_DIR)) {
                // 这里考虑到 如果是有dir类型的文件需要部署 还是重头进行一次部署为好 所以只针对单obb文件做部署检测
                return false;
            } else {
                if (!dataInfoItem.has(XPK_FILE) || !dataInfoItem.has(XPK_MD5) || !dataInfoItem.has(XPK_DEST)) {
                    // 一些字段不全。直接返回，当做失败
                    return false;
                }
                String fileName = dataInfoItem.getString(XPK_FILE);
                String fileMd5 = dataInfoItem.getString(XPK_MD5);
                String fileDest = dataInfoItem.getString(XPK_DEST);
                LogTool.i(TAG, "fileName " + fileName);
                LogTool.i(TAG, "fileMd5 " + fileMd5);
                LogTool.i(TAG, "fileDest " + fileDest);
                String localFileMd5 = MD5Helper.getFileMD5(new File(fileDest + File.separator + fileName));
                if (!MD5Helper.compareMD5(localFileMd5, fileMd5))
                    return false;
            }
        }
        return true;
    }


    /**
     * 直接解压到目标的文件夹，不需要先解压到临时文件夹（解压到临时文件夹会占空间）
     */
    public static InstallXpkResult installXpkLite(Context context, File file, String destination, XpkInstallObsv obsv) {
        InstallXpkResult result = new InstallXpkResult();
        result.mExStr = "";

        // 解压中状态通知
        notifyObsv(obsv, XPK_UNZIP_STATE.BEGIN);

        // 解压config
        notifyObsv(obsv, XPK_UNZIP_STATE.UNZIP_CONFIG);
        UpZip.UpZipResult unzipConfigResult = UpZip.unZipFiles(file, context, destination, XPK_CONFIG_FILE, true);
        if (!unzipConfigResult.isUpzipSucc) {
            LogTool.i(TAG, "解压配置文件失败");
            result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_UPZIP_CONFIG_FAIL;
            result.mExStr = unzipConfigResult.mExStr;
            return result;
        }

        // 解密配置文件
        File packageFile = new File(destination + XPK_CONFIG_FILE);
        String stringAfterDecrypt = XpkAppTool.DecryptConfigFile(packageFile);
        if (TextUtils.isEmpty(stringAfterDecrypt)) {
            LogTool.i(TAG, "解密配置文件失败");
            result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_DECRYPT_CONFIG_FAIL;
            return result;
        }

        // 直接拷贝文件和文件夹，拷贝前判断是否已经存在，存在就不拷贝
        notifyObsv(obsv, XPK_UNZIP_STATE.UNZIP_RES);
        try {
            result.mCopyFilePath = new ArrayList<String>();
            JSONObject obj = new JSONObject(stringAfterDecrypt);
            JSONArray thumbnail = (JSONArray) obj.get(XPK_DATA_INFO);
            for (int i = 0; i < thumbnail.length(); ++i) {
                JSONObject dataInfoItem = thumbnail.getJSONObject(i);
                if (dataInfoItem.has(XPK_DIR)) {
                    // 文件夹类型
                    if (!dataInfoItem.has(XPK_DEST)) {
                        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_CONFIG_FAIL;
                        return result;
                    }
                    String dirName = dataInfoItem.getString(XPK_DIR);
                    String fileDest = dataInfoItem.getString(XPK_DEST);
                    LogTool.i(TAG, "dirName " + dirName);
                    LogTool.i(TAG, "fileDest " + fileDest);
                    // 解压文件夹到 dest
                    UpZip.UpZipResult unzipFileResult = UpZip.unZipFiles(file, context, fileDest + File.separator, dirName
                            + File.separator, true);
                    result.mCopyFilePath.addAll(unzipFileResult.upzipFilePaths);
                    if (!unzipFileResult.isUpzipSucc) {
                        LogTool.i(TAG, "unzip dir " + dirName + "to fileDest " + fileDest + " fail");
                        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_UPZIP_RES_DATA_FAIL;
                        result.mExStr = unzipFileResult.mExStr;
                        return result;
                    }
                } else if (dataInfoItem.has(XPK_FILE)) {
                    // 文件类型
                    if (!dataInfoItem.has(XPK_MD5) || !dataInfoItem.has(XPK_DEST)) {
                        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_CONFIG_FAIL;
                        return result;
                    }
                    String fileName = dataInfoItem.getString(XPK_FILE);
                    String fileMd5 = dataInfoItem.getString(XPK_MD5);
                    String fileDest = dataInfoItem.getString(XPK_DEST);
                    LogTool.i(TAG, "fileName " + fileName);
                    LogTool.i(TAG, "fileMd5 " + fileMd5);
                    LogTool.i(TAG, "fileDest " + fileDest);
                    String targetFilePath = fileDest + File.separator + fileName;

                    // 先判断是否已经拷贝过去了。上一次已经拷贝过去，但是没安装apk zemin
                    File targetFile = new File(targetFilePath);
                    if (targetFile.exists() && targetFile.isFile() && MD5Helper.compareMD5(MD5Helper.getFileMD5(targetFile), fileMd5)) {
                        continue;
                    }

                    // 解压文件到dest, 再检验md5。配置文件有md5了，就不需要在解压的时候检查了 zemin
                    UpZip.UpZipResult unzipFileResult = UpZip.unZipFiles(file, context, fileDest + File.separator, fileName,
                            false);
                    result.mCopyFilePath.addAll(unzipFileResult.upzipFilePaths);
                    if (!unzipFileResult.isUpzipSucc) {
                        LogTool.i(TAG, "unzip file " + fileName + "to fileDest " + fileDest + " fail");
                        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_UPZIP_RES_DATA_FAIL;
                        result.mExStr = unzipFileResult.mExStr;
                        return result;
                    }
                    String targetFileMd5 = MD5Helper.getFileMD5(new File(targetFilePath));
                    LogTool.i(TAG, targetFileMd5 + " " + targetFilePath);
                    if (!MD5Helper.compareMD5(targetFileMd5, fileMd5)) {
                        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_DEPLOY_MD5_NOT_EQUAL_FAIL;
                        return result;
                    }
                }
            }
        } catch (Exception e) {
            // 这里还有异常，就是json文件的问题了
            LogTool.i(TAG, e.toString());
            result.mExStr = e.toString();
            result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_INSTALL_EX;
            return result;
        }

        // 解压apk到destination
        notifyObsv(obsv, XPK_UNZIP_STATE.UNZIP_APK);
        UpZip.UpZipResult unzipApkResult = UpZip.unZipFiles(file, context, destination, XPK_APK_FILE, true);
        if (!unzipApkResult.isUpzipSucc) {
            LogTool.i(TAG, "unzip apk fail.");
            result.mExStr = unzipApkResult.mExStr;
            result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_UPZIP_APK_FAIL; // 解压apk失败，一般是空间不够了
            return result;
        }

        // 安装这里不做处理,通知外部安装即可
        notifyObsv(obsv, XPK_UNZIP_STATE.FINISH);

        result.mCode = INSTALL_XPK_ERR_CODE.INSTALL_XPK_UPZIP_FINISH;
        return result;
    }


    /***
     * 追加path到这sp的key中
     *
     * @param downloadMask
     * @param paths
     */
    public static void saveXpkCopyPathToSp(String downloadMask, List<String> paths) {
        if (null == paths || paths.size() == 0) {
            return;
        }
        Set<String> allPaths = SPCenter.getStringSet(XXLibSpkey.XXLIB_XPK_COPY_PATH_PREFIX_ + downloadMask, ":");
        if (null == allPaths) {
            allPaths = new TreeSet<String>();
        }
        allPaths.addAll(paths);
        SPCenter.putStringSet(XXLibSpkey.XXLIB_XPK_COPY_PATH_PREFIX_ + downloadMask, allPaths, ":");
    }

    /**s
     * 如果用户删除xpk的下载任务，该任务有拷贝一些文件到sdcard，也一并删除
     */
    public static void deleteXpkCopyFile(String downloadMask) {
        Set<String> paths = SPCenter.getStringSet(XXLibSpkey.XXLIB_XPK_COPY_PATH_PREFIX_ + downloadMask, ":");
        if (null != paths && paths.size() != 0) {
            LogTool.i(TAG, paths.toString());
            for (String path : paths) {
                FileUtil.deleteFileSafely(path);
            }
        }
        SPCenter.remove(XXLibSpkey.XXLIB_XPK_COPY_PATH_PREFIX_ + downloadMask);
    }

}
