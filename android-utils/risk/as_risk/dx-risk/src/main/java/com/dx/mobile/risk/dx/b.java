package com.dx.mobile.risk.dx;

import android.Manifest;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Build;
import android.provider.Settings;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.PermissionUtils;
import com.dx.mobile.risk.utils.StringUtils;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;

/**
 *
 * StorageUtils 的实现类，为了混淆java2jni的方法
 * Created by white on 2018/12/17.
 */

public class b {

    public static final int GLOBAL = 0;
    public static final int LOCAL = 1;

    public static boolean put(String key, String value, int scope) {

        if(StringUtils.isEmpty(key) || StringUtils.isEmpty(value))
            return false;

        scope = scope == GLOBAL ? GLOBAL : LOCAL;

        return pn(key, value, scope);
    }

    public static String get(String key, int scope) {

        return get(key, scope, null);
    }

    public static String get(String key, int scope, String defValue) {

        if(StringUtils.isEmpty(key))
            return defValue;

        scope = scope == GLOBAL ? GLOBAL : LOCAL;

        String result;
        result =  gn(key, scope, defValue);
        return result;
    }

    public static boolean remove(String key, int scope) {

        if(StringUtils.isEmpty(key))
            return false;

        scope = scope == GLOBAL ? GLOBAL : LOCAL;

        return rn(key, scope);
    }

    public static String getInSharedPreferences(String key, String dbName) {

        Context context = ContextFinder.getApplication();
        SharedPreferences sharedPreferences = context.getSharedPreferences(dbName, Context.MODE_PRIVATE);
        String result = sharedPreferences.getString(key, "");

        return result;

    }

    public static void putInSharedPreferences(String key, String value, String dbName) {

        Context context = ContextFinder.getApplication();
        SharedPreferences.Editor editor = context.getSharedPreferences(dbName, Context.MODE_PRIVATE).edit();

        editor.putString(key, value);
        editor.commit();

    }

    public static void removeInSharedPreferences(String key, String dbName) {

        Context context = ContextFinder.getApplication();
        SharedPreferences.Editor editor = context.getSharedPreferences(dbName, Context.MODE_PRIVATE).edit();

        editor.remove(key);
        editor.commit();
    }

    //添加版本判断，当Android6.0后不往setting数据库读取与存储数据,以解决Android 6.0后申请WRITE_SETTING权限后的崩溃问题
    public static Object getInSetting(String key) {

        String value;
        if(PermissionUtils.checkPermissions(Manifest.permission.WRITE_SETTINGS)){
            try {
                value = Settings.System.getString(ContextFinder.getApplication().getContentResolver(), key);

                return value;
            } catch (Exception e) {
                return "";
            }
        }else {
            return "";
        }
    }

    public static boolean putInSetting(String key, String value) {

        return Build.VERSION.SDK_INT < Build.VERSION_CODES.M &&
                PermissionUtils.checkPermissions(Manifest.permission.WRITE_SETTINGS) &&
                Settings.System.putString(ContextFinder.getApplication().getContentResolver(), key, value);
    }

    public static void removeInSetting(String key) {

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M &&
                PermissionUtils.checkPermissions(Manifest.permission.WRITE_SETTINGS)) {
            Settings.System.putString(ContextFinder.getApplication().getContentResolver(), key, "");
        }else {
            return ;
        }
    }

    public static String getInSQLite(String key) {

        String result;
        SQLiteDatabase db = null;
        Cursor cursor = null;

        try {
            db = ContextFinder.getApplication().openOrCreateDatabase("db.db", 0, null);
            cursor = db.rawQuery("select * from db where pkgkey =?", new String[]{key});
            cursor.moveToFirst();
            result = cursor.getString(cursor.getColumnIndex("value"));
        } catch (Exception e) {
            return "";
        } finally {
            if (cursor != null) {
                try {
                    cursor.close();
                } catch (Exception e) {
                }
            }
            if(db != null)
                try {
                    db.close();
                } catch (Exception e) {
                }
        }

        return result;
    }

    public static boolean putInSQLite(String key, String value) {

        SQLiteDatabase db = null;

        try {
            db = ContextFinder.getApplication().openOrCreateDatabase("db.db", 0, null);
            db.execSQL("create table if not exists db( id integer primary key autoincrement, pkgkey text unique, value text)");
            db.execSQL("insert or replace into db(pkgkey, value) values( '" + key + "'," + "'" + value + "')");
            db.close();
        } catch (Exception e) {
            //e.printStackTrace();
            return false;
        } finally {
            if(db != null)
                try {
                    db.close();
                } catch (Exception e) {
                }
        }

        return true;
    }

    public static boolean removeInSQLite(String key) {

        SQLiteDatabase db = null;

        try {
            db = ContextFinder.getApplication().openOrCreateDatabase("db.db", 0, null);
            db.execSQL("delete from db where pkgkey=?", new String[]{key});
        } catch (Exception e) {
            return false;
        } finally {
            if(db != null)
                try {
                    db.close();
                } catch (Exception e) {
                }
        }

        return true;
    }

    public static byte[] getAssetsFileData(String fileName) {

        AssetManager assetManager = ContextFinder.getApplication().getAssets();
        byte[] bytes = null;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try {
            InputStream is = assetManager.open(fileName);
            byte[] buff = new byte[256];
            int len;

            while ((len = is.read(buff)) > 0){

                bos.write(buff, 0, len);

            }

            bos.flush();
            bytes = bos.toByteArray();

        } catch (Exception e) {
        }
        return bytes;

    }

    /**
     * putNative
     * @param key
     * @param value
     * @param scope
     * @return
     */
    private static native boolean pn(String key, String value, int scope);

    /**
     * getNative
     * @param key
     * @param scope
     * @param defValue
     * @return
     */
    private static native String gn(String key, int scope, String defValue);

    /**
     * removeNative
     * @param key
     * @param scope
     * @return
     */
    private static native boolean rn(String key, int scope);


}
