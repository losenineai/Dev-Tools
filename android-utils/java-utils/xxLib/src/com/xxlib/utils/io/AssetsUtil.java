package com.xxlib.utils.io;

import android.content.Context;

import com.xxlib.utils.FileUtils;
import com.xxlib.utils.base.LogTool;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by jammy on 15/11/6.
 */
public class AssetsUtil {

    private static final String TAG = "xxlib_AssetsUtil";

    public static void copyFromAssets(Context context, String origFileName, String outpath) {
        try {
            File destFile = new File(outpath);
            File dir = destFile.getParentFile();
            if (!dir.exists()) {
                dir.mkdirs();
            }
            if (!destFile.exists()) {
                destFile.createNewFile();
            }

            InputStream myInput;
            OutputStream myOutput = new FileOutputStream(outpath);
            myInput = context.getAssets().open(origFileName);
            byte[] buffer = new byte[1024];
            int length = myInput.read(buffer);
            while (length > 0) {
                myOutput.write(buffer, 0, length);
                length = myInput.read(buffer);
            }
            myOutput.flush();
            myInput.close();
            myOutput.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static boolean isHasFileInAsset(Context context, String fileName) {
        try {
            InputStream is = context.getAssets().open(fileName);
            if (null == is) {
                LogTool.i(TAG, "input stream is null");
                return false;
            }
            try {
                if (null != is) {
                    is.close();
                }
            } catch (Exception e) {
                LogTool.i(TAG, "close input stream"+e.toString());
            }
            return true;
        } catch (Exception e) {
            LogTool.i(TAG, e.toString());
        }
        return false;
    }


    /**
     * 读取小文件，大文件慎重
     * @return 文件的全部内容返回
     */
    public static String readAssetFile(Context context, String origFileName) {
        try {
            InputStream input = context.getAssets().open(origFileName);
            String str = FileUtils.readFile(input, "utf-8");
            try {
                if (null != input) {
                    input.close();
                }
            } catch (Exception e) {
            }
            return str;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }
}
