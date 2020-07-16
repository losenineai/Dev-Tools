package com.dx.mobile.risk.collect.hook;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.util.*;

/**
 * @author white
 * @description：
 * @date 2019-11-06
 */
public class Xposed {


    public static String getXposedInfo(){

        JSONObject jsonRoot = new JSONObject();

        try{
            String moduleInfo = getXposedModuleInfo();
            if (!TextUtils.isEmpty(moduleInfo)) {
                JSONUtils.jsonPut(jsonRoot, "K140", getXposedModuleInfo());
            }

            Map<String, List<String>> hookMap = getXposedHookMap();
            if (hookMap != null && hookMap.size() > 0) {
                JSONObject hookJson = new JSONObject();
                for (Map.Entry<String, List<String>> entry : hookMap.entrySet()) {
                    hookJson.put(entry.getKey(), entry.getValue().toString());
                }
                JSONUtils.jsonPut(jsonRoot, "K141", hookJson.toString());
            }
        }catch(Throwable e){

        }

        return jsonRoot.toString();
    }

    private static String getXposedModuleInfo(){
        String result = "";
        try {
            Context ctx = ContextFinder.getApplication();
            PackageManager pm = ctx.getPackageManager();

            List<PackageInfo> pkgList = pm.getInstalledPackages(PackageManager.GET_META_DATA);

            JSONArray arrayJson = new JSONArray();

            if (pkgList != null) {
                for (PackageInfo info : pkgList) {
                    Bundle bundle = info.applicationInfo.metaData;
                    if (bundle != null && bundle.containsKey("xposeddescription")) {
                        String muduleDesc = bundle.getString("xposeddescription");
                        JSONObject dataJson = new JSONObject();
                        dataJson.put("module_pkg", info.applicationInfo.packageName);
                        dataJson.put("module_name", info.applicationInfo.loadLabel(pm).toString());
                        dataJson.put("module_desc", muduleDesc);
                        arrayJson.put(dataJson);
                    }
                }
            }
            if (arrayJson.length() > 0) {
                result = arrayJson.toString();
            }
        } catch (Throwable e) {
        }


        return result;
    }


    private static Map<String, List<String>> getXposedHookMap() {

        Map<String, List<String>> hookMap = new HashMap();

        int flag = -1;

        Field field_hookedMethodCallbacks = null;

        try {

            Class clz_xposed = ClassLoader.getSystemClassLoader().loadClass("de.robv.android.xposed.XposedBridge");

            for (Field field : clz_xposed.getDeclaredFields()) {
                if ("sHookedMethodCallbacks".equals(field.getName())) {
                    field_hookedMethodCallbacks = field;
                    flag = 0;
                    break;
                }
                if ("hookedMethodCallbacks".equals(field.getName())) {
                    field_hookedMethodCallbacks = field;
                    flag = 1;
                    break;
                }
            }

            if (field_hookedMethodCallbacks != null) {
                field_hookedMethodCallbacks.setAccessible(true);

                Method meth_getSnapshot = null;
                Class clz_copyOnWriteSortedSet = null;
                Map<Member, Object> hookedMethodCallbacks = (Map) field_hookedMethodCallbacks.get(null);

                if (flag == 0) {
                    clz_copyOnWriteSortedSet = ClassLoader.getSystemClassLoader().loadClass("de.robv.android.xposed.XposedBridge$CopyOnWriteSortedSet");
                    if (clz_copyOnWriteSortedSet != null) {
                        meth_getSnapshot = clz_copyOnWriteSortedSet.getDeclaredMethod("getSnapshot", new Class[0]);
                    }
                }

                for (Map.Entry entry : hookedMethodCallbacks.entrySet()) {

                    Object[] snaps = null;

                    Member hookMethod = (Member) entry.getKey();
                    Object callbacks = entry.getValue();

                    if (clz_copyOnWriteSortedSet != null && clz_copyOnWriteSortedSet.isInstance(callbacks)) {
                        snaps = (Object[]) meth_getSnapshot.invoke(callbacks, new Object[0]);
                    } else if (TreeSet.class.isInstance(callbacks)) {
                        snaps = ((TreeSet) callbacks).toArray();
                    }

                    for (Object snap : snaps) {
                        String[] split = snap.getClass().getClassLoader().toString().split("\"");
                        if (split.length > 1) {
                            String moduleName = split[1];

                            if (moduleName.endsWith("XposedBridge.jar")){
                                break;
                            }

                            List<String> callbackList = hookMap.get(moduleName);

                            if(callbackList == null){
                                callbackList = new ArrayList();
                            }

                            callbackList.add(hookMethod.toString());

                            hookMap.put(moduleName, callbackList);
                        }
                    }
                }
            }
        } catch (Exception e) {
        }

        return hookMap;
    }
}
