package com.auric.intell.commonlib.utils;

import android.content.Context;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import jxl.Sheet;
import jxl.Workbook;

/**
 * Created by Tin on 2017/11/8.
 * 使用本类需要导入第三方Jar包jxl
 * 暂不支持xlns文件的解析，因为懒。。。
 */

public class ExcelAnalyseUtil {



    /**
     * XLS文件解析
     */
    public static List<List<String>> readXls(Context context, String path) throws Exception {
        List<List<String>> lists = new ArrayList<>();
        InputStream is = new FileInputStream(path);
        FileOutputStream os;
        Workbook workbook;
        //拿到文件的数据流
        File tempFile = new File(context.getCacheDir(), "sell.xls");
        os = new FileOutputStream(tempFile);
        byte[] bytes = new byte[1024];
        int len;
        while ((len = is.read(bytes)) > 0) {
            os.write(bytes, 0, len);
        }
        os.close();
        is.close();
        //转换为Workbook类型数据
        workbook = Workbook.getWorkbook(tempFile);
        Sheet[] sheets = workbook.getSheets();   //拿到Sheets[]数组，再根据sheet->column->row的顺序获取每个单元格的数据
        for (Sheet sheet : sheets) {
            int rows = sheet.getRows();
            int columns = sheet.getColumns();
            for (int i = 0; i < columns; i++) {
                List<String> list = new ArrayList<>();
                for (int j = 0; j < rows; j++) {
                    String content = sheet.getCell(i, j).getContents();  //获取sheet单元格[i, j]的数据
                    list.add(content);
                }
                lists.add(list);
            }
        }
        return lists;
    }

}
