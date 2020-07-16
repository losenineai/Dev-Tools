package com.dx.mobile.risk.linker;

import android.content.Context;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import com.dx.mobile.risk.DXRiskErrorException;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.MessageDigest;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

/**
 * Created by blue on 2019/07/08.
 */

public class ReLinkerErrorHandler {

    public static void handleLoadLibraryError(Context context, LinkageError e, LinkageError ori_e) {
        handleErrorInternal("DXRisk.loadLibrary()", context, e, ori_e);
    }

    public static void handleSetupError(Context context, LinkageError e) {
        handleErrorInternal("DXRisk.setup()", context, e, null);
    }

    public static void handleGetTokenError(Context context, LinkageError e) {
        handleErrorInternal("DXRisk.getToken()", context, e, null);
    }

    private static void handleErrorInternal(String funcLocation, Context context, LinkageError e, LinkageError ori_e) {

        StringBuilder expMsgBuilder = new StringBuilder();

        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("MM-dd HH:mm:ss.SSS");
        String dateString = dateFormat.format(date);

        if(ori_e != null) {

            expMsgBuilder
                    .append("\r\n============================================")
                    .append("\r\nori exp: " + Log.getStackTraceString(ori_e));
        }

        expMsgBuilder
                .append("\r\n============================================")
                //.append("\r\n" + e.getMessage())
                .append("\r\n" + Log.getStackTraceString(e))
                .append("\r\n============================================")
                .append("\r\n" + funcLocation + "  " + dateString)
                .append("\r\n============================================")
                .append("\r\nSo In Apk With Md5:   ")
                .append("\r\n============================================")
                .append("\r\n")
                .append(getSoMd5ListInApk(context))
                .append(getSoMd5ListInLib(context))
                .append(getSoMd5ListInAppLib(context))
                .append(getSoListInMaps(context));

        throw new DXRiskErrorException(expMsgBuilder.toString());
    }

    private static String byteArrayToHexString(byte[] byteArray) {

        if (byteArray == null || byteArray.length < 1) {
            return "";
        }

        final StringBuilder hexString = new StringBuilder();
        for (int i = 0; i < byteArray.length; i++) {

            if ((byteArray[i] & 0xff) < 0x10) {
                hexString.append("0");
            }

            hexString.append(Integer.toHexString(0xFF & byteArray[i]));
        }

        return hexString.toString().toLowerCase();
    }

    private static String getMD5(String file) {

        try {
            if(new File(file).exists())
                return getMD5(new FileInputStream(file));
        } catch (FileNotFoundException e) {

        }

        return "";
    }

    private static String getMD5(InputStream inputStream) {

        try {
            MessageDigest MD5 = MessageDigest.getInstance("MD5");

            byte[] buffer = new byte[8192];
            int length;
            while ((length = inputStream.read(buffer)) != -1) {
                MD5.update(buffer, 0, length);
            }
            return byteArrayToHexString(MD5.digest());
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                if (inputStream != null) {
                    inputStream.close();
                }
            } catch (IOException e) {
            }
        }
    }

    private static String getSoMd5ListInZipFile(String file) throws Exception {

        StringBuilder fileListMd5Builder = new StringBuilder();

        ZipFile zf = new ZipFile(file);
        InputStream in = new BufferedInputStream(new FileInputStream(file));
        ZipInputStream zin = new ZipInputStream(in);
        ZipEntry ze;

        while ((ze = zin.getNextEntry()) != null) {

            if (!ze.isDirectory()) {

                String fileMd5 = "";

                if (ze.getSize() > 0) {
                    fileMd5 = getMD5(zf.getInputStream(ze));
                }

                String fileName = ze.getName();
                if(fileName.contains("../")) {
                    boolean isDX = false;

                    if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
                        isDX = true;

                    if (!isDX)
                        continue;
                }

                if(!fileName.endsWith(".so"))
                    continue;

                fileListMd5Builder.append(fileName).append(" ").append(fileMd5).append("\r\n");
            }
        }

        zin.closeEntry();

        fileListMd5Builder.delete(fileListMd5Builder.lastIndexOf("\r\n"), fileListMd5Builder.length());

        return fileListMd5Builder.toString();
    }

    private static String getSoMd5ListInApk(Context context) {

        StringBuilder soFileListMd5 = new StringBuilder();

        for (String sourceDir : ApkLibraryInstaller.sourceDirectories(context)) {

            try {
                String soListMd5 = getSoMd5ListInZipFile(sourceDir);

                if(!TextUtils.isEmpty(soListMd5)) {
                    soFileListMd5.append("apk: " + sourceDir).append("\r\n");
                    soFileListMd5.append(soListMd5).append("\r\n");
                    soFileListMd5.append("- -").append("\r\n");
                }
            } catch (Exception exp) {
                exp.printStackTrace();
            }
        }

        return soFileListMd5.toString();
    }

    private static String getSoMd5ListInAppLib(Context context) {

        StringBuilder soFileListMd5 = new StringBuilder();

        File dir = context.getDir("lib", Context.MODE_PRIVATE);
        File[] files = dir.listFiles();

        soFileListMd5.append("app_lib: " + dir.getAbsolutePath()).append("\r\n");

        if(files.length <= 0)
            return "";

        for (File file : files) {
            soFileListMd5.append(file.getName() + "   " + getMD5(file.getAbsolutePath())).append("\r\n");
        }

        soFileListMd5.append("- -").append("\r\n");

        return soFileListMd5.toString();
    }

    private static String getSoMd5ListInLib(Context context){

        StringBuilder soFileListMd5 = new StringBuilder();

        File dir = new File(context.getApplicationInfo().nativeLibraryDir);
        File[] files = dir.listFiles();

        if(files.length <= 0)
            return "";

        soFileListMd5.append("lib: " + dir.getAbsolutePath()).append("\r\n");

        for (File file : files) {
            soFileListMd5.append(file.getName() + "   " + getMD5(file.getAbsolutePath())).append("\r\n");
        }

        soFileListMd5.append("- -").append("\r\n");

        return soFileListMd5.toString();
    }

    private static String getSoListInMaps(Context context) {

        StringBuilder soFileListMd5 = new StringBuilder();

        ArrayList<String> soList = new ArrayList<>();

        BufferedReader mapsReader = null;
        try {
            mapsReader = new BufferedReader(new InputStreamReader(new FileInputStream("/proc/" + android.os.Process.myPid() + "/maps"), "iso-8859-1"));
            String line;

            while ((line = mapsReader.readLine()) != null) {

                if(!line.endsWith(".so"))
                    continue;

                String[] blocks = line.split(" ");
                String block = blocks[blocks.length-1];

                if(soList.contains(block))
                    continue;

                soList.add(block);
            }

        } catch (Throwable ignore) {
        } finally {
            try {
                if (mapsReader != null) {
                    mapsReader.close();
                }
            } catch (Throwable ignore) {
            }
        }

        if(soList.size() <= 0)
            return "";

        soFileListMd5.append("maps:").append("\r\n");

        for(String line: soList) {
            soFileListMd5.append(line).append("\r\n");
        }

        soFileListMd5.append("- -").append("\r\n");

        return soFileListMd5.toString();
    }
}
