package com.dx.mobile.risk.utils;

import org.json.JSONObject;

public class JSONUtils {

    public static void jsonPut(JSONObject json, String key, Object value) {
        try {
            if (value instanceof String[]) {
                String[] vals = (String[]) value;
                StringBuilder sb = new StringBuilder();
                for (String val : vals) {
                    sb.append(val + ",");
                }

                json.put(key, sb.toString());
            } else if (value instanceof String) {
                json.put(key, value);
            } else {
                json.put(key, value + "");
            }

        } catch (Throwable e) {
//            e.printStackTrace();
        }
    }
}
