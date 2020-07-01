package com.auric.intell.commonlib.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Properties;

/**
 * @author : jinlongfeng
 * @version : V 1.5
 * @description: Properties 工具操作类
 * @time : 16/7/26-11:42
 */
public class PropertyUtil {

    public static Properties loadConfig(String file) {
        Properties properties = new Properties();
        try {

            File saveFile = new File(file);

            if(!saveFile.exists()){
                saveFile.getParentFile().mkdir();
                saveFile.createNewFile();
            }

            FileInputStream s = new FileInputStream(file);
            properties.load(s);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return properties;
    }

    public static void saveConfig(String file, Properties properties) {
        try {
            FileOutputStream s = new FileOutputStream(file, false);
            properties.store(s, "");
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}
