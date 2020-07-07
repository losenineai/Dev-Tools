package com.dingxiang.util;

import java.io.*;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

/**
 * 文件类型检测器
 *
 * @author lai
 */
public final class FileTypesDetector {

    /**
     * 将文件头转换成16进制字符串
     *
     * @param src File bytes
     * @return 16进制字符串
     */
    private static String bytesToHexString(byte[] src) {

        StringBuilder stringBuilder = new StringBuilder();
        if (src == null || src.length <= 0) {
            return null;
        }
        //for (int i = 0; i < src.length; i++) {
        for (byte aSrc : src) {
            int v = aSrc & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString().toUpperCase();
    }

    /**
     * 判断文件类型
     *
     * @param is 文件
     * @return 文件类型
     */
    public static String getType(InputStream is) {

        int len = 100;
        int off = 0;

        try{

            if (is.available() <= 0) {
                return null;
            }

            if (is.available() < 100) {
                len = is.available();
            }

        }catch (IOException e){
            e.printStackTrace();
        }


        InputStream inputStream;
        if (!is.markSupported()) {
            inputStream = new BufferedInputStream(is);
        } else {
            inputStream = is;
        }

        inputStream.mark(len);

        String fileHead;
        byte[] bytesArr = new byte[len];
        int total = 0;

        try {
            total = inputStream.read(bytesArr, off, len);
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (total != -1) {
            fileHead = bytesToHexString(bytesArr);
        } else {
            return null;
        }

        if (fileHead == null || fileHead.length() == 0) {
            return null;
        }

        FileType[] fileTypes = FileType.values();

        for (FileType type : fileTypes) {
            if (fileHead.startsWith(type.getValue())) {
                return type.toString();
            }
        }

        return null;
    }

    /**
     * 判断文件类型
     *
     * @param filePath 文件路径
     * @return 文件类型
     */
    public static String getType(String filePath)  {

        try(BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream(filePath))){
            return getType(inputStream);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public static boolean isAPK(InputStream inputStream) {
        ZipInputStream zipInputStream = new ZipInputStream(inputStream);
        ZipEntry entry;
        try {
            while (true) {
                entry = zipInputStream.getNextEntry();
                if (entry == null) {
                    break;
                }

                if (entry.getName().equals("classes.dex")) {
                    return true;
                }
            }

        }
        catch (ZipException e) {
            System.out.println("It's a encrypted ZIP.");
            return false;
        }  catch (IOException e) {
            e.printStackTrace();
            return false;
        }  finally {
            try {
                zipInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return false;
    }

    /**
     * 判断文件是否为 ZIP 文件
     *
     * @param pFile 文件
     * @return 为ZIP文件返回 true, 否则, 返回false.
     */
    public static boolean isZIP(File pFile) {
        return "ZIP".equals(FileTypesDetector.getType(pFile.getAbsolutePath()));
    }


    /**
     * 判断文件是否为 APK 文件
     *
     * @param pFile 文件
     * @return 为APK文件返回 true, 否则, 返回false.
     */
    public static boolean isAPK(File pFile) {

        if(!isZIP(pFile)){
            return false;
        }

        ZipFile zipFile = null;
        try {

            zipFile = new ZipFile(pFile);
            ZipEntry classEntry = zipFile.getEntry("classes.dex");
            ZipEntry ManifestEntry = zipFile.getEntry("AndroidManifest.xml");
            return (classEntry != null) && (ManifestEntry != null);

        } catch (IOException e) {
            return false;
        } finally {
            if (zipFile != null) {
                try {
                    zipFile.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 判断文件是否为 AAR 文件
     *
     * @param pFile 文件
     * @return 为APK文件返回 true, 否则, 返回false.
     */
    public static boolean isAAR(File pFile) {

        if(!isZIP(pFile)){
            return false;
        }

        ZipFile zipFile = null;

        try {
            zipFile = new ZipFile(pFile);
            boolean flag = false;
            boolean flag2 = false;

            ZipEntry zipEntry = zipFile.getEntry("classes.jar");
            if (zipEntry != null){
                flag = true;
            }

            zipEntry = zipFile.getEntry("AndroidManifest.xml");
            if (zipEntry != null){
                flag2 = true;
            }

            return flag && flag2;

        } catch (IOException e) {

            e.printStackTrace();
            return false;

        } finally {
            if (zipFile != null) {
                try {
                    zipFile.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 判断文件是否为 AAR 文件
     *
     * @param pFile 文件
     * @return 为APK文件返回 true, 否则, 返回false.
     */
    public static boolean isJAR(File pFile) {

        if(!isZIP(pFile)){
            return false;
        }

        try (ZipFile zipFile = new ZipFile(pFile)){

            Enumeration emu = zipFile.entries();

            while(emu.hasMoreElements()){
                ZipEntry entry = (ZipEntry)emu.nextElement();
                //会把目录作为一个file读出一次，所以只建立目录就可以，之下的文件还会被迭代到。
                if (entry.isDirectory())
                    continue;

                BufferedInputStream bis = new BufferedInputStream(zipFile.getInputStream(entry));

                byte data[] = new byte[4];
                if (bis.read(data) != -1){
                    String magic = bytesToHexString(data);

                    if (magic.toLowerCase().equals("cafebabe")){
                        return true;
                    }
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }
}
