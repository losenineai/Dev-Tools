package com.dx.mobile.risk.collect.hardware;

import android.app.Service;
import android.content.Context;
import android.hardware.Camera;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraManager;
import android.os.Build;
import android.util.Log;
import android.util.Size;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.PermissionUtils;

import org.json.JSONObject;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

/**
 * Time: 2019-11-01
 * Author: blue
 * Description:
 */
public class Cameras {

    public static String getCamerasInfo() {

        JSONObject action_json = new JSONObject();
        Context ctx = ContextFinder.getApplication();

        int curKey = 2000;
        if(!PermissionUtils.checkCamerPermission()){
            return "";
        }

        JSONUtils.jsonPut(action_json, JNIConst.KEY_APP_KOCK_INDEX6, JNIConst.VALUE_APP_KOCK);
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {

            Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
            int numberOfCameras = Camera.getNumberOfCameras();

            Log.e("DXRiisk", "numberOfCameras:" + numberOfCameras);

            for (int index = 0; index < numberOfCameras; index++) {

                JSONObject camera_json = new JSONObject();

                Camera.getCameraInfo(index, cameraInfo);

                JSONUtils.jsonPut(camera_json, "facing", cameraInfo.facing == 1 ? 1 : 2);

                Camera open = Camera.open(index);
                Camera.Parameters parameters = open.getParameters();
                Camera.Size size = parameters.getSupportedPictureSizes().get(0);

                JSONUtils.jsonPut(camera_json, "pixels", (size.height * size.width) / 10000);
                JSONUtils.jsonPut(camera_json, "focals", parameters.getFocalLength());

                HashMap<String, String> hashMap = new HashMap();

                for (String split : parameters.flatten().split(";")) {
                    String[] split2 = split.split("=");
                    hashMap.put(split2[0], split2[1]);
                }

                for (Map.Entry entry : hashMap.entrySet()) {
                    if ((entry.getKey()).equals("rotation")) {
                        JSONUtils.jsonPut(camera_json, "rotation",  entry.getValue());
                    } else if ((entry.getKey()).equals("focus-distances")) {
                        JSONUtils.jsonPut(camera_json, "focusDistance",  entry.getValue());
                    } else if ((entry.getKey()).equals("max-num-focus-areas")) {
                        JSONUtils.jsonPut(camera_json, "maxfocusAreaNum",  entry.getValue());
                    } else if ((entry.getKey()).equals("scene-mode-values")) {
                        JSONUtils.jsonPut(camera_json, "scene",  entry.getValue());
                    } else if (( entry.getKey()).equals("zoom-supported")) {
                        JSONUtils.jsonPut(camera_json, "zoomSupported",  entry.getValue());
                    } else if ((entry.getKey()).equals("max-zoom")) {
                        JSONUtils.jsonPut(camera_json, "maxZoom",  entry.getValue());
                    } else if ((entry.getKey()).equals("zoom-ratios")) {
                        JSONUtils.jsonPut(camera_json, "zoomRatios",  entry.getValue());
                    }
                }

                open.release();

                JSONUtils.jsonPut(action_json, "K" + (curKey + index), camera_json.toString());
            }
        } else {
            CameraManager cameraManager = (CameraManager) ctx.getSystemService(Service.CAMERA_SERVICE);

            try {
                String[] cameraIdList = cameraManager.getCameraIdList();

                int length = cameraIdList.length;

                for (int i = 0; i < length; i++) {

                    JSONObject camera_json = new JSONObject();

                    CameraCharacteristics camera = cameraManager.getCameraCharacteristics(cameraIdList[i]);
                    Integer num = camera.get(CameraCharacteristics.LENS_FACING);
                    if (num != null && num.intValue() == 0) {
                        JSONUtils.jsonPut(camera_json, "facing", 1);
                    } else if (num != null && num.intValue() == 1) {
                        JSONUtils.jsonPut(camera_json, "facing", 2);
                    }

                    JSONUtils.jsonPut(camera_json, "pixels", getPixels(camera));

                    float[] fArr = camera.get(CameraCharacteristics.LENS_INFO_AVAILABLE_FOCAL_LENGTHS);
                    if (fArr != null) {
                        Arrays.sort(fArr);
                        JSONUtils.jsonPut(camera_json, "focals", fArr[fArr.length - 1]);
                    }

                    JSONUtils.jsonPut(camera_json, "orientation", camera.get(CameraCharacteristics.SENSOR_ORIENTATION).intValue());
                    JSONUtils.jsonPut(camera_json, "compensation",  camera.get(CameraCharacteristics.CONTROL_AE_COMPENSATION_RANGE).getLower());
                    JSONUtils.jsonPut(camera_json, "aeRegions", camera.get(CameraCharacteristics.CONTROL_MAX_REGIONS_AE).intValue());
                    JSONUtils.jsonPut(camera_json, "afRegions", camera.get(CameraCharacteristics.CONTROL_MAX_REGIONS_AF).intValue());
                    JSONUtils.jsonPut(camera_json, "size", camera.get(CameraCharacteristics.SENSOR_INFO_PHYSICAL_SIZE));

                    JSONUtils.jsonPut(action_json, "K" + (curKey + i), camera_json.toString());

                }
            } catch (Exception e) {  }
        }

        return action_json.length() > 0 ? action_json.toString() : "";
    }

    private static int getPixels(CameraCharacteristics camera) {
        Size size = Collections.max(Arrays.asList((camera.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)).getOutputSizes(256)), new Comparator<Size>() {
            @Override
            public int compare(Size size, Size size2) {
                return Long.signum((((long) size.getWidth()) * ((long) size.getHeight())) - (((long) size2.getWidth()) * ((long) size2.getHeight())));
            }


        });
        return (size.getHeight() * size.getWidth()) / 10000;
    }

}
