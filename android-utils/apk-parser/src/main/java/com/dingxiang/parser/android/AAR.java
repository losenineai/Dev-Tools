package com.dingxiang.parser.android;

import com.dingxiang.base.BaseFile;

import java.io.File;
import java.io.IOException;

public class AAR extends BaseFile {

    private String mEntryClass;

    public AAR(File file) throws IOException {
        super(file);
        mEntryClass = "";
    }

    public AAR(File file, String entryClass) throws IOException {
        super(file);
        mEntryClass = entryClass;
    }

    public String getEntryClass(){
        return mEntryClass;
    }

}
