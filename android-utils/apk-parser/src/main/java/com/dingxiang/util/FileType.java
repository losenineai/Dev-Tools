package com.dingxiang.util;

/**
 * 文件类型定义
 *
 * @author lai
 */
public enum FileType {

    // --------------------------------------------- Android File  -----------------------------------------------------

    //  Android binary XML
    ANDROID_BINARY_XML("03000800"),

    // Android Dalvik Executable File
    DEX("6465780A30333500"),

    // --------------------------------------------- iOS File  -----------------------------------------------------
    IOS_FAT_MAGIC_32("cafebabe".toUpperCase()),
    IOS_FAT_MAGIC_64("cafebabf".toUpperCase()),

    IOS_THIN_MAGIC_32("feedface".toUpperCase()),
    IOS_THIN_MAGIC_64("feedfacf".toUpperCase()),

    // ZIP Archive.
    ZIP("504B0304");

    // -------------------------------------------   END    ----------------------------------------------------
    //File Type value
    private String value = "";


    /**
     * Constructor.
     *
     * @param value type value
     */
    private FileType(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

}