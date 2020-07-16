package com.dx.mobile.risk.collect.hardware;

import android.Manifest;
import android.os.Environment;
import android.os.StatFs;

import com.dx.mobile.risk.data.JNIConst;
import com.dx.mobile.risk.utils.FileUtils;
import com.dx.mobile.risk.utils.JSONUtils;
import com.dx.mobile.risk.utils.PermissionUtils;

import org.json.JSONObject;

import java.io.File;

/**
 * Time: 2019-10-31
 * Author: blue
 * Description:
 */
public class Memory {

    public static String getMemInfo() {

        try {

            JSONObject action_info = new JSONObject();
            StringBuilder allMem = new StringBuilder();

            // ram
            {
                byte[] meminfo_data = FileUtils.readAllFromFile("/proc/meminfo");
                if (meminfo_data != null) {
                    String lines[] = new String(meminfo_data).split("\n");

                    int index = 0;
                    long memTotal = 0, memFree = 0;

                    for (String line : lines) {
                        if (line.contains("MemTotal")) {
                            String seg[] = line.split("\\s+");
                            if (seg != null && seg.length >= 3) {

                                try {
                                    memTotal = Long.parseLong(seg[1]) * 1024;
                                } catch (Exception e) {
                                }
                            }
                        } else if (line.contains("MemFree")) {
                            String seg[] = line.split("\\s+");
                            if (seg != null && seg.length >= 3) {

                                try {
                                    memFree = Long.parseLong(seg[1]) * 1024;
                                } catch (Exception e) {
                                }
                            }
                        }

                        if (++index >= 4)
                            break;
                    }

                    allMem.append("-").append(memTotal);
                    JSONUtils.jsonPut(action_info, "K66", memTotal + "," + memFree);
                    JSONUtils.jsonPut(action_info, JNIConst.KEY_APP_KOCK_INDEX9, JNIConst.VALUE_APP_KOCK);
                }
            }

            // rom
            {
                String path = Environment.getDataDirectory().getPath();
                StatFs stat = new StatFs(path);

                long availableBlock = stat.getAvailableBlocks();
                long blockSize = stat.getBlockSize();
                long totalBlocks = stat.getBlockCount();
                long avaliableMemory = availableBlock * blockSize;
                long totalMemory = totalBlocks * blockSize;

                JSONUtils.jsonPut(action_info, "K67", avaliableMemory + "," + totalMemory);
                allMem.append("-").append(totalMemory);

                if (FileUtils.existSDCard() &&
                        PermissionUtils.checkPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                    File sdcardDir = FileUtils.getSDCardRootDir();
                    StatFs sf = new StatFs(sdcardDir.getPath());

                    long aBlock = sf.getAvailableBlocks();
                    long bSize = sf.getBlockSize();
                    long bCount = sf.getBlockCount();
                    long ex_avaliableMemory = aBlock * bSize;
                    long ex_totalMemory = bCount * bSize;

                    JSONUtils.jsonPut(action_info, "K68", ex_avaliableMemory + "," + ex_totalMemory);
                    allMem.append("-").append(ex_totalMemory);
                }
            }

            if (allMem.length() > 0) {
                allMem.deleteCharAt(0);
            }

            JSONUtils.jsonPut(action_info, "K65", allMem.toString());

            return action_info.toString();

        } catch (Throwable t) {

        }

        return "";
    }
}
