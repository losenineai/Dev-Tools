package com.xxlib.utils;

import java.text.DecimalFormat;

public class NumberTool {

	public static String percent(int y, int z) {  
        String baifenbi = "";// 接受百分比的值  
        double baiy = y * 1.0;  
        double baiz = z * 1.0;  
        double fen = baiy / baiz;  
        // NumberFormat nf = NumberFormat.getPercentInstance(); 注释掉的也是一种方法  
        // nf.setMinimumFractionDigits( 2 ); 保留到小数点后几位  
        DecimalFormat df1 = new DecimalFormat("##%"); // ##.00%  
                                                            // 百分比格式，后面不足2位的用0补齐  
        baifenbi = df1.format(fen);  
        return baifenbi;  
    }  
}
