package com.dingxiang.parser.android;

import com.dingxiang.parser.android.axml.ManifestInfo;
import com.dingxiang.parser.android.axml.Parser;
import com.dingxiang.base.BaseFile;
import com.dingxiang.util.Base64;
import com.dingxiang.util.FileTypesDetector;
import com.dingxiang.util.HashTool;
import pxb.android.axml.Util;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class APK extends BaseFile {

    private String dexMd5 = null;
    private ManifestInfo manifestInfo;
    private String icon = null;

    public APK(File apk) throws IOException {
        super(apk);

        if (!FileTypesDetector.isAPK(apk)) {
            throw new IOException("IT IS NOT A APK FILE.");
        }

        ZipFile zipFile = new ZipFile(apk);
        ZipEntry zipEntry = zipFile.getEntry("classes.dex");
        if (zipEntry != null) {
            dexMd5 = HashTool.getSHA256(Util.readIs(zipFile.getInputStream(zipEntry)));
        }
        zipFile.close();


        Parser parser = new Parser(apk);
        manifestInfo = parser.getManifestInfo();
        if (manifestInfo == null) {
            throw new FileNotFoundException("No AndroidManifest.xml");
        }

        if (manifestInfo.iconPath != null) {
            byte[] iconByte = getEntryInZip(apk, manifestInfo.iconPath);
            icon = Base64.encodeBytes(iconByte);
        } else {
            icon = "";
        }
    }

    /**
     * 获取包名
     *
     * @return 包名
     */
    public String getPackageName() {
        return manifestInfo.packageName;
    }

    /**
     * 获取版本号
     *
     * @return 版本号
     */
    public String getVersionCode() {
        return manifestInfo.versionCode;
    }

    /**
     * 获取版本名
     *
     * @return 版本名
     */
    public String getVersionName() {
        return manifestInfo.versionName;
    }

    /**
     * 获取dexMD5
     *
     * @return dex md5
     */
    @SuppressWarnings("UnusedDeclaration")
    public String getDexMD5() {
        return dexMd5;
    }

    /**
     * 获取应用名
     *
     * @return 应用名
     */
    public String getLabel() {
        return manifestInfo.label;
    }

    /**
     * 获取应用名
     *
     * @return 应用名
     */
    public String getIcon() {
        return icon;
    }

    /**
     * 获取权限列表
     *
     * @return ArrayList<String> 权限列表
     */
    public ArrayList<String> getPermissions() {
        return manifestInfo.requestedPermissions;
    }

    /**
     * 获取接收器信息，包含了对应的 INTENT
     *
     * @return 接收器信息
     */
    public HashMap<String, ArrayList<String>> getReceivers() {
        return manifestInfo.receivers;
    }

    /**
     * 获取服务列表
     *
     * @return 服务列表
     */
    public ArrayList<String> getServices() {
        return manifestInfo.services;
    }

    /**
     * 获取 Activity 列表，包含了对应的 INTENT
     *
     * @return Activity 列表
     */
    public HashMap<String, ArrayList<String>> getActivities() {
        return manifestInfo.activities;
    }

    private static byte[] getEntryInZip(File ipa, String entryName) {

        byte[] result;
        try {
            ZipFile zipFile = new ZipFile(ipa);
            ByteArrayOutputStream _copy = new ByteArrayOutputStream();

            ZipEntry entry = zipFile.getEntry(entryName);
            InputStream is = zipFile.getInputStream(entry);
            int chunk = 0;
            byte[] data = new byte[9806];
            while (-1 != (chunk = is.read(data))) {
                _copy.write(data, 0, chunk);
            }

            result = _copy.toByteArray();
            _copy.close();
        } catch (Exception e) {
            return null;
        }

        return result;
    }
}
