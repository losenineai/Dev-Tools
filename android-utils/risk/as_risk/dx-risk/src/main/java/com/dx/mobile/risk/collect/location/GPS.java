package com.dx.mobile.risk.collect.location;

import android.content.Context;
import android.location.Location;
import android.location.LocationManager;
import android.os.Build;
import android.provider.Settings;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.PermissionUtils;
import com.dx.mobile.risk.utils.ReflectUtil;

import org.json.JSONObject;

import java.util.List;

import static android.os.Build.VERSION_CODES.JELLY_BEAN_MR2;
import static android.os.Build.VERSION_CODES.P;

/**
 * Time: 2019-12-29
 * Author: white
 * Description:
 */
public class GPS {

    private static final String TAG = "DXGPS";


    public static String getGPSInfo() {
        if (!PermissionUtils.checkLocation()) {
            return "";
        }

        JSONObject gpsJson = new JSONObject();

        try {
            addGpsByApi(gpsJson);
            addGpsByReflect(gpsJson);
        } catch (Throwable e) {
        }


//        Log.d(TAG, "getGPSInfo:" + gpsJson.toString());

        return gpsJson.length() > 0 ? gpsJson.toString() : "";
    }


    /**
     * 正常API获取GPS信息，获取上一次GPS缓存定位的信息
     *
     * @return
     */
    private static Location addGpsByApi(JSONObject gpsJson) {

        if (!PermissionUtils.checkLocation()) {
            return null;
        }

        Location location = null;

        try {
            LocationManager locationManager = (LocationManager) ContextFinder.getApplication().getSystemService(Context.LOCATION_SERVICE);
            List<String> providers = locationManager.getAllProviders();

            for (String provider : providers) {
                Location l = locationManager.getLastKnownLocation(provider);
                if (l == null) {
                    continue;
                }
                if (location == null || l.getAccuracy() < location.getAccuracy()) {
                    location = l;
                }
            }

            if (location != null) {
                JSONUtils.jsonPut(gpsJson, JNIConst.KEY_APP_KOCK_INDEX12, JNIConst.VALUE_APP_KOCK);
                JSONUtils.jsonPut(gpsJson, "K123", isMock(location));
                JSONUtils.jsonPut(gpsJson, "K124", location.getSpeed());
                JSONUtils.jsonPut(gpsJson, "K125", location.getLatitude() + "," + location.getLongitude());
                JSONUtils.jsonPut(gpsJson, "K126", location.getTime());
                JSONUtils.jsonPut(gpsJson, "K127", location.getAltitude());
                JSONUtils.jsonPut(gpsJson, "K128", location.getAccuracy());
                JSONUtils.jsonPut(gpsJson, "K129", location.getBearing());

            }

        } catch (Throwable t) {
//            t.printStackTrace();
        }

        return location;
    }


    private static Boolean isMock(Location location) {

        Context ctx = ContextFinder.getApplication();

        boolean isMock = false;
        if (location != null && Build.VERSION.SDK_INT >= JELLY_BEAN_MR2) {
            isMock = location.isFromMockProvider();
        } else if (Settings.Secure.getString(ctx.getContentResolver(), Settings.Secure.ALLOW_MOCK_LOCATION).equals("0")) {
            isMock = true;
        }

        return isMock;
    }

    /**
     * 通过反射内部mService获取GPS信息
     *
     * @return
     */
    private static void addGpsByReflect(JSONObject gpsJson) {

        /**
         * 在Android9或以上系统，因非SDK接口（黑/灰 API）无法通过反射获取
         */
        if (Build.VERSION.SDK_INT >= P) {
            return;
        }

        if (!PermissionUtils.checkLocation()) {
            return;
        }

        Location location = null;

        try {
            LocationManager locationManager = (LocationManager) ContextFinder.getApplication().getSystemService(Context.LOCATION_SERVICE);
            List<String> providers = locationManager.getAllProviders();

            String packageName = ContextFinder.getApplication().getPackageName();
            for (String provider : providers) {


                Location l = null;
                Object locationRequestObj = createFromDeprecatedProvider(provider, 0, 0, true);

                Object mServiceObj = ReflectUtil.getFieldValue(locationManager, "mService");

                l = (Location) ReflectUtil.invokeMethod(mServiceObj, "getLastLocation", locationRequestObj, packageName);

                if (l == null) {
                    continue;
                }
                if (location == null || l.getAccuracy() < location.getAccuracy()) {
                    location = l;
                }
            }

            if (location != null) {
                JSONUtils.jsonPut(gpsJson, JNIConst.KEY_APP_KOCK_INDEX17, JNIConst.VALUE_APP_KOCK);
                JSONUtils.jsonPut(gpsJson, "K2200", ReflectUtil.getFieldValue(location, "mSpeed"));
                JSONUtils.jsonPut(gpsJson, "K2201", ReflectUtil.getFieldValue(location, "mLatitude") + "," + ReflectUtil.getFieldValue(location, "mLongitude"));
                JSONUtils.jsonPut(gpsJson, "K2202", ReflectUtil.getFieldValue(location, "mTime"));
                JSONUtils.jsonPut(gpsJson, "K2204", ReflectUtil.getFieldValue(location, "mAltitude"));
                JSONUtils.jsonPut(gpsJson, "K2205", ReflectUtil.getFieldValue(location, "mHorizontalAccuracyMeters"));
                JSONUtils.jsonPut(gpsJson, "K2206", ReflectUtil.getFieldValue(location, "mBearing"));
            }

        } catch (Throwable e) {
//            e.printStackTrace();
        }

    }


    private static final int POWER_NONE = 200;
    private static final int ACCURACY_FINE = 100;
    private static final int POWER_LOW = 201;

    /**
     * 使用反射方式new LocationRequest 的实例，因为LocationRequest.createFromDeprecatedProvider 是 SystemApi
     *
     * @param provider
     * @param minTime
     * @param minDistance
     * @param singleShot
     * @return
     */
    private static Object createFromDeprecatedProvider(String provider, long minTime, float minDistance, boolean singleShot) {
        if (minTime < 0) minTime = 0;
        if (minDistance < 0) minDistance = 0;

        int quality;
        if (LocationManager.PASSIVE_PROVIDER.equals(provider)) {
            quality = POWER_NONE;
        } else if (LocationManager.GPS_PROVIDER.equals(provider)) {
            quality = ACCURACY_FINE;
        } else {
            quality = POWER_LOW;
        }

        try {
            Object locationRequestObj = ReflectUtil.invokeMethod("android.location.LocationRequest", "create", null);

            ReflectUtil.setFieldValue(locationRequestObj, "mProvider", provider);
            ReflectUtil.setFieldValue(locationRequestObj, "mQuality", quality);
            ReflectUtil.setFieldValue(locationRequestObj, "mInterval", minTime);
            ReflectUtil.setFieldValue(locationRequestObj, "mFastestInterval", minTime);
            ReflectUtil.setFieldValue(locationRequestObj, "mSmallestDisplacement", minDistance);
            if (singleShot) {
                ReflectUtil.setFieldValue(locationRequestObj, "mNumUpdates", 1);
            }
            return locationRequestObj;
        } catch (Throwable e) {
//            e.printStackTrace();
        }

        return null;
    }
}
