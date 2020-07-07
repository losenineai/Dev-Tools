package com.dingxiang.base;

import com.dingxiang.util.HashTool;

import java.io.File;
import java.io.IOException;

abstract public class BaseFile {
    private String absolutePath;
    private String fileName;
    private String fileMd5;
    private long fileSize;

    public BaseFile(File file) throws IOException {
            absolutePath = file.getAbsolutePath();
            fileName = file.getName();
            fileMd5 = HashTool.getMD5(file);
            fileSize = file.length();
    }

    /**
     * 获取文件名
     * @return 文件名
     */
    public String getFileName() {
        return fileName;
    }

    /**
     * 获取文件路径
     * @return 文件路径
     */
    public String getAbsolutePath(){
        return absolutePath;
    }

    /**
     * 获取文件md5
     * @return 文件md5
     */
    public String getFileMd5(){
        return fileMd5;
    }

    /**
     * 获取文件大小，MB
     * @return 文件大小，MB
     */
    public int getFileSizeMB(){
        return (int)(fileSize/(1024*1024));
    }

    /**
     * 获取文件大小，KB
     * @return 文件大小，KB
     */
    public int getFileSizeKB(){
        return (int)(fileSize/1024);
    }

    /**
     * 获取文件大小
     * @return 文件大小
     */
    public long getFileSize(){
        return fileSize;
    }
}
