package com.dx.mobile.risk.collect.hardware;

import android.os.Build;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.JSONUtils;

import org.json.JSONObject;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Builds {

    public static String getBuildStrings() {

        JSONObject action_json = new JSONObject();

        // v5.3.0 关闭
//        try {
//
//            FileReader localFileReader = new FileReader("/proc/version");
//            BufferedReader localBufferedReader = new BufferedReader(
//                    localFileReader, 8192);
//            String kernelVersion = localBufferedReader.readLine();
//            localBufferedReader.close();
//
//             JSONUtils.jsonPut(action_json, "K10", kernelVersion);
//
//        } catch (Throwable e) {
//        }

         JSONUtils.jsonPut(action_json, "K11", Build.VERSION.RELEASE);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
             JSONUtils.jsonPut(action_json, "K12", Build.VERSION.SECURITY_PATCH);

         JSONUtils.jsonPut(action_json, "K13", Build.FINGERPRINT);
         JSONUtils.jsonPut(action_json, "K14", Build.HARDWARE);
         JSONUtils.jsonPut(action_json, "K15", Build.HOST);
         JSONUtils.jsonPut(action_json, "K16", Build.TIME);
         JSONUtils.jsonPut(action_json, "K17", Build.DEVICE);
         JSONUtils.jsonPut(action_json, "K18", Build.MODEL);
         JSONUtils.jsonPut(action_json, "K19", Build.BRAND);
         JSONUtils.jsonPut(action_json, "K20", Build.PRODUCT);
         JSONUtils.jsonPut(action_json, JNIConst.KEY_APP_KOCK_INDEX5, JNIConst.VALUE_APP_KOCK);
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
             JSONUtils.jsonPut(action_json, "K21", Build.CPU_ABI + "," + Build.CPU_ABI2);

             JSONUtils.jsonPut(action_json, "K900", Build.CPU_ABI);
             JSONUtils.jsonPut(action_json, "K901", Build.CPU_ABI2);

        } else {
            StringBuilder sb = new StringBuilder();

            for (String abi : Build.SUPPORTED_ABIS) {
                sb.append(abi).append(",");
            }

            sb.deleteCharAt(sb.length() - 1);

             JSONUtils.jsonPut(action_json, "K21", sb.toString());

             JSONUtils.jsonPut(action_json, "K902", sb.toString());

        }
         JSONUtils.jsonPut(action_json, "K22", Build.DISPLAY);
         JSONUtils.jsonPut(action_json, "K23", Build.ID);
         JSONUtils.jsonPut(action_json, "K24", Build.MANUFACTURER);
         JSONUtils.jsonPut(action_json, "K25", Build.BOARD);
         JSONUtils.jsonPut(action_json, "K26", Build.BOOTLOADER);
         JSONUtils.jsonPut(action_json, "K27", Build.RADIO);
         JSONUtils.jsonPut(action_json, "K28", Build.TAGS);
         JSONUtils.jsonPut(action_json, "K29", Build.USER);

         JSONUtils.jsonPut(action_json, "K903", Build.TYPE);
         JSONUtils.jsonPut(action_json, "K904", Build.getRadioVersion());
         if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
             JSONUtils.jsonPut(action_json, "K905", Build.VERSION.BASE_OS);
         }
         JSONUtils.jsonPut(action_json, "K906", Build.VERSION.SDK_INT);
         JSONUtils.jsonPut(action_json, "K907", Build.VERSION.CODENAME);
         JSONUtils.jsonPut(action_json, "K908", Build.VERSION.INCREMENTAL);

        return action_json.toString();
    }
}
