package com.xxlib.utils.io;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Created by white on 16/1/13.
 */
public class ZipUtil {


    /**
     *
     * @param zipFile
     * @throws Exception
     */
    public static boolean unZip(String zipFile, String dstDir) {
        try{
            ZipFile zFile = new ZipFile(zipFile);
//            System.out.println(zFile.getName());
            Enumeration en = zFile.entries();
            ZipEntry entry = null;
            while (en.hasMoreElements()) {
                // 得到其中一项ZipEntry
                entry = (ZipEntry) en.nextElement();
                // 如果是文件夹则不处理
                if (entry.isDirectory()) {
                } else {
                    // 如果是文件则写到输出目录
                    copyFile(zFile, dstDir, entry);
                }
            }
            zFile.close();

            return true;
        }catch(Exception er){
            er.printStackTrace();
        }
       return false;
    }

    private static void copyFile(ZipFile source, String baseDir, ZipEntry entry)
            throws Exception {
        // 以ZipEntry为参数得到一个InputStream，并写到OutputStream中
        // 是否需要创建目录
        mkdirs(baseDir, entry.getName());
        // 建立输出流
        OutputStream os = new BufferedOutputStream(new FileOutputStream(
                new File(baseDir, entry.getName())));
        // 取得对应ZipEntry的输入流
        InputStream is = new BufferedInputStream(source.getInputStream(entry));
        int readLen = 0;
        byte[] buf = new byte[1024];
        // 复制文件
        System.out.println("Extracting: " + entry.getName() + "t"
                + entry.getSize() + "t" + entry.getCompressedSize());
        while ((readLen = is.read(buf, 0, 1024)) != -1) {
            os.write(buf, 0, readLen);
        }
        is.close();
        os.close();
        System.out.println("Extracted: " + entry.getName());
    }

    /**
     *
     *
     * @param baseDir
     *                指定根目录
     * @param absFileName
     *                相对路径名，来自于ZipEntry中的name
     * @return java.io.File 实际的文件
     */
    /**
     * 给定根目录，返回一个相对路径所对应的实际文件名.
     * @param baseDir
     * @param relativeFileName
     */
    private static void mkdirs(String baseDir, String relativeFileName) {
        String[] dirs = relativeFileName.split("/");
        File ret = new File(baseDir);
        if (dirs.length > 1) {
            for (int i = 0; i < dirs.length - 1; i++) {
                ret = new File(ret, dirs[i]);
            }
        }
        if (!ret.exists()) {
            ret.mkdirs();
        }
    }
}
