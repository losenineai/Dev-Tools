package com.dx.mobile.risk.collect.hardware;

import android.content.Context;
import android.hardware.SensorManager;
import android.os.Build;

import com.dx.mobile.risk.utils.ContextFinder;

import org.json.JSONArray;

import java.util.List;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Sensor {

    public static String getSensorInfo() {
        try {

            JSONArray jsonArr = new JSONArray();
            Context ctx = ContextFinder.getApplication();
            SensorManager sm = (SensorManager) ctx.getSystemService(Context.SENSOR_SERVICE);

            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.CUPCAKE) {
                List<android.hardware.Sensor> sensors = sm.getSensorList(android.hardware.Sensor.TYPE_ALL);

                for (android.hardware.Sensor s : sensors) {
                    jsonArr.put(s.getName());
                }
            }
            return jsonArr.length() > 0 ? jsonArr.toString() : "";
        } catch (Throwable t) {
        }

        return "";
    }


}
