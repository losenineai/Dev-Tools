package com.dx.mobile.risk.utils;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Process;

/**
 * Created by boyliang on 2017/6/11.
 */

public final class PermissionUtils {
//    public static String READ_PHONE_STATE = "android.permission.READ_PHONE_STATE";
//    public static String WRITE_EXTERNAL_STORAGE = "android.permission.WRITE_EXTERNAL_STORAGE";
//    public static String CAMERA = "android.permission.CAMERA";
//    public static String INTERNET = "android.permission.INTERNET";
//    public static String ACCESS_WIFI_STATE = "android.permission.ACCESS_WIFI_STATE";
//    public static String BLUETOOTH = "android.permission.BLUETOOTH";

    public static boolean checkPermissions(String ... perms){
        try{
            Context ctx = ContextFinder.getApplication();
            for(String perm : perms){
                if(ctx.checkPermission(perm, Process.myPid(), Process.myUid()) != PackageManager.PERMISSION_GRANTED){
                    return false;
                }
            }
            return true;
        }catch(Throwable t){
        }

        return false;
        

    }

    //检查是否同时具有读写删文件的权限
    public static boolean checkWRPermission(){
        return checkPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE);

    }

    public static boolean checkLocation() {
        return checkPermissions(Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.ACCESS_COARSE_LOCATION);
    }

    public static boolean checkCamerPermission(){
        return checkPermissions(Manifest.permission.CAMERA);
    }
}
