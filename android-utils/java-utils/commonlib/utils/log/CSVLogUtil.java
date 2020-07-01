package com.auric.intell.commonlib.utils.log;

import android.text.TextUtils;

import com.auric.intell.commonlib.utils.ContextFinder;
import com.auric.intell.commonlib.utils.LogTool;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVPrinter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;


/**
 * @author white
 * @description：csv格式的log
 * @date 2016/12/14
 */

public class CSVLogUtil {

    private static final String TAG = "CSVLogUtil";

    public static final String BASE_LOG_PATH = ContextFinder.getApplication().getExternalFilesDir("") + File.separator + "log" + File.separator;

    public static final String ROBOTX_DIALOG_LOG_FILE_PATH = BASE_LOG_PATH + "rbx_dialog_log.csv";
    // 嘉佳
    private static final String[] ROBOTX_LOG_HEADER = new String[]{
            "时间",
            "ASR结果",
            "ASR耗时(ms)",
            "本地DB耗时(ms)",
            "Http耗时(ms)",
            "总耗时(ms)",
            "ASR结果详情",
            "本地应答",
            "Http响应内容",
            };


    /**
     * 用于Robotx
     * @param itemList
     */
    public static void writeLineForRbx(List<String> itemList){
        writeLine(ROBOTX_DIALOG_LOG_FILE_PATH, ROBOTX_LOG_HEADER, itemList);
    }

    public static void writeLine(String[] csvHeader, List<String> itemList){
        writeLine(ROBOTX_DIALOG_LOG_FILE_PATH, csvHeader, itemList);
    }


    /**
     * 通用api, 写入一行记录
     * @param filePath
     * @param csvHeader
     * @param itemList
     */
    public static void writeLine(String filePath, String[] csvHeader, List<String> itemList){
        if (TextUtils.isEmpty(filePath) || itemList == null || itemList.size() == 0) {
            return;
        }
        LogTool.d(TAG, "writeLine :"+itemList);

        // time
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        String timeStr = dateFormat.format(new Date(System.currentTimeMillis()));
        itemList.add(0, timeStr);

        CSVPrinter printer = null;

        try {
            File file = new File(filePath);

            // 第一创建文件,才添加Header
            CSVFormat format = CSVFormat.EXCEL;
            if (csvHeader != null && !file.exists()) {
                LogTool.d(TAG, "file.exists()"+file.exists());
                format = format.withHeader(csvHeader);
            }

            if (!file.getParentFile().exists()) {
                file.getParentFile().mkdirs();
            }
            if (!file.exists()) {
                file.createNewFile();
            }

            Writer out = new FileWriter(filePath, true);
            printer = new CSVPrinter(out, format);
            printer.printRecord(itemList);
            printer.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            if (printer != null) {
                try {
                    printer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }


    }

}
