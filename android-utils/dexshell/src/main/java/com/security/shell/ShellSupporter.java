package com.security.shell;

public class ShellSupporter {

    public static boolean isArtEnable(){
        String version = System.getProperty("java.vm.version");
        return version.startsWith("2.");
    }

    public static native void initNative(String apkPath, String extractPath, String releasePath, String pkgName, int encSize);
    public static native int initDexOptDexLoad(int payloadType, int dexEncLen);
    public static native int initNormalDexLoad(int payloadType, int dexEncLen);
    public static native byte[] getSrcAddr();
    public static native void cleanup();
}
