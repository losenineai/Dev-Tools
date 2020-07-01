package com.xxlib.utils.io;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import com.xxlib.utils.MD5Helper;
import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;


/**
 * zip解压缩工具类
 *
 * @author linbin
 *         2013
 */
public class UpZip {

    private static final String TAG = "UpZip";

    /**
     * 解压的返回结果
     * <p>版本号    作者    日期    相关修改
     * <p>1.0   jammy   2015-6-6  创建
     * <p>2015-6-6   --  下午5:29:43
     */
    public static class UpZipResult {
        public UpZipResult() {
        }

        public boolean isUpzipSucc;
        public List<String> upzipFilePaths;
        public String mExStr;
    }

    /**
     * @param pluginuid
     * @param zipFile
     * @param context
     * @param descDir
     * @throws IOException
     * @description 解压辅助压缩包
     */
    public static void unAssistZipFiles(int pluginuid, File zipFile, Context context, String descDir) throws IOException {
        File pathFile = new File(descDir);
        if (!pathFile.exists()) {
            pathFile.mkdirs();
        }
        ZipFile zip = new ZipFile(zipFile);
        for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
            ZipEntry entry = (ZipEntry) entries.nextElement();
            String zipEntryName = entry.getName();
            InputStream in = zip.getInputStream(entry);
            String outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
            ;
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if (!file.exists()) {
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if (new File(outPath).isDirectory()) {
                continue;
            }
            //输出文件路径信息
//	            System.out.println(outPath);

            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            while ((len = in.read(buf1)) > 0) {
                out.write(buf1, 0, len);
            }
            in.close();
            out.flush();
            out.close();
            //"su "+
            //Runtime.getRuntime().exec("chmod 777 " + outPath);
            UpFilePermison(outPath);
            //Runtime.getRuntime().exec("chmod 777 " + outPath);
        }
        zip.close();
        //Runtime.getRuntime().exec("su");
//	        Runtime.getRuntime().exec("chmod 777 " + "/data/data/com.xxAssistant/app_plugin/"+pluginuid);
//          Runtime.getRuntime().exec("chmod 777 " + "/data/data/com.xxAssistant/app_plugin");
        UpFilePermison("/data/data/"+context.getPackageName()+"/app_plugin/" + pluginuid);
        UpFilePermison("/data/data/"+context.getPackageName()+"/app_plugin");
//	        System.out.println("******************解压完毕********************"); 
        zipFile.delete();
    }


    /**
     * 普通的解压文件到desDir
     */
    public static UpZipResult unZipFiles(File zipFile, Context context, String desDir) {
        return unZipFiles(zipFile, context, desDir, "");
    }


    /**
     * 解压特定目录和文件名的文件到desDir
     */
    public static UpZipResult unZipFiles(File zipFile, Context context, String descDir, String filePathPrefix) {
        return unZipFiles(zipFile, context, descDir, filePathPrefix, false);
    }


    private static boolean isMatchPathPrefix(String zipEntryName, String filePathPrefix) {
        if (!TextUtils.isEmpty(filePathPrefix)) {
            if (filePathPrefix.endsWith(File.separator) && !zipEntryName.startsWith(filePathPrefix)) {
                // 前缀filePathPrefix是文件夹的形式
                return false;
            }
            if (!filePathPrefix.endsWith(File.separator) && !zipEntryName.equals(filePathPrefix)) {
                // 前缀是文件形式，必须全部路劲相等才解压
                return false;
            }
        }
        return true;
    }


