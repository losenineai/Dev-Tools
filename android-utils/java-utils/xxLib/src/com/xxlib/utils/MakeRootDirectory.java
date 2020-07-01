package com.xxlib.utils;

import java.io.File;

/**
 * 创建文件路径
 * @author linbin
 * 2014
 */
public class MakeRootDirectory {

	public static void make(String filePath) {  
        File file = null;  
        try { 
            file = new File(filePath);  
            if (!file.exists()) {  
                file.mkdir();  
            }  
        } catch (Exception e) {  
  
        }  
	}
	
}
