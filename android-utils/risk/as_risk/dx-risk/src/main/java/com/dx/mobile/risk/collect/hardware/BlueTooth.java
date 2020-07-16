package com.dx.mobile.risk.collect.hardware;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.provider.Settings;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.MacUtils;
import com.dx.mobile.risk.utils.PermissionUtils;

import org.json.JSONObject;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class BlueTooth {



    public static String getBTInfo() {

        JSONObject action_info = new JSONObject();
        Context ctx = ContextFinder.getApplication();

        String mac = null;
        String name = null;

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.CUPCAKE) {
                mac = Settings.Secure.getString(ctx.getContentResolver(), "bluetooth_address");
                name = Settings.Secure.getString(ctx.getContentResolver(), "bluetooth_name");

                if (mac != null) {
                    JSONUtils.jsonPut(action_info, "K80", mac);
                    JSONUtils.jsonPut(action_info, "K81", name);
                }
            }
        } catch (Exception e) {
            // 三星 Note2 4.2。 此处有会抛 java.lang.NoClassDefFoundError，找不到android.provider.Settings$Secure
            //e.printStackTrace();
            mac = null;
        }

        if (PermissionUtils.checkPermissions(Manifest.permission.BLUETOOTH)
                && Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR) {

            BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();

            try {
                if (Build.VERSION_CODES.M > Build.VERSION.SDK_INT
                        && Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR) {

                    if (adapter != null && PermissionUtils.checkPermissions(Manifest.permission.BLUETOOTH_ADMIN)) {
                        mac = adapter.getAddress();
                        name = adapter.getName();
                    }

                } else {

                    String fields[] = {"mService", "mManagerService"};

                    for (String fn : fields) {
                        Field mService_f = BluetoothAdapter.class.getDeclaredField(fn);
                        mService_f.setAccessible(true);
                        Object mService = adapter != null ? mService_f.get(adapter) : null;

                        if (mService != null) {
                            Class<?> mService_c = mService.getClass();
                            Method getAddress_m = mService_c.getDeclaredMethod("getAddress");
                            Method getName_m = mService_c.getDeclaredMethod("getName");

                            if (getAddress_m != null) {
                                getAddress_m.setAccessible(true);
                                mac = (String) getAddress_m.invoke(mService);
                            }

                            if (getName_m != null) {
                                getName_m.setAccessible(true);
                                name = (String) getName_m.invoke(mService);
                            }
                        }

                        if (MacUtils.isValidMac(mac)) {
                            break;
                        } else {
                            mac = null;
                        }
                    }
                }// if(Build.VERSION_CODES.M < Build.VERSION.SDK_INT)
            } catch (Throwable e) {
                mac = null;
            }

            if (mac != null) {
                JSONUtils.jsonPut(action_info, "K82", mac);
                JSONUtils.jsonPut(action_info, "K83", name);
            }
        }

        JSONUtils.jsonPut(action_info, "K85", getVersion(ctx));
        JSONUtils.jsonPut(action_info, "K86", isEnabled());
        JSONUtils.jsonPut(action_info, "K87", isExist(ctx));
        JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX4, JNIConst.VALUE_APP_KOCK);

        return action_info.length() > 0 ? action_info.toString() : null;
    }

    public static String getVersion(Context context) {

        String str = "N/A";
        if (context == null) {
            return str;
        }

        PackageManager packageManager = context.getPackageManager();
        return packageManager != null ? packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE) ? "4+" :
                packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH) ? "2.x/3.x" : str : str;
    }

    public static boolean isEnabled() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        return adapter == null ? false : adapter.isEnabled();
    }

    public static boolean isExist(Context context) {
        boolean isNull = BluetoothAdapter.getDefaultAdapter() != null;
        if (!isNull) {
            PackageManager packageManager = context.getPackageManager();
            if (packageManager != null) {
                return packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE) ||
                        packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH);
            }
        }
        return isNull;
    }
}