    public static void printZipEntryFileMd5(String path) {
        File file = new File(path);
        if (!file.exists()) {
            return;
        }
        try {
            ZipFile zip = new ZipFile(file);
            for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
                ZipEntry entry = (ZipEntry) entries.nextElement();
                String zipEntryName = entry.getName();

                if (zipEntryName.endsWith("/")) {
                    continue;
                }
                InputStream in = zip.getInputStream(entry);
                String fileInZipMd5 = MD5Helper.getFileMD5(in);
                LogTool.i(TAG, "zip entry " + zipEntryName + " md5 is " + fileInZipMd5);
            }
            zip.close();
        } catch (Exception e) {
        }
    }

    /**
     * 获取zip文件解压后的大小, 如果出错返回-1
     * @param zipFile
     */
    public static long getZipFileOriginSize(File zipFile) {
        long total=0;
        try {
            ZipFile zip = new ZipFile(zipFile);
            for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
                ZipEntry entry = (ZipEntry) entries.nextElement();
                total += entry.getSize();
            }
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }
        return total;
    }

    /**
     * @param filePathPrefix          只解压zip包里面的文件路劲是begin with filePathPrefix，如果null或者空字符串，那么全部解压
     *                                <p> 例如，package.cfg，那么只会解压 package.cfg，不会解压package.cfg1 和 package.cfg2
     *                                <p> 例如，obb/ ，会解压 obb/dirName/fileName 等这些文件夹和文件
     * @param isCheckTargetExistFirst 是否先检查目标文件是否存在，存在就不拷贝，用md5检查
     */
    public static UpZipResult unZipFiles(File zipFile, Context context, String descDir, String filePathPrefix, boolean isCheckTargetExistFirst) {
        UpZipResult result = new UpZipResult();
        result.upzipFilePaths = new ArrayList<String>();
        try {
            File pathFile = new File(descDir);
            if (!pathFile.exists()) {
                pathFile.mkdirs();
            }
            // 进行解压
            ZipFile zip = new ZipFile(zipFile);
            for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
                ZipEntry entry = (ZipEntry) entries.nextElement();
                String zipEntryName = entry.getName();
                // 看是否要过滤一些文件不解压
                if (!isMatchPathPrefix(zipEntryName, filePathPrefix)) {
                    continue;
                }

                //判断路径是否存在,不存在则创建文件路径
                String outPath = "";
                if (filePathPrefix.endsWith(File.separator))
                    outPath = (descDir + zipEntryName.substring(filePathPrefix.length())).replaceAll("\\*", "/");
                else
                    outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
                LogTool.i(TAG, zipEntryName + " unzip to " + outPath);
                File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
                if (file.exists())
                    file.delete();
                if (!file.exists()) {
                    file.mkdirs();
                }
                //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
                if (new File(outPath).isDirectory()) {
                    continue;
                }

                // 先检查是否这个文件已经存在
                File targetFile = new File(outPath);
                if (isCheckTargetExistFirst && targetFile.isFile() && targetFile.exists()) {
                    // 比较文件大小、文件md5，两个都必须相等才判断为相同文件
                    long targetFileSize = targetFile.length();
                    long zipEntryFileSize = entry.getSize();
                    LogTool.i(TAG, "targetFileSize " + targetFileSize + " zipEntryFileSize " + zipEntryFileSize);
                    if (targetFileSize == zipEntryFileSize) {
                        InputStream in = zip.getInputStream(entry);
                        String fileInZipMd5 = MD5Helper.getFileMD5(in);
                        String targetFileMd5 = MD5Helper.getFileMD5(targetFile);
                        LogTool.i(TAG, "md5 is " + fileInZipMd5 + " from zip entry " + zipEntryName);
                        LogTool.i(TAG, "md5 is " + targetFileMd5 + " from " + outPath);
                        if (MD5Helper.compareMD5(targetFileMd5, fileInZipMd5)) {
                            continue;
                        }
                    }
                }

                // 开始进行解压部分和拷贝部分
                LogTool.i(TAG, "begin copy " + zipEntryName + " to " + outPath);
                InputStream in = zip.getInputStream(entry);
                result.upzipFilePaths.add(outPath);
                OutputStream out = new FileOutputStream(outPath);
                byte[] buf1 = new byte[1024];
                int len;
                int num = 0;
                while ((len = in.read(buf1)) > 0) {
                    out.write(buf1, 0, len);
                    num++;
                    if (num > 1000 * 100) {
                        num = 0;
                    }
                }
                in.close();
                out.flush();
                out.close();

                // 由于是解压一个文件，解压完就完事了。
                if (!filePathPrefix.endsWith(File.separator)) {
                    break;
                }
            }
            zip.close();
        } catch (Exception e) {
            LogTool.w(TAG, e.toString());
            result.isUpzipSucc = false;
            result.mExStr = e.toString();
            return result;
        }
        result.isUpzipSucc = true;
        result.mExStr = "succ";
        return result;
    }

    /**
     * 解压辅助
     * @param pluginuid 辅助id
     * @param zipFile 解压文件
     * @param context context
     * @param descDir 解压路径
     * @throws IOException
     */
    public static void unZipFiles(int pluginuid, File zipFile, Context context, String descDir) throws IOException {
        File pathFile = new File(descDir);
        if (!pathFile.exists()) {
            pathFile.mkdirs();
        }
        ZipFile zip = new ZipFile(zipFile);
        for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
            ZipEntry entry = (ZipEntry) entries.nextElement();
            String zipEntryName = entry.getName();
            InputStream in = zip.getInputStream(entry);
            String outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
            ;
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if (!file.exists()) {
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if (new File(outPath).isDirectory()) {
                continue;
            }

            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            while ((len = in.read(buf1)) > 0) {
                out.write(buf1, 0, len);
            }
            in.close();
            out.flush();
            out.close();

            Runtime.getRuntime().exec("chmod 777 " + outPath);
        }

        Runtime.getRuntime().exec("chmod 777 " + "/data/data/" + context.getPackageName() + "/app_plugin/" + pluginuid);
        Runtime.getRuntime().exec("chmod 777 " + "/data/data/" + context.getPackageName() + "/app_plugin");

        zipFile.delete();
    }

    /**
     * @description 解压XPK中的配置文件 package.cfg
     */
    public static ArrayList<String> unZipXpkConfigFiles(File zipFile, Context context, String descDir) throws IOException {
        File pathFile = new File(descDir);
        if (!pathFile.exists()) {
            pathFile.mkdirs();
        }
//	        Log.e("开始解压", "配置文件");
        ArrayList<String> files = new ArrayList<String>();

        ZipFile zip = new ZipFile(zipFile);
        for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
            ZipEntry entry = (ZipEntry) entries.nextElement();
            String zipEntryName = entry.getName();
//	            Log.e("开始解压", "配置文件"+zipEntryName);
            InputStream in = zip.getInputStream(entry);
            String outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
            ;
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if (file.exists())
                file.delete();

            if (!file.exists()) {
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if (new File(outPath).isDirectory()) {
                continue;
            }
            //输出文件路径信息
//	            System.out.println(outPath);
            files.add(outPath);
            if (!outPath.contains("application.apk") && !outPath.contains("package.cfg")) {
                in.close();
                continue;
            }
            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            int num = 0;
            while ((len = in.read(buf1)) > 0) {
                out.write(buf1, 0, len);
                num++;
                if (num > 1000 * 100) {
                    num = 0;
                }
            }
            in.close();
            out.flush();
            out.close();
        }
        zip.close();
//	        zipFile.delete();
        return files;
    }

    /**
     * @param zipFile
     * @param context
     * @param descDir
     * @throws IOException
     * @description 解压XPK中的obb文件
     */
    public static ArrayList<String> unZipXpkObbFiles(File zipFile, Context context, String obbName, String descDir) throws IOException {
        File pathFile = new File(descDir);
        if (!pathFile.exists()) {
            pathFile.mkdirs();
        }
//	        Log.e("开始解压OBB", "1");
        ArrayList<String> files = new ArrayList<String>();

        ZipFile zip = new ZipFile(zipFile);
        for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
            ZipEntry entry = (ZipEntry) entries.nextElement();
            String zipEntryName = entry.getName();
            InputStream in = zip.getInputStream(entry);
            String outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
            ;
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if (file.exists())
                file.delete();

            if (!file.exists()) {
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if (new File(outPath).isDirectory()) {
                continue;
            }
            //输出文件路径信息
//	            System.out.println(outPath);
            files.add(outPath);
            if (!outPath.substring(outPath.lastIndexOf('/'), outPath.length()).contains(obbName)) {
                in.close();
                continue;
            }
            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            int num = 0;
            while ((len = in.read(buf1)) > 0) {
                out.write(buf1, 0, len);
                num++;
                if (num > 1000 * 100) {
                    num = 0;
                }
            }
            in.close();
            out.flush();
            out.close();
        }
        zip.close();
//	        zipFile.delete();
        return files;
    }

    /**
     * @param zipFile
     * @param context
     * @param dirFileDir
     * @param descDir
     * @throws IOException
     * @description 解压XPK中的文件类型数据
     */
    public static ArrayList<String> unZipXpkDirFiles(File zipFile, Context context, String dirFileDir, String descDir) throws IOException {
        File pathFile = new File(descDir);
        if (!pathFile.exists()) {
            pathFile.mkdirs();
        }
        Log.e("开始解压Dir", "1");
        ArrayList<String> files = new ArrayList<String>();

        ZipFile zip = new ZipFile(zipFile);
        for (Enumeration entries = zip.entries(); entries.hasMoreElements(); ) {
            ZipEntry entry = (ZipEntry) entries.nextElement();
            String zipEntryName = entry.getName();
            InputStream in = zip.getInputStream(entry);
            String outPath = (descDir + zipEntryName).replaceAll("\\*", "/");
            if (outPath.contains(descDir.subSequence(descDir.lastIndexOf("/"), descDir.length()) + dirFileDir + "/")) {
                String descDir2 = descDir.substring(0, descDir.length() - 1);
                outPath = outPath.replace(descDir.subSequence(descDir2.lastIndexOf("/"), descDir2.length()) + "/" + dirFileDir + "/", descDir2.subSequence(descDir2.lastIndexOf("/"), descDir2.length()) + "/");
//	            	 Log.e("进行替换data  "+descDir.subSequence(descDir2.lastIndexOf("/"), descDir.length()) + dirFileDir+"/", outPath.replace(descDir.subSequence(descDir.lastIndexOf("/"), descDir.length()) + dirFileDir+"/", descDir2.subSequence(descDir.lastIndexOf("/"), descDir.length()) ));
            }

            Log.e(outPath, zipEntryName);
            //判断路径是否存在,不存在则创建文件路径
            File file = new File(outPath.substring(0, outPath.lastIndexOf('/')));
            if (file.exists())
                file.delete();

            if (!file.exists()) {
                file.mkdirs();
            }
            //判断文件全路径是否为文件夹,如果是上面已经上传,不需要解压
            if (new File(outPath).isDirectory()) {
                continue;
            }
            //输出文件路径信息
//	            System.out.println(outPath);
            files.add(outPath);
            if (outPath.contains("application.apk") || outPath.contains("package.cfg") || outPath.contains(".obb")) {
                Log.e("不解压这些文件", "2" + outPath);
                in.close();
                continue;
            }
            OutputStream out = new FileOutputStream(outPath);
            byte[] buf1 = new byte[1024];
            int len;
            int num = 0;
            while ((len = in.read(buf1)) > 0) {
                out.write(buf1, 0, len);
                num++;
                if (num > 1000 * 100) {
                    num = 0;
                }
            }
            in.close();
            out.flush();
            out.close();
        }

//	        zipFile.delete();
        return files;
    }

    /**
     * 提高文件权限的工具类
     *
     * @return
     */
    private static void UpFilePermison(String path) throws IOException {
        Runtime.getRuntime().exec("chmod 777 " + path);
    }
}
