package com.security.shell;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.os.Build;

import com.security.mobile.annotation.JXCStub;
import com.security.mobile.utils.CpuAbiTools;
import com.security.mobile.utils.DXLog;
import com.keep.DebugUtils;
import com.security.mobile.utils.LoadSoHelper;
import com.security.mobile.utils.RefInvoke;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.util.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class MemShell extends ShellBase {
    private boolean mIsTestIn;
    private int mPayloadType;
    private String mPkgName;
    private String mInjectSo;
    private int mOatEncSize;
    private int mDexEncSize;

    public MemShell(Context ctx, String payLoadZipPath, String pkgName, String extractDir, int dexCount, int oatEncSize, int dexEncSize, int isTestIn, int payloadType) {
        super(ctx, payLoadZipPath, extractDir, dexCount);

        mPayloadType = payloadType;
        mPkgName = pkgName;
        mInjectSo = "";

        if (isTestIn == 0){
            mIsTestIn = false;
        } else {
            mIsTestIn = true;
        }

        mOatEncSize = oatEncSize;
        mDexEncSize = dexEncSize;

        DXLog.i("**CpuAbiTools mProcessAbi:%s", mProcessAbi);
    }

    @JXCStub
    private void makeFakeDexs(String descDir, int dexNum) throws IOException {

        String fileName;
        for (int i=0; i<dexNum; i++){
            if (i == 0){
                fileName = "d-classes.dex";
            } else {
                int tmp = i+1;
                fileName = "d-classes" + tmp + ".dex";
            }

            File dexFile = new File(descDir, fileName);

            if (!dexFile.exists()) {
                boolean s = dexFile.createNewFile();
                if (!s)
                    DXLog.e("makeFakeDexs make fake file fail:%s", fileName);
            }
        }
    }

    @JXCStub
    private Map<String, String> extractShellSos(String extraDir) {

        Map<String, String> map = new HashMap<>();
        boolean isX86 = CpuAbiTools.getMachineAbi().contains("x86");

        String libDirName;
        if (isX86){
            DXLog.i("running on x86 platform, loading x86 so");

            libDirName = "x86";
            String path;
            if (!isLibPathContain(mCtx, "x86") && !isLibPathContain(mCtx, "x86_64")) {
                path = extractSo(extraDir, libDirName, "libcsn_x86.so");
                map.put("csn", path);
            }

            path = extractSo(extraDir, libDirName, "libcsn2_x86.so");
            map.put("csn2", path);
        } else {
            libDirName = "armeabi-v7a";
            String path = extractSo(extraDir, libDirName, "libcsn2.so");
            map.put("csn2", path);
        }

        return map;
    }

    private boolean isLibPathContain(Context ctx, String abi) {
        String pattern = "";

        if (abi.equals("armeabi") || abi.equals("armeabi-v7a")){
            pattern = "arm";
        } else if (abi.equals("arm64-v8a")) {
            pattern = "arm64";
        } else if (abi.equals("x86")) {
            pattern = "x86";
        } else if (abi.equals("x86_64")) {
            pattern = "x86_64";
        }

        String path = ctx.getApplicationInfo().nativeLibraryDir;
        String[] libPaths = path.split("/");
        String libPathArch = libPaths[libPaths.length-1];
        DXLog.i("** isLibPathContain libpath:%s pattern:%s abi:%s", path, pattern, abi);
        return libPathArch.equals(pattern);
    }

    @JXCStub
    private String extractSo(String extraDir, String libDirName, String soName) {

        File outSo = new File(extraDir, soName);
        try {
            String apk = mCtx.getApplicationInfo().sourceDir;
            ZipFile zipFile = new ZipFile(apk);
            ZipEntry entry = zipFile.getEntry("assets/"+soName);
            if (entry == null) {
                DXLog.i("%s not in asset try lib", soName);
                entry = zipFile.getEntry(String.format("lib/%s/%s", libDirName, soName));
            }
            if (entry == null) {
                //??????so???????????????????????????????????????????????????dex2oat
                DXLog.e("no suitable so find skip");
                return "";
            }

            long crc = getLongValue(mCtx, soName);
            long crcEntry = entry.getCrc();
            if (crc == crcEntry) {
                return outSo.toString();
            }

            DXLog.i("crc so not equal crc1=0x%08x crcEntry=0x%08x", crc, crcEntry);

            FileOutputStream fos = new FileOutputStream(outSo);
            InputStream localZipInputStream = zipFile.getInputStream(entry);
            InputStream is = new BufferedInputStream(localZipInputStream);
            _copyTo(is, fos);
            fos.close();
            is.close();
            zipFile.close();
            setLongValue(mCtx, soName, crcEntry);
        } catch (IOException e) {
            e.printStackTrace();
        }
        DXLog.i("after extract so");
        return outSo.toString();

    }

    private boolean isMainProcess() {

        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader("/proc/self/cmdline"));
            String processName = reader.readLine();
            if (!processName.isEmpty()) {
                processName = processName.trim();
            }

            reader.close();
            String pkgName = mCtx.getPackageName();
            DXLog.d("isMainProcess processName:%s pkgName:%s", processName, pkgName);
            return processName.equalsIgnoreCase(pkgName);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private void runDexOpt(Context ctx, String releaseDexDir, int payloadType, String payLoadZipPath, String pkgName, String injectSo, String optDir, String pattern){
        int k = ShellSupporter.initDexOptDexLoad(payloadType, mDexEncSize);
        if (k < 1) return;

        File[] _dexes = listDexs(releaseDexDir, pattern);
        final ArrayList<File> _files = new ArrayList<>(Arrays.asList(_dexes));

        //????????????????????????dex2oat
        DXLog.i("trigger dex2oat background");

        int flags = mCtx.getApplicationInfo().flags;
        DXLog.i("app flags %08x, isSystemApp=%d, isSystemUpdateApp=%d", flags, flags & ApplicationInfo.FLAG_SYSTEM,
                flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP);
        boolean isSystemApp = (flags & ApplicationInfo.FLAG_SYSTEM)!=0;

        DXDexOptTools dxDexOptTools = new DXDexOptTools(payLoadZipPath, pkgName, injectSo,
                _files, optDir, Build.VERSION.SDK_INT, mOatEncSize, isSystemApp, mProcessAbi);
        int result = dxDexOptTools.doDexOpt();

        if (result == 0) {
            DXLog.i("dex2oat background success code:%d!", result);
            setBooleanValue(ctx, "opt-finish", true);
        } else {
            DXLog.i("dex2oat background fail coed:%d", result);
        }
    }

    @Override
    protected void extractDexs(String payLoadPath, int numOfDexs) throws IOException {

        ZipFile zipFile = new ZipFile(mPayLoadZipPath);

        ZipEntry entry = null;
        if (mPayloadType == 0){
            entry = zipFile.getEntry("classes.dex");
        } else if (mPayloadType == 1){
            entry = zipFile.getEntry("assets/csn-enc.data");
        }

        if (entry == null) {
            return;
        }

        long crc = getLongValue(mCtx, "crc");
        long crc2 = entry.getCrc();

        long sdkIntOld = getLongValue(mCtx, "sdk_int");
        long sdkIntNow = Build.VERSION.SDK_INT;

        //???????????????android???????????????id????????????git????????????????????????????????????????????????????????????????????????
        String incrementalOld = getStringValue(mCtx, "incremental");
        String incrementalNow = Build.VERSION.INCREMENTAL;

        DXLog.i("crc1=%s,crc2=%s,sdkIntOld=%d,sdkIntNow=%d,incrementalOld=%s,incrementalNow=%s", Long.toHexString(crc), Long.toHexString(crc2),
                sdkIntOld, sdkIntNow, incrementalOld, incrementalNow);

        if ((crc != crc2 ||
                sdkIntNow != sdkIntOld ||
                !incrementalNow.equals(incrementalOld)) && isMainProcess()) {
            if (mPayloadType == 1){
                InputStream localZipInputStream = zipFile.getInputStream(entry);
                InputStream is = new BufferedInputStream(localZipInputStream);
                FileOutputStream fileOutputStream = new FileOutputStream(payLoadPath);

                _copyTo(is, fileOutputStream);

                fileOutputStream.close();
                is.close();
                zipFile.close();
            }

            setBooleanValue(mCtx, "opt-finish", false);
            setLongValue(mCtx, "crc", crc2);
            setLongValue(mCtx, "sdk_int", sdkIntNow);
            setStringValue(mCtx, "incremental", incrementalNow);

            //delete dirs
            FileTools.DeleteDir(new File(mReleaseDexsDir));
            FileTools.DeleteDir(new File(mOptDexDir));
            DXLog.i("extract src files number:%d", numOfDexs);
        }


        new File(mReleaseDexsDir).mkdirs();
        new File(mOptDexDir).mkdirs();

        makeFakeDexs(mReleaseDexsDir, numOfDexs);
    }

    private void fixSupportAbi() {

        if (Build.VERSION.SDK_INT < 21)
            return;

        String[] supportAbis = Build.SUPPORTED_ABIS;

        String primaryCpuAbi = supportAbis[0];
        if (primaryCpuAbi.contains("64") && !mProcessAbi.contains("64")){

            if (isLibPathContain(mCtx, primaryCpuAbi))
                return;

            String[] newAbis = new String[supportAbis.length-1];
            int i=0;
            for (String s : supportAbis){
                if (!s.equals(primaryCpuAbi)){
                    newAbis[i] = s;
                    i++;
                }
            }

            RefInvoke.setStaticOjbect(Build.class.getName(), "SUPPORTED_ABIS", newAbis);

        }

    }

    @Override
    protected void init2() {
        String apkPath = mCtx.getApplicationInfo().sourceDir;
        Map<String, String> soMap = extractShellSos(mExtractDir);
        String csn1 = soMap.get("csn");
        if (csn1 == null) {
            LoadSoHelper.verboseLoadLibrary("csn");
        }
        else {
            DXLog.i("run on x86 platform, load x86 so!");
            System.load(csn1);
        }

        //??????????????????arm64 so???apk???????????? android6 ?????????????????????????????????????????????64????????????apk??????32?????????????????????
        //???????????????????????????abi?????????so???????????????????????????????????????????????????32?????????apk??????64???so????????????????????????SUPPORTED_ABIS
        //??????64?????????
        fixSupportAbi();

        //????????????32???64???????????????????????????????????????libcsn2??????app???lib???arm64v8??????armv7???????????????32????????????dex2oat??????32?????????
        List<String> candicate = new ArrayList<>();
        if (apkPath.startsWith("/system")) {
            //apk??????system?????????????????????
            //???7.0????????????selinux?????????????????????scontext???platform_app/system_app,????????????/data/data/<pkg>/??????????????????
            //??????????????????/system/lib,/system/lib64??????????????????
            candicate.add("/system/lib/libcsn2.so");
            candicate.add("/system/lib64/libcsn2.so");
        }
        else {
            File apkFile = new File(apkPath);
            String appdir = apkFile.getParentFile().toString();
            String arch = CpuAbiTools.getMachineAbi();
            if (arch.contains("arm")) {
                candicate.add(String.format("%s/lib/arm/libcsn2.so", appdir));
                candicate.add(String.format("%s/lib/arm64/libcsn2.so", appdir));
            }
            else if (arch.contains("x86")){
                //x86
                candicate.add(String.format("%s/lib/x86/libcsn2.so", appdir));
                candicate.add(String.format("%s/lib/x86_64/libcsn2.so", appdir));
            }
            //apk???/data/app/xxx?????????????????????????????????????????????app??????????????????/data/data/<pkg>????????????????????????
        }
        candicate.add(soMap.get("csn2"));
        for (String item : candicate) {
            File f = new File(item);
            DXLog.i("try path %s", f.toString());
            if (f.exists() && CpuAbiTools.isElf32(item)) {
                mInjectSo = item;
                break;
            }
        }
        DXLog.i("inject so path %s", mInjectSo);
        ShellSupporter.initNative(apkPath, mExtractDir, mReleaseDexsDir, mCtx.getPackageName(), mOatEncSize);
    }

    @JXCStub
    private File[] listDexs(String dexDir, final String pattern) {
        File []dexes = new File(dexDir).listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                //DXLog.i("files:"+name);
                return name.toLowerCase().endsWith(".dex") && name.toLowerCase().startsWith(pattern);
            }

        });

        //?????????dex??????????????????????????????????????????????????????????????????????????????????????????load
        Arrays.sort(dexes, new Comparator<File>() {

            @Override
            public int compare(File t1, File t2) {
                String s1 = t1.toString();
                String s2 = t2.toString();
                return s1.compareTo(s2);
            }
        });
        return dexes;
    }

    @Override
    public ClassLoader createClassLoader(final ClassLoader parentLoader) {
        long mTimeInit = System.currentTimeMillis();

        //?????????????????????DexClassLoader
        ClassLoader theLoader = parentLoader;

        boolean isArt = ShellSupporter.isArtEnable();
        File optDirFile = new File(mOptDexDir);

        String pattern = "classes";
        if (mIsTestIn && isArt){
            //???????????????????????????????????????oat??????????????????
            //runDexOpt(mCtx, mReleaseDexsDir, mPayloadType, mPayLoadZipPath, mPkgName, mInjectSo, mOptDexDir, pattern);
            DXLog.d("before initDexOpt memory : %d", DebugUtils.getMemorySize());
            ShellSupporter.initDexOptDexLoad(mPayloadType, mDexEncSize);
            DXLog.d("after initDexOpt memory : %d", DebugUtils.getMemorySize());
        } else {
            boolean optFinish = getBooleanValue(mCtx, "opt-finish");
            if (!optFinish) {
                DXLog.d("shell createClassLoader initNormalDexLoad 0:%d optFinish:"+optFinish, System.currentTimeMillis()- mTimeInit);
                DXLog.d("before initNormalDexLoad memory : %d", DebugUtils.getMemorySize());
                ShellSupporter.initNormalDexLoad(mPayloadType, mDexEncSize);
                DXLog.d("after initNormalDexLoad memory : %d", DebugUtils.getMemorySize());
                DXLog.d("shell createClassLoader initNormalDexLoad 0-1:%d optFinish:"+optFinish, System.currentTimeMillis()- mTimeInit);
                pattern = "d-";
            }
        }
        File []dexes = listDexs(mReleaseDexsDir, pattern);
        if (dexes.length != mDexCount){
            DXLog.e("dexes file count not match!");
            throw new RuntimeException();
        }

        final ArrayList<File> files = new ArrayList<>(Arrays.asList(dexes));
        DXLog.d("shell createClassLoader step 1:%d", System.currentTimeMillis()- mTimeInit);

        File[] fs = optDirFile.listFiles();
        for (File f : fs){
            DXLog.d("shell createClassloader before multiMake f:%s", f.getAbsolutePath());
        }

        DXLog.d("before multi make memory : %d", DebugUtils.getMemorySize());
        multiThreadMakeDexs(files, theLoader, optDirFile);
        DXLog.d("after multi make memory : %d", DebugUtils.getMemorySize());

        DXLog.d("shell createClassLoader step 2:%d", System.currentTimeMillis()- mTimeInit);

        boolean optFinish = getBooleanValue(mCtx, "opt-finish");

        if (isArt && !optFinish
                 && !mIsTestIn && isMainProcess()) {

            //?????????4.4???????????????????????????dex2oat????????????????????????????????????,?????????????????????dex
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    runDexOpt(mCtx, mReleaseDexsDir, mPayloadType, mPayLoadZipPath, mPkgName, mInjectSo, mOptDexDir, "classes");
                }
            });
            thread.start();
        }
        DXLog.d("shell createClassLoader step 3:%d", System.currentTimeMillis()- mTimeInit);

        DXLog.d("end of createClassLoader memory : %d", DebugUtils.getMemorySize());

        return theLoader;
    }

}
