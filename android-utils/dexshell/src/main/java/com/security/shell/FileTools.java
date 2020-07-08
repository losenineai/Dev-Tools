package com.security.shell;

import java.io.File;

public class FileTools {
    /**
     * 递归删除文件和文件夹
     * @param file    要删除的根目录
     */
    public static void DeleteDir(File file){

        if(file.isFile()){
            file.delete();
            return;
        }

        if(file.isDirectory()){
            File[] childFile = file.listFiles();
            if(childFile == null || childFile.length == 0){
                file.delete();
                return;
            }
            for(File f : childFile){
                DeleteDir(f);
            }
            file.delete();
        }
    }

}
