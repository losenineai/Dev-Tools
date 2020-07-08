package com.security.shell;

import android.util.Log;

import com.security.mobile.annotation.JXCStub;
import com.security.mobile.utils.DXLog;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class DXDexOptTools {

    private String mOptDirStr;
    private ArrayList<File> mDexs;
    private int mSdk_int;
    private boolean mIsSystemApp;
    private String mProcessAbi;
    private Map<String, String> mEnvMap;

    public DXDexOptTools(String srcApkStr, String pkgName, String preloadSo, ArrayList<File> dexs, String optDirStr, int sdk_int, int encSize, boolean isSystemApp, String abi){
        mOptDirStr = optDirStr;
        mDexs = dexs;
        mSdk_int = sdk_int;
        mIsSystemApp = isSystemApp;
        mProcessAbi = abi;

        mEnvMap = new TreeMap<>();
        mEnvMap.put("ANDROID_ROOT", "/system");
        mEnvMap.put("apkPath", srcApkStr);
        mEnvMap.put("pkgName", pkgName);
        mEnvMap.put("encSize", Integer.toString(encSize));
        mEnvMap.put("LD_PRELOAD", preloadSo);
    }

    private String replaceExt(String filePath, String ext) {
        int id = filePath.lastIndexOf('.');
        String newPath = filePath.substring(0, id) + "."+ext;
        return newPath;
    }

    private int checkOatFileIsOK(String oat_path){
        int result = 0;
        if (mSdk_int >= 26) {
            String vdex_path = replaceExt(oat_path, "vdex");
            File vdexFile = new File(vdex_path);
            if (!vdexFile.exists() || vdexFile.length() < 1){
                DXLog.e("DexOptTools checkOatFileIsOK vdex file not exists");
                result = 2;
            }
        }

        File oatFile = new File(oat_path);
        if (!oatFile.exists() || oatFile.length() < 1){
            DXLog.e("DexOptTools checkOatFileIsOK oat file not exists");
            result = 1;
        }

        return result;
    }

    private void addOatParams(List<String> args) {
        args.add("/system/bin/dex2oat");
        args.add("--boot-image=/system/framework/boot.art");
        String iSe = String.format("--instruction-set=%s", mProcessAbi);
        args.add(iSe);

        if (mIsSystemApp && mSdk_int >= 24) {
            //android 7以上因为seliux的限制
            //系统应用无法使用native优化，无法执行/data/data/pkg/目录下的文件，包括oat,selinux限制
            args.add("--compiler-filter=verify");
        }
        else {
            args.add("--compiler-filter=speed");
        }
        if (mSdk_int > 22) {
            args.add("--inline-max-code-units=0");
        }
    }

    private int runDexOpt(List<String> args) {
        int result = -1;
        ProcessBuilder pb = new ProcessBuilder(args);
        Map<String, String> env = pb.environment();
        env.putAll(mEnvMap);
        try {
            pb.redirectErrorStream(true);
            Process p = pb.start();
            p.waitFor();
            result = p.exitValue();
        } catch (Exception e) {
            DXLog.e("DexOptTools doDexOpt fail, see error stack!");
            e.printStackTrace();
        }
        return result;
    }

    public int doDexOpt(){

        if (mDexs.size() < 1) {
            return -1;
        }

        int result = -1;
        for (String k : mEnvMap.keySet()) {
            DXLog.d("dex2oat env key:%s,val:%s", k, mEnvMap.get(k));
        }

        String optPath = mOptDirStr;
        if (mSdk_int <= 23) {
            //android 6以下，将所有dex生成一个oat
            List<String> args = new ArrayList<>();
            addOatParams(args);
            for (File dex : mDexs) {
                String dex_file = String.format("--dex-file=%s", dex.getAbsolutePath());
                args.add(dex_file);
            }
            String oatName = "classes.oat";
            String oat = String.format("%s/%s", optPath, oatName);
            String oat_file = String.format("--oat-file=%s", oat);
            args.add(oat_file);
            result = runDexOpt(args);
            if (0 == result) {
                result = checkOatFileIsOK(oat);
            }
            return result;

        }
        else {
            //6.0之后，一个dex对应一个oat
            for (File dex : mDexs){
                ArrayList<String> args = new ArrayList<>();
                addOatParams(args);
                String dex_file = String.format("--dex-file=%s", dex.getAbsolutePath());
                args.add(dex_file);

                String oatName = dex.getName();
                if (mSdk_int >= 26) {
                    oatName = replaceExt(oatName, "odex");
                }
                String oat = String.format("%s/%s", optPath, oatName);
                String oat_file = String.format("--oat-file=%s", oat);
                args.add(oat_file);

                result = runDexOpt(args);
                if (result == 0) {
                    result = checkOatFileIsOK(oat);
                }
                if (result != 0) {
                    return result;
                }
            }
        }

        return result;
    }

}
